@echo off
set /p type="Enter the type of project files to generate: "

cd %~dp0\bin\premake 
premake5 --file=..\..\premake.lua %type%
pause