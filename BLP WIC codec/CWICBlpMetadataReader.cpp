#include "stdafx.h"

#include "CWICBlpMetadataReader.h"
#include "CWICBlpMetadataEnumerator.h"

#include "BLPFormat.h"

#include <fstream>


STDMETHODIMP CWICBlpMetadataReader::Init(IStream* pStream)
{
	BLP_header hdr;

	LARGE_INTEGER ofs = { 0 };
	HRESULT hr = pStream->Seek(ofs, STREAM_SEEK_SET, NULL);
	if (FAILED(hr))
		return hr;

	ULONG cbRead = 0;
	hr = pStream->Read(&hdr, sizeof(BLP_header), &cbRead);
	if (FAILED(hr))
		return hr;

	BlpMetadataPropVariant propVariant;
	hr = InitPropVariantFromString(L"App0", &propVariant.Schema);
	hr = InitPropVariantFromString(L"App0", &propVariant.Schema);
	hr = InitPropVariantFromString(L"App0", &propVariant.Schema);

	

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetMetadataFormat(
	/* [out] */ __RPC__out GUID *pguidMetadataFormat)
{
	*pguidMetadataFormat = GUID_MetadataFormatApp0;

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetMetadataHandlerInfo(
	/* [out] */ __RPC__deref_out_opt IWICMetadataHandlerInfo **ppIHandler)
{	
	return QueryInterface(_ATL_IIDOF(IWICMetadataHandlerInfo), (void**)ppIHandler);
}

STDMETHODIMP CWICBlpMetadataReader::GetCount(
	/* [out] */ __RPC__out UINT *pcCount)
{	
	*pcCount = static_cast<UINT>(m_metadataList.size());
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetValueByIndex(
	/* [in] */ UINT nIndex,
	/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarSchema,
	/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarId,
	/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue)
{	
	if (nIndex > m_metadataList.size())
		return E_FAIL;

	PropVariantCopy(pvarSchema, &m_metadataList[nIndex].Schema);
	PropVariantCopy(pvarId, &m_metadataList[nIndex].ID);
	PropVariantCopy(pvarValue, &m_metadataList[nIndex].Value);
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetValue(
	/* [unique][in] */ __RPC__in_opt const PROPVARIANT *pvarSchema,
	/* [in] */ __RPC__in const PROPVARIANT *pvarId,
	/* [unique][out][in] */ __RPC__inout_opt PROPVARIANT *pvarValue)
{	
	for (auto it : m_metadataList)
	{
		if (!PropVariantCompare(it.Schema, *pvarSchema) &&
			!PropVariantCompare(it.ID, *pvarId))
		{
			return PropVariantCopy(pvarValue, &it.Value);
		}
	}

	return E_FAIL;
}

STDMETHODIMP CWICBlpMetadataReader::GetEnumerator(
	/* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadata)
{	
	CComPtr<CComObject<CWICBlpMetadataEnumerator>> pEnumerator;
	HRESULT hr = CComObject<CWICBlpMetadataEnumerator>::CreateInstance(&pEnumerator);

	if (FAILED(hr))
		return hr;


	hr = pEnumerator->Init(&m_metadataList[0].Schema, &m_metadataList[0].ID, &m_metadataList[0].Value, sizeof(BlpMetadataPropVariant) * 3, static_cast<UINT>(m_metadataList.size()));

	if (SUCCEEDED(hr))
		hr = pEnumerator->QueryInterface(ppIEnumMetadata);
		
	return hr;
}


STDMETHODIMP CWICBlpMetadataReader::GetContainerFormats(
	/* [in] */ UINT cContainerFormats,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cContainerFormats) GUID *pguidContainerFormats,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	if (pguidContainerFormats == NULL || cContainerFormats < 1)
	{
		*pcchActual = 1;
		return S_OK;
	}

	*pguidContainerFormats = GUID_ContainerFormatBlp;
	*pcchActual = 1;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetDeviceManufacturer(
	/* [in] */ UINT cchDeviceManufacturer,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceManufacturer) WCHAR *wzDeviceManufacturer,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	std::wstring manufacturerName = L"";

	*pcchActual = static_cast<UINT>(manufacturerName.length());

	if (wzDeviceManufacturer == NULL)
		return S_OK;

	manufacturerName._Copy_s(wzDeviceManufacturer, cchDeviceManufacturer, manufacturerName.length());
	if (cchDeviceManufacturer > manufacturerName.size())
		wzDeviceManufacturer[manufacturerName.size()] = L'\0';

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetDeviceModels(
	/* [in] */ UINT cchDeviceModels,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchDeviceModels) WCHAR *wzDeviceModels,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	std::wstring deviceModels = L"";

	*pcchActual = static_cast<UINT>(deviceModels.length());

	if (wzDeviceModels == NULL)
		return S_OK;

	deviceModels._Copy_s(wzDeviceModels, cchDeviceModels, deviceModels.length());
	if (cchDeviceModels > deviceModels.size())
		wzDeviceModels[deviceModels.size()] = L'\0';

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::DoesRequireFullStream(
	/* [out] */ __RPC__out BOOL *pfRequiresFullStream)
{	
	*pfRequiresFullStream = TRUE;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::DoesSupportPadding(
	/* [out] */ __RPC__out BOOL *pfSupportsPadding)
{	
	*pfSupportsPadding = FALSE;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::DoesRequireFixedSize(
	/* [out] */ __RPC__out BOOL *pfFixedSize)
{	
	*pfFixedSize = TRUE;
	return S_OK;
}


STDMETHODIMP CWICBlpMetadataReader::GetComponentType(
	/* [out] */ __RPC__out WICComponentType *pType)
{	
	if (pType == nullptr)
		return E_INVALIDARG;

	*pType = WICMetadataReader;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetCLSID(
	/* [out] */ __RPC__out CLSID *pclsid)
{	
	if (pclsid == nullptr)
		return E_INVALIDARG;

	*pclsid = CLSID_WICBlpMetadataReader;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetSigningStatus(
	/* [out] */ __RPC__out DWORD *pStatus)
{	
	if (pStatus == nullptr)
		return E_INVALIDARG;

	*pStatus = WICComponentSigned;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetAuthor(
	/* [in] */ UINT cchAuthor,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchAuthor) WCHAR *wzAuthor,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	std::wstring author = L"MyCompany Inc.";

	*pcchActual = static_cast<UINT>(author.length());

	if (wzAuthor == NULL)
		return S_OK;

	author._Copy_s(wzAuthor, cchAuthor, author.length());
	if (cchAuthor > author.size())
		wzAuthor[author.size()] = L'\0';

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetVendorGUID(
	/* [out] */ __RPC__out GUID *pguidVendor)
{	
	if (pguidVendor == nullptr)
		return E_INVALIDARG;

	*pguidVendor = GUID_VendorBlizzard;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetVersion(
	/* [in] */ UINT cchVersion,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchVersion) WCHAR *wzVersion,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	std::wstring version = L"1.0.0.0";

	*pcchActual = static_cast<UINT>(version.length());

	if (wzVersion == NULL)
		return S_OK;

	version._Copy_s(wzVersion, cchVersion, version.length());
	if (cchVersion > version.size())
		wzVersion[version.size()] = L'\0';

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetSpecVersion(
	/* [in] */ UINT cchSpecVersion,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchSpecVersion) WCHAR *wzSpecVersion,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	std::wstring specVersion = L"1.0.0.0";

	*pcchActual = static_cast<UINT>(specVersion.length());

	if (wzSpecVersion == NULL)
		return S_OK;

	specVersion._Copy_s(wzSpecVersion, cchSpecVersion, specVersion.length());
	if (cchSpecVersion > specVersion.size())
		wzSpecVersion[specVersion.size()] = L'\0';

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataReader::GetFriendlyName(
	/* [in] */ UINT cchFriendlyName,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(cchFriendlyName) WCHAR *wzFriendlyName,
	/* [out] */ __RPC__out UINT *pcchActual)
{	
	if (pcchActual == nullptr)
		return E_INVALIDARG;

	std::wstring friendlyName = L"Blizzard BLP Metadata Reader";

	*pcchActual = static_cast<UINT>(friendlyName.size() + 1);

	if (wzFriendlyName == NULL)
		return S_OK;

	friendlyName._Copy_s(wzFriendlyName, cchFriendlyName, friendlyName.size());
	if(cchFriendlyName > friendlyName.size())
		wzFriendlyName[friendlyName.size()] = L'\0';

	return S_OK;
}