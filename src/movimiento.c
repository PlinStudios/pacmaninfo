#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

//funcion que dira si un movimiento es valido o no antes de hacerlo
bool mov(int a[2], char **matr , int move){

    if(move == 1){
        if(matr[a[0] - 1][a[1]] != -1){

            return true;
        }
    }
    else if(move == 3){
        if(matr[a[0] + 1][a[1]] != -1){

            return true;
        }
    }
    else if(move == 2){
        if(matr[a[0]][a[1] - 1] != -1){

            return true;
        }
    }
    else if(move == 0){
        if(matr[a[0]][a[1] + 1] != -1){

            return true;
        }
    }
    return false;
}
//crear funcion para saber si perdio al se alcanzado por un fantasma
bool perder(int largo, int ancho, int matr[][10]){

    for (int i = 0; i < largo; i++){
        for(int j = 0; j < ancho; j++){
            if (matr [i][j]== 2 ){
                return false;
            }
        }
    } 
    return true;
}

// funcion para mover el pacman
//los punteros tienen que apuntar al primer valor de la matriz(personaje, matriz)
//actcont es para activar el contador para un personaje o no
void avance(char **matriz , int mover, int *personaje, int ancho, int *cont, bool actcont){
    if (!mov(personaje,matriz,mover)) return;

    if (mover == 1){//ariba
        if(matriz[*personaje-1][*(personaje + 1)] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)] = 0;
        }
        *personaje -= 1;       
    }
    else if (mover == 3){//abajo

        if(matriz[*personaje+1][*(personaje + 1)] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)] = 0;
        }
        
        *personaje += 1;       
    }
    else if (mover == 2){//izquieda

        if(matriz[*personaje][*(personaje + 1)-1] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)] = 0;
        }
        
        *(personaje + 1) -= 1;       
    }
    else if (mover == 0){//derecha

        if(matriz[*personaje][*(personaje + 1)+1] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)] = 0;
        }
        
        *(personaje + 1) += 1;       
    }
}

/*
int main(){

    srand(time(NULL));

    //en move se almacenaran los movimientos 1:arriba 2:abajo 3:izquierda 4:derecha 0: no hay movimiento
    0: derecha 1: arriba 2: izquierda 3: abajo
    //a[0] = y ,a[1] = x
    int pacman[2]= {5 ,5}, rojo[2] = {}, rosa[2] = {}, cyan[2] = {}, naranja[2] = {}, move = 0 , contador = 0, array[10][10] ={{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1,  0,  1,  0,  1,  0,  0,  0,  1, -1},
                        {-1,  0, -1, -1, -1, -1, -1,  0, -1, -1},
                        {-1,  0,  1,  0,  0,  0,  1,  0,  1, -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                        {-1,  0,  0,  0,  1,  0,  0,  0,  0, -1},
                        {-1,  0, -1, -1, -1, -1, -1, 0,  -1, -1},
                        {-1,  0,  1,  0,  1,  0,  1,  0,  1, -1},
                        {-1,  0,  0,  0, -1, -1, -1,  0,  0, -1},
                        {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

    bool lose = false;

    //id fantasma 3 id pacman 2

    //el 2 representa a pacman
    array[pacman[0]][pacman[1]] = 2;

    //el 3 en la matriz representa los fantasmas

    move = 1;

    for(int i =0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%2d  ", array[i][j]);
        }
        printf("\n");
    }

    printf ("%d" ,mov(pacman, array,move));
    avance(&array[0][0], move, &pacman[0], 10, &contador, 2, true);


    printf("%d\n" ,contador);
    for(int i =0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%2d  ", array[i][j]);
        }
        printf("\n");
    }
    

    return 0;
}*/