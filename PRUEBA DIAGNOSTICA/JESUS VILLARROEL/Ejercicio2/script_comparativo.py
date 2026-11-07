import argparse
import re
import statistics
import subprocess
import sys
import tempfile
from pathlib import Path


N = 10
X = 10
TIME_PATTERN = re.compile(r"Tiempo de ejecucion:\s*([0-9.eE+-]+)\s*segundos")


def ejecutar(command, language, working_directory):
    process = subprocess.run(
        command,
        input=f"{N}\n{X}\n",
        text=True,
        capture_output=True,
        cwd=working_directory,
        timeout=30,
        check=False,
    )

    if process.returncode != 0:
        raise RuntimeError(
            f"{language} terminó con código {process.returncode}.\n"
            f"Salida:\n{process.stdout}\nError:\n{process.stderr}"
        )

    match = TIME_PATTERN.search(process.stdout)
    if match is None:
        raise RuntimeError(
            f"No se encontró el tiempo de ejecución de {language}.\n"
            f"Salida:\n{process.stdout}"
        )

    return float(match.group(1)), process.stdout


def main():
    parser = argparse.ArgumentParser(
        description="Compara C y Python para n=10 y x=100."
    )
    parser.add_argument(
        "--c-exe",
        type=Path,
        default=Path(__file__).with_name("versionc.exe"),
        help="Ejecutable C (por defecto: versionc.exe junto al script).",
    )
    parser.add_argument(
        "--python-script",
        type=Path,
        default=Path(__file__).with_name("versionpython.py"),
        help="Script Python (por defecto: versionpython.py junto al comparador).",
    )
    parser.add_argument(
        "--runs",
        type=int,
        default=100,
        help="Repeticiones medidas por lenguaje (por defecto: 100).",
    )
    parser.add_argument(
        "--warmup",
        type=int,
        default=10,
        help="Ejecuciones iniciales descartadas por lenguaje (por defecto: 10).",
    )
    parser.add_argument(
        "--txt",
        type=Path,
        default=Path(__file__).with_name("resultados_comparacion.txt"),
        help="TXT que recibirá todas las mediciones y el resumen.",
    )
    args = parser.parse_args()

    if args.runs < 1 or args.warmup < 0:
        parser.error("--runs debe ser mayor que 0 y --warmup no puede ser negativo.")

    c_executable = args.c_exe.resolve()
    python_script = args.python_script.resolve()
    if not c_executable.is_file():
        parser.error(f"No se encontró el ejecutable C: {c_executable}")
    if not python_script.is_file():
        parser.error(f"No se encontró el script Python: {python_script}")

    commands = {
        "C": [str(c_executable)],
        "Python": [sys.executable, str(python_script)],
    }
    timings = {language: [] for language in commands}

    with tempfile.TemporaryDirectory(prefix="benchmark_ejercicio2_") as temp_dir:
        for _ in range(args.warmup):
            for language, command in commands.items():
                ejecutar(command, language, temp_dir)

        for run_number in range(1, args.runs + 1):
            languages = list(commands)
            if run_number % 2 == 0:
                languages.reverse()

            for language in languages:
                elapsed_seconds, _ = ejecutar(
                    commands[language], language, temp_dir
                )
                timings[language].append(elapsed_seconds)

    report_lines = [
        "Comparacion de tiempos de ejecucion: C y Python",
        f"n (N) = {N}",
        f"x (X) = {X}",
        f"Repeticiones medidas por lenguaje: {args.runs}",
        f"Ejecuciones de calentamiento descartadas por lenguaje: {args.warmup}",
        (
            "Los tiempos son los medidos internamente por cada programa; "
            "no incluyen el arranque del proceso ni el registro posterior."
        ),
        "",
    ]

    for language, values in timings.items():
        report_lines.append(f"Resumen {language}:")
        report_lines.append(f"  Mediana: {statistics.median(values):.9f} segundos")
        report_lines.append(f"  Promedio: {statistics.mean(values):.9f} segundos")
        report_lines.append(f"  Minimo: {min(values):.9f} segundos")
        report_lines.append(f"  Maximo: {max(values):.9f} segundos")
        report_lines.append(f"  Mediciones individuales ({language}):")
        report_lines.extend(
            f"    repeticion {run_number}: {elapsed_seconds:.9f} segundos"
            for run_number, elapsed_seconds in enumerate(values, start=1)
        )
        report_lines.append("")

    report = "\n".join(report_lines)
    print(report)

    txt_path = args.txt.resolve()
    txt_path.parent.mkdir(parents=True, exist_ok=True)
    txt_path.write_text(report + "\n", encoding="utf-8")
    print(f"Metricas guardadas en un solo archivo: {txt_path}")


if __name__ == "__main__":
    main()
