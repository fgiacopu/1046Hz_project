@echo off
set PROJECT_ROOT=%~dp0

start "SuperCollider Audio Engine" ^
"C:\Program Files\SuperCollider-3.14.1\sclang.exe" "%PROJECT_ROOT%supercollider\audio_engine.scd"

timeout /t 3 > nul

start "" "%PROJECT_ROOT%juce\GUI\Builds\VisualStudio2026\x64\Debug\App\GUI.exe"