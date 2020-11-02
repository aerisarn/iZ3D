@set PATH=..\..\bin\others\;%PATH%
@md Shaders_dx10
@del Shaders_dx10\*.h
@set PARAMS=/nologo /Tps_4_0 /Qstrip_reflect
@fxc.exe shaders_dx10.psh %PARAMS% /EshaderHorizontalSideBySide /FhShaders_dx10\shaderHorizontalSideBySide.h
@fxc.exe shaders_dx10.psh %PARAMS% /EshaderVerticalSideBySide /FhShaders_dx10\shaderVerticalSideBySide.h

@set PARAMS2=/nologo /Tvs_4_0 /Qstrip_reflect
@fxc.exe shaders_dx10.vsh %PARAMS2% /EshaderFullScreenQuadVS /FhShaders_dx10\shaderFullScreenQuadVS.h
@pause