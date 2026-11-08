@echo off
echo ============================================================
echo   INSTALACION Y COMPILACION - EJERCICIO 2
echo   Lenguajes y Compiladores - Prueba Diagnostica
echo   Estudiante: Franner Bermudez
echo ============================================================

echo.
echo [1/3] Verificando entorno de Python...
python --version
if errorlevel 1 (
    echo [ERROR] Python no esta instalado o no se encuentra en el PATH.
    exit /b 1
)

echo.
echo [2/3] Verificando compilador GCC...
gcc --version
if errorlevel 1 (
    echo [ERROR] GCC no esta instalado o no se encuentra en el PATH.
    exit /b 1
)

echo.
echo [3/3] Compilando pascal.c con optimizacion -O2...
gcc -O2 pascal.c -o pascal.exe -lm
if errorlevel 1 (
    echo [ERROR] Fallo la compilacion de pascal.c.
    exit /b 1
) else (
    echo [OK] Compilacion exitosa: pascal.exe generado.
)

echo.
echo ============================================================
echo   PROCESO COMPLETADO SATISFACTORIAMENTE
echo   Ejecute 'ejecutar_pruebas.bat' para correr las pruebas.
echo ============================================================
