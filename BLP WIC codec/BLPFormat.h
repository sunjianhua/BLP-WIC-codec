#pragma once
#include <Windows.h>

struct BLP_header
{
	/*0x00*/ UINT32	Magic;				//	always 'BLP2'
	/*0x04*/ UINT32	Version;			//	always 1
	/*0x08*/ UINT8	Compression;		// : 1 for uncompressed, 2 for DXTC, 3 (cataclysm) for plain A8R8G8B8 textures(see remarks)
	/*0x09*/ UINT8	AlphaDepth;			//	channel bit depth : 0, 1 or 8
	/*0x0A*/ UINT8	CompressionMode;	//	DXTC mode (0 : DXT1, 1 = DXT3, 7 = DXT5)
	/*0x0B*/ UINT8	Unknown2;			//	when 0 there is only 1 mipmaplevel.compressed : 0, 1 or 2, uncompressed : 1 or 2 (mostly 1)
	/*0x0C*/ UINT32	Width;				//	resolution(power of 2)
	/*0x10*/ UINT32	Height;				//	Y resolution(power of 2)
	/*0x14*/ UINT32	MipmapOfs[16];		//	offsets for every mipmap level(or 0 when there is no more mipmap level)
	/*0x54*/ UINT32	MipmapSize[16];		//	sizes for every mipmap level(or 0 when there is no more mipmap level)
};

#define BLP_COMPRESSION_MODE_DXT1	0
#define BLP_COMPRESSION_MODE_DXT3	0x1
#define BLP_COMPRESSION_MODE_DXT5	0x7


enum BLP_COMPRESSION
{
	BLP_COMPRESSION_UNCOMPRESSED	= 1,
	BLP_COMPRESSION_DXTC			= 2,
	BLP_COMPRESSION_A8R8G8B8		= 3,
};
