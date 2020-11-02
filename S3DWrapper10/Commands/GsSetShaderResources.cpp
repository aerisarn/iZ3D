#include "stdafx.h"
#include "GsSetShaderResources.h"
#include "..\Streamer\CodeGenerator.h"
#include "D3DDeviceWrapper.h"
#include "ShaderResourceViewWrapper.h"

namespace Commands
{

	void GsSetShaderResources::Execute( D3DDeviceWrapper *pWrapper )
	{
		for (UINT i = 0; i < NumValues_; i++)
		{
			if (Values_[i].pDrvPrivate)
			{
#ifndef FINAL_RELEASE
				ShaderResourceViewWrapper* pSRV;
				InitWrapper(Values_[i], pSRV);
				ResourceWrapper* pResWrp = pSRV->GetResourceWrapper();
				int resID = pWrapper->m_ResourceManager.GetID(pResWrp->GetHandle());
				DEBUG_TRACE3(L"\t\t GS SRV %d: %s%d", i, pResWrp->GetResourceName(), resID);
#endif
				phTempShaderResourceViews_[i] = UNWRAP_CV_SRV_HANDLE(Values_[i]);
			}
			else
				phTempShaderResourceViews_[i].pDrvPrivate = NULL;
		}

		BEFORE_EXECUTE(this);
		pWrapper->OriginalDeviceFuncs.pfnGsSetShaderResources(pWrapper->hDevice, Offset_, NumValues_, phTempShaderResourceViews_);
		AFTER_EXECUTE(this); 

		SetLastResources(pWrapper);
	}

	bool GsSetShaderResources::WriteToFile( D3DDeviceWrapper *pWrapper ) const
	{
		return WriteToFileEx(pWrapper, "GsSetShaderResources");
	}
}

VOID ( APIENTRY GsSetShaderResources )( D3D10DDI_HDEVICE  hDevice, UINT  Offset, UINT  NumViews, 
									   CONST D3D10DDI_HSHADERRESOURCEVIEW*  phShaderResourceViews )
{
#ifndef EXECUTE_IMMEDIATELY_G1
	Commands::GsSetShaderResources* command = new Commands::GsSetShaderResources(Offset, NumViews, phShaderResourceViews);
	D3D10_GET_WRAPPER()->AddCommand(command);
#else
	Commands::fast_vector<D3D10DDI_HSHADERRESOURCEVIEW>::type  SRVBuffer(NumViews);
	for (UINT i = 0; i < NumViews; i++)
		SRVBuffer[i] = UNWRAP_CV_SRV_HANDLE(phShaderResourceViews[i]);
	{
		THREAD_GUARD_D3D10();
		D3D10_GET_ORIG().pfnGsSetShaderResources(D3D10_DEVICE, Offset, NumViews, GetPointerToVector(SRVBuffer));
	}
#endif	
}