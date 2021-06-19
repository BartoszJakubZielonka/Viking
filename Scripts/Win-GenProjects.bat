@echo off
pushd %~dp0\..\
call Vendor\premake\bin\premake5.exe vs2019
popd