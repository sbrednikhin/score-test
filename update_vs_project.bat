@echo off
echo Updating Visual Studio project for sw_battle_test...

REM Save current directory
set "ORIGINAL_DIR=%CD%"

REM Change to project root directory
cd /d "%~dp0"

echo.
if exist build (
    echo Build directory exists.
    cd build

    if exist CMakeCache.txt (
        echo Using existing project configuration.
    ) else (
        echo CMakeCache.txt not found, regenerating project...
        cmake .. -G "Visual Studio 17 2022" -A x64
    )
) else (
    echo Build directory not found, creating and generating project...
    mkdir build
    cd build
    cmake .. -G "Visual Studio 17 2022" -A x64
)

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: CMake configuration failed!
    cd "%ORIGINAL_DIR%"
    pause
    exit /b 1
)

echo.
echo Building project in Debug configuration...
cmake --build . --config Debug --parallel

if %ERRORLEVEL% neq 0 (
    echo.
    echo ERROR: Build failed!
    cd "%ORIGINAL_DIR%"
    pause
    exit /b 1
)

echo.
echo Configuring debug launch arguments...
REM Create or update the .vcxproj.user file to set command line arguments
set "VCXPROJ_USER=sw_battle_test.vcxproj.user"

echo ^<?xml version="1.0" encoding="utf-8"?^> > "%VCXPROJ_USER%"
echo ^<Project ToolsVersion="Current" xmlns="http://schemas.microsoft.com/developer/msbuild/2003"^> >> "%VCXPROJ_USER%"
echo   ^<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'"^> >> "%VCXPROJ_USER%"
echo     ^<LocalDebuggerCommandArguments^>commands_example.txt^</LocalDebuggerCommandArguments^> >> "%VCXPROJ_USER%"
echo     ^<DebuggerFlavor^>WindowsLocalDebugger^</DebuggerFlavor^> >> "%VCXPROJ_USER%"
echo     ^<LocalDebuggerWorkingDirectory^>$(ProjectDir)\..^</LocalDebuggerWorkingDirectory^> >> "%VCXPROJ_USER%"
echo   ^</PropertyGroup^> >> "%VCXPROJ_USER%"
echo   ^<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'"^> >> "%VCXPROJ_USER%"
echo     ^<LocalDebuggerCommandArguments^>commands_example.txt^</LocalDebuggerCommandArguments^> >> "%VCXPROJ_USER%"
echo     ^<DebuggerFlavor^>WindowsLocalDebugger^</DebuggerFlavor^> >> "%VCXPROJ_USER%"
echo     ^<LocalDebuggerWorkingDirectory^>$(ProjectDir)\..^</LocalDebuggerWorkingDirectory^> >> "%VCXPROJ_USER%"
echo   ^</PropertyGroup^> >> "%VCXPROJ_USER%"
echo ^</Project^> >> "%VCXPROJ_USER%"

echo.
echo Verifying build output...
if exist "Debug\sw_battle_test.exe" (
    echo SUCCESS: Debug executable created at build\Debug\sw_battle_test.exe
) else (
    echo WARNING: Debug executable not found!
)

if exist "Release\sw_battle_test.exe" (
    echo SUCCESS: Release executable created at build\Release\sw_battle_test.exe
) else (
    echo INFO: Release executable not built (only Debug was requested).
)

echo.
echo Checking exception handling settings...
findstr "ExceptionHandling" sw_battle_test.vcxproj >nul
if %ERRORLEVEL% equ 0 (
    echo SUCCESS: Exception handling is enabled in project settings.
) else (
    echo WARNING: Could not verify exception handling settings.
)

echo.
echo Project update completed successfully!
echo.
echo To run the project:
echo 1. Open build\sw_battle_test.sln in Visual Studio
echo 2. Set sw_battle_test as startup project
echo 3. Press F5 to run in Debug mode (will use commands_example.txt as argument)
echo.
echo Or run manually:
echo   build\Debug\sw_battle_test.exe commands_example.txt

REM Return to original directory
cd "%ORIGINAL_DIR%"

echo Press any key to continue...
pause >nul
