#include "stdafx.h"

#include "CWICBlpFrameDecoder.h"

#include "../../squish-1.11/squish.h"

#include <fstream>

#define LOG_STREAM std::ofstream("C:\\Users\\Myself\\Desktop\\BLP WIC codec.log", std::ofstream::out | std::ofstream::app)


STDMETHODIMP CWICBlpFrameDecoder::GetMetadataQueryReader(
	/* [out] */ __RPC__deref_out_opt IWICMetadataQueryReader **ppIMetadataQueryReader)
{
	LOG_STREAM << __FUNCTION__ << std::endl;
	HRESULT hr = m_pWICComponentFactory->CreateQueryReaderFromBlockReader(
		this,
		ppIMetadataQueryReader);
		
	return hr;
}

STDMETHODIMP CWICBlpFrameDecoder::GetColorContexts(
	/* [in] */ UINT cCount,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cCount) IWICColorContext **ppIColorContexts,
	/* [out] */ __RPC__out UINT *pcActualCount)
{
	*pcActualCount = 1;
	if (cCount > 0)
	{
		ppIColorContexts[0]->InitializeFromExifColorSpace(1);
	}
	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::GetThumbnail(
	/* [out] */ __RPC__deref_out_opt IWICBitmapSource **ppIThumbnail)
{
	return WINCODEC_ERR_CODECNOTHUMBNAIL;
}

STDMETHODIMP CWICBlpFrameDecoder::GetSize(
	/* [out] */ __RPC__out UINT *puiWidth,
	/* [out] */ __RPC__out UINT *puiHeight)
{
	*puiWidth = m_hdr.Width;
	*puiHeight = m_hdr.Height;

	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::GetPixelFormat(
	/* [out] */ __RPC__out WICPixelFormatGUID *pPixelFormat)
{
	*pPixelFormat = GUID_WICPixelFormat32bppRGBA;
	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::GetResolution(
	/* [out] */ __RPC__out double *pDpiX,
	/* [out] */ __RPC__out double *pDpiY)
{
	*pDpiX = 96;
	*pDpiY = 96;

	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::CopyPalette(
	/* [in] */ __RPC__in_opt IWICPalette *pIPalette)
{
	return WINCODEC_ERR_PALETTEUNAVAILABLE;
}

HRESULT CWICBlpFrameDecoder::LoadA8R8G8B8()
{
	std::vector<UINT8> buffer(m_hdr.MipmapSize[0]);

	LARGE_INTEGER li_offset;
	li_offset.QuadPart = m_hdr.MipmapOfs[0];
	m_pStream->Seek(li_offset, STREAM_SEEK_SET, nullptr);

	ULONG cbRead = 0;
	m_pStream->Read(&buffer[0], static_cast<ULONG>(buffer.size()), &cbRead);

	for (unsigned int i = 0; i < m_hdr.Width * m_hdr.Height; ++i)
	{
		//	A8R8G8B8 => R8G8B8A8
		m_data[i * 4] = buffer[i * 4 + 1];
		m_data[i * 4 + 1] = buffer[i * 4 + 2];
		m_data[i * 4 + 2] = buffer[i * 4 + 3];
		m_data[i * 4 + 3] = buffer[i * 4 + 0];
	}

	return S_OK;
}
HRESULT CWICBlpFrameDecoder::LoadUncompressed()
{
	std::vector<UINT8> buffer(m_hdr.MipmapSize[0]);

	//	Read data
	LARGE_INTEGER li_offset;
	li_offset.QuadPart = m_hdr.MipmapOfs[0];
	m_pStream->Seek(li_offset, STREAM_SEEK_SET, nullptr);

	ULONG cbRead = 0;
	m_pStream->Read(&buffer[0], static_cast<ULONG>(buffer.size()), &cbRead);

	//	Read palette
	UINT8 palette[256 * 4];

	LARGE_INTEGER li_paletteOffset;
	li_paletteOffset.QuadPart = sizeof(BLP_header);
	m_pStream->Seek(li_paletteOffset, STREAM_SEEK_SET, nullptr);

	m_pStream->Read(palette, 256 * 4, &cbRead);
	
	//	Uncompress
	switch (m_hdr.AlphaDepth)
	{
	case 0:
		for (unsigned int i = 0; i < m_hdr.Width * m_hdr.Height; ++i)
		{
			m_data[i * 4] = palette[buffer[i] * 4 + 2];
			m_data[i * 4 + 1] = palette[buffer[i] * 4 + 1];
			m_data[i * 4 + 2] = palette[buffer[i] * 4];
			m_data[i * 4 + 3] = 255;
		}
		break;
	case 1:
	{
		UINT alphaOfs = m_hdr.Width * m_hdr.Height;
		for (unsigned int i = 0; i < m_hdr.Width * m_hdr.Height; ++i)
		{
			m_data[i * 4] = palette[buffer[i] * 4 + 2];
			m_data[i * 4 + 1] = palette[buffer[i] * 4 + 1];
			m_data[i * 4 + 2] = palette[buffer[i] * 4];

			UINT iAlpha = i / 8;
			UINT bitAlpha = i % 8;

			if ((buffer[alphaOfs + iAlpha] >> bitAlpha) & 0x1)
				m_data[i * 4 + 3] = 255;
			else
				m_data[i * 4 + 3] = 0;
		}
		break;
	}
	case 4:
		for (unsigned int i = 0; i < m_hdr.Width * m_hdr.Height; ++i)
		{
			m_data[i * 4] = palette[buffer[i] * 4 + 2];
			m_data[i * 4 + 1] = palette[buffer[i] * 4 + 1];
			m_data[i * 4 + 2] = palette[buffer[i] * 4];
			m_data[i * 4 + 3] = 255;	//	broken
		}
		break;
	case 72:
	case 8:
	{
		int alphaOfs = m_hdr.Width * m_hdr.Height;
		for (unsigned int i = 0; i < m_hdr.Width * m_hdr.Height; ++i)
		{
			m_data[i * 4] = palette[buffer[i] * 4 + 2];
			m_data[i * 4 + 1] = palette[buffer[i] * 4 + 1];
			m_data[i * 4 + 2] = palette[buffer[i] * 4];
			m_data[i * 4 + 3] = buffer[alphaOfs + i];
		}
		break;
	}
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CWICBlpFrameDecoder::LoadDXTC()
{
	//	Check width and height are multiple of 4
	if ((m_hdr.Width & 0x3) || (m_hdr.Height & 0x3))
		return E_FAIL;

	std::vector<UINT8> buffer(m_hdr.MipmapSize[0]);

	LARGE_INTEGER li_offset;
	li_offset.QuadPart = m_hdr.MipmapOfs[0];
	m_pStream->Seek(li_offset, STREAM_SEEK_SET, nullptr);

	ULONG cbRead = 0;
	m_pStream->Read(&buffer[0], static_cast<ULONG>(buffer.size()), &cbRead);

	UINT nYBlocks = m_hdr.Height / 4;
	UINT nXBlocks = m_hdr.Width / 4;

	switch (m_hdr.CompressionMode)
	{
	case BLP_COMPRESSION_MODE_DXT1:
		//	BC1
		for (unsigned int y = 0; y < nYBlocks; ++y)
		{
			for (unsigned int x = 0; x < nXBlocks; ++x)
			{
				UINT bufferOfs = (y * nXBlocks + x) * 8;

				UINT8 uncompressedBlock[16 * 4];
				squish::Decompress(uncompressedBlock, &buffer[bufferOfs], squish::kDxt1);

				for (int i = 0; i < 4; ++i)
				{
					UINT lineSizeInBytes = nXBlocks * 4 * 4;
					UINT dataOfs = (y * 4 + i) * lineSizeInBytes + x * 4 * 4;
					memcpy(&m_data[dataOfs], &uncompressedBlock[i * 4 * 4], 16);
				}
			}
		}
		break;
		//	BC1
	case BLP_COMPRESSION_MODE_DXT3:
		//	BC2
		for (unsigned int y = 0; y < nYBlocks; ++y)
		{
			for (unsigned int x = 0; x < nXBlocks; ++x)
			{
				UINT bufferOfs = (y * nXBlocks + x) * 16;

				UINT8 uncompressedBlock[16 * 4];
				squish::Decompress(uncompressedBlock, &buffer[bufferOfs], squish::kDxt3);

				for (int i = 0; i < 4; ++i)
				{
					UINT lineSizeInBytes = nXBlocks * 4 * 4;
					UINT dataOfs = (y * 4 + i) * lineSizeInBytes + x * 4 * 4;

					memcpy(&m_data[dataOfs], &uncompressedBlock[i * 4 * 4], 16);
				}
			}
		}
		break;
	case BLP_COMPRESSION_MODE_DXT5:
		//	BC2
		for (unsigned int y = 0; y < nYBlocks; ++y)
		{
			for (unsigned int x = 0; x < nXBlocks; ++x)
			{
				UINT bufferOfs = (y * nXBlocks + x) * 16;

				UINT8 uncompressedBlock[16 * 4];
				squish::Decompress(uncompressedBlock, &buffer[bufferOfs], squish::kDxt5);

				for (int i = 0; i < 4; ++i)
				{
					UINT lineSizeInBytes = nXBlocks * 4 * 4;
					UINT dataOfs = (y * 4 + i) * lineSizeInBytes + x * 4 * 4;

					memcpy(&m_data[dataOfs], &uncompressedBlock[i * 4 * 4], 16);
				}
			}
		}
		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}
STDMETHODIMP CWICBlpFrameDecoder::CopyPixels(
	/* [unique][in] */ __RPC__in_opt const WICRect *prc,
	/* [in] */ UINT cbStride,
	/* [in] */ UINT cbBufferSize,
	/* [size_is][out] */ __RPC__out_ecount_full(cbBufferSize) BYTE *pbBuffer)
{
	if (prc->Y + prc->Height > static_cast<INT>(m_hdr.Height))
		return E_INVALIDARG;

	if (prc->X + prc->Width > static_cast<INT>(m_hdr.Width))
		return E_INVALIDARG;

	if (!m_isUncompressed)
	{
		//	Resize buffer
		m_data.resize(m_hdr.Width * m_hdr.Height * 4);

		switch (m_hdr.Compression)
		{
		case BLP_COMPRESSION_UNCOMPRESSED:
			//	Uncompressed
			switch (m_hdr.AlphaDepth)
			{
			case 0:
			case 1:
			case 4:
			case 8:
			case 72:
				if(FAILED(LoadUncompressed()))
					return E_FAIL;
				break;
			default:
				return E_FAIL;
			}
			break;
		case BLP_COMPRESSION_DXTC:
			//	DXTC
			switch (m_hdr.AlphaDepth)
			{
			case 0:
			case 1:
			case 4:
			case 8:
			case 72:
				if (FAILED(LoadDXTC()))
					return E_FAIL;
				break;
			default:
				return E_FAIL;
			}
			break;
		case BLP_COMPRESSION_A8R8G8B8:
			//	A8R8G8B8
			if (FAILED(LoadA8R8G8B8()))
				return E_FAIL;
			break;
		default:
			return E_FAIL;
		}

		m_isUncompressed = true;
	}

	for (int y = 0; y < prc->Height; ++y)
	{
		//	Have to copy line by line cause stride can be different
		UINT dataOffset = (prc->Y + y) * m_hdr.Width * 4 + prc->X * 4;
		memcpy(pbBuffer, &m_data[dataOffset], prc->Width * 4);

		pbBuffer += cbStride;
	}

	
	///!\ TEST
	/*
	UINT8* pLine = pbBuffer;
	for (int y = 0; y < prc->Height; ++y)
	{
		for (int x = 0; x < prc->Width; ++x)
		{
			pLine[x * 4 + 0] = 255;
			pLine[x * 4 + 1] = 0;
			pLine[x * 4 + 2] = 0;
			pLine[x * 4 + 3] = 255;
		}
		pLine += cbStride;
	}*/
	return S_OK;
}


STDMETHODIMP CWICBlpFrameDecoder::GetContainerFormat(
	/* [out] */ __RPC__out GUID *pguidContainerFormat)
{
	*pguidContainerFormat = GUID_ContainerFormatBlp;
	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::GetCount(
	/* [out] */ __RPC__out UINT *pcCount)
{
	*pcCount = 1;
	return S_OK;
}

STDMETHODIMP CWICBlpFrameDecoder::GetReaderByIndex(
	/* [in] */ UINT nIndex,
	/* [out] */ __RPC__deref_out_opt IWICMetadataReader **ppIMetadataReader)
{
	LOG_STREAM << __FUNCTION__ << std::endl;


	CComPtr<IInternalBlpMetadataReader> pInternalBLPMetadataReader;
	HRESULT hr = CWICBlpMetadataReader::CreateInstance(&pInternalBLPMetadataReader);
	if (FAILED(hr))
		return hr;

	hr = pInternalBLPMetadataReader->Init(m_pStream);
	if (FAILED(hr))
		return hr;

	pInternalBLPMetadataReader.QueryInterface(ppIMetadataReader);
	/*HRESULT hr = m_pWICComponentFactory->CreateMetadataReaderFromContainer(
		GUID_ContainerFormatBlp, //GUID_ContainerFormatTiff,
		NULL,
		WICPersistOptions::WICPersistOptionDefault |
		WICMetadataCreationOptions::WICMetadataCreationDefault,
		m_pStream,
		ppIMetadataReader);*/

	//HRESULT hr = E_NOTIMPL;
	return hr;
}

STDMETHODIMP CWICBlpFrameDecoder::GetEnumerator(
	/* [out] */ __RPC__deref_out_opt IEnumUnknown **ppIEnumMetadata)
{
	LOG_STREAM << __FUNCTION__ << std::endl;


	//	Create enum
	typedef CComEnum<IEnumUnknown,
			&_ATL_IIDOF(IEnumUnknown),
			IUnknown*,
			_CopyInterface<IUnknown>>
		EnumType;
	CComPtr<CComObject<EnumType>> pEnum;
	HRESULT hr = CComObject<EnumType>::CreateInstance(&pEnum);
	
	//	pEnum ref count is 0 right here. need to call QueryInterface.
	if (FAILED(hr))
		return hr;

	//	Create metadata reader
	CComPtr<IWICMetadataReader> pMetadataReader;
	hr = m_pWICComponentFactory->CreateMetadataReaderFromContainer(
		GUID_ContainerFormatBlp, //GUID_ContainerFormatTiff,
		NULL,
		WICPersistOptions::WICPersistOptionDefault |
		WICMetadataCreationOptions::WICMetadataCreationDefault,
		m_pStream,
		&pMetadataReader);

	IUnknown* pMetadataReaderUnk = pMetadataReader;
	IUnknown** begin = &pMetadataReaderUnk;	//	faux
	IUnknown** end = begin + 1;

	hr = pEnum->Init(begin, end, nullptr, ATL::CComEnumFlags::AtlFlagCopy);

	//if (SUCCEEDED(hr))
	hr = pEnum->QueryInterface(ppIEnumMetadata);
	
	return hr;
}