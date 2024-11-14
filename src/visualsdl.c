#include <SDL2/SDL.h>

#include "movimiento.c"
#include "leer_archivo.c"
#include "visual.c"

#define ORIGINAL_TILE_SIZE 8
#define OG_PACMANSIZE 13
#define OG_GHOSTSIZE 14
#define PACMAN_FRAMES 3
#define GHOST_FRAMES 2

int scalefact=2;
int bs;
//screen
int upoffset=22;
int downoffset=16;
//pacman and ghost position
int pp[]={6,6};
int pacmansize;

int gn=4;
int gp[]={3,1,5,3,7,1,8,3};
int ghostsize;

int looptime=150;

SDL_Texture* loadTexture(SDL_Renderer *render, const char* path) {
    SDL_Surface* tempSurface = SDL_LoadBMP(path);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render, tempSurface);
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
                SDL_RenderCopy(render, texture, NULL, &rect);
            }
        }
    }

    //vuelve a renderizar la pantalla
    SDL_SetRenderTarget(render, NULL);

    return labtxt;
}

void DrawPacman(SDL_Renderer *render, int pp[2], SDL_Texture* pacmantxt[], int pacmanframe, int pdir, SDL_Rect pacmanrect){
    pacmanrect.x=pp[1]*bs-3*scalefact;
    pacmanrect.y=pp[0]*bs+(upoffset-2)*scalefact;
    if (pacmanframe!=0)
        SDL_RenderCopy(render, pacmantxt[pdir*(PACMAN_FRAMES-1)+pacmanframe], NULL, &pacmanrect);
    else
        SDL_RenderCopy(render, pacmantxt[0], NULL, &pacmanrect);
}

void DrawGhosts(SDL_Renderer *render, int gp[], SDL_Texture* ghosttxt[], int ghostframe, int ghostdir[], SDL_Rect ghostrect){
    for (int g=0; g<gn; g++){
        ghostrect.x=gp[2*g+1]*bs-3*scalefact;
        ghostrect.y=gp[2*g]*bs+(upoffset-2)*scalefact;
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

int main(int argc, char *argv[]){
    //calcula tamaño de objetos
    bs=ORIGINAL_TILE_SIZE*scalefact;
    pacmansize=OG_PACMANSIZE*scalefact;
    ghostsize=OG_GHOSTSIZE*scalefact;

    //iniciar lista
    //int M[31][28]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,0,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,0,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,1,1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,1,1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,0,-1,-1,0,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,0,-1,-1,0,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,0,0,0,0,0,0,0,0,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,-1,-1,0,0,-1,-1,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,0,0,0,0,0,0,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{0,0,0,0,0,0,1,0,0,0,-1,0,0,0,0,0,0,-1,0,0,0,1,0,0,0,0,0,0},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,0,0,0,0,0,0,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,0,0,0,0,0,0,0,0,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1,-1,0,-1,-1,1,-1,-1,-1,-1,-1,-1},{-1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,1,-1,-1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,1,-1,-1,-1,-1,1,-1},{-1,0,1,1,-1,-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1,-1,1,1,0,-1},{-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1},{-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,1,-1,-1,-1},{-1,1,1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,-1,-1,1,1,1,1,1,1,-1},{-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1},{-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1,-1,1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,1,-1},{-1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
    char** M;
    int n, m;
    cargar_matriz("og_solid2.txt", &M, &n, &m);

    //inicia sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *win = SDL_CreateWindow("pacman", 
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, 
                                    m*ORIGINAL_TILE_SIZE*scalefact, 
                                    (n*ORIGINAL_TILE_SIZE+upoffset+downoffset)*scalefact, 
                                    SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    const Uint8* key = SDL_GetKeyboardState(NULL);
    SDL_Event event;

    //posicion de fantasmas
    int gp[]={3,1,5,3,7,1,8,3};

    unsigned char **DIS;
    //obtener lista de caracteres
    maptodisp(M,&DIS,n,m);

    //carga texturas
    SDL_Texture* _text[6];
    loadTextures(render, _text,"border",6);
    //crea textura con todo el laberinto
    SDL_Rect labrect={0, upoffset*scalefact, m*bs, n*bs};
    SDL_Texture* LABTEXTURE=RenderMap(render,n,m,DIS,_text);

    //texturas de pacman y fantasmas y pellets
    SDL_Rect pacmanrect={pp[0], pp[1], pacmansize, pacmansize};
    SDL_Texture* pacmantxt[9];
    int pacmanframe=0;
    int pacanimdir=1; //direccion de la animacion 1 para abrir la boca -1 para cerrar
    int npdir=0;
    int pdir=0; //direccion de pacman
    loadTextures(render, pacmantxt,"pacman",9);

    SDL_Rect ghostrect={0, 0, ghostsize, ghostsize};
    SDL_Texture* ghosttxt[40];
    int ghostframe=0;
    int nghostdir[gn]={};
    int ghostdir[gn]={};
    loadTextures(render, ghosttxt,"ghost",40);

    SDL_Rect pelletrect={0, 0, bs, bs};
    SDL_Texture* pellettxt=loadTexture(render,"data/pellet0.bmp");
    int pelletcount=0; //puntos comidos por pacman

    //variables de animacion
    int currentTime=SDL_GetTicks();
    int lastTime=SDL_GetTicks();

    //loop principal
    int quit = 0;
    while(!quit){
        if (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            } else if (event.type == SDL_KEYDOWN) {
                if(key[SDL_SCANCODE_W])
                    npdir=1;
                if(key[SDL_SCANCODE_A])
                    npdir=2;
                if(key[SDL_SCANCODE_S])
                    npdir=3;
                if(key[SDL_SCANCODE_D])
                    npdir=0;
            }
        }

        //GAMEPLAY
        //avance(&M[0][0], 0, &pp[0], n, &pelletcount, 2, true);
        
        //RENDER

        //Renderiza el laberinto completo
        SDL_RenderCopy(render, LABTEXTURE, NULL, &labrect);

        //dibuja pellets
        DrawPellets(render,n,m,M,pellettxt,pelletrect);

        //cuenta los frames
        currentTime=SDL_GetTicks();
        if (currentTime>lastTime+looptime){
            lastTime=currentTime;
            if (mov(pp,M,npdir)) pdir=npdir;
            avance(M, pdir, &pp[0], n, &pelletcount, true);
            //nghostdir[0]=rand()%4;
            if (mov(&gp[0],M,(ghostdir[0]-1)%4)) ghostdir[0]=(ghostdir[0]-1)%4;
            avance(M, ghostdir[0], &gp[0], n, &pelletcount, false);

            //pacman frame
            pacmanframe+=pacanimdir;
            if (pacmanframe==0 || pacmanframe==PACMAN_FRAMES-1) pacanimdir=-pacanimdir;
            //ghost frame
            ghostframe++;
            if (ghostframe==GHOST_FRAMES) ghostframe=0;
        }
        
        //renderiza a pacman
        DrawPacman(render,pp,pacmantxt,pacmanframe,pdir,pacmanrect);
        //renderiza fantasmas
        DrawGhosts(render,gp,ghosttxt,ghostframe,ghostdir,ghostrect);

        //actualizar el render
        SDL_RenderPresent(render);
    }

    //cerrar sdl
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}