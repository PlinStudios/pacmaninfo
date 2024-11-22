#include <SDL2/SDL.h>
#include <math.h>

#define pi 3.1452f

void loadSurface(SDL_Surface** surface, const char* name){
    char path[50];
    snprintf(path, sizeof(path), "data/%s.bmp",name);
    *surface = SDL_LoadBMP(path);
    SDL_LockSurface(*surface);
}

Uint32 getpixel(SDL_Surface *surface, int x, int y)
{
    //de https://stackoverflow.com/questions/53033971/how-to-get-the-color-of-a-specific-pixel-from-sdl-surface
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    return p[0] | p[1] << 8 | p[2] << 16;
}

int getPixelRGB(SDL_Surface *surface, int x, int y, Uint8 *r, Uint8 *g, Uint8 *b) {
    //obrener pixel en x,y
    Uint32 *pixels = (Uint32 *)surface->pixels;
    Uint32 pixel = getpixel(surface,x,y);

    // Get RGB values from pixel
    SDL_GetRGB(pixel, surface->format, r, g, b);

    return 0;
}

char isblock(char **MAP,int fils,int cols,float xf,float yf){
    int xx=(int)xf%fils;
    int yy=(int)yf%cols;
    if (xx<0) xx+=fils;
    if (yy<0) yy+=cols;

    return (MAP[xx][yy]==-1);
}

void RaycastDis(int fils, int cols, char **MAP, float x, float y, float dir, float *distance, float *lumi){
    float disx=9999;
    float disy=9999;
    float sind=sin(dir);
    float cosd=cos(dir);
    int xj,yj;
    //x case
    if (cosd!=0){
        //float fx=ceil(x)-x;
        float fx=(0<cosd) ? ceil(x)-x:x-floor(x)+0.000001;
        xj=(0<cosd) ? 1:-1;
        float xx=x+fx*xj;
        float yy=y+fx*xj*sind/cosd;
        while (1){
            if (isblock(MAP,fils,cols,xx,yy)){
                float dx=xx-x;
                float dy=yy-y;
                disx=sqrt(dx*dx+dy*dy);
                break;
            }
            xx+=xj;
            yy+=xj*sind/cosd;
        }
    }
    //y case
    if (sind!=0){
        //float fy=ceil(y)-y;
        float fy=(0<sind) ? ceil(y)-y:y-floor(y)+0.000001;
        yj=(0<sind) ? 1:-1;
        float yy=y+fy*yj;
        float xx=x+fy*yj*cosd/sind;
        while (1){
            if (isblock(MAP,fils,cols,xx,yy)){
                float dx=xx-x;
                float dy=yy-y;
                disy=sqrt(dx*dx+dy*dy);
                break;
            }
            yy+=yj;
            xx+=yj*cosd/sind;
        }
    }
    //reuturn distance and luminance
    if (disx<disy){
        *distance=disx;
        *lumi=xj*cos(dir);
    }else{
        *distance=disy;
        *lumi=yj*sin(dir);
    }
}

void RenderCast(SDL_Renderer *render, int fils, int cols, char **MAP, int ppi[2], int pdir, float plerp, int gpi[8], int ghostdir[8], float glerp, SDL_Surface *ghosts[4], float currentdir, float previoudir, float fov, int step){
    //draw background
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 0, 0, 255, 255);

    float dir=currentdir*plerp + previoudir*(1-plerp);

    float x,y;
    float gp[8];
    x=ppi[0]+0.5 + ((pdir==3)-(pdir==1))*plerp;
    y=ppi[1]+0.5 + ((pdir==0)-(pdir==2))*plerp;
    for (int g=0; g<4; g++){
        gp[2*g]  =gpi[2*g] + ((ghostdir[g]==3)-(ghostdir[g]==1))*glerp;
        gp[2*g+1]=gpi[2*g+1] + ((ghostdir[g]==0)-(ghostdir[g]==2))*glerp;
    }

    float relang;
    float ang;
    float sz;
    for (int a=0; a<640; a+=step){
        ang=-fov*(a-320)/320;
        relang=dir+ang;
        if (relang>pi) relang-=2*pi;
        if (relang<-pi) relang+=2*pi;
        float dis;
        float lumi;
        RaycastDis(fils,cols,MAP,x,y,relang,&dis,&lumi);
        SDL_SetRenderDrawColor(render, 0, 0, 255*(0.2+lumi*0.8), 255);
        sz=1/(dis*cos(ang))*240;
        SDL_RenderDrawLine(render,a,240-sz,a,240+sz);

        //draw ghosts
        int nearest=9999; //distancia del fantasma mas cercano, para no dibujar encima
        for (int mghost = 0; mghost < 4; mghost++) {
            float dx = gp[2*mghost]+0.5f - x;
            float dy = gp[2*mghost+1]+0.5f - y;
            float gdis = sqrt(dx*dx+dy*dy);
            //saltar si el fantama esta detras de una pared
            if (gdis>dis) continue;

            // Calcular el ángulo relativo del fantasma respecto al jugador
            float gang = atan2(dy, dx);
            float regang = gang - relang;

            // Ajustar el ángulo relativo para que esté en el rango (-PI, PI)
            if (regang>pi) regang-=2*pi;
            if (regang<-pi) regang+=2*pi;

            // Solo dibujar fantasmas dentro del campo de visión
            float fgsz=30/gdis*pi/180;
            if (fabs(regang)<fgsz) {
                int gsz =240/gdis;
                int xx=7*(regang+fgsz)/fgsz; //x en la textura del fantasma (son 14x14)
                float pix=gsz/7*0.7; //equivalente a un pixel del fantasma en vertical

                //saltar si esta más cerca que otro para no dibujar encima
                if (gdis<nearest)
                    nearest=gdis;
                else
                    continue;

                for (int k=0; k<14; k++){
                    Uint8 r,g,b;
                    getPixelRGB(ghosts[mghost],xx,k,&r,&g,&b);
                    if (r!=0 || g!=0 || b!=0){
                        SDL_SetRenderDrawColor(render, r, g, b, 255);
                        SDL_RenderDrawLine(render,a,240+(k-7)*pix,a,240+(k-6)*pix);
                    }
                }
            }
        }
    }
}