@echo off

REM Set here the full path to your Python interpreter
set PYTHON_EXE=C:\Users\Federico\miniconda3\envs\cmls\python.exe

cd /d "%~dp0"
cd python

"%PYTHON_EXE%" main.py

pause