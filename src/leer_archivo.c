#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCHAR 1000

// filaxcol

int cargar_matriz(char* rutaArchivo, char*** M, int* filas, int* columnas) {
    FILE* archivo = fopen(rutaArchivo, "r");

    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    char buffer[MAXCHAR];
    char* line = fgets(buffer, MAXCHAR, archivo);
    if (!line) {
        printf("Error al leer una línea\n");
        fclose(archivo);
        return 1;
    }
    char* token = strtok(line, "x");
    *filas = atoi(token);
    token = strtok(NULL, " ");
    *columnas = atoi(token);

    if (*filas == 0 || *columnas == 0) {
        printf("Error al leer las filas y/o columnas\n");
        return 1;
    }

    *M = (char**) malloc((*filas) * sizeof(char*));
    for (int i = 0; i < (*filas); i++) {
        (*M)[i] = (char*)malloc((*columnas) * sizeof(char));
    }

    int contadorLinea = 0;
    do {
        char* line = fgets(buffer, MAXCHAR, archivo);
        if (!line) {
            break;
        }
        for (int i = 0; i < MAXCHAR; i++) {
            char caracter = line[i];
            if (caracter == '\n' || caracter == '\0') {
                break;
            }
            if (caracter=='0')
                (*M)[contadorLinea][i] = -1;
            else if (caracter=='1')
                (*M)[contadorLinea][i] = 1;
            else
                (*M)[contadorLinea][i] = 0;
        }
        contadorLinea++;
    } while (feof(archivo) != EOF && contadorLinea < (*filas));

    fclose(archivo);

    return 0;
}

int guardar_matriz(char* rutaArchivo, char*** M, int* filas, int* columnas) {
    FILE* archivo = fopen(rutaArchivo, "w");

    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    // Escribiendo el tamaño de la matriz
    fprintf(archivo, "%dx%d\n", *filas, *columnas);

    // Escribiendo cada elemento de la matriz en el archivo
    for (int i = 0; i < *filas; i++) {
        for (int j = 0; j < *columnas; j++) {
            fprintf(archivo, "%c", (*M)[i][j]);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);

    return 1;
}
/*
int main(int argc, char const *argv[]) {
    char** M;
    int f, c;

    cargar_matriz("mapa.txt", &M, &f, &c);

    printf("======================\n");
    printf("filas, columnas: %d, %d\n", f, c);

    guardar_matriz("mapaGuardado.txt", &M, &f, &c);
    
    for (int i = 0; i < f; i++) {
        for (int j = 0; j < c; j++) {
            printf("%c", M[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < f; i++) {
        free(M[i]);
    }
    free(M);
    
    return 0;
}*/
