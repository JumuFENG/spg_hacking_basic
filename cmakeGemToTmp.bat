@echo off
set curFolder=%cd%
set cmklst=%curFolder%\CMakeLists.txt
for /f "delims=(,) tokens=1,2" %%i in (%cmklst%) do (
    if "%%i"=="project" (set Proj=%%j)
)
cd /d %public%
IF NOT EXIST CMakeTemp (mkdir CMakeTemp)
cd CMakeTemp
set dstFolder=%cd%
IF NOT EXIST %Proj% (mkdir %Proj%)
cd %Proj%
IF NOT EXIST build (mkdir build)
cd build
cmake -G "Visual Studio 10" "%curFolder%"
cd /d %curFolder%
copy ShortCut.bat ShortCutTmp.bat
(echo.shortcut "%dstFolder%\%Proj%\build\","%curFolder%\build.lnk")>>ShortCutTmp.bat
(echo.shortcut "%dstFolder%\%Proj%\build\%Proj%.sln","%curFolder%\%Proj%.sln.lnk")>>ShortCutTmp.bat
call ShortCutTmp.bat
del ShortCutTmp.bat
echo %curFolder%
echo %dstFolder%
echo %Proj%
exit