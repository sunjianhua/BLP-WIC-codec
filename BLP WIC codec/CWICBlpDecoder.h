// CWICBlpDecoder.h : Declaration of the CWICBlpDecoder

#pragma once
#include "resource.h"       // main symbols

#include "stdafx.h"

#include <fstream>

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;

typedef class WICBlpDecoder WICBlpDecoder;
class DECLSPEC_UUID("9F6C2DAB-CF43-4CFA-B1D4-51B35287BEB3") WICBlpDecoder;

// {9F6C2DAB-CF43-4CFA-B1D4-51B35287BEB3}
DEFINE_GUID(CLSID_WICBlpDecoder,
	0x9f6c2dab, 0xcf43, 0x4cfa, 0xb1, 0xd4, 0x51, 0xb3, 0x52, 0x87, 0xbe, 0xb3);

// CWICBlpDecoder
class ATL_NO_VTABLE CWICBlpDecoder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWICBlpDecoder, &CLSID_WICBlpDecoder>,
	public IWICBitmapDecoder
{
private:
	CComPtr<IWICImagingFactory>		m_pWICImagingFactory;
	CComPtr<IWICComponentFactory>	m_pWICComponentFactory;
	CComPtr<IStream>				m_pStream;
public:
	CWICBlpDecoder()
	{
		HRESULT hr = m_pWICImagingFactory.CoCreateInstance(CLSID_WICImagingFactory);
		hr = m_pWICImagingFactory.QueryInterface(&m_pWICComponentFactory);
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_WICBLPDECODER)

	DECLARE_NOT_AGGREGATABLE(CWICBlpDecoder)

	BEGIN_COM_MAP(CWICBlpDecoder)
		COM_INTERFACE_ENTRY(IWICBitmapDecoder)
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
	
	STDMETHOD(QueryCapability)(
		/* [in] */ __RPC__in_opt IStream *pIStream,
		/* [out] */ __RPC__out DWORD *pdwCapability);

	STDMETHOD(Initialize)(
		/* [in] */ __RPC__in_opt IStream *pIStream,
		/* [in] */ WICDecodeOptions cacheOptions);

	STDMETHOD(GetContainerFormat)(
		/* [out] */ __RPC__out GUID *pguidContainerFormat);

	STDMETHOD(GetDecoderInfo)(
		/* [out] */ __RPC__deref_out_opt IWICBitmapDecoderInfo **ppIDecoderInfo);

	STDMETHOD(CopyPalette)(
		/* [in] */ __RPC__in_opt IWICPalette *pIPalette);

	STDMETHOD(GetMetadataQueryReader)(
		/* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIMetadataQueryReader);

	STDMETHOD(GetPreview)(
		/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIBitmapSource);

	STDMETHOD(GetColorContexts)(
		/* [in] */ UINT cCount,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cCount) IWICColorContext **ppIColorContexts,
		/* [out] */ __RPC__out UINT *pcActualCount);

	STDMETHOD(GetThumbnail)(
		/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIThumbnail);

	STDMETHOD(GetFrameCount)(
		/* [out] */ __RPC__out UINT *pCount);

	STDMETHOD(GetFrame)(
		/* [in] */ UINT index,
		/* [out] */ __RPC__deref_out_opt IWICBitmapFrameDecode **ppIBitmapFrame);

};

OBJECT_ENTRY_AUTO(__uuidof(WICBlpDecoder), CWICBlpDecoder)
