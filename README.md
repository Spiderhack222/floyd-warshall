# Floyd–Warshall Implementation (C)

Estructura del proyecto:
- `src/` : código fuente en C
- `tests/` : archivos de entrada de ejemplo
- `REPORT.md` : reporte técnico (puedes convertir a PDF con pandoc)

Compilar:

```bash
mkdir -p bin
gcc -o bin/floyd src/*.c -Wall -O2
```

Ejecutar (con archivo):

```bash
./bin/floyd tests/test1.txt
```

Ejecutar (por consola):

```bash
./bin/floyd
# luego ingresa n y la matriz, usando INF para no arista
```

Convertir reporte a PDF (opcional, requiere `pandoc`):

```bash
pandoc REPORT.md -o REPORT.pdf
```

Formato de los archivos de prueba (`tests/*.txt`):
- Primera línea: `n` (número de nodos)
- Siguientes `n` líneas: `n` tokens por línea con pesos o `INF` para ausencia de arista
- Nodos se identifican internamente como `1..n`

Notas sobre el formato:
- El token para ausencia de arista debe ser `INF` (mayúsculas o minúsculas).
- No se interpreta `-1` como `INF` para evitar ambigüedades; usa `INF`.

Salida del programa:
- Imprime la matriz inicial
- Imprime la matriz final de distancias D (INF donde no hay camino)
- Imprime la matriz `next` para reconstrucción de rutas
- Muestra hasta 3 rutas reconstruidas (origen, destino, distancia y ruta)
