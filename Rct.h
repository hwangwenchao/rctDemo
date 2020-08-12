#ifndef __RCT_H__
#define __RCT_H__

#include <windows.h>
#include <wbemidl.h>
#include <istream>
#include <string>


#pragma comment(lib, "wbemuuid.lib")

class Rct
{
public:
	Rct();
	~Rct();
	void initComComponent();
	void createComInstance();
	void connectWmiNamespace();
	void createSnapshot(LPCWSTR vmName);
	void convertCheckpoint();
#if 0
	void getProperty(PCWSTR pPropertyName,
		LPVARIANT pValue,
		UINT type,
		IWbemClassObject *pObject);
	CComBSTR GetStrProperty(PCWSTR pPropertyName, IWbemClassObject *pObject = NULL);
	void getMethodParams(LPCWSTR pObjectPath,
		LPCWSTR pMethodName,
		IWbemClassObject **ppInParams);
	void waitForJobCompletion(IWbemClassObject *pOutParameters);
#endif
	HRESULT getRelevance(PWSTR sAssociatePath, 
		PWSTR sResultClass, 
		IWbemClassObject** ppResultObject);
	void retrieveChangeBlocks(PCWSTR virtualDiskPath);
	void releaseResource();
	IEnumWbemClassObject* doExecQuery(std::string sql);
	void releaseCom();
private:

	IWbemLocator*     m_pWbemLocator;
	IWbemServices*	  m_pWbemServices;
	IWbemClassObject* m_pWbemClassObject;

};
#endif // ! __RCT_H__