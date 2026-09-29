rem Emscripten expected to be installed to `c:\emsdk`
rem     Info) https://emscripten.org
rem     Download) clone https://github.com/emscripten-core/emsdk.git to c:\emsdk
rem Ninja must be installed to c:\Apps\ninja
rem     Info) https://ninja-build.org/
rem     Download) https://github.com/ninja-build/ninja/releases
rem Node needs installing for running a local web server
rem     Info) https://nodejs.org
rem     Download) https://nodejs.org/en/download

path=%path%;c:\Apps\ninja

set EMSCRIPTEN=c:\emsdk

call %EMSCRIPTEN%\emsdk_env.bat

echo --------------------------------------------------------------------------
echo ---------------------------- Building Release ----------------------------
echo --------------------------------------------------------------------------
call emcmake cmake -S . -B build-em\release -DCMAKE_BUILD_TYPE=Release
call xcopy Data build-em\release\Data\ /s /d /y
call cmake --build build-em\release
if %ERRORLEVEL% GEQ 1 goto done
call cmd /k npx http-server build-em\release
echo --------------------------------------------------------------------------
echo ----------------------- Finished Building Release ------------------------
echo --------------------------------------------------------------------------

:done
pause