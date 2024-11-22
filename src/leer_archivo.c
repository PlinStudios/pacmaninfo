#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAXCHAR 1000

// filaxcol

int cargar_matriz(char* rutaArchivo, char*** M, int* filas, int* columnas, int* pelletcount, int* pp, int* gp) {
    FILE* archivo = fopen(rutaArchivo, "r");

    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 1;
    }

    char buffer[MAXCHAR];

    // Cargando posiciones de pacman y fantasmas
    char* line = fgets(buffer, MAXCHAR, archivo);
    char* token;
    if (!line) {
        printf("Error al leer una línea\n");
        fclose(archivo);
        return 1;
    }
    token = strtok(line, ",");
    pp[0] = atoi(token);
    token = strtok(NULL, ",");
    pp[1] = atoi(token);

    for (int i = 0; i < 8; i++) {
        token = strtok(NULL, ",");
        gp[i] = atoi(token);
    }

    // Cargando puntos
    line = fgets(buffer, MAXCHAR, archivo);
    if (!line) {
        printf("Error al leer una línea\n");
        fclose(archivo);
        return 1;
    }
    *pelletcount = atoi(line);

    // Cargando filas y columnas
    line = fgets(buffer, MAXCHAR, archivo);
    if (!line) {
        printf("Error al leer una línea\n");
        fclose(archivo);
        return 1;
    }
    token = strtok(line, "x");
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

int guardar_matriz(char* rutaArchivo, char*** M, int* filas, int* columnas, int* pelletcount, int* pp, int* gp) {
    FILE* archivo = fopen(rutaArchivo, "w");

    if (!archivo) {
        printf("Error al abrir el archivo\n");
        return 0;
    }

    fprintf(archivo, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d\n", pp[0], pp[1], gp[0], gp[1], gp[2], gp[3], gp[4], gp[5], gp[6], gp[7]);
    fprintf(archivo, "%d\n", *pelletcount);
    
    // Escribiendo el tamaño de la matriz
    fprintf(archivo, "%dx%d\n", *filas, *columnas);

    char caracter;
    // Escribiendo cada elemento de la matriz en el archivo
    for (int i = 0; i < *filas; i++) {
        for (int j = 0; j < *columnas; j++) {
            caracter = (*M)[i][j];
            if (caracter==-1)
                fprintf(archivo, "0");
            else if (caracter==1)
                fprintf(archivo, "1");
            else if (caracter==0)
                fprintf(archivo, " ");
            else
                fprintf(archivo, "%c", caracter);
        }
        fprintf(archivo, "\n");
    }

    fclose(archivo);

    printf("estado guardado en %s\n",rutaArchivo);

    return 1;
}
