/* IZ3D_FILE: $Id$ 
*
* Project : iZ3D Stereo Driver
* Copyright (C) iZ3D Inc. 2002 - 2010
*
* $Author$
* $Revision$
* $Date$
* $LastChangedBy$
* $URL$
*/
// LenovoOutput.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <tinyxml.h>
#include "Output_dx9.h"
#include "resource.h"
#include "CommonUtils\System.h"
#include <d3dx9.h>

using namespace DX9Output;

#include "Shaders_dx9/shaderAnaglyph.h"
#include "Shaders_dx9/shaderColorAnaglyph.h"
#include "Shaders_dx9/shaderYellowBlueAnaglyph.h"
#include "Shaders_dx9/shaderOptAnaglyph.h"
#include "Shaders_dx9/shaderOptAnaglyphAndGamma.h"
#include "Shaders_dx9/shaderMono.h"
#include "Shaders_dx9/shaderMonoAndGamma.h"

static HMODULE g_hModule = NULL;

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
					   DWORD  ul_reason_for_call,
					   LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		g_hModule = hModule;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

OUTPUT_API void* CALLBACK CreateOutputDX9(DWORD mode, DWORD spanMode)
{
	return new LenovoOutput(mode, spanMode);
}

OUTPUT_API DWORD CALLBACK GetOutputCaps()
{
	return 0;
}

OUTPUT_API void CALLBACK GetOutputName(char* name, DWORD size)
{
	strcpy_s(name, size, "Lenovo");
}

OUTPUT_API BOOL CALLBACK EnumOutputModes(DWORD num, char* name, DWORD size)
{
	switch(num)
	{
	case 0:
		strcpy_s(name, size, "Color anaglyph");
		return TRUE;
	case 1:
		strcpy_s(name, size, "Optimized anaglyph");
		return TRUE;
	case 2:
		strcpy_s(name, size, "Black & White anaglyph");
		return TRUE;
	case 3:
		strcpy_s(name, size, "Yellow/Blue anaglyph");
		return TRUE;
	default:
		return FALSE;
	}
}

LenovoOutput::LenovoOutput(DWORD mode, DWORD spanMode)
: PostprocessedOutput(mode, spanMode)
{
	if(m_OutputMode == 1)
		m_bProcessGammaRamp = true;
}

LenovoOutput::~LenovoOutput(void)
{
}


HRESULT LenovoOutput::InitializeResources()
{
	HRESULT res = S_OK;
	res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps14_shaderMono, &m_pShaderMono);
	res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps20_shaderMonoAndGamma, &m_pShaderMonoAndGamma);
	switch(m_OutputMode)
	{
	case 1: // Optimized anaglyph
		res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps20_shaderOptAnaglyph, &m_pShader1stPass);
		res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps20_shaderOptAnaglyphAndGamma, &m_pShader1stPassAndGamma);
		break;
	case 2: // Black & White anaglyph
		res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps14_shaderAnaglyph, &m_pShader1stPass);
		break;
	case 3: // Yellow/Blue anaglyph
		res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps14_shaderYellowBlueAnaglyph, &m_pShader1stPass);
		break;
	default: // Color anaglyph
		res = m_pd3dDevice->CreatePixelShader((DWORD*)g_ps14_shaderColorAnaglyph, &m_pShader1stPass);
		break;
	}
	if (FAILED(res))
		return res;

	CComPtr<IDirect3DStateBlock9> pSavedState;
	res = m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &pSavedState);
	{
		m_pd3dDevice->SetTexture(0, 0);
		m_pd3dDevice->SetTexture(1, 0);
		m_pd3dDevice->SetTexture(2, 0);
		m_pd3dDevice->SetRenderState(D3DRS_WRAP0,               0);
		m_pd3dDevice->SetRenderState(D3DRS_ZENABLE,             FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,    FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE,     FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FOGENABLE,           FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_FILLMODE,            D3DFILL_SOLID);
		m_pd3dDevice->SetRenderState(D3DRS_CULLMODE,            D3DCULL_NONE);
		m_pd3dDevice->SetRenderState(D3DRS_LIGHTING,			FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_STENCILENABLE,		FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPING,			FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_CLIPPLANEENABLE,		FALSE);
		m_pd3dDevice->SetRenderState(D3DRS_SRGBWRITEENABLE,		FALSE);
		for(int i=0; i < 3; i++)
		{
			m_pd3dDevice->SetTextureStageState(i, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE); 
			m_pd3dDevice->SetTextureStageState(i, D3DTSS_COLOROP,   D3DTOP_SELECTARG1 );	
			m_pd3dDevice->SetTextureStageState(i, D3DTSS_COLORARG1, D3DTA_TEXTURE);
			m_pd3dDevice->SetTextureStageState(i, D3DTSS_ALPHAOP,   D3DTOP_DISABLE );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_MINFILTER, D3DTEXF_POINT );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_MIPMAPLODBIAS, 0 );
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_MAXMIPLEVEL, 0);
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_MAXANISOTROPY, 1);
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_SRGBTEXTURE, 0);
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_ELEMENTINDEX, 0);
			m_pd3dDevice->SetSamplerState( i, D3DSAMP_DMAPOFFSET, 0);
		}
		m_pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
		m_pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );

		m_pd3dDevice->SetFVF( D3DFVF_D3DTVERTEX_1T );
		if(m_OutputMode == 1)
		{
			m_pd3dDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);
			m_pd3dDevice->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, TRUE);
			m_pd3dDevice->SetSamplerState( 1, D3DSAMP_SRGBTEXTURE, TRUE);
		}
		m_pd3dDevice->SetPixelShader(m_pShader1stPass);

		res = m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pState);

		if(m_OutputMode == 1)
		{
			m_pd3dDevice->SetSamplerState( 0, D3DSAMP_SRGBTEXTURE, FALSE);
			m_pd3dDevice->SetSamplerState( 1, D3DSAMP_SRGBTEXTURE, FALSE);
			m_pd3dDevice->SetSamplerState( 2, D3DSAMP_SRGBTEXTURE, TRUE);
			m_pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP );
			m_pd3dDevice->SetSamplerState( 2, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP );
			m_pd3dDevice->SetPixelShader(m_pShader1stPassAndGamma);

			res = m_pd3dDevice->CreateStateBlock(D3DSBT_ALL, &m_pStateWithGamma);
		}
	}
	res = pSavedState->Apply();

	//ResourceLoader logoFile;
	//if (logoFile.Load(g_hModule, MAKEINTRESOURCE(IDB_LOGO), _T("PNG")))
	//{
	//	D3DXCreateTextureFromFileInMemory(m_pd3dDevice, logoFile.GetData(), logoFile.GetSize(), &m_pLogo);
	//}

	return res;
}

