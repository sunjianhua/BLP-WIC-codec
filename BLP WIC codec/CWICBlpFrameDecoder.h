// CWICBlpFrameDecoder.h : Declaration of the CWICBlpFrameDecoder

#pragma once
#include "resource.h"       // main symbols

#include "stdafx.h"

#include "BLPFormat.h"

#include "CWICBlpMetadataReader.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif

using namespace ATL;


typedef class WICBlpFrameDecoder WICBlpFrameDecoder;
class DECLSPEC_UUID("3843B368-937A-4B77-B328-1C868A163495") WICBlpFrameDecoder;

// {3843B368-937A-4B77-B328-1C868A163495}
DEFINE_GUID(CLSID_WICBlpFrameDecoder,
	0x3843b368, 0x937a, 0x4b77, 0xb3, 0x28, 0x1c, 0x86, 0x8a, 0x16, 0x34, 0x95);

// {707487DA-7D04-4982-9812-47AA3F65BF74}
DEFINE_GUID(GUID_ContainerFormatBlp,
	0x707487da, 0x7d04, 0x4982, 0x98, 0x12, 0x47, 0xaa, 0x3f, 0x65, 0xbf, 0x74);

/*// {6E86F4BD-302B-4513-BB12-F9B175D7577F}
DEFINE_GUID(<< name >> ,
	0x6e86f4bd, 0x302b, 0x4513, 0xbb, 0x12, 0xf9, 0xb1, 0x75, 0xd7, 0x57, 0x7f);*/


MIDL_INTERFACE("6E86F4BD-302B-4513-BB12-F9B175D7577F")
IInternalBlpFrameDecoder : public IUnknown
{
public:
	virtual HRESULT STDMETHODCALLTYPE Initialize(IWICImagingFactory* pWICImagingFactory,
		IWICComponentFactory* pWICComponentFactory,
		IStream* pStream) = 0;

};

// CWICBlpFrameDecoder
class ATL_NO_VTABLE CWICBlpFrameDecoder :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWICBlpFrameDecoder, &CLSID_WICBlpFrameDecoder>,
	public IWICBitmapFrameDecode,
	public IInternalBlpFrameDecoder,
	public IWICMetadataBlockReader
{
private:
	CComPtr<IWICImagingFactory>		m_pWICImagingFactory;
	CComPtr<IWICComponentFactory>	m_pWICComponentFactory;
	CComPtr<IStream>				m_pStream;
	
	CComPtr<IUnknown>				m_pMetadataReader;

	BLP_header m_hdr;

	bool m_isUncompressed = false;
	std::vector<UINT8> m_data;

	HRESULT LoadA8R8G8B8();
	HRESULT LoadUncompressed();
	HRESULT LoadDXTC();
public:
	CWICBlpFrameDecoder()
	{
	}

	STDMETHOD(Initialize)(IWICImagingFactory* pWICImagingFactory,
		IWICComponentFactory* pWICComponentFactory,
		IStream* pStream)
	{
		m_pWICImagingFactory = pWICImagingFactory;
		m_pWICComponentFactory = pWICComponentFactory;
		m_pStream = pStream;

		m_pStream->Seek({ 0 }, STREAM_SEEK_SET, nullptr);

		ULONG cbRead = 0;
		m_pStream->Read(&m_hdr, sizeof(m_hdr), &cbRead);

		/*CComPtr<IInternalBlpMetadataReader> pInternalBlpMetadataReader;
		HRESULT hr = CWICBlpMetadataReader::CreateInstance(&pInternalBlpMetadataReader);
		if (FAILED(hr))
			return hr;

		hr = pInternalBlpMetadataReader->Init(m_pStream);

		pInternalBlpMetadataReader->QueryInterface(&m_pMetadataReader);*/

		return S_OK;
	}

	DECLARE_REGISTRY_RESOURCEID(IDR_WICBLPFRAMEDECODER)

	DECLARE_NOT_AGGREGATABLE(CWICBlpFrameDecoder)

	BEGIN_COM_MAP(CWICBlpFrameDecoder)
		COM_INTERFACE_ENTRY(IWICBitmapSource)
		COM_INTERFACE_ENTRY(IWICBitmapFrameDecode)
		COM_INTERFACE_ENTRY(IInternalBlpFrameDecoder)
		COM_INTERFACE_ENTRY(IWICMetadataBlockReader)
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

	STDMETHOD(GetMetadataQueryReader)(
		/* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIMetadataQueryReader);

	STDMETHOD(GetColorContexts)(
		/* [in] */ UINT cCount,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cCount) IWICColorContext **ppIColorContexts,
		/* [out] */ __RPC__out UINT *pcActualCount);

	STDMETHOD(GetThumbnail)(
		/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIThumbnail);

	STDMETHOD(GetSize)(
		/* [out] */ __RPC__out UINT *puiWidth,
		/* [out] */ __RPC__out UINT *puiHeight);

	STDMETHOD(GetPixelFormat)(
		/* [out] */ __RPC__out WICPixelFormatGUID *pPixelFormat);

	STDMETHOD(GetResolution)(
		/* [out] */ __RPC__out double *pDpiX,
		/* [out] */ __RPC__out double *pDpiY);

	STDMETHOD(CopyPalette)(
		/* [in] */ __RPC__in_opt IWICPalette *pIPalette);

	STDMETHOD(CopyPixels)(
		/* [unique][in] */ __RPC__in_opt const WICRect *prc,
		/* [in] */ UINT cbStride,
		/* [in] */ UINT cbBufferSize,
		/* [size_is][out] */ __RPC__out_ecount_full(cbBufferSize) BYTE *pbBuffer);

	STDMETHOD(GetContainerFormat)(
		/* [out] */ __RPC__out GUID *pguidContainerFormat);

	STDMETHOD(GetCount)(
		/* [out] */ __RPC__out UINT *pcCount);

	STDMETHOD(GetReaderByIndex)(
		/* [in] */ UINT nIndex,
		/* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIMetadataReader);

	STDMETHOD(GetEnumerator)(
		/* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumMetadata);

};

OBJECT_ENTRY_AUTO(__uuidof(WICBlpFrameDecoder), CWICBlpFrameDecoder)
