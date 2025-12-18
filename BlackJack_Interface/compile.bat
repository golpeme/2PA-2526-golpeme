@echo off
echo Iniciando compilacion...

cd build

echo Compilando...

"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" BlackJack.sln /p:Configuration=Debug /p:Platform=x64

echo Compilacion terminada
pause