@echo off
setlocal EnableExtensions
title Ejercicio 1 - Instalacion de Python
cd /d "%~dp0"

echo ============================================================
echo   INSTALACION DEL ENTORNO - EJERCICIO 1
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================
echo.
echo Este ejercicio solo requiere Python 3.
echo El script verifica si Python esta instalado y, si falta,
echo intenta instalarlo automaticamente con winget.
echo.
echo Si la instalacion falla por permisos, ejecute este archivo
echo como Administrador.
echo.

call :ensure_python
if errorlevel 1 goto :fallo

echo.
echo ============================================================
echo   PROCESO COMPLETADO SATISFACTORIAMENTE
echo   Ejecute 'ejecutar_pruebas.bat' para correr el programa.
echo ============================================================
exit /b 0

:fallo
echo.
echo ============================================================
echo   PROCESO DETENIDO
echo   Instale Python manualmente desde:
echo     https://www.python.org/downloads/
echo   Marque la opcion "Add python.exe to PATH".
echo ============================================================
exit /b 1


REM ============================================================
REM  DETECCION E INSTALACION DE PYTHON
REM ============================================================
:ensure_python
call :detectar_python
if not errorlevel 1 exit /b 0

echo [AVISO] Python 3 no esta instalado o no esta en el PATH.
echo [INFO] Intentando instalar Python 3.13 con winget...
call :instalar_winget "Python.Python.3.13"
call :detectar_python
if not errorlevel 1 exit /b 0

echo [INFO] Reintentando con el paquete generico Python.Python.3...
call :instalar_winget "Python.Python.3"
call :detectar_python
if not errorlevel 1 exit /b 0

echo [ERROR] No se pudo instalar Python automaticamente.
exit /b 1

:detectar_python
python --version >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%v in ('python --version 2^>^&1') do (
        echo [OK] %%v detectado en el PATH.
        exit /b 0
    )
)
py -3 --version >nul 2>&1
if not errorlevel 1 (
    for /f "delims=" %%v in ('py -3 --version 2^>^&1') do (
        echo [OK] %%v detectado mediante el lanzador 'py'.
        exit /b 0
    )
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
exit /b 0
