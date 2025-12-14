#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "floyd_warshall.h"

int main(int argc, char **argv) {
    ll **dist = NULL;
    int **next = NULL;
    int n = 0;
    int ok = 0;
    if (argc == 2) {
        ok = read_matrix_from_file(argv[1], &dist, &n);
        if (!ok) {
            fprintf(stderr, "Error leyendo archivo %s\n", argv[1]);
            return 1;
        }
    } else {
        ok = read_matrix_from_stdin(&dist, &n);
        if (!ok) {
            fprintf(stderr, "Error leyendo entrada por consola\n");
            return 1;
        }
    }

    // ensure diagonal zero
    for (int i = 0; i < n; ++i) dist[i][i] = 0;

    // initialize next matrix
    next = alloc_int_matrix(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) next[i][j] = j;
            else if (dist[i][j] < INF) next[i][j] = j;
            else next[i][j] = -1;
        }
    }

    printf("Matriz inicial (INF = no arista):\n");
    print_ll_matrix(dist, n, stdout);
    printf("\nEjecutando Floyd-Warshall...\n\n");

    floyd_warshall(dist, next, n);

    // Detectar ciclos negativos
    if (detect_negative_cycle(dist, n)) {
        fprintf(stdout, "\nADVERTENCIA: Se detecto al menos un ciclo negativo (D[i][i] < 0).\n");
    }

    printf("Matriz final de distancias D:\n");
    print_ll_matrix(dist, n, stdout);

    printf("\nMatriz de 'next' (para reconstruir rutas):\n");
    print_int_matrix(next, n, stdout);

    printf("\nMostrando hasta 3 rutas reconstruidas (origen -> destino):\n");
    int printed = 0;
    for (int i = 0; i < n && printed < 3; ++i) {
        for (int j = 0; j < n && printed < 3; ++j) {
            if (i == j) continue;
            if (dist[i][j] >= INF) continue;
            int len = 0;
            int *path = reconstruct_path(next, i, j, &len);
            if (!path || len == 0) continue;
            printf("%d -> %d : distancia = %lld ; ruta: ", i+1, j+1, dist[i][j]);
            for (int k = 0; k < len; ++k) {
                printf("%d", path[k]+1);
                if (k+1 < len) printf(" -> ");
            }
            printf("\n");
            free(path);
            printed++;
        }
    }
    if (printed == 0) printf("No hay pares conectados para mostrar rutas.\n");

    // Cleanup
    free_ll_matrix(dist, n);
    free_int_matrix(next, n);
    return 0;
}
