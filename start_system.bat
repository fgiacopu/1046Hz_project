@echo off

cd /d "C:\Users\Federico\Documents\polimi\corsi\cmls\1046Hz_project\supercollider"

start "SuperCollider Audio Engine" ^
"C:\Program Files\SuperCollider-3.14.1\sclang.exe" audio_engine_sclang.scd

timeout /t 3 > nul

start "" ^
"C:\Users\Federico\Documents\polimi\corsi\cmls\1046Hz_project\juce\GUI\Builds\VisualStudio2026\x64\Debug\App\GUI.exe"