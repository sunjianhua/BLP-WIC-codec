#include "stdafx.h"
#include "CWICBlpMetadataEnumerator.h"

STDMETHODIMP CWICBlpMetadataEnumerator::Init(
	PROPVARIANT* pSchema,
	PROPVARIANT* pID,
	PROPVARIANT* pValue,
	UINT stride,
	UINT count)
{
	m_pSchemaBegin = m_pSchemaIter = pSchema;
	m_pIDBegin = m_pIDIter = pID;
	m_pValueBegin = m_pValueIter = pValue;

	m_pSchemaBegin = pSchema + count;
	m_pIDBegin = pID + count;
	m_pValueBegin = pValue + count;

	m_stride = stride;

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataEnumerator::Next(
	/* [in] */ ULONG celt,
	/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(celt) PROPVARIANT *rgeltSchema,
	/* [size_is][out][in] */ __RPC__inout_ecount_full(celt) PROPVARIANT *rgeltId,
	/* [size_is][optional][out][in] */ __RPC__inout_ecount_full(celt) PROPVARIANT *rgeltValue,
	/* [optional][out] */ __RPC__out ULONG *pceltFetched)
{
	if (pceltFetched != nullptr)
		*pceltFetched = 0;

	for (unsigned int i = 0; i < celt; ++i)
	{
		if (m_pSchemaIter >= m_pSchemaEnd)
			break;

		if (rgeltSchema != nullptr)
			*rgeltSchema = *m_pSchemaIter;
		if (rgeltId != nullptr)
			*rgeltId = *m_pIDIter;
		if (rgeltValue != nullptr)
			*rgeltValue = *m_pValueIter;

		++m_pSchemaIter;
		++m_pIDIter;
		++m_pValueIter;
		if (rgeltSchema != nullptr)
			++rgeltSchema;
		if (rgeltId != nullptr)
			++rgeltId;
		if (rgeltValue != nullptr)
			++rgeltValue;
		if (pceltFetched != nullptr)
			++pceltFetched;
	}

	return S_OK;
}

STDMETHODIMP CWICBlpMetadataEnumerator::Skip(
	/* [in] */ ULONG celt)
{
	ULONG max = static_cast<ULONG>(m_pSchemaEnd - m_pSchemaIter);
	if (celt > max)
		celt = max;

	m_pSchemaIter += celt;
	m_pIDIter += celt;
	m_pValueIter += celt;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataEnumerator::Reset(void)
{
	m_pSchemaIter = m_pSchemaBegin;
	m_pIDIter = m_pIDBegin;
	m_pValueIter = m_pValueBegin;
	return S_OK;
}

STDMETHODIMP CWICBlpMetadataEnumerator::Clone(
	/* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadataItem)
{
	CComPtr<CComObject<CWICBlpMetadataEnumerator>> pEnumerator;
	HRESULT hr = CComObject<CWICBlpMetadataEnumerator>::CreateInstance(&pEnumerator);

	if (FAILED(hr))
		return hr;


	hr = pEnumerator->Init(m_pSchemaBegin, m_pIDBegin, m_pValueBegin, m_stride, static_cast<UINT>(m_pSchemaEnd - m_pSchemaBegin));

	if (SUCCEEDED(hr))
		pEnumerator->Skip(static_cast<ULONG>(m_pSchemaIter - m_pSchemaBegin));

	if (SUCCEEDED(hr))
		hr = pEnumerator->QueryInterface(ppIEnumMetadataItem);

	return hr;
}