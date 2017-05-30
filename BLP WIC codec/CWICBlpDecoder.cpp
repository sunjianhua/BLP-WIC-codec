#include "stdafx.h"
#include "CWICBlpDecoder.h"

#include "BLPFormat.h"

#include "CWICBlpFrameDecoder.h"

STDMETHODIMP CWICBlpDecoder::QueryCapability(
	/* [in] */ __RPC__in_opt IStream *pIStream,
	/* [out] */ __RPC__out DWORD *pdwCapability)
{
	*pdwCapability = 0;

	if (pIStream == nullptr)
		return E_INVALIDARG;

	//	Save current stream position
	LARGE_INTEGER seekPos = { 0 };
	ULARGE_INTEGER startPos;

	if (FAILED(pIStream->Seek(seekPos, STREAM_SEEK_CUR, &startPos)))
		return E_FAIL;

	//	Read BLP header
	BLP_header hdr;
	if (FAILED(pIStream->Seek(seekPos, STREAM_SEEK_SET, nullptr)))
		return E_FAIL;

	ULONG cbRead = 0;
	pIStream->Read(&hdr, sizeof(BLP_header), &cbRead);
	
	//	Check header compression / alpha depth
	if ((cbRead == sizeof(BLP_header)) && (hdr.Magic == '2PLB'))
	{
		switch (hdr.Compression)
		{
		case 1:
			//	Uncompressed
			switch (hdr.AlphaDepth)
			{
			case 0:
			case 1:
			case 4:
			case 8:
			case 72:
				*pdwCapability = WICBitmapDecoderCapabilityCanDecodeAllImages;
				break;
			default:
				*pdwCapability = 0;
				break;
			}
			break;
		case 2:
			//	DXTC
			switch (hdr.AlphaDepth)
			{
			case 0:
			case 1:
			case 4:
			case 8:
			case 72:
				*pdwCapability = WICBitmapDecoderCapabilityCanDecodeAllImages;
				break;
			default:
				*pdwCapability = 0;
				break;
			}
			break;
		case 3:
			//	A8R8G8B8
			*pdwCapability = WICBitmapDecoderCapabilityCanDecodeAllImages;
			break;
		default:
			*pdwCapability = 0;
			break;
		}
	}
	else
	{
		*pdwCapability = 0;
	}

	//	Revert stream position
	LARGE_INTEGER revertPos;
	revertPos.QuadPart = startPos.QuadPart;

	if (FAILED(pIStream->Seek(revertPos, STREAM_SEEK_SET, nullptr)))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWICBlpDecoder::Initialize(
	/* [in] */ __RPC__in_opt IStream *pIStream,
	/* [in] */ WICDecodeOptions cacheOptions)
{
	m_pStream = pIStream;
	return S_OK;
}

STDMETHODIMP CWICBlpDecoder::GetContainerFormat(
	/* [out] */ __RPC__out GUID *pguidContainerFormat)
{
	*pguidContainerFormat = GUID_ContainerFormatBlp;
	return S_OK;
}

STDMETHODIMP CWICBlpDecoder::GetDecoderInfo(
	/* [out] */ __RPC__deref_out_opt IWICBitmapDecoderInfo **ppIDecoderInfo)
{
	CComPtr<IWICComponentInfo> pComponentInfo;
	HRESULT hr;

	hr = m_pWICImagingFactory->CreateComponentInfo(CLSID_WICBlpDecoder, &pComponentInfo);
	if (FAILED(hr))
		return E_FAIL;

	hr = pComponentInfo->QueryInterface(IID_IWICBitmapDecoderInfo, (void**)ppIDecoderInfo);
	if (FAILED(hr))
		return E_FAIL;

	return S_OK;
}

STDMETHODIMP CWICBlpDecoder::CopyPalette(
	/* [in] */ __RPC__in_opt IWICPalette *pIPalette)
{
	return WINCODEC_ERR_PALETTEUNAVAILABLE;
}

STDMETHODIMP CWICBlpDecoder::GetMetadataQueryReader(
	/* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIMetadataQueryReader)
{
	return E_NOTIMPL;
}

STDMETHODIMP CWICBlpDecoder::GetPreview(
	/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIBitmapSource)
{
	CComPtr<IWICBitmapFrameDecode> pFrameDecode;

	HRESULT hr = GetFrame(0, &pFrameDecode);

	if (pFrameDecode)
		(*ppIBitmapSource = pFrameDecode)->AddRef();
	else
		*ppIBitmapSource = nullptr;

	return hr;
}

STDMETHODIMP CWICBlpDecoder::GetColorContexts(
	/* [in] */ UINT cCount,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cCount) IWICColorContext **ppIColorContexts,
	/* [out] */ __RPC__out UINT *pcActualCount)
{
	return E_NOTIMPL;
}

STDMETHODIMP CWICBlpDecoder::GetThumbnail(
	/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIThumbnail)
{
	CComPtr<IWICBitmapFrameDecode> pFrameDecode;
	
	HRESULT hr = GetFrame(0, &pFrameDecode);

	if (pFrameDecode)
		(*ppIThumbnail = pFrameDecode)->AddRef();
	else
		*ppIThumbnail = nullptr;

	return hr;
}

STDMETHODIMP CWICBlpDecoder::GetFrameCount(
	/* [out] */ __RPC__out UINT *pCount)
{
	*pCount = 1;
	return S_OK;
}

STDMETHODIMP CWICBlpDecoder::GetFrame(
	/* [in] */ UINT index,
	/* [out] */ __RPC__deref_out_opt IWICBitmapFrameDecode **ppIBitmapFrame)
{
	*ppIBitmapFrame = nullptr;

	if (index > 0)
		return E_INVALIDARG;

	CComPtr<IInternalBlpFrameDecoder> pInternalBLPFrameDecoder;
	HRESULT hr = CWICBlpFrameDecoder::CreateInstance(&pInternalBLPFrameDecoder);
	if (FAILED(hr))
		return hr;

	hr = pInternalBLPFrameDecoder->Initialize(m_pWICImagingFactory, m_pWICComponentFactory, m_pStream);
	if (FAILED(hr))
		return hr;

	hr = pInternalBLPFrameDecoder.QueryInterface(ppIBitmapFrame);

	return hr;
	//*ppIBitmapFrame = new CWICBlpFrameDecoder(m_pWICImagingFactory, m_pStream);
}