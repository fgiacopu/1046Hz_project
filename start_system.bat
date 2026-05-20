@echo off
set ROOT=%~dp0

start "SuperCollider" ^
"C:\Program Files\SuperCollider-3.14.1\sclang.exe" "%ROOT%supercollider\audio_engine.scd"

timeout /t 2 > nul

start "Python" ^
"C:\Users\Federico\miniconda3\envs\cmls\python.exe" "%ROOT%python\main.py"

timeout /t 2 > nul

start "GUI" ^
"%ROOT%juce\GUI\Builds\VisualStudio2026\x64\Debug\App\GUI.exe"