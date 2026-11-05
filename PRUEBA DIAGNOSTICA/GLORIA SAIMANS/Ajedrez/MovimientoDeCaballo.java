import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Scanner;

public class MovimientoDeCaballo {
    private static final String COLUMNAS = "abcdefgh";
    private static final String[] MOVIMIENTOS_DEL_PDF = """
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
            """.trim().split("\\s+");

    private record Posicion(int x, int y) {}

    private static Posicion convertirPosicion(String posicion) {
        String normalizada = posicion.toLowerCase();
        if (!normalizada.matches("[a-h][1-8]")) {
            throw new IllegalArgumentException("Posición inválida: " + posicion);
        }
        return new Posicion(COLUMNAS.indexOf(normalizada.charAt(0)) + 1,
                Character.digit(normalizada.charAt(1), 10));
    }

    public static boolean esMovimientoValido(String actual, String nueva) {
        Posicion origen = convertirPosicion(actual);
        Posicion destino = convertirPosicion(nueva);
        int diferenciaX = Math.abs(origen.x() - destino.x());
        int diferenciaY = Math.abs(origen.y() - destino.y());
        return (diferenciaX == 2 && diferenciaY == 1)
                || (diferenciaX == 1 && diferenciaY == 2);
    }

    public static String validarSecuencia(String inicio, int cantidad, String cadena) {
        String posicionActual;
        try {
            posicionActual = inicio.toLowerCase();
            convertirPosicion(posicionActual);
        } catch (RuntimeException error) {
            return "movimiento INCORRECTO: " + error.getMessage();
        }

        if (cantidad < 0) {
            return "movimiento INCORRECTO: la cantidad debe ser no negativa";
        }

        List<String> movimientos = cadena.trim().isEmpty()
                ? List.of()
                : Arrays.asList(cadena.trim().split("\\s+"));
        if (movimientos.size() < cantidad) {
            return "movimiento INCORRECTO: faltan movimientos";
        }

        for (int i = 0; i < cantidad; i++) {
            String movimiento = movimientos.get(i);
            if (!movimiento.startsWith("K")) {
                return "movimiento INCORRECTO: falla la letra "
                        + (movimiento.isEmpty() ? "(vacía)" : movimiento.charAt(0))
                        + " no pertenece a un caballo";
            }
            if (!movimiento.matches("K[a-h][1-8]")) {
                return "movimiento INCORRECTO: notación inválida en el movimiento " + (i + 1);
            }

            String nueva = movimiento.substring(1).toLowerCase();
            if (!esMovimientoValido(posicionActual, nueva)) {
                return "movimiento INCORRECTO: falla en el movimiento " + (i + 1);
            }
            posicionActual = nueva;
        }
        return "movimiento CORRECTO";
    }

    public static List<String> obtenerSecuenciaDeRendimiento(int cantidad) {
        List<String> secuencia = new ArrayList<>(cantidad);
        for (int i = 0; i < cantidad; i++) {
            secuencia.add(MOVIMIENTOS_DEL_PDF[i % MOVIMIENTOS_DEL_PDF.length]);
        }
        return secuencia;
    }

    public static long medirRendimiento() {
        List<String> secuencia = obtenerSecuenciaDeRendimiento(500);
        String cadena = String.join(" ", secuencia);
        long inicio = System.nanoTime();
        for (int i = 0; i < 10; i++) {
            validarSecuencia("a1", secuencia.size(), cadena);
        }
        return System.nanoTime() - inicio;
    }

    public static void main(String[] args) {
        try (Scanner scanner = new Scanner(System.in)) {
            System.out.print("Posición inicial (ej. a1): ");
            String inicio = scanner.nextLine();
            System.out.print("Cantidad de movimientos: ");
            int cantidad = Integer.parseInt(scanner.nextLine());
            System.out.print("Movimientos (ej. Kc2 Kd4): ");
            String cadena = scanner.nextLine();
            System.out.println(validarSecuencia(inicio, cantidad, cadena));
            System.out.printf("Tiempo de 10 verificaciones de 500 movimientos: %.4f ms%n",
                    medirRendimiento() / 1_000_000.0);
        }
    }
}
