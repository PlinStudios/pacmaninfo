#include "movimiento.c"
#include "leer_archivo.c"
#include "visual.c"
#include "visualsdl.c"

#include "menu.c"
#include "audio.c"
#include "raycast.c"

//pacman and ghost position
int pp[]={6,6};

int gn=4;
int gp[]={3,1,5,3,7,1,8,3};

int looptime=150;
int pacloop=200;
int gloop=300;

char perdi=0;
char paccaster=0;

int main(int argc, char *argv[]){
    SetScale(2);

    //iniciar lista
    char** M;
    int n, m;
    int pelletcount=0; //puntos comidos por pacman
    int resp = showMenu();
    if (resp==0)
        cargar_matriz("oglevel.txt", &M, &n, &m, &pelletcount, pp, gp);
    else if (resp==1)
        cargar_matriz("save.txt", &M, &n, &m, &pelletcount, pp, gp);
    else if (resp==2){
        cargar_matriz("oglevel.txt", &M, &n, &m, &pelletcount, pp, gp);
        paccaster=true;
    }else
        return 0;

    //inicia sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    int winwidth=m*ORIGINAL_TILE_SIZE*scalefact;
    int winheight=(n*ORIGINAL_TILE_SIZE+upoffset+downoffset)*scalefact;
    if (paccaster){
        winwidth=640;
        winheight=480;
        SetScale(0.7);
        upoffset=15;
        downoffset=0;
    }

    SDL_Window *win = SDL_CreateWindow("pacman", 
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, 
                                    winwidth, 
                                    winheight, 
                                    SDL_WINDOW_SHOWN);
    SDL_Renderer *render = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    const Uint8* key = SDL_GetKeyboardState(NULL);
    SDL_Event event;

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
    SDL_Texture* pacmantxt[TOTAL_PACMAN_FRAMES];
    int pacmanframe=0;
    int pacanimdir=1; //direccion de la animacion 1 para abrir la boca -1 para cerrar
    int npdir=0;
    int pdir=0; //direccion de pacman
    int ppdir=0;
    int pactime=SDL_GetTicks();
    int paclast=SDL_GetTicks();
    loadTextures(render, pacmantxt,"pacman",TOTAL_PACMAN_FRAMES);

    //carga los sonidos de pacaman
    initMixer();
    Mix_Music* music = createMusic("data/waka-waka.wav");
    Mix_Chunk* sound = createSound("data/death.wav");

    SDL_Rect ghostrect={0, 0, ghostsize, ghostsize};
    SDL_Texture* ghosttxt[40];
    int ghostframe=0;
    int ngdir[gn]={};
    int gdir[gn]={};
    int gtime=SDL_GetTicks();
    int glast=SDL_GetTicks();
    loadTextures(render, ghosttxt,"ghost",40);

    SDL_Rect pelletrect={0, 0, bs, bs};
    SDL_Texture* pellettxt=loadTexture(render,"data/pellet0.bmp");

    //points
    SDL_Texture *oneup=loadTexture(render,"data/oneup.bmp");
    SDL_Rect oneuprect={26, 0, 22*scalefact, 7*scalefact};
    SDL_Texture *nums[10];
    loadTextures(render,nums,"num",10);
    SDL_Rect numrect={0, 0, 7*scalefact, 7*scalefact};

    //variables de animacion
    Uint32 currentTime=SDL_GetTicks();
    Uint32 lastTime=SDL_GetTicks();

    //raycaster
    SDL_Surface *ghostssurf[4];
    if (paccaster){
        loadSurface(&ghostssurf[0],"ghost1");
        loadSurface(&ghostssurf[1],"ghost9");
        loadSurface(&ghostssurf[2],"ghost17");
        loadSurface(&ghostssurf[3],"ghost25");
    }

    //loop principal
    int quit = 0;

    //inicia el sonido de waka waka
    playMusic(music);
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
                if(key[SDL_SCANCODE_G])
                    guardar_matriz("save.txt", &M, &n, &m, &pelletcount, pp, gp);
                if (paccaster){//en modo paccaster usa flechas para doblar
                    if(key[SDL_SCANCODE_LEFT])
                        npdir=(npdir+1)%4;
                    if(key[SDL_SCANCODE_RIGHT])
                        npdir=(npdir+3)%4;
                    if(key[SDL_SCANCODE_DOWN])
                        npdir=(npdir+2)%4;
                }
            }
        }

        //GAMEPLAY
        if (!perdi){

        pactime=SDL_GetTicks()-paclast;
        if (pactime>=pacloop){
            paclast+=pacloop;
            pactime-=pacloop;

            ppdir=pdir; //previous dir

            int ppellet=pelletcount;//numero previo de pellet count
            avance(M, pdir, &pp[0], n, &pelletcount, true);
            salto(pp,n,m);
            if (inmatrix(n,m,pp) && mov(pp,M,npdir)) pdir=npdir;

            //si no comio pellet apaga musica, si no sigue
            if (ppellet==pelletcount)
                pauseMusic();
            else
                continueMusic();

            if (perder(gn,gp,pp,gdir,pdir)){//al perder activa perdi y deja pacmanframe donde empieza la animacionde perder
                perdi=1;
                pacmanframe=9;
                pauseMusic();
                playSound(sound);
            }
        }

        gtime=SDL_GetTicks()-glast;
        if (gtime>=gloop){
            glast+=gloop;
            gtime-=gloop;

            for (int g=0; g<gn; g++){
                ngdir[g]=rand()%4;
                avance(M, gdir[g], &gp[2*g], n, &pelletcount, false);
                salto(&gp[2*g],n,m);
                if (inmatrix(n,m,&gp[2*g]) && mov(&gp[2*g],M,ngdir[g])) gdir[g]=ngdir[g];
            }
        }

        }
        
        //RENDER
        //limpia la pantalla
        SDL_SetRenderDrawColor(render,0,0,0,255);
        SDL_RenderClear(render);

        //suaviza movimiento
        float paclerp=(mov(pp,M,pdir)) ? pactime/(float)pacloop : 0;
        if (paclerp>1) paclerp=1;
        float glerp=gtime/(float)gloop;
        if (glerp>1) glerp=1;

        if (paccaster)
            RenderCast(render,n,m,M,pp,pdir,paclerp,gp,gdir,glerp,ghostssurf,(float)(pdir+1)*pi/2,(float)(ppdir+1)*pi/2,pi/3,1);

        //Renderiza el laberinto completo
        SDL_RenderCopy(render, LABTEXTURE, NULL, &labrect);

        //renderiza los puntos
        SDL_RenderCopy(render, oneup, NULL, &oneuprect);
        DrawPoints(render,nums,numrect,pelletcount);

        //dibuja pellets
        if (!perdi)
            DrawPellets(render,n,m,M,pellettxt,pelletrect);

        //cuenta los frames
        currentTime=SDL_GetTicks();
        if (currentTime>lastTime+looptime){
            lastTime=currentTime;

            if (!perdi){
                //pacman frame
                pacmanframe+=pacanimdir;
                if (pacmanframe==0 || pacmanframe==PACMAN_FRAMES-1) pacanimdir=-pacanimdir;

                //ghost frame
                ghostframe++;
                if (ghostframe==GHOST_FRAMES) ghostframe=0;
            }else if (pacmanframe<TOTAL_PACMAN_FRAMES+4)
                pacmanframe++;
            else
                return 1; //sale del juego despues de perder
        }
        if (pacmanframe<TOTAL_PACMAN_FRAMES){
            //renderiza a pacman
            DrawPacman(render,pp,pacmantxt,pacmanframe,paclerp,pdir,pacmanrect);
        }
        
        if (!perdi){
            //renderiza fantasmas
            DrawGhosts(render,gp,gn,ghosttxt,ghostframe,glerp,gdir,ghostrect,M);
        }

        //actualizar el render
        SDL_RenderPresent(render);
    }

    //cerrar sdl
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}