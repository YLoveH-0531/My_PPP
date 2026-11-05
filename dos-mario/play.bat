@echo off
cd /d "%~dp0"
if not exist "game\MARIO.EXE" powershell -NoProfile -Command "Expand-Archive -Force -LiteralPath MARIO.zip -DestinationPath game"
if not exist "dosbox-x\bin\x64\Release\dosbox-x.exe" powershell -NoProfile -Command "Expand-Archive -Force -LiteralPath dosbox-x-vsbuild-win64-2026.10.01-portable.zip -DestinationPath dosbox-x"
if not exist "dosbox-x\bin\x64\Release\dosbox-x.exe" (echo Failed to unpack DOSBox-X. & pause & exit /b 1)
"dosbox-x\bin\x64\Release\dosbox-x.exe" -conf mario.conf
