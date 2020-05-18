#ifndef __AB_RCT_COMMON_H__
#define __AB_RCT_COMMON_H__


#define CHK_HRES(op) \
    { \
        HRESULT tmp_hresult___ = op; \
        if (FAILED(tmp_hresult___)) \
            throw CAtlException(tmp_hresult___); \
    }

enum HYPERV_STATUS
{
	NoError = 0,
	JobStarted = 0x1000,
	Failed = 0x8000,
	AccessDenied = 0x8001,
	NotSupported = 0x8002,
	UnknownStatus = 0x8003,
	Timeout = 0x8004,
	InvalidParameter = 0x8005,
	SystemIsInUse = 0x8006,
	InvalidState = 0x8007,
	IncorrectDataType = 0x8008,
	SystemIsNotAvailable = 0x8009,
	OutOfMemory = 0x800A
};

enum JOB_STATE
{
	JobNew = 2,
	JobStarting = 3,
	JobRunning = 4,
	JobSuspended = 5,
	JobShuttingDown = 6,
	JobCompleted = 7,
	JobTerminated = 8,
	JobKilled = 9,
	JobException = 10,
	JobService = 11
};

HRESULT HRESULTFromHVStatus(HYPERV_STATUS status)
{
	switch (status)
	{
	case NoError:
	case JobStarted:
		return S_OK;
	case AccessDenied:
		return E_ACCESSDENIED;
	case NotSupported:
		return E_NOTIMPL;
	case InvalidParameter:
		return E_INVALIDARG;
	case OutOfMemory:
		return E_OUTOFMEMORY;
	default:
		return E_FAIL;
	}
}
#endif // !__AB_RCT_COMMON_H__


