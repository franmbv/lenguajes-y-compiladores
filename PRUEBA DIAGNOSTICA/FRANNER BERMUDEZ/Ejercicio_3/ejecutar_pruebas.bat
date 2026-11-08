@echo off
setlocal EnableExtensions
title Ejercicio 3 - Ejecucion de pruebas
cd /d "%~dp0"

echo ============================================================
echo   EJECUTANDO TRADUCCION - EJERCICIO 3
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================

echo.
echo [1/2] Ejecucion del traductor sobre ejemplo.c:
echo ------------------------------------------------------------
if not exist "traductor.exe" (
    echo [AVISO] Compilando traductor.exe...
    gcc -O2 traductor.c -o traductor.exe
    if errorlevel 1 (
        echo [ERROR] Fallo la compilacion de traductor.c.
        exit /b 1
    )
)
traductor.exe ejemplo.c salida_traducida.txt
if errorlevel 1 (
    echo [ERROR] Fallo la ejecucion del traductor.
    exit /b 1
)

echo.
echo [2/2] Contenido de salida_traducida.txt:
echo ------------------------------------------------------------
type salida_traducida.txt

echo.
echo ============================================================
echo   Proceso completado. Revise 'salida_traducida.txt'.
echo ============================================================
exit /b 0
