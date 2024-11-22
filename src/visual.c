char getborderchar(char **M, int i, int j, int fil, int col){
    //obtiene si estan ocupadas las celdas tangentes, verdadero si sale de rango
    char up = (!(i>0) || M[i-1][j] == -1);
    char down = (!(i<fil-1) || M[i+1][j] == -1);
    char left = (!(j>0) || M[i][j-1] == -1);
    char right = (!(j<col-1) || M[i][j+1] == -1);

    if (!up && !left && right && down) {
        return 218; //218: ┌
    } else if (!up && !right && left && down) {
        return 191; //191: ┐
    } else if (!down && !left && right && up) {
        return 192; //192: └
    } else if (!down && !right && left && up) {
        return 217; //217: ┘
    } else if (up && down && !(left && right)) {
        return 179; //179: │
    } else if (left && right && !(up && down)) {
        return 196; //196: ─
    } else {
        //totalmente rodeado
        if (i>0 && j>0 && M[i-1][j-1]!=-1){   //izquierda arriba esta vacio
            return 217; //217: ┘
        }else if (i>0 && j<col-1 && M[i-1][j+1]!=-1){   //derecha arriba esta vacio
            return 192; //192: └
        }else if (i<fil-1 && j>0 && M[i+1][j-1]!=-1){   //izquierda abajo esta vacio
            return 191; //191: ┐
        }else if (i<fil-1 && j<col-1 && M[i+1][j+1]!=-1){   //derecha abajo esta vacio
            return 218; //218: ┌
        }else{
            return ' ';
        }
    }
}

void maptodisp(char **M, unsigned char ***C, int fil, int col){
    (*C)=malloc(fil*sizeof(char *));
    for (int i=0; i<fil; i++){
        (*C)[i]=malloc(col*sizeof(char));
        for (int j=0; j<col; j++){
            if (M[i][j]==-1)
                (*C)[i][j]=getborderchar(M,i,j,fil,col);
            else if (M[i][j]==0)
                (*C)[i][j]=' ';
            else if (M[i][j]==1)
                (*C)[i][j]='.';
        }
    }
}

void printlab(unsigned char **C, int fil, int col){
    for (int i=0; i<fil; i++){
        for (int j=0; j<col; j++){
            printf("%c",C[i][j]);
        }
        printf("\n");
    }
}

void printscene(unsigned char **C, int fil, int col, int px, int py, int gn, int *gp){
    for (int i=0; i<fil; i++){
        for (int j=0; j<col; j++){
            //pacman
            if (i==px && j==py){
                printf("%c",1);
                continue;
            }
            //fantasmas
            char gisdrawn=0;
            for (int g=0; g<gn; g++){
                if (i==gp[g*2] && j==gp[g*2+1]){
                    printf("%c",153);
                    gisdrawn=1;
                    break;
                }
            }
            if (!gisdrawn) printf("%c",C[i][j]);
        }
        printf("\n");
    }
}