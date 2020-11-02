#include "stdafx.h"
#include "AbandonCommandList11_0.h"
#include "..\Streamer\CodeGenerator.h"
#include "D3DDeviceWrapper.h"

namespace Commands
{

	void AbandonCommandList11_0::Execute( D3DDeviceWrapper *pWrapper )
	{
		BEFORE_EXECUTE(this);
		pWrapper->OriginalDeviceFuncs11.pfnAbandonCommandList(pWrapper->hDevice);
		AFTER_EXECUTE(this); 
	}

	bool AbandonCommandList11_0::WriteToFile( D3DDeviceWrapper *pWrapper ) const
	{
		WriteStreamer::CmdBegin( pWrapper->GetWrapperHandle(), ( Command* )this,  "AbandonCommandList11_0" );
		WriteStreamer::CmdEnd();
		return true;
	}

	bool AbandonCommandList11_0::ReadFromFile()
	{
		D3D10DDI_HDEVICE hDevice = { NULL };
		ReadStreamer::CmdBegin( hDevice );
		ReadStreamer::CmdEnd();
		return true;
	}

}

VOID ( APIENTRY AbandonCommandList11_0 )( D3D10DDI_HDEVICE  hDevice )
{
	_ASSERT(D3D10_GET_WRAPPER()->GetD3DVersion() >= TD3DVersion_11_0);
#ifndef EXECUTE_IMMEDIATELY_G2
	Commands::AbandonCommandList11_0* command = new Commands::AbandonCommandList11_0();
	D3D10_GET_WRAPPER()->AddCommand(command, true );
#else
	D3D10_GET_WRAPPER()->ProcessCB();
	D3D11_GET_ORIG().pfnAbandonCommandList(D3D10_DEVICE);
#endif
}