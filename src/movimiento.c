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
char perder(int ghostnumber, int ghosts[], int pacman[2], int gdir[], int pdir){
    float px=pacman[0]+((pdir==0)-(pdir==2))*0.5;
    float py=pacman[1]+((pdir==3)-(pdir==1))*0.5;

    for (int i = 0; i < ghostnumber; i++){
        float gx=ghosts[2*i]+((pdir==0)-(pdir==2))*0.5;
        float gy=ghosts[2*i+1]+((pdir==3)-(pdir==1))*0.5;
        if (gx==px && gy==py) return true;
    }
    return false;
}

//para saltar al llegar a un tunel
void salto(int *personaje, int fils, int cols){

        //x
        if(*personaje >= fils){
            *personaje = -1;
        }
        else if (*personaje <= -1){
            *personaje = fils;
        }
        if(*(personaje + 1) >= cols){
            *(personaje + 1) = -1;
        }
        else if (*(personaje + 1)  <= -1){
            *(personaje + 1)  = cols;
        }
}

int inmatrix(int fils, int cols, int *a){
    if (*a>=0 && *a<fils && *(a+1)>=0 && *(a+1)<cols) return 1;
    else return 0;
}

// funcion para mover el pacman
//los punteros tienen que apuntar al primer valor de la matriz(personaje, matriz)
//actcont es para activar el contador para un personaje o no
void avance(char **matriz , int mover, int *personaje, int ancho, int *cont, bool actcont){
    if (!mov(personaje,matriz,mover)) return;

    if (mover == 1){//ariba
        if(matriz[*personaje-1][*(personaje + 1)] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje-1][*(personaje + 1)] = 0;
        }
        *personaje -= 1;       
    }
    else if (mover == 3){//abajo

        if(matriz[*personaje+1][*(personaje + 1)] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje+1][*(personaje + 1)] = 0;
        }
        
        *personaje += 1;       
    }
    else if (mover == 2){//izquieda

        if(matriz[*personaje][*(personaje + 1)-1] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)-1] = 0;
        }
        
        *(personaje + 1) -= 1;       
    }
    else if (mover == 0){//derecha

        if(matriz[*personaje][*(personaje + 1)+1] == 1 && actcont == true){

            *cont = *cont + 1;
            matriz[*personaje][*(personaje + 1)+1] = 0;
        }
        
        *(personaje + 1) += 1;       
    }
}