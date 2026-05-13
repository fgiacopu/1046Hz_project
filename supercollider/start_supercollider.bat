@echo off

cd /d "%~dp0"

cd supercollider

REM Start SuperCollider language interpreter
"C:\Program Files\SuperCollider-3.14.1\sclang.exe" audio_engine.scd