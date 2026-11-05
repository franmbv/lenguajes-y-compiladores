const COLUMNAS = 'abcdefgh';

function dividirMovimientos(cadena) {
  if (Array.isArray(cadena)) {
    return cadena.map((movimiento) => String(movimiento));
  }

  return String(cadena ?? '')
    .trim()
    .split(/\s+/)
    .filter(Boolean);
}

function posicionANumeros(posicion) {
  const normalizada = String(posicion).toLowerCase();

  if (!/^[a-h][1-8]$/.test(normalizada)) {
    throw new Error(`Posición inválida: ${posicion}`);
  }

  return {
    x: COLUMNAS.indexOf(normalizada[0]) + 1,
    y: Number(normalizada[1]),
  };
}

function validarNotacion(cadena) {
  const movimientos = dividirMovimientos(cadena);

  for (let i = 0; i < movimientos.length; i += 1) {
    const movimiento = movimientos[i];

    if (!movimiento.startsWith('K')) {
      return {
        correcta: false,
        movimiento: i + 1,
        mensaje: `falla la letra ${movimiento[0] ?? '(vacía)'} no pertenece a un caballo`,
      };
    }

    if (!/^K[a-h][1-8]$/.test(movimiento)) {
      return {
        correcta: false,
        movimiento: i + 1,
        mensaje: `la notación "${movimiento}" no es válida`,
      };
    }
  }

  return { correcta: true };
}

function esMovimientoValido(posicionActual, posicionNueva) {
  const actual = posicionANumeros(posicionActual);
  const nueva = posicionANumeros(posicionNueva);
  const diferenciaX = Math.abs(actual.x - nueva.x);
  const diferenciaY = Math.abs(actual.y - nueva.y);

  return (
    (diferenciaX === 2 && diferenciaY === 1)
    || (diferenciaX === 1 && diferenciaY === 2)
  );
}

function validarSecuencia(posicionInicial, cantidad, cadena) {
  let posicionActual;
  try {
    posicionActual = String(posicionInicial).toLowerCase();
    posicionANumeros(posicionActual);
  } catch (error) {
    return { correcta: false, mensaje: error.message };
  }

  if (!Number.isInteger(cantidad) || cantidad < 0) {
    return { correcta: false, mensaje: 'La cantidad de movimientos debe ser un entero no negativo' };
  }

  const movimientos = dividirMovimientos(cadena);
  if (movimientos.length < cantidad) {
    return {
      correcta: false,
      mensaje: `Se esperaban ${cantidad} movimientos y se recibieron ${movimientos.length}`,
    };
  }

  const notacion = validarNotacion(movimientos.slice(0, cantidad));
  if (!notacion.correcta) {
    return { correcta: false, movimiento: notacion.movimiento, mensaje: notacion.mensaje };
  }

  for (let i = 0; i < cantidad; i += 1) {
    const posicionNueva = movimientos[i].slice(1).toLowerCase();

    if (!esMovimientoValido(posicionActual, posicionNueva)) {
      return {
        correcta: false,
        movimiento: i + 1,
        mensaje: `falla en el movimiento ${i + 1}`,
      };
    }

    posicionActual = posicionNueva;
  }

  return { correcta: true, mensaje: 'movimiento CORRECTO' };
}

const MOVIMIENTOS_DEL_PDF = `
Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5
Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6
Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3
Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2
Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6
Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7
Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4
Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2
Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5
Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4
Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4
Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2
Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4
Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8
`.trim().split(/\s+/);

function obtenerSecuenciaDeRendimiento(cantidad = 500) {
  return Array.from(
    { length: cantidad },
    (_, indice) => MOVIMIENTOS_DEL_PDF[indice % MOVIMIENTOS_DEL_PDF.length],
  );
}

function medirRendimiento() {
  const secuencia = obtenerSecuenciaDeRendimiento();
  const inicio = process.hrtime.bigint();

  for (let repeticion = 0; repeticion < 10; repeticion += 1) {
    validarSecuencia('a1', secuencia.length, secuencia);
  }

  const fin = process.hrtime.bigint();
  return Number(fin - inicio) / 1e6;
}

async function ejecutarCLI() {
  const readline = require('node:readline/promises');
  const interfaz = readline.createInterface({
    input: process.stdin,
    output: process.stdout,
  });

  try {
    const inicio = await interfaz.question('Posición inicial (ej. a1): ');
    const cantidad = Number(await interfaz.question('Cantidad de movimientos: '));
    const cadena = await interfaz.question('Movimientos (ej. Kc2 Kd4): ');
    const resultado = validarSecuencia(inicio, cantidad, cadena);
    console.log(resultado.correcta ? 'movimiento CORRECTO' : `movimiento INCORRECTO: ${resultado.mensaje}`);
    console.log(`Tiempo de 10 verificaciones de 500 movimientos: ${medirRendimiento().toFixed(4)} ms`);
  } finally {
    interfaz.close();
  }
}

if (require.main === module) {
  ejecutarCLI();
}

module.exports = {
  esMovimientoValido,
  medirRendimiento,
  obtenerSecuenciaDeRendimiento,
  validarNotacion,
  validarSecuencia,
};