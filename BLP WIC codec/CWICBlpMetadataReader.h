// CWICBlpFrameDecoder.h : Declaration of the CWICBlpFrameDecoder

#pragma once
#include "resource.h"       // main symbols

#include "stdafx.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

typedef class WICBlpMetadataReader WICBlpMetadataReader;
class DECLSPEC_UUID("F6F3AD62-2961-4215-AB4D-8C4D74F20D14") WICBlpMetadataReader;

// {F6F3AD62-2961-4215-AB4D-8C4D74F20D14}
DEFINE_GUID(CLSID_WICBlpMetadataReader,
	0xf6f3ad62, 0x2961, 0x4215, 0xab, 0x4d, 0x8c, 0x4d, 0x74, 0xf2, 0xd, 0x14);


// {707487DA-7D04-4982-9812-47AA3F65BF74}
DEFINE_GUID(GUID_ContainerFormatBlp,
	0x707487da, 0x7d04, 0x4982, 0x98, 0x12, 0x47, 0xaa, 0x3f, 0x65, 0xbf, 0x74);

// {95FAAFA8-124C-49EA-9111-F8984C1F5B54}
DEFINE_GUID(CLSID_WICBlpMetadataReader,
0x95faafa8, 0x124c, 0x49ea, 0x91, 0x11, 0xf8, 0x98, 0x4c, 0x1f, 0x5b, 0x54);

// {32A655DF-FAC8-4F57-9F2E-A42FA88E0373}
DEFINE_GUID(GUID_VendorBlizzard,
	0x32a655df, 0xfac8, 0x4f57, 0x9f, 0x2e, 0xa4, 0x2f, 0xa8, 0x8e, 0x3, 0x73);


MIDL_INTERFACE("95FAAFA8-124C-49EA-9111-F8984C1F5B54")
IInternalBlpMetadataReader : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Init(IStream* pStream) = 0;

};

struct BlpMetadataPropVariant
{
	PROPVARIANT	Schema;
	PROPVARIANT	ID;
	PROPVARIANT	Value;
};
// CWICBlpMetadataReader
class ATL_NO_VTABLE CWICBlpMetadataReader :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWICBlpMetadataReader, &CLSID_WICBlpMetadataReader>,
	public IInternalBlpMetadataReader,
	public IWICMetadataReader,
	public IWICMetadataHandlerInfo
{
private:
	std::vector<BlpMetadataPropVariant> m_metadataList;
public:
	CWICBlpMetadataReader()
	{
	}


	DECLARE_REGISTRY_RESOURCEID(IDR_WICBLPMETADATAREADER)

	DECLARE_NOT_AGGREGATABLE(CWICBlpMetadataReader)

	BEGIN_COM_MAP(CWICBlpMetadataReader)
		COM_INTERFACE_ENTRY(IInternalBlpMetadataReader)
		COM_INTERFACE_ENTRY(IWICMetadataReader)
		COM_INTERFACE_ENTRY(IWICMetadataHandlerInfo)
		COM_INTERFACE_ENTRY(IWICComponentInfo)
	END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(Init)(IStream* pStream);



	STDMETHOD(GetMetadataFormat)(
		/* [out] */ __RPC__out GUID *pguidMetadataFormat);

	STDMETHOD(GetMetadataHandlerInfo)(
		/* [out] */ __RPC__deref_out_opt IWICMetadataHandlerInfo **ppIHandler);

	STDMETHOD(GetCount)(
		/* [out] */ __RPC__out UINT *pcCount);

	STDMETHOD(GetValueByIndex)(
		/* [in] */ UINT nIndex,
		/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarSchema,
		/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarId,
		/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);

	STDMETHOD(GetValue)(
		/* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
		/* [in] */ __RPC__in const PROPVARIANT *pvarId,
		/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue);

	STDMETHOD(GetEnumerator)(
		/* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadata);


	//STDMETHOD(IWICMetadataHandlerInfo::GetMetadataFormat)(
	//	/* [out] */ __RPC__out GUID *pguidMetadataFormat);

	STDMETHOD(GetContainerFormats)(
		/* [in] */ UINT cContainerFormats,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(GetDeviceManufacturer)(
		/* [in] */ UINT cchDeviceManufacturer,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(GetDeviceModels)(
		/* [in] */ UINT cchDeviceModels,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(DoesRequireFullStream)(
		/* [out] */ __RPC__out BOOL *pfRequiresFullStream);

	STDMETHOD(DoesSupportPadding)(
		/* [out] */ __RPC__out BOOL *pfSupportsPadding);

	STDMETHOD(DoesRequireFixedSize)(
		/* [out] */ __RPC__out BOOL *pfFixedSize);


	STDMETHOD(GetComponentType)(
		/* [out] */ __RPC__out WICComponentType *pType);

	STDMETHOD(GetCLSID)(
		/* [out] */ __RPC__out CLSID *pclsid);

	STDMETHOD(GetSigningStatus)(
		/* [out] */ __RPC__out DWORD *pStatus);

	STDMETHOD(GetAuthor)(
		/* [in] */ UINT cchAuthor,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchAuthor) WCHAR *wzAuthor,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(GetVendorGUID)(
		/* [out] */ __RPC__out GUID *pguidVendor);

	STDMETHOD(GetVersion)(
		/* [in] */ UINT cchVersion,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchVersion) WCHAR *wzVersion,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(GetSpecVersion)(
		/* [in] */ UINT cchSpecVersion,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchSpecVersion) WCHAR *wzSpecVersion,
		/* [out] */ __RPC__out UINT *pcchActual);

	STDMETHOD(GetFriendlyName)(
		/* [in] */ UINT cchFriendlyName,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchFriendlyName) WCHAR *wzFriendlyName,
		/* [out] */ __RPC__out UINT *pcchActual);

};

OBJECT_ENTRY_AUTO(__uuidof(WICBlpMetadataReader), CWICBlpMetadataReader)
