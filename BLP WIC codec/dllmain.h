// dllmain.h : Declaration of module class.

class CBLPWICCodecModule : public ATL::CAtlDllModuleT< CBLPWICCodecModule >
{
public:
	//DECLARE_LIBID(LIBID_ATLProject1Lib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_BLPWICCODEC, "{B6D05F0B-46F8-4A6C-A8DE-CD76390A635F}")
};

extern class CBLPWICCodecModule _AtlModule;