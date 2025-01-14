/* IZ3D_FILE: $Id: Trace.cpp 3062 2010-03-15 02:44:50Z dragon $ 
*
* Project : iZ3D Stereo Driver
* Copyright (C) iZ3D Inc. 2002 - 2010
*
* $Author: dragon $
* $Revision: 3062 $
* $Date: 2010-03-15 05:44:50 +0300$
* $LastChangedBy: dragon $
* $URL: svn://svn.neurok.ru/dev/driver/trunk/S3DWrapper9/Trace.cpp $
*/

#include "stdafx.h"
#include "GlobalData.h"
#include "..\S3DAPI\ShaderProfileData.h"

GlobalData gData;

// special for debugging, because external variables not work in watch window
GlobalInfo& gInfo = _gInfo;

ProfileData& g_ProfileData = _g_ProfileData;
