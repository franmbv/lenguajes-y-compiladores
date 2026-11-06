import javax.swing.JOptionPane;
import java.util.Arrays;
import java.util.List;

public class MovimientoDeCaballo {
    private static final String COLUMNAS = "abcdefgh";

    private record Posicion(int x, int y) {}

    private static Posicion convertirPosicion(String posicion) {
        String normalizada = posicion.toLowerCase();
        if (!normalizada.matches("[a-h][1-8]")) {
            throw new IllegalArgumentException("Posición inválida");
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
            return "INCORRECTO falla en el movimiento 1.";
        }

        List<String> movimientos = cadena.trim().isEmpty()
                ? List.of()
                : Arrays.asList(cadena.trim().split("\\s+"));

        for (int i = 0; i < cantidad; i++) {
            if (i >= movimientos.size()) {
                return "INCORRECTO falla en el movimiento " + (i + 1) + ".";
            }

            String movimiento = movimientos.get(i);
            
            if (!movimiento.startsWith("K")) {
                char pieza = movimiento.isEmpty() ? ' ' : movimiento.charAt(0);
                return "movimiento INCORRECTO falla letra " + pieza + " no pertenece a un caballo.";
            }
            
            if (!movimiento.matches("K[a-h][1-8]")) {
                return "INCORRECTO falla en el movimiento " + (i + 1) + ".";
            }

            String nueva = movimiento.substring(1).toLowerCase();
            if (!esMovimientoValido(posicionActual, nueva)) {
                return "INCORRECTO falla en el movimiento " + (i + 1) + ".";
            }
            
            posicionActual = nueva;
        }
        return "movimiento CORRECTO.";
    }

    public static void medirRendimiento() {
        //Posicion a2 inicial 
        String inicio = "a2";
        int cantidad = 500;
        
        // Cadena de 500 movimientos truncada aquí por espacio 
        String cadena500 = """
            Kb4 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2
            Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8
            Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 
            Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5
            Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6
            Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6
            Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1
            Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3
            Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7
            Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4
            Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1
            Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4
            Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5
            Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2
            Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4
            Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4
            Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6
            Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3
            Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5
            Ka4 Kc3 Ka2 Kc1 Ke2 Kg1 Kh3 Kg5 Ke6 Kc5 Ka4 Kc3 Ka2 Kc1 Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7
            Kb5 Kd6 Kb7 Ka5 Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Ka6 Kb8 Kd7 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4 Kc3 Ka2 Kc1
            Ke2 Kg3 Kh5 Kg7 Ke8 Kc7 Ka8 Kc7 Kb5 Kd4 Kb3 Ka1 Kc2 Ka3 Kc4 Ka5 Kc6 Ka7 Kc8 Ka7 Kc6 Ka5
            Kc4 Ka3 Kc2 Ka1 Kc2 Kb4 Kd5 Kb6 Ka8 Kc7 Ka6 Kc5 Ka4
            """.replace("\n", " ").trim();

        long tiempoTotal = 0;
        
        for (int i = 0; i < 10; i++) {
            long inicioMedicion = System.nanoTime();
            validarSecuencia(inicio, cantidad, cadena500);
            long finMedicion = System.nanoTime();
            tiempoTotal += (finMedicion - inicioMedicion);
        }
        
        double tiempoMs = tiempoTotal / 1_000_000.0;
        JOptionPane.showMessageDialog(null, 
            String.format("Tiempo de verificar 10 veces los 500 movimientos: %.4f ms", tiempoMs), 
            "Prueba de Rendimiento", 
            JOptionPane.INFORMATION_MESSAGE);
    }

    public static void main(String[] args) {
        //Ingresar posición inicial
        String inicio = JOptionPane.showInputDialog(null, 
            "Posición inicial (ej. a1):", 
            "Entrada de Datos", 
            JOptionPane.QUESTION_MESSAGE);
            
        // Si el usuario cancela o cierra la ventana, se termina el programa
        if (inicio == null || inicio.trim().isEmpty()) return;

        // Salida para ingresar la cantidad n
        String cantidadStr = JOptionPane.showInputDialog(null, 
            "Cantidad de movimientos n:", 
            "Entrada de Datos", 
            JOptionPane.QUESTION_MESSAGE);
            
        if (cantidadStr == null || cantidadStr.trim().isEmpty()) return;
        
        int cantidad;
        try {
            cantidad = Integer.parseInt(cantidadStr.trim());
        } catch (NumberFormatException e) {
            JOptionPane.showMessageDialog(null, "Error: La cantidad debe ser un número entero.", "Error", JOptionPane.ERROR_MESSAGE);
            return;
        }

        // Salida para la entrada de datos 
        String cadena = JOptionPane.showInputDialog(null, 
            "Movimientos (ej. Kc2 Kd4):", 
            "Entrada de Datos", 
            JOptionPane.QUESTION_MESSAGE);
            
        if (cadena == null) return;

        // Resultado de la validación
        String resultado = validarSecuencia(inicio, cantidad, cadena);
        JOptionPane.showMessageDialog(null, 
            "salida: " + resultado, 
            "Resultado de Validación", 
            JOptionPane.INFORMATION_MESSAGE);

        // Ejecutar y mostrar automáticamente la prueba de rendimiento de los 500 movimientos
        medirRendimiento();
    }
}