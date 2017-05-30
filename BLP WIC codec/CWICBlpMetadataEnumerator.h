#pragma once

#include "stdafx.h"

using namespace ATL;

// {4D46A6E6-3545-4FEA-B3AE-6B8701C46564}
/*DEFINE_GUID(<< name >> ,
	0x4d46a6e6, 0x3545, 0x4fea, 0xb3, 0xae, 0x6b, 0x87, 0x1, 0xc4, 0x65, 0x64);*/


MIDL_INTERFACE("4D46A6E6-3545-4FEA-B3AE-6B8701C46564")
IBlpInternalEnumMetadataItem
	: public IWICEnumMetadataItem
{
	virtual HRESULT STDMETHODCALLTYPE Init(
		PROPVARIANT* pSchema,
		PROPVARIANT* pID,
		PROPVARIANT* pValue,
		UINT stride,
		UINT count) = 0;
};

class ATL_NO_VTABLE CWICBlpMetadataEnumerator :
	public IBlpInternalEnumMetadataItem,
	public CComObjectRootEx<CComObjectThreadModel>
{
	PROPVARIANT* m_pSchemaBegin = nullptr;
	PROPVARIANT* m_pIDBegin = nullptr;
	PROPVARIANT* m_pValueBegin = nullptr;

	PROPVARIANT* m_pSchemaEnd = nullptr;
	PROPVARIANT* m_pIDEnd = nullptr;
	PROPVARIANT* m_pValueEnd = nullptr;

	PROPVARIANT* m_pSchemaIter = nullptr;
	PROPVARIANT* m_pIDIter = nullptr;
	PROPVARIANT* m_pValueIter = nullptr;

	UINT m_stride = 0;

public:
	BEGIN_COM_MAP(CWICBlpMetadataEnumerator)
		COM_INTERFACE_ENTRY(IBlpInternalEnumMetadataItem)
	END_COM_MAP()

	STDMETHOD(Init)(
		PROPVARIANT* pSchema,
		PROPVARIANT* pID,
		PROPVARIANT* pValue,
		UINT stride,
		UINT count);
	STDMETHOD(Next)(
		/* [in] */ ULONG celt,
		/* [size_is][unique][out][in] */ __RPC__inout_ecount_full_opt(celt) PROPVARIANT *rgeltSchema,
		/* [size_is][out][in] */ __RPC__inout_ecount_full(celt) PROPVARIANT *rgeltId,
		/* [size_is][optional][out][in] */ __RPC__inout_ecount_full(celt) PROPVARIANT *rgeltValue,
		/* [optional][out] */ __RPC__out ULONG *pceltFetched);

	STDMETHOD(Skip)(
		/* [in] */ ULONG celt);

	STDMETHOD(Reset)(void);

	STDMETHOD(Clone)(
		/* [out] */ __RPC__deref_out_opt IWICEnumMetadataItem **ppIEnumMetadataItem);
};