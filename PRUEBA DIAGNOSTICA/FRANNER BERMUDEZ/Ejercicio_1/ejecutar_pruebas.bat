@echo off
setlocal EnableExtensions
title Ejercicio 1 - Ejecucion de pruebas
cd /d "%~dp0"

echo ============================================================
echo   EJECUTANDO PRUEBAS - EJERCICIO 1
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================
echo.

echo [1/1] Ejecucion en Python (caballo.py):
echo ------------------------------------------------------------
python caballo.py
if errorlevel 1 (
    echo [ERROR] Fallo la ejecucion de caballo.py.
    exit /b 1
)

echo.
echo ============================================================
echo   Proceso completado. Reporte generado en 'tiempos.txt'.
echo ============================================================
exit /b 0
