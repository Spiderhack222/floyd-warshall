#ifndef FLOYD_WARSHALL_H
#define FLOYD_WARSHALL_H

#include <stdio.h>
#include <stdlib.h>

// Valor utilizado para representar "infinito" (no existe arista/camino)
#define INF 1000000000000000LL

typedef long long ll;

// Helpers para matrices dinámicas
ll **alloc_ll_matrix(int n);
int **alloc_int_matrix(int n);
void free_ll_matrix(ll **m, int n);
void free_int_matrix(int **m, int n);

// Lectura de matriz de adyacencia desde archivo o stdin.
// Formato: primera línea `n`, luego `n`x`n` tokens (numérico o "INF").
// Devuelven 1 en éxito, 0 en error.
int read_matrix_from_file(const char *filename, ll ***dist_out, int *n_out);
int read_matrix_from_stdin(ll ***dist_out, int *n_out);

// Impresión de matrices (mejor formato para legibilidad)
void print_ll_matrix(ll **m, int n, FILE *out);
void print_int_matrix(int **m, int n, FILE *out);

// Algoritmo principal y reconstrucción de rutas
void floyd_warshall(ll **dist, int **next, int n);
int *reconstruct_path(int **next, int u, int v, int *len_out);

// Detecta si hay ciclos negativos (dist[i][i] < 0)
int detect_negative_cycle(ll **dist, int n);

#endif // FLOYD_WARSHALL_H
