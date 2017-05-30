#define INITGUID
#include <initguid.h>
#include <Windows.h>

typedef class WICBlpDecoder WICBlpDecoder;
class DECLSPEC_UUID("9F6C2DAB-CF43-4CFA-B1D4-51B35287BEB3") WICBlpDecoder;

// {9F6C2DAB-CF43-4CFA-B1D4-51B35287BEB3}
DEFINE_GUID(CLSID_WICBlpDecoder,
	0x9f6c2dab, 0xcf43, 0x4cfa, 0xb1, 0xd4, 0x51, 0xb3, 0x52, 0x87, 0xbe, 0xb3);


typedef class WICBlpFrameDecoder WICBlpFrameDecoder;
class DECLSPEC_UUID("3843B368-937A-4B77-B328-1C868A163495") WICBlpFrameDecoder;

// {3843B368-937A-4B77-B328-1C868A163495}
DEFINE_GUID(CLSID_WICBlpFrameDecoder,
	0x3843b368, 0x937a, 0x4b77, 0xb3, 0x28, 0x1c, 0x86, 0x8a, 0x16, 0x34, 0x95);



// {707487DA-7D04-4982-9812-47AA3F65BF74}
DEFINE_GUID(GUID_ContainerFormatBlp,
	0x707487da, 0x7d04, 0x4982, 0x98, 0x12, 0x47, 0xaa, 0x3f, 0x65, 0xbf, 0x74);


// {32A655DF-FAC8-4F57-9F2E-A42FA88E0373}
DEFINE_GUID(GUID_VendorBlizzard,
	0x32a655df, 0xfac8, 0x4f57, 0x9f, 0x2e, 0xa4, 0x2f, 0xa8, 0x8e, 0x3, 0x73);


// {95FAAFA8-124C-49EA-9111-F8984C1F5B54}
DEFINE_GUID(CLSID_WICBlpMetadataReader,
	0x95faafa8, 0x124c, 0x49ea, 0x91, 0x11, 0xf8, 0x98, 0x4c, 0x1f, 0x5b, 0x54);