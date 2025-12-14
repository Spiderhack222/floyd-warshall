#include "floyd_warshall.h"
#include <string.h>

ll **alloc_ll_matrix(int n) {
    ll **m = malloc(n * sizeof(ll*));
    for (int i = 0; i < n; ++i) {
        m[i] = malloc(n * sizeof(ll));
    }
    return m;
}

int **alloc_int_matrix(int n) {
    int **m = malloc(n * sizeof(int*));
    for (int i = 0; i < n; ++i) {
        m[i] = malloc(n * sizeof(int));
    }
    return m;
}

void free_ll_matrix(ll **m, int n) {
    if (!m) return;
    for (int i = 0; i < n; ++i) free(m[i]);
    free(m);
}

void free_int_matrix(int **m, int n) {
    if (!m) return;
    for (int i = 0; i < n; ++i) free(m[i]);
    free(m);
}

static int parse_token_to_ll(const char *tok, ll *out) {
    if (tok == NULL) return 0;
    if (strcmp(tok, "INF") == 0 || strcmp(tok, "inf") == 0) {
        *out = INF;
        return 1;
    }
    char *endptr;
    long long val = strtoll(tok, &endptr, 10);
    if (endptr == tok) return 0;
    *out = (ll)val;
    return 1;
}

int read_matrix_from_file(const char *filename, ll ***dist_out, int *n_out) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    int n;
    if (fscanf(f, "%d", &n) != 1) { fclose(f); return 0; }
    if (n <= 0) { fclose(f); return 0; }
    ll **dist = alloc_ll_matrix(n);
    char tok[128];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (fscanf(f, "%s", tok) != 1) {
                fprintf(stderr, "Error: token faltante en archivo %s (pos %d,%d)\n", filename, i+1, j+1);
                free_ll_matrix(dist, n); fclose(f); return 0;
            }
            ll val;
            if (!parse_token_to_ll(tok, &val)) {
                fprintf(stderr, "Error: token invalido '%s' en archivo %s (pos %d,%d)\n", tok, filename, i+1, j+1);
                free_ll_matrix(dist, n); fclose(f); return 0;
            }
            dist[i][j] = val;
        }
    }
    fclose(f);
    *dist_out = dist;
    *n_out = n;
    return 1;
}

int read_matrix_from_stdin(ll ***dist_out, int *n_out) {
    printf("Introduce n (numero de nodos):\n");
    int n;
    if (scanf("%d", &n) != 1) return 0;
    if (n <= 0) return 0;
    ll **dist = alloc_ll_matrix(n);
    printf("Introduce la matriz %dx%d (usa INF para no arista):\n", n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            char tok[128];
            if (scanf("%s", tok) != 1) { fprintf(stderr, "Error: token faltante en entrada (pos %d,%d)\n", i+1, j+1); free_ll_matrix(dist, n); return 0; }
            ll val;
            if (!parse_token_to_ll(tok, &val)) { fprintf(stderr, "Error: token invalido '%s' en entrada (pos %d,%d)\n", tok, i+1, j+1); free_ll_matrix(dist, n); return 0; }
            dist[i][j] = val;
        }
    }
    *dist_out = dist;
    *n_out = n;
    return 1;
}

void print_ll_matrix(ll **m, int n, FILE *out) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (m[i][j] >= INF/2) fprintf(out, "%10s", "INF");
            else fprintf(out, "%10lld", m[i][j]);
            if (j+1 < n) fprintf(out, " ");
        }
        fprintf(out, "\n");
    }
}

void print_int_matrix(int **m, int n, FILE *out) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (m[i][j] == -1) fprintf(out, "%6s", "-");
            else fprintf(out, "%6d", m[i][j] + 1);
            if (j+1 < n) fprintf(out, " ");
        }
        fprintf(out, "\n");
    }
}

void floyd_warshall(ll **dist, int **next, int n) {
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            if (dist[i][k] >= INF) continue;
            for (int j = 0; j < n; ++j) {
                if (dist[k][j] >= INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}

int detect_negative_cycle(ll **dist, int n) {
    for (int i = 0; i < n; ++i) {
        if (dist[i][i] < 0) return 1;
    }
    return 0;
}

int *reconstruct_path(int **next, int u, int v, int *len_out) {
    if (next[u][v] == -1) { *len_out = 0; return NULL; }
    int capacity = 16;
    int *path = malloc(capacity * sizeof(int));
    int cur = u;
    int len = 0;
    path[len++] = cur;
    while (cur != v) {
        cur = next[cur][v];
        if (cur == -1) { free(path); *len_out = 0; return NULL; }
        if (len >= capacity) { capacity *= 2; path = realloc(path, capacity * sizeof(int)); }
        path[len++] = cur;
        if (len > 1000000) break; // safeguard
    }
    *len_out = len;
    return path;
}
