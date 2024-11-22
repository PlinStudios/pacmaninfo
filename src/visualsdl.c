#include <SDL2/SDL.h>

#define ORIGINAL_TILE_SIZE 8
#define OG_PACMANSIZE 13
#define OG_GHOSTSIZE 14
#define PACMAN_FRAMES 3
#define TOTAL_PACMAN_FRAMES 17
#define GHOST_FRAMES 2

float scalefact=2;
int bs;
int pacmansize;
int ghostsize;
//screen
int upoffset=24;
int downoffset=16;

void SetScale(float scale){
    scalefact=scale;

    //calcula tamaño de objetos
    bs=ORIGINAL_TILE_SIZE*scalefact;
    pacmansize=OG_PACMANSIZE*scalefact;
    ghostsize=OG_GHOSTSIZE*scalefact;
}

SDL_Texture* loadTexture(SDL_Renderer *render, const char* path) {
    SDL_Surface* tempSurface = SDL_LoadBMP(path);
    //hace los pixeles negros transparentes
    Uint32 colorKey = SDL_MapRGB(tempSurface->format, 0, 0, 0);
    SDL_SetColorKey(tempSurface, SDL_TRUE, colorKey);
    //crea textura
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, tempSurface);
    //borra la surface
    SDL_FreeSurface(tempSurface);
    return texture;
}

void loadTextures(SDL_Renderer *render, SDL_Texture* textures[], const char* name, int amount){
    char path[50];
    for (int i=0; i<amount; i++) {
        snprintf(path, sizeof(path), "data/%s%d.bmp",name,i);
        textures[i] = loadTexture(render, path);
    }
}

SDL_Texture* RenderMap(SDL_Renderer *render, int fils, int cols, unsigned char **MAP, SDL_Texture* textures[6]){
    Uint32 pixelFormat;
    SDL_QueryTexture(textures[0], &pixelFormat, NULL, NULL, NULL);
    //crea la textura
    SDL_Texture* labtxt = SDL_CreateTexture(render, pixelFormat, SDL_TEXTUREACCESS_TARGET, cols*ORIGINAL_TILE_SIZE, fils*ORIGINAL_TILE_SIZE);

    //cambia el dibujado a la textura
    SDL_SetRenderTarget(render, labtxt);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render,0,0,255,255);

    //dibuja mapa
    SDL_Rect rect={0, 0, ORIGINAL_TILE_SIZE, ORIGINAL_TILE_SIZE};
    for (int i=0; i<fils; i++){
        for (int j=0; j<cols; j++){
            if (MAP[i][j]!=' ' && MAP[i][j]!='.'){
                SDL_Texture* texture;
                //decide textura
                if (MAP[i][j]==196)
                    texture=textures[0];
                else if (MAP[i][j]==179)
                    texture=textures[1];
                else if (MAP[i][j]==191)
                    texture=textures[2];
                else if (MAP[i][j]==218)
                    texture=textures[3];
                else if (MAP[i][j]==217)
                    texture=textures[4];
                else if (MAP[i][j]==192)
                    texture=textures[5];
                else
                    texture=textures[0];

                rect.x = j*ORIGINAL_TILE_SIZE;
                rect.y = i*ORIGINAL_TILE_SIZE;
                if (scalefact>=1)
                    SDL_RenderCopy(render, texture, NULL, &rect);
                else
                    SDL_RenderFillRect(render, &rect);
            }
        }
    }

    //vuelve a renderizar la pantalla
    SDL_SetRenderTarget(render, NULL);

    return labtxt;
}

void DrawPacman(SDL_Renderer *render, int pp[2], SDL_Texture* pacmantxt[], int pacmanframe, float paclerp, int pdir, SDL_Rect pacmanrect){
    pacmanrect.x=pp[1]*bs-3*scalefact + ((pdir==0)-(pdir==2))*paclerp*bs;
    pacmanrect.y=pp[0]*bs+(upoffset-2)*scalefact + ((pdir==3)-(pdir==1))*paclerp*bs;
    if (pacmanframe!=0)
        if (pacmanframe<=PACMAN_FRAMES)
            SDL_RenderCopy(render, pacmantxt[pdir*(PACMAN_FRAMES-1)+pacmanframe], NULL, &pacmanrect);
        else
            SDL_RenderCopy(render, pacmantxt[pacmanframe], NULL, &pacmanrect);
    else
        SDL_RenderCopy(render, pacmantxt[0], NULL, &pacmanrect);
}

void DrawGhosts(SDL_Renderer *render, int gp[], int gn, SDL_Texture* ghosttxt[], int ghostframe, float glerp, int ghostdir[], SDL_Rect ghostrect, char **M){
    for (int g=0; g<gn; g++){
        float truglerp=(mov(&gp[2*g],M,ghostdir[g])) ? glerp : 0;
        ghostrect.x=gp[2*g+1]*bs-3*scalefact + ((ghostdir[g]==0)-(ghostdir[g]==2))*truglerp*bs;
        ghostrect.y=gp[2*g]*bs+(upoffset-2)*scalefact + ((ghostdir[g]==3)-(ghostdir[g]==1))*truglerp*bs;
        SDL_RenderCopy(render, ghosttxt[g*gn*GHOST_FRAMES+ghostdir[g]*GHOST_FRAMES+ghostframe], NULL, &ghostrect);
    }
}

void DrawPellets(SDL_Renderer *render, int fils, int cols, char **DIS, SDL_Texture* pellettxt, SDL_Rect pelletrect){
    for (int i=0; i<fils; i++)
        for (int j=0; j<cols; j++)
            if (DIS[i][j]==1){
                pelletrect.x=j*bs;
                pelletrect.y=i*bs+(upoffset)*scalefact;
                SDL_RenderCopy(render, pellettxt, NULL, &pelletrect);
        }
}
void DrawPoints(SDL_Renderer *render, SDL_Texture *nums[], SDL_Rect numrect, int count){
    numrect.x=49*scalefact;
    numrect.y=9*scalefact;
    //muestra un 0 primero y despues los pellets comidos
    SDL_RenderCopy(render, nums[0], NULL, &numrect);
    numrect.x-=8*scalefact;
    for (int i=0; i==0 || count!=0; i++){
        SDL_RenderCopy(render, nums[count%10], NULL, &numrect);
        numrect.x-=8*scalefact;
        count/=10;
    }
}