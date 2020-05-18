#include <windows.h>
#include <virtdisk.h>
#include <tchar.h>
#include <comdef.h>
#include "abRctCommon.h"
#include "abRctDemo.h"
#include <iostream>
#include <atlcomcli.h>
#include <string>
#include <atlstr.h>


using namespace std;

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "virtdisk.lib")

abRctDemo::abRctDemo()
	: m_pWbemLocator(NULL)
	, m_pWbemServices(NULL)
	,m_pWbemClassObject(NULL)

{
	try {
		// 初始化com
		initComComponent();

		// 创建实例
		createComInstance();
	}
	catch (abRctDemo &e) {
		releaseResource();
		throw exception(_T("Failed to create  instance of  wmi."));
	}
	catch (...) {
		releaseResource();
		throw exception(_T("create ncHypervWmi obj unknow errorr."));
	}
}

abRctDemo::~abRctDemo()
{
	releaseResource();
}


void
abRctDemo::initComComponent()
{
	HRESULT hres;

	// 1、初始化com库
	try {
		hres = CoInitializeEx(NULL,	// 预留的参数，必须为空
			COINIT_MULTITHREADED);	// 设置并发模式的枚举值
		if (FAILED(hres)) {
			cout << "Failed to excute(CoInitializeEx)." << endl;
			return;
		}
	}
	catch (const std::exception &e) {
		// 
		// 不知哪里已经初始化了com库
		//
		cout << "Failed to excute(CoInitializeEx)." << endl;
		return;
	}

	// 2、设置com的安全等级,调用c++的默认等级
	hres = CoInitializeSecurity(NULL,
		-1,							// COM authentication
		NULL,						// Authentication services
		NULL,						// Reserved
		RPC_C_AUTHN_LEVEL_DEFAULT,	// Default authentication 
		RPC_C_IMP_LEVEL_IMPERSONATE,// Default Impersonation 客户端所扮演的角色 
		NULL,						// Authentication info
		EOAC_NONE,					// 设置额外的能力 
		NULL);						// 预留参数，为空

	if (FAILED(hres)) {
		if (hres != RPC_E_TOO_LATE) {
			cout << "Failed to excute(CoInitializeSecurity)." << endl;
			return;
		}
	}
}

void
abRctDemo::createComInstance()
{
	// 对com进行标准调用后，需要通过调用
	HRESULT hres;

	// 根据CLSID创建该类的实例
	hres = CoCreateInstance(CLSID_WbemLocator,		// 需要创建类的CLSID           
		NULL,					// 不是集合的一部分 
		CLSCTX_INPROC_SERVER,	// 类上下文
		IID_IWbemLocator,
		(LPVOID *)&m_pWbemLocator);		// 返回新创建对象的指针地址		  

	if (FAILED(hres)) {
		cout << "Failed to excute(CoCreateInstance)." << endl;
		return;
	}
}

void
abRctDemo::connectWmiNamespace()
{
	HRESULT hres;
	BSTR wmiNamespace;

	wmiNamespace = ::SysAllocString(L"\\\\.\\Root\\Virtualization\\V2");
	// 根据ip、用户名和密码连接虚拟化的名称空间
	hres = m_pWbemLocator->ConnectServer(wmiNamespace,
		NULL,
		NULL,
		0,
		NULL,
		_bstr_t(L""),
		0,
		&m_pWbemServices);

	if (FAILED(hres)) {
		cout << "Failed to excute(ConnectServer)." << endl;
		return;
	}
	::SysFreeString(wmiNamespace);

	// 为代理设置安全等级
	hres = CoSetProxyBlanket(m_pWbemServices,						// The proxy to be set.
		RPC_C_AUTHN_WINNT,			// RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,			// RPC_C_AUTHZ_xxx
		NULL,						// Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,		// RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE,// RPC_C_IMP_LEVEL_xxx
		NULL,						// client identity
		EOAC_NONE);					// proxy capabilities 

	if (FAILED(hres)) {
		cout << "Failed to excute(CoSetProxyBlanket)." << endl;
		return;
	}
}

IEnumWbemClassObject*
abRctDemo::doExecQuery(string sql)
{

	HRESULT hres;
	// 使用IWbemServices来查询要获取的信息 
	IEnumWbemClassObject* pEnumerator = NULL;

	hres = m_pWbemServices->ExecQuery(
		_bstr_t("WQL"),
		_bstr_t(sql.c_str()),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hres)) {
		cout << "Failed to excute(ExecQuery)." << endl;
		return NULL;
	}

	hres = CoSetProxyBlanket(
		pEnumerator,				// Indicates the proxy to set
		RPC_C_AUTHN_WINNT,			// RPC_C_AUTHN_xxx
		RPC_C_AUTHZ_NONE,			// RPC_C_AUTHZ_xxx
		NULL,						// Server principal name 
		RPC_C_AUTHN_LEVEL_CALL,		// RPC_C_AUTHN_LEVEL_xxx 
		RPC_C_IMP_LEVEL_IMPERSONATE,// RPC_C_IMP_LEVEL_xxx
		NULL,						// client identity
		EOAC_NONE);					// proxy capabilities

	if (FAILED(hres)) {
		cout << "Failed to excute(CoSetProxyBlanket)." << endl;
		return NULL;
	}
	return pEnumerator;
}

void
abRctDemo::releaseCom()
{
	// 释放com资源
	CoUninitialize();
}

void
abRctDemo::releaseResource()
{
	try {
		if (m_pWbemLocator) {
			m_pWbemLocator->Release();
			m_pWbemLocator = NULL;
		}

		if (m_pWbemServices) {
			m_pWbemServices->Release();
			m_pWbemServices = NULL;
		}

		releaseCom();
	}
	catch (const std::exception &e) {
		// do nothing
	}
	catch (...) {
		// do nothing
	}
}

void
abRctDemo::createSnapshot(LPCWSTR vmName)
{
	HRESULT hr;

	IWbemClassObject *pInClass = NULL;
	IWbemClassObject *pOutClass = NULL;
	
	IWbemContext	 *pCtx = 0;
	IWbemCallResult  *pResult = 0;
	cout << "##Step 1: Create snapshot..." << endl;
	cout << "-----------------------------------------------------------";
	cout << "-----------------------------------------------------------" << endl;
	BSTR snapshotClassPath = SysAllocString(L"Msvm_VirtualSystemSnapshotService");
	hr = m_pWbemServices->GetObject(
		snapshotClassPath, 
		0, 
		NULL, 
		&m_pWbemClassObject, 
		NULL);
	if (FAILED(hr))
	{
		std::cout << "Failed to get class object(Msvm_VirtualSystemSnapshotService). Error code = 0x" << hex << hr << std::endl;
		return;
	}

	// Create snapshot method
	BSTR MethodNameOne = SysAllocString(L"CreateSnapshot");

	hr = m_pWbemClassObject->GetMethod(
		MethodNameOne,
		0,
		&pInClass,
		&pOutClass
	);
	if (FAILED(hr))
	{
		std::cout << "Failed to get method(CreateSnapshot). Error code = 0x" << hex << hr << endl;
	}
	cout << "Succeeded to get method(CreateSnapshot)." << endl;


	BSTR virtualSystemSnaphotSettingData = SysAllocString(L"Msvm_VirtualSystemSnapshotSettingData");
	// Get the Msvm_VirtualSystemSnapshotSettingData Class object
	IWbemClassObject * pvirtualSystemSnaphotSettingData = NULL;

	hr = m_pWbemServices->GetObject(
		virtualSystemSnaphotSettingData,
		0,
		pCtx,
		&pvirtualSystemSnaphotSettingData,
		&pResult
	);
	if (FAILED(hr))
	{
		wprintf(L"Error GetObject Msvm_VirtualSystemSnapshotSettingData:0x%08lx\n", hr);
	}
	std::cout << "Succeeded to get object(Msvm_VirtualSystemSnapshotSettingData)." << std::endl;

	// Create instance of Msvm_VirtualSystemSnapshotSettingData class
	IWbemClassObject * pInpInstOfSnapshotSettingData = NULL;
	hr = pvirtualSystemSnaphotSettingData->SpawnInstance(
		0,
		&pInpInstOfSnapshotSettingData);
	if (FAILED(hr))
	{
		wprintf(L"Error SpawnInstance Msvm_VirtualSystemSnapshotSettingData:0x%08lx\n", hr);
	}
	std::cout << "Succeeded to created instance(Msvm_VirtualSystemSnapshotSettingData )." << std::endl;

	// Set the property(Consistency Level)
	BSTR memberOneForVirtualSystemSnapshotSettingData = SysAllocString(L"ConsistencyLevel");
	VARIANT consistencyLevel;
	VariantInit(&consistencyLevel);
	V_VT(&consistencyLevel) = VT_BSTR;
	V_BSTR(&consistencyLevel) = SysAllocString(L"1");
	hr = pInpInstOfSnapshotSettingData->Put(
		memberOneForVirtualSystemSnapshotSettingData,
		0,
		&consistencyLevel,
		0);
	if (FAILED(hr))
	{
		wprintf(L"Error memberOneForVirtualSystemSnapshotSettingData:0x%08lx\n", hr);
	}
	wprintf(L"Succeeded to set property(ConsistencyLevel), Consistency level:%s\n", consistencyLevel.bstrVal);
	VariantClear(&consistencyLevel);

	// Set the property(ElementName)
	BSTR memberThreeForVirtualSystemSnapshotSettingData = SysAllocString(L"ElementName");
	VARIANT elementName;
	VariantInit(&elementName);
	V_VT(&elementName) = VT_BSTR;
	V_BSTR(&elementName) = SysAllocString(vmName);

	hr = pInpInstOfSnapshotSettingData->Put(
		memberThreeForVirtualSystemSnapshotSettingData,
		0,
		&elementName,
		0);
	if (FAILED(hr))
	{
		wprintf(L"Failed to set property(ElementName), Erorr code:0x%08lx\n", hr);
	}
	wprintf(L"Succeeded to set property(ElementName):%s\n", elementName.bstrVal);
	VariantClear(&elementName);

#if 0
	// Use the IWbemServices pointer to make requests of WMI. 
	IEnumWbemClassObject* pEnumerator = NULL;
	hr = pNamespace->ExecQuery(
		BSTR(L"WQL"),
		//BSTR(L"select * from CIM_ComputerSystem where Caption=\"Virtual Machine\""),
		BSTR(L"SELECT * FROM CIM_ComputerSystem WHERE Caption=\"虚拟机\""),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator);

	if (FAILED(hr)) {
		wprintf(L"Failed to retrieve virtual machines, Error code: %d\n", hr);
	}
	BSTR ArgNameOne = SysAllocString(L"AffectedSystem");
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;

	std::cout << "Retrieve virtual machines..." << std::endl;
	while (1)
	{
		hr = pEnumerator->Next(
			WBEM_INFINITE,	//是否等待对象回去	
			1,				//每次获取对象的个数
			&pclsObj,
			&uReturn);		//返回对象的个数

		if (FAILED(hr) || !uReturn)
			break;

		VARIANT vtProp12;
		// Get the value of the Name property
		hr = pclsObj->Get(L"ElementName", 0, &vtProp12, 0, 0);
		if (FAILED(hr))
		{
			wprintf(L"Failed to get ElementName.\n");
		}
		wprintf(L"ElementName: %s\n", vtProp12.bstrVal);

		if (0 == wcscmp(vtProp12.bstrVal, L"CentOS7"))
		{
			VARIANT vtProp;
			hr = pclsObj->Get(L"__Path", 0, &vtProp, 0, 0);
			if (FAILED(hr))
			{
				wprintf(L"Failed to get Path.\n");
			}
			wprintf(L"Path: %s\n", vtProp12.bstrVal);

			hr = pInInst->Put(ArgNameOne, 0, &vtProp, 0);
			if (FAILED(hr))
			{
				wprintf(L"Failed to set property(AffectedSystem).\n");
			}
			VariantClear(&vtProp);
		}
		// 释放该对象
		VariantClear(&vtProp12);
		pclsObj->Release();
		pclsObj = NULL;
	}
#endif
	// Set the property(ignore disks that aren't compatible with snapshots)
	BSTR memberTwoForVirtualSystemSnapshotSettingData = SysAllocString(L"IgnoreNonSnapshottableDisks");
	VARIANT ignoreNonSnapshottableDisks;
	ignoreNonSnapshottableDisks.vt = VT_BOOL;
	ignoreNonSnapshottableDisks.boolVal = true;

	hr = pInpInstOfSnapshotSettingData->Put(
		memberTwoForVirtualSystemSnapshotSettingData,
		0,
		&ignoreNonSnapshottableDisks,
		0);
	if (FAILED(hr))
	{
		wprintf(L"Error memberTwoForVirtualSystemSnapshotSettingData:0x%08lx\n", hr);
	}
	wprintf(L"Succeeded to set property(IgnoreNonSnapshottableDisks):%d\n", ignoreNonSnapshottableDisks.boolVal);
	VariantClear(&ignoreNonSnapshottableDisks);

	// Create or update instance
	hr = m_pWbemServices->PutInstance(
		pInpInstOfSnapshotSettingData,
		0,
		pCtx,
		&pResult);
	if (FAILED(hr))
	{
		wprintf(L"Error PutInstance:0x%08lx\n", hr);
	}
	wprintf(L"Succeeded to update the instance.\n");

	// Get textual rendering of the object in the MOF syntax
	BSTR objString = NULL;
	hr = pInpInstOfSnapshotSettingData->GetObjectText(0, &objString);
	if (FAILED(hr))
	{
		wprintf(L"Error GetObjectText:0x%08lx\n", hr);
	}
	wprintf(L"Succeeded to get textual rendering.\n");

	BSTR ArgNameTwo = SysAllocString(L"SnapshotSettings");
	VARIANT v;
	V_VT(&v) = VT_BSTR;
	V_BSTR(&v) = objString;
	wprintf(L"Object text: %s\n", objString);

	// Pass the second Argument to CreateSnapshot method
	hr = pInClass->Put(ArgNameTwo, 0, &v, 0);
	if (FAILED(hr))
	{
		wprintf(L"Error ArgNameTwo:0x%08lx\n", hr);
	}
	VariantClear(&v);
	wprintf(L"Succeeded to set property(SnapshotSettings).\n");

	// Method2
#if 0
	BSTR ArgNameTwo = SysAllocString(L"SnapshotSettings");
	VARIANT v;
	V_VT(&v) = VT_BSTR;
	V_BSTR(&v) = SysAllocString(L"");

	// Pass the second Argument to CreateSnapshot method
	hr = pInInst->Put(ArgNameTwo, 0, &v, 0);
	if (FAILED(hr))
	{
		wprintf(L"Error ArgNameTwo:0x%08lx", hr);
	}
	VariantClear(&v);
	wprintf(L"Succeeded to set property(SnapshotSettings).\n");
#endif

	// set property(AffectedSystem)
	VARIANT vtProp;
	m_pWbemClassObject->Get(L"__Path", 0, &vtProp, 0, 0);
	hr = pInClass->Put(L"AffectedSystem", 0, &vtProp, NULL);
	if (FAILED(hr))
	{
		std::cout << "Failed to set property(AffectedSystem)." << std::endl;
	}
	wprintf(L"AffectedSystem: %s\n", vtProp.bstrVal);
	VariantClear(&vtProp);

	// Pass the Third Argument to CreateSnapshot method
	BSTR ArgNameThree = SysAllocString(L"SnapshotType");
	VARIANT var;
	VariantInit(&var);
	V_VT(&var) = VT_BSTR;
	V_BSTR(&var) = SysAllocString(L"32768");
	hr = pInClass->Put(ArgNameThree, 0, &var, 0);
	if (FAILED(hr))
	{
		std::cout << "Failed to set property(SnapshotType)." << std::endl;
	}
	wprintf(L"SnapshotType: %s\n", var.bstrVal);
	VariantClear(&var);

	// Call the CreateSnapshot method.
	IEnumWbemClassObject* pEnumOb1 = NULL;
	hr = m_pWbemServices->ExecQuery(
		BSTR(L"WQL"),
		BSTR(L"SELECT * FROM Msvm_VirtualSystemSnapshotService"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumOb1
	);
	if (FAILED(hr)) {
		std::cout << "SELECT * FROM Msvm_VirtualSystemSnapshotService, Error code:0x%08lx" << hex << hr << endl;
	}
	std::cout << "Succeeded to query snapshot." << std::endl;

	IWbemClassObject *pclsObj1 = NULL;
	ULONG uReturn1 = 0;

	std::cout << "Create snapshot..." << std::endl;
	while (1)
	{
		HRESULT hr = pEnumOb1->Next(WBEM_INFINITE, 1, &pclsObj1, &uReturn1);

		if (FAILED(hr) || !uReturn1)
		{
			break;
		}
		VARIANT vt;
		// Get the value of the path property from Msvm_VirtualSystemSnapshotService Class
		hr = pclsObj1->Get(L"__Path", 0, &vt, 0, 0);
		if (FAILED(hr))
		{
			std::wcout << "Failed to get vm's path." << std::endl;
			break;
		}
		wprintf(L"__PATH:%s \n", vt.bstrVal);

		//Call the CreateSnapshot method
		hr = m_pWbemServices->ExecMethod(
			vt.bstrVal,
			MethodNameOne,
			0,
			NULL,
			pInClass,
			&pOutClass,
			NULL);
		if (FAILED(hr))
		{
			wprintf(L"Failed to execute(CreateSnapshot), Erorr code:0x%08lx\n", hr);
			break;
		}
		wprintf(L"Succeeded to create snapshot.\n");
		Sleep(10);

		pclsObj1->Release();
		pclsObj1 = NULL;
	}

}

void
abRctDemo::convertCheckpoint()
{
	HRESULT hr;

	IWbemClassObject *pInClass = NULL;
	IWbemClassObject *pOutClass = NULL;

	IWbemClassObject *pClass = NULL;
	IWbemContext	 *pCtx = 0;
	IWbemCallResult  *pResult = 0;

	cout << "##Step 2: Convert checkpoint..." << endl;
	cout << "-----------------------------------------------------------" << endl;
	BSTR snapshotClassPath = SysAllocString(L"Msvm_VirtualSystemSnapshotService");
	hr = m_pWbemServices->GetObject(
		snapshotClassPath,
		0,
		NULL,
		&pClass,
		NULL);
	if (FAILED(hr))
	{
		std::cout << "Failed to get class object(Msvm_VirtualSystemSnapshotService). Error code = 0x" << hex << hr << std::endl;
		return;
	}

	BSTR MethodNameTwo = SysAllocString(L"ConvertToReferencePoint");
	hr = pClass->GetMethod(
		MethodNameTwo,
		0,
		&pInClass,
		&pOutClass);
	if (FAILED(hr))
	{
		wprintf(L"Failed to get conversion method, Erorr code:0x%08lx\n", hr);
	}
	cout << "Succeeded to get method(ConvertToReferencePoint)." << endl;

	//Create instance of ConvertToReferencePoint method
	IWbemClassObject * pInInstanceOfConvertMethod = NULL;
	hr = pInClass->SpawnInstance(0, &pInInstanceOfConvertMethod);
	if (FAILED(hr))
	{
		wprintf(L"Failed to spawn instance, Erorr code:0x%08lx\n", hr);
	}
	cout << "Successed to create the instance." << endl;

#if 0
	// Set the property(ElementName)
	BSTR captionName = SysAllocString(L"ElementName");
	VARIANT elementName1;
	VariantInit(&elementName1);
	V_VT(&elementName1) = VT_BSTR;
	V_BSTR(&elementName1) = SysAllocString(L"CentOS7");

	hr = pInInstOfConvertMethod->Put(
		captionName,
		0,
		&elementName1,
		0);
	if (FAILED(hr))
	{
		wprintf(L"Failed tp set property(ElementName):0x%08lx\n", hr);
	}
	wprintf(L"Successfully set property(ElementName):%s\n", elementName1.bstrVal);
	VariantClear(&elementName1);
#endif
	// Create or update instance
	hr = m_pWbemServices->PutInstance(
		pInInstanceOfConvertMethod,
		0,
		pCtx,
		&pResult);
	if (FAILED(hr))
	{
		wprintf(L"Error PutInstance:0x%08lx\n", hr);
	}
	wprintf(L"succeeded to update the instance.\n");

	//Pass the First argument of ConvertToReferencePoint method
	VARIANT v2;
	pClass->Get(L"__Path", 0, &v2, 0, 0);
	BSTR ArgNameFive = SysAllocString(L"AffectedSnapshot");
	hr = pInInstanceOfConvertMethod->Put(ArgNameFive, 0, &v2, 0);
	if (FAILED(hr))
	{
		wprintf(L"Failed to set property(AffectedSnapshot), Erorr code:0x%08lx\n", hr);
	}
	VariantClear(&v2);
	cout << "Succeeded to set property(AffectedSnapshot)." << endl;

	//Pass the Second argument of ConvertToReferencePoint method
	BSTR ArgNameSix = SysAllocString(L"ReferencePointSettings");
	VARIANT varForRefPoint;
	varForRefPoint.vt = VT_BSTR;
	//Optional empty string for Argument of ReferencePointSettings
	varForRefPoint.bstrVal = SysAllocString(L"");
	hr = pInInstanceOfConvertMethod->Put(ArgNameSix, 0, &varForRefPoint, 0);
	if (FAILED(hr))
	{
		wprintf(L"Failed to set property(ReferencePointSettings), Erorr code:0x%08lx\n", hr);
	}
	VariantClear(&varForRefPoint);
	cout << "Succeeded to set property(ReferencePointSettings)." << endl;

	// Call the ConvertToReferencePoint method.
	IEnumWbemClassObject* pEnumOb2 = NULL;

	hr = m_pWbemServices->ExecQuery(
		BSTR(L"WQL"),
		BSTR(L"SELECT * FROM Msvm_VirtualSystemSnapshotService"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumOb2
	);
	if (FAILED(hr)) {
		std::cout << "Failed to query checkpoint, Error code:0x%08lx" << hex << hr << endl;
	}

	IWbemClassObject *pclsObj2 = NULL;
	ULONG uReturn2 = 0;
	while (1)
	{
		hr = pEnumOb2->Next(WBEM_INFINITE, 1, &pclsObj2, &uReturn2);

		if (FAILED(hr) || !uReturn2)
		{
			break;
		}
		VARIANT vt;
		// Get the value of the path property from Msvm_VirtualSystemSnapshotService Class
		hr = pclsObj2->Get(L"__PATH", 0, &vt, 0, 0);
		if (FAILED(hr))
		{
			std::wcout << "Failed to get vm's path." << std::endl;
			break;
		}
		wprintf(L"__PATH:%s \n", vt.bstrVal);

		IWbemClassObject *pOutInstanceOfConvertMethod = NULL;
		//Call the ConvertToReferencePoint method
		hr = m_pWbemServices->ExecMethod(
			vt.bstrVal,
			MethodNameTwo,
			0,
			NULL,
			pInInstanceOfConvertMethod,
			&pOutInstanceOfConvertMethod,
			NULL);
		if (FAILED(hr))
		{
			wprintf(L"Failed to execute(ConverToReferencePoint), Erorr code:0x%08lx\n", hr);
			break;
		}
		// 获取返回值
		/*	if (pOutClass) {
		if (!isExecuteSuccessed(pOutClass)) {
		cout << "Failed to get return value." << endl;
		}
		}*/

		VARIANT jobPath;
		hr = pOutInstanceOfConvertMethod->Get(L"Job", 0, &jobPath, NULL, NULL);
		if (FAILED(hr))
		{
			wprintf(L"Failed to get property(Job), Erorr code:0x%08lx\n", hr);
		}
		wprintf(L"Job: %s \n", jobPath.bstrVal);

		BSTR referenceCheckpointClassPath = ::SysAllocString(L"Msvm_VirtualSystemReferencePoint");
#if 0
		char sqlStatement[256];
		sprintf_s(sqlStatement, "ASSOCIATORS OF {%ls} WHERE ResultClass = %ls", jobPath.bstrVal, referenceCheckpointClassPath);
		wprintf(L"WMI query statement:%hs\n", sqlStatement);
		IEnumWbemClassObject* pEnumOb3;
		hr = m_pWbemServices->ExecQuery(
			BSTR(L"WQL"),
			BSTR(sqlStatement),
			WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
			NULL,
			&pEnumOb3
		);
		if (FAILED(hr))
		{
			wprintf(L"Failed to query(Msvm_VirtualSystemReferencePoint), Erorr code:0x%08lx \n.", hr);
		}

		ULONG uReturn3 = 0;
		IWbemClassObject* pObject3;
		hr = pEnumOb3->Next(
			WBEM_INFINITE, 
			1, 
			&pObject3, 
			&uReturn3
		);
		if (FAILED(hr) || !uReturn3)
		{
			wprintf(L"Failed to query, Erorr code:0x%08lx, total number:%ld \n.", hr, uReturn3);
			break;
		}
#endif
#if 1
		ULONG relevance = 0L;
		CComPtr<IWbemClassObject> pResult;
		relevance = getRelevance(jobPath.bstrVal, L"Msvm_VirtualSystemReferencePoint", &pResult);
		cout << "Ralevance:" << relevance << endl;
#endif 		
		wprintf(L"Succeeded to convert checkpoint.\n");
		pOutInstanceOfConvertMethod->Release();
		VariantClear(&jobPath);
		VariantClear(&vt);
		pclsObj2->Release();
		pclsObj2 = NULL;
	}
}
#if 0
CComBSTR abRctDemo::GetStrProperty(PCWSTR pPropertyName, IWbemClassObject *pObject=NULL)
{
	VARIANT value;
	::VariantInit(&value);
	this->getProperty(pPropertyName, &value, VT_BSTR, pObject);

	CComBSTR bstr;
	bstr.Attach(value.bstrVal);
	return bstr;
}

void 
abRctDemo::getProperty(PCWSTR pPropertyName,
	LPVARIANT pValue,
	UINT type,
	IWbemClassObject *pObject)
{
	if (pObject == NULL)
		pObject = m_pVmWbemClassObject;

	_ASSERT(pObject != NULL);

	CHK_HRES(pObject->Get(pPropertyName, 0, pValue, NULL, NULL));

	if (type != VT_EMPTY && pValue->vt != type)
	{
		::VariantClear(pValue);
		wprintf(L"Invalid type of the property");
	}
}

void 
abRctDemo::getMethodParams(LPCWSTR pObjectPath,
	LPCWSTR pMethodName,
	IWbemClassObject **ppInParams)
{
	CComPtr<IWbemClassObject> pClass;
	CComPtr<IWbemClassObject> pInParamsDefinition;

	CHK_HRES(m_pWbemServices->GetObject(CComBSTR(pObjectPath), 0, NULL, &pClass, NULL));
	CHK_HRES(pClass->GetMethod(pMethodName, 0, &pInParamsDefinition, NULL));
	CHK_HRES(pInParamsDefinition->SpawnInstance(0, ppInParams));
}

void 
abRctDemo::waitForJobCompletion(IWbemClassObject *pOutParameters)
{
	const DWORD ProgressPeriod = 500;

	CComVariant retVal;

	this->getProperty(L"ReturnValue", &retVal, VT_I4, pOutParameters);

	if (retVal.intVal == NoError)
		return;

	if (retVal.intVal != JobStarted)
		throw CAtlException(HRESULTFromHVStatus((HYPERV_STATUS)retVal.intVal));

	CComVariant jobPath;
	this->getProperty(L"Job", &jobPath, VT_BSTR, pOutParameters);

	CComPtr<IWbemClassObject> pJob;

	while (true)
	{
		pJob.Release();

		CHK_HRES(m_pWbemServices->GetObject(jobPath.bstrVal,
			WBEM_FLAG_RETURN_WBEM_COMPLETE,
			NULL,
			&pJob,
			NULL));

		retVal.Clear();
		this->getProperty(L"JobState", &retVal, VT_I4, pJob);

		if (retVal.intVal != JobRunning && retVal.intVal != JobStarting)
			break;

		::Sleep(ProgressPeriod);
	}

	if (retVal.intVal != JobCompleted)
	{
		retVal.Clear();
		this->getProperty(L"ErrorCode", &retVal, VT_I4, pJob);
		if (retVal.intVal != NoError)
			throw CAtlException(HRESULTFromHVStatus((HYPERV_STATUS)retVal.intVal));
	}
}
#endif

HRESULT 
abRctDemo::getRelevance(PWSTR sAssociatePath, PWSTR sResultClass, IWbemClassObject** ppResultObject) 
{
	CStringW sqlSentence;
	sqlSentence.Format(L"associators of {%s} where ResultClass = %s", sAssociatePath, sResultClass);
	//char sqlSentence[256];
	//sprintf_s(sqlSentence, "ASSOCIATORS OF {%ls} WHERE ResultClass = %ls", sAssociatePath, sResultClass);

	CComPtr<IEnumWbemClassObject> pEnumOb;

	HRESULT hr = m_pWbemServices->ExecQuery(
		BSTR(L"WQL"),
		CComBSTR(sqlSentence),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumOb);

	ULONG uReturn = 0;
	CComPtr<IWbemClassObject> pObject;
	hr = pEnumOb->Next(WBEM_INFINITE, 1, &pObject, &uReturn);
	return hr;
}
void
abRctDemo::retrieveChangeBlocks(PCWSTR virtualDiskPath)
{
	cout << "##Step 3: Retrieve change blocks..." << endl;
	cout << "-----------------------------------------------------------" << endl;
	HANDLE vhdHandle = NULL;
	VIRTUAL_STORAGE_TYPE storageType = {};
	ULONG  openStatus = 0L;

	//
	// Specify UNKNOWN for both device and vendor so the system will use the
	// file extension to determine the correct VHD format.
	//

	storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_VHDX;
	//storageType.DeviceId = VIRTUAL_STORAGE_TYPE_DEVICE_UNKNOWN;
	//storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_UNKNOWN;
	storageType.VendorId = VIRTUAL_STORAGE_TYPE_VENDOR_MICROSOFT;

	//storageType.VendorId = { 0xec984aec, 0xa0f9, 0x47e9,{ 0x90, 0x1f, 0x71, 0x41, 0x5a, 0x66, 0x34, 0x5b } };

	//virtualDiskPath = L"D:\\Hyper-V\\CentOS7\\Virtual Hard Disks\\CentOS7.vhdx";
	//virtualDiskPath = L"D:\\Hyper-V\\linux-8g机器\\Virtual Hard Disks\\linux-8g机器.vhdx";


#if 1
	OPEN_VIRTUAL_DISK_PARAMETERS* pOpenParameter = NULL;

	// OpenVirtualDisk
	openStatus = OpenVirtualDisk(
		&storageType,
		virtualDiskPath,
		//VIRTUAL_DISK_ACCESS_NONE,
		//VIRTUAL_DISK_ACCESS_GET_INFO,
		VIRTUAL_DISK_ACCESS_ALL,
		//VIRTUAL_DISK_ACCESS_READ,
		//VIRTUAL_DISK_ACCESS_WRITABLE,
		OPEN_VIRTUAL_DISK_FLAG_NO_PARENTS,
		//OPEN_VIRTUAL_DISK_FLAG_NONE,
		pOpenParameter,
		&vhdHandle
	);

	if (openStatus != ERROR_SUCCESS)
	{
		wprintf(L"OpenVirtualDisk failed! Disk path: %s\n", virtualDiskPath);
		wprintf(L"Erorr code: %ld\n", openStatus);
		getchar();
		return;
	}
	cout << "Succeeded to call method(OpenVirtualDisk)." << endl;
#endif
#if 1
	//
	//The buffer passed for GET_VIRTUAL_DISK_INFO_CHANGE_TRACKING_STATE
	//is small which could be causing the API to crash.
	//bufferSize = sizeof(GET_VIRTUAL_DISK_INFO) + RCT Id length.
	//On an earlier side you could also try something similar as below.RCTID is generally 50 chars.
	//
	DWORD sizeUsed = 0;
	WCHAR changeTrackingInfo[2048];
	::ZeroMemory(changeTrackingInfo, sizeof(changeTrackingInfo));
	PGET_VIRTUAL_DISK_INFO virtualDiskInfo = (GET_VIRTUAL_DISK_INFO *)changeTrackingInfo;
	virtualDiskInfo->Version = (GET_VIRTUAL_DISK_INFO_VERSION)GET_VIRTUAL_DISK_INFO_CHANGE_TRACKING_STATE;
	ULONG virtualDiskInfoSize = sizeof(changeTrackingInfo);
	openStatus = GetVirtualDiskInformation(
		vhdHandle,
		&virtualDiskInfoSize,
		virtualDiskInfo,
		&sizeUsed);
	if (openStatus != ERROR_SUCCESS)
	{
		wprintf(L"Get virtual disk infomation failed! Erorr code: %ld\n", openStatus);
		getchar();
		return;
	}

	printf("Disk Infomation:\n");
	GUID virtualDiskGuid = virtualDiskInfo->Identifier;
	wchar_t szGUID[64] = { 0 };
	StringFromGUID2(virtualDiskGuid, szGUID, 64);
	wprintf(L"   ChangeTrackingState.Enabled:%d\n", virtualDiskInfo->ChangeTrackingState.Enabled);
	wprintf(L"   ChangeTrackingState.MostRecentId:%s\n", virtualDiskInfo->ChangeTrackingState.MostRecentId);
	wprintf(L"   Identifier   = %s\n", szGUID);
	wprintf(L"   physicalSize = %I64u\n", virtualDiskInfo->Size.PhysicalSize);
	wprintf(L"   virtualSize  = %I64u\n", virtualDiskInfo->Size.VirtualSize);
	wprintf(L"   sectorSize   = %u\n", virtualDiskInfo->Size.SectorSize);
	wprintf(L"   blockSize    = %u\n", virtualDiskInfo->Size.BlockSize);

#endif
#if 1
	// QueryChangesVirtualDisk
	cout << "Query the changes of VirtualDisk..." << endl;
	PCWSTR    changeTrackingId = virtualDiskInfo->ChangeTrackingState.MostRecentId;
	ULONG64   byteOffset = 0L;
	ULONG64   byteLength = virtualDiskInfoSize;
	PQUERY_CHANGES_VIRTUAL_DISK_RANGE pQueryChangeRange = NULL;
	ULONG     rangeCount = 0L;
	ULONG64	  processedLength = 0L;
	openStatus = QueryChangesVirtualDisk(
		vhdHandle,							  // A handle to the open VHD
		changeTrackingId,					  // A pointer to a string that specifies the change tracking identifier
		byteOffset,						      // Specifies the distance from the start of the VHD to the beginning of the area of the VHD
		byteLength,							  // Specifies the length of the area of the VHD that you want to check for changes
		QUERY_CHANGES_VIRTUAL_DISK_FLAG_NONE, // Reserved
		pQueryChangeRange,					  // Indicates the areas of the virtual disk that have changed
		&rangeCount,						  // The number of QUERY_CHANGES_VIRTUAL_DISK_RANGE structures that the array that the Ranges parameter points to can hold
		&processedLength					  // Indicates the total number of bytes that the method processed
	);

	if (openStatus != ERROR_SUCCESS)
	{
		wprintf(L"Failed to call method(QueryChangesVirtualDisk), Erorr code: %ld\n", openStatus);
		wprintf(L"Virtual disk path: %s\n", virtualDiskPath);
		wprintf(L"%s\n", changeTrackingId);
		wprintf(L"Start offset: %llu\n", byteOffset);
		wprintf(L"End offset: %lu\n", virtualDiskInfoSize);
		getchar();
		return;
	}
	wprintf(L"The number of changes:%ld\n", rangeCount);
	wprintf(L"The total number of processing bytes:%I64u\n", processedLength);
	if (pQueryChangeRange)
	{
		wprintf(L"The begin offset:%I64u\n", pQueryChangeRange->ByteOffset);
		wprintf(L"The checked length:%I64u\n", pQueryChangeRange->ByteLength);
	}
#endif

}