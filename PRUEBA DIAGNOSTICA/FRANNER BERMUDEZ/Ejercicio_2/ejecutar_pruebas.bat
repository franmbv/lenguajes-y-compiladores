@echo off
echo ============================================================
echo   EJECUTANDO PRUEBAS Y BENCHMARK - EJERCICIO 2
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================

echo.
echo [1/2] Ejecucion en C (pascal.exe):
echo ------------------------------------------------------------
if not exist "pascal.exe" (
    echo [AVISO] Compilando pascal.exe...
    gcc -O2 pascal.c -o pascal.exe -lm
)
pascal.exe

echo.
echo [2/2] Ejecucion en Python (pascal.py):
echo ------------------------------------------------------------
python pascal.py

echo.
echo ============================================================
echo   Reporte generado con exito en 'benchmark.txt'.
echo ============================================================
