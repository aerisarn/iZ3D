#include "stdafx.h"

UINT BitsPerPixel( D3DFORMAT fmt )
{
	UINT fmtU = (UINT)fmt;
	switch( fmtU )
	{
	case D3DFMT_A32B32G32R32F:
		return 128;

	case D3DFMT_A16B16G16R16:
	case D3DFMT_Q16W16V16U16:
	case D3DFMT_A16B16G16R16F:
	case D3DFMT_G32R32F:
		return 64;

	case D3DFMT_A8R8G8B8:
	case D3DFMT_X8R8G8B8:
	case D3DFMT_A2B10G10R10:
	case D3DFMT_A8B8G8R8:
	case D3DFMT_X8B8G8R8:
	case D3DFMT_G16R16:
	case D3DFMT_A2R10G10B10:
	case D3DFMT_Q8W8V8U8:
	case D3DFMT_V16U16:
	case D3DFMT_X8L8V8U8:
	case D3DFMT_A2W10V10U10:
	case D3DFMT_D32:
	case D3DFMT_D24S8:
	case D3DFMT_D24X8:
	case D3DFMT_D24X4S4:
	case D3DFMT_D32F_LOCKABLE:
	case D3DFMT_D24FS8:
	case D3DFMT_INDEX32:
	case D3DFMT_G16R16F:
	case D3DFMT_R32F:
		return 32;

	case D3DFMT_R8G8B8:
	case FOURCC_DF24:
		return 24;

	case D3DFMT_A4R4G4B4:
	case D3DFMT_X4R4G4B4:
	case D3DFMT_R5G6B5:
	case D3DFMT_L16:
	case D3DFMT_A8L8:
	case D3DFMT_X1R5G5B5:
	case D3DFMT_A1R5G5B5:
	case D3DFMT_A8R3G3B2:
	case D3DFMT_V8U8:
	case D3DFMT_CxV8U8:
	case D3DFMT_L6V5U5:
	case D3DFMT_G8R8_G8B8:
	case D3DFMT_R8G8_B8G8:
	case D3DFMT_D16_LOCKABLE:
	case D3DFMT_D15S1:
	case D3DFMT_D16:
	case D3DFMT_INDEX16:
	case D3DFMT_R16F:
	case D3DFMT_YUY2:
	case D3DFMT_A8P8:
	case FOURCC_DF16:
		return 16;

	case D3DFMT_R3G3B2:
	case D3DFMT_A8:
	case D3DFMT_P8:
	case D3DFMT_L8:
	case D3DFMT_A4L4:
		return 8;

	case D3DFMT_DXT1:
	case FOURCC_ATI1:
		return 4;
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
	case FOURCC_ATI2:
		return  8;

		// From DX docs, reference/d3d/enums/d3dformat.asp
		// (note how it says that D3DFMT_R8G8_B8G8 is "A 16-bit packed RGB format analogous to UYVY (U0Y0, V0Y1, U2Y2, and so on)")
	case D3DFMT_UYVY:
		return 16;

		// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/directshow/htm/directxvideoaccelerationdxvavideosubtypes.asp
	case MAKEFOURCC( 'A', 'I', '4', '4' ):
	case MAKEFOURCC( 'I', 'A', '4', '4' ):
		return 8;

	case MAKEFOURCC( 'Y', 'V', '1', '2' ):
		return 12;

	case MAKEFOURCC( 'N', 'U', 'L', 'L' ):
		return 0;

	default:
		//assert(FALSE); // unhandled format
		return 32;
	}
}

bool IsCompressedFormat( D3DFORMAT Format )
{
	if (Format == D3DFMT_DXT1 ||
		Format == D3DFMT_DXT2 ||
		Format == D3DFMT_DXT3 ||
		Format == D3DFMT_DXT4 ||
		Format == D3DFMT_DXT5 ||
		Format == FOURCC_ATI1 ||
		Format == FOURCC_ATI2)
	{
		return true;
	}
	return false;
}

#define D3DCAPS2_ALWAYSSUPPORTED			(D3DCAPS2_CANRENDERWINDOWED|D3DCAPS2_NO2DDURING3DSCENE)
#define D3DPMISCCAPS_ALWAYSSUPPORTED		(D3DPMISCCAPS_LINEPATTERNREP)
#define D3DPRASTERCAPS_ALWAYSSUPPORTED		(D3DPRASTERCAPS_PAT|D3DPRASTERCAPS_ZBIAS)
#define D3DVTXPCAPS_ALWAYSSUPPORTED			(D3DVTXPCAPS_NO_VSDT_UBYTE4)

void ConvertCaps( D3DCAPS9 &caps9, D3DCAPS8* pCaps )
{
	memcpy(pCaps, &caps9, sizeof D3DCAPS8);
	pCaps->Caps2 |= D3DCAPS2_ALWAYSSUPPORTED;
	pCaps->PrimitiveMiscCaps |= D3DPMISCCAPS_ALWAYSSUPPORTED;
	pCaps->RasterCaps |= D3DPRASTERCAPS_ALWAYSSUPPORTED;
	pCaps->VertexProcessingCaps |= D3DVTXPCAPS_ALWAYSSUPPORTED;
	pCaps->MaxVertexShaderConst = min(caps9.MaxVertexShaderConst, 96);
	pCaps->VertexShaderVersion = min(caps9.VertexShaderVersion, D3DVS_VERSION(1, 1));
	pCaps->PixelShaderVersion = min(caps9.PixelShaderVersion, D3DPS_VERSION(1, 4));
}

#define D3DSWAPEFFECT_COPY_VSYNC	4
#define D3DPRESENT_RATE_UNLIMITED   0x7fffffff
void ConvertPresentParameters(D3DPRESENT_PARAMETERS8 *pPpDx8, D3DPRESENT_PARAMETERS *pPpDx9)
{
	pPpDx9->BackBufferWidth		= pPpDx8->BackBufferWidth;
	pPpDx9->BackBufferHeight	= pPpDx8->BackBufferHeight;
	pPpDx9->BackBufferFormat	= pPpDx8->BackBufferFormat;
	pPpDx9->BackBufferCount		= pPpDx8->BackBufferCount;
	pPpDx9->MultiSampleType		= pPpDx8->MultiSampleType;
	pPpDx9->MultiSampleQuality	= 0;
	if(pPpDx8->SwapEffect == D3DSWAPEFFECT_COPY_VSYNC)
		pPpDx9->SwapEffect		= D3DSWAPEFFECT_COPY;
	else
		pPpDx9->SwapEffect		= pPpDx8->SwapEffect;
	pPpDx9->hDeviceWindow		= pPpDx8->hDeviceWindow;
	pPpDx9->EnableAutoDepthStencil	= pPpDx8->EnableAutoDepthStencil;
	pPpDx9->AutoDepthStencilFormat	= pPpDx8->AutoDepthStencilFormat;
	pPpDx9->Flags=pPpDx8->Flags;
	pPpDx9->FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	if(pPpDx8->Windowed && pPpDx8->SwapEffect == D3DSWAPEFFECT_COPY_VSYNC)
		pPpDx9->PresentationInterval	= D3DPRESENT_INTERVAL_ONE;
	else if(pPpDx8->SwapEffect == D3DPRESENT_INTERVAL_DEFAULT)
		pPpDx9->PresentationInterval	= D3DPRESENT_INTERVAL_IMMEDIATE;
	else
		pPpDx9->PresentationInterval	= pPpDx8->FullScreen_PresentationInterval;

	pPpDx9->Windowed					= pPpDx8->Windowed;
	//#ifdef _DEBUG
	//	pPpDx9->Windowed					= true;
	//	pPpDx9->SwapEffect					= D3DSWAPEFFECT_DISCARD;
	//	pPpDx9->FullScreen_RefreshRateInHz	= D3DPRESENT_RATE_DEFAULT;
	//#endif
	DEBUG_TRACE1("D3DPRESENT_PARAMETERS8 pPresentationParameters = { BackBufferWidth=%d, BackBufferHeight=%d, BackBufferFormat=%s, BackBufferCount=%d, MultiSampleType=%d, SwapEffect=%d, hDeviceWindow=%d, Windowed=%d, EnableAutoDepthStencil=%d, AutoDepthStencilFormat=%s, Flags=%d, FullScreen_RefreshRateInHz=%d, FullScreen_PresentationInterval=%d}\n", 
		pPpDx8->BackBufferWidth, pPpDx8->BackBufferHeight, GetFormatString(pPpDx8->BackBufferFormat), pPpDx8->BackBufferCount, pPpDx8->MultiSampleType, pPpDx8->SwapEffect, pPpDx8->hDeviceWindow, pPpDx8->Windowed, pPpDx8->EnableAutoDepthStencil, GetFormatString(pPpDx8->AutoDepthStencilFormat), pPpDx8->Flags, pPpDx8->FullScreen_RefreshRateInHz, pPpDx8->FullScreen_PresentationInterval);
}
