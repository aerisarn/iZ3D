@ECHO OFF

SET ORIGPATH="%CD%"
CD %~dp0/../..

set BRANCH_NAME=
lib\python27\python BuildSetup\SetBranchFolder\SetBranchFolder.py
call setbranchname.bat
del setbranchname.bat

cd S3DDriverSetup
..\lib\InnoSetup\ISCC.exe SetupScript.iss /dsetupType="Taerim" "/dOUTDIR=..\bin\Final Release - Taerim" "/dFILESDIR=..\bin\Final Release" /O"..\BuildSetup\TaerimEdition\%BRANCH_NAME%"

CD %ORIGPATH%
exit /b %ERRORLEVEL%