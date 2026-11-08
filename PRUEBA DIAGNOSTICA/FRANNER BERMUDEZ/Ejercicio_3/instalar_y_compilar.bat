@echo off
setlocal EnableExtensions
title Ejercicio 3 - Instalacion y compilacion
cd /d "%~dp0"

echo ============================================================
echo   INSTALACION Y COMPILACION - EJERCICIO 3
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================
echo.
echo Este script detecta e instala GCC si falta y compila
echo traductor.c. Si la instalacion falla por permisos,
echo ejecute este archivo como Administrador.

call :paso "[1/2] Verificando e instalando GCC"
call :ensure_gcc
if errorlevel 1 goto :fallo

call :paso "[2/2] Compilando traductor.c"
call :compilar
if errorlevel 1 goto :fallo

echo.
echo ============================================================
echo   PROCESO COMPLETADO SATISFACTORIAMENTE
echo   Ejecute 'ejecutar_pruebas.bat' para traducir ejemplo.c.
echo ============================================================
exit /b 0

:fallo
echo.
echo ============================================================
echo   PROCESO DETENIDO
echo   Revise los mensajes anteriores. Si el problema es de
echo   permisos, ejecute este script como Administrador.
echo ============================================================
exit /b 1


REM ============================================================
REM  SUBRUTINAS
REM ============================================================

:paso
echo.
echo ------------------------------------------------------------
echo   %~1
echo ------------------------------------------------------------
exit /b 0

:ensure_gcc
call :detectar_gcc
if not errorlevel 1 exit /b 0

call :detectar_gcc_local
call :detectar_gcc
if not errorlevel 1 exit /b 0

echo [AVISO] GCC no esta instalado o no esta en el PATH.
echo [INFO] Intentando instalar GCC WinLibs con winget...
call :instalar_winget "BrechtSanders.WinLibs.POSIX.UCRT" "WinLibs GCC"
call :detectar_gcc_local
call :detectar_gcc
if not errorlevel 1 exit /b 0

echo [INFO] Reintentando con Chocolatey, paquete mingw...
call :instalar_choco "mingw" "MinGW GCC"
call :detectar_gcc_local
call :detectar_gcc
if not errorlevel 1 exit /b 0

echo [ERROR] No se pudo instalar GCC automaticamente.
echo         Opciones manuales:
echo           * MSYS2:   https://www.msys2.org/
echo           * WinLibs: https://winlibs.com/
echo         Agregue la carpeta 'bin' de GCC al PATH.
exit /b 1

:detectar_gcc
gcc --version >nul 2>&1
if errorlevel 1 exit /b 1
for /f "delims=" %%v in ('gcc --version 2^>^&1') do (
    echo [OK] %%v
    exit /b 0
)
exit /b 0

:detectar_gcc_local
set "GCC_EXE="
for %%R in (
    "%LOCALAPPDATA%\Microsoft\WinGet\Packages"
    "C:\msys64"
    "C:\ProgramData\chocolatey"
    "C:\Program Files\mingw-w64"
    "C:\mingw64"
) do (
    if exist "%%~R" (
        for /f "delims=" %%G in ('where /r "%%~R" gcc.exe 2^>nul') do (
            if not defined GCC_EXE set "GCC_EXE=%%G"
        )
    )
)
if defined GCC_EXE (
    for %%D in ("%GCC_EXE%") do set "PATH=%%~dpD;%PATH%"
    echo [OK] GCC localizado en: %GCC_EXE%
    exit /b 0
)
exit /b 1

:instalar_winget
where winget >nul 2>&1
if errorlevel 1 (
    echo [AVISO] winget no esta disponible en este equipo.
    exit /b 1
)
echo [INFO] winget install --id %~1 --silent
winget install --id %~1 -e --source winget --accept-package-agreements --accept-source-agreements --silent
if errorlevel 1 (
    echo [AVISO] winget devolvio un error. Puede que el paquete ya este instalado.
)
exit /b 0

:instalar_choco
where choco >nul 2>&1
if errorlevel 1 (
    echo [AVISO] Chocolatey no esta disponible en este equipo.
    exit /b 1
)
echo [INFO] choco install %~1 -y
choco install %~1 -y
if errorlevel 1 (
    echo [AVISO] choco devolvio un error. Puede que el paquete ya este instalado.
)
exit /b 0

:compilar
if not exist "traductor.c" (
    echo [ERROR] No se encontro traductor.c en esta carpeta.
    exit /b 1
)
echo [INFO] gcc -O2 traductor.c -o traductor.exe
gcc -O2 traductor.c -o traductor.exe
if errorlevel 1 (
    echo [ERROR] Fallo la compilacion de traductor.c.
    exit /b 1
)
echo [OK] Compilacion exitosa: traductor.exe generado.
exit /b 0
