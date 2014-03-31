#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL.h>

#define YMAX 10  // MIN 0 !!!
#define YMIN -10 // MAX 0 !!!
#define XMAX 5  // MIN 0 !!!
#define XMIN 0 // MAX 0 !!!

#define HAUTEUR 800
#define LARGEUR 1500

#define UNITEVERTICALE HAUTEUR/(YMAX-YMIN)
#define ORIGINEY UNITEVERTICALE*YMAX

#define UNITEHORIZONTALE LARGEUR/(XMAX-XMIN)
#define ORIGINEX UNITEHORIZONTALE*fabs(XMIN)

#define PAS (XMAX-XMIN)/LARGEUR

double f(double x){ return 5*cos(15*x)*sin(x); }
double g(double x){ return (10*cos(15*x)*exp(-x)+9*exp(-(x-5)*(x-5)/0.5)); }
double h(double x){ return (x!=0)?(1/x):-2; }

void drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2, int R, int G, int B);  // Bresenham
unsigned long Color(int R, int G, int B);
void PutPixel(SDL_Surface *surface, int x, int y, unsigned long pixel);

void plot(SDL_Surface*, double (*function)(double));
void pause();
int adapt(int);

int main(int argc, char *argv[])
{

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface *ecran = NULL;
    ecran = SDL_SetVideoMode(LARGEUR, HAUTEUR, 32, SDL_HWSURFACE);

    if (ecran == NULL)
    {
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_FillRect(ecran, NULL, SDL_MapRGB(ecran->format, 255, 255, 255));
    SDL_Flip(ecran);


    SDL_Surface *rectangle = NULL;
    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, LARGEUR, 1, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    SDL_Rect position;
    position.x = 0;
    position.y = ORIGINEY;

    SDL_BlitSurface(rectangle, NULL, ecran, &position);
    SDL_Flip(ecran);

    rectangle = SDL_CreateRGBSurface(SDL_HWSURFACE, 1, HAUTEUR, 32, 0, 0, 0, 0);
    SDL_FillRect(rectangle, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    position.x = ORIGINEX;
    position.y = 0;

    SDL_BlitSurface(rectangle, NULL, ecran, &position);
    SDL_Flip(ecran);

    SDL_Surface *fibre = NULL;
    fibre = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 11, 32, 0, 0, 0, 0);
    SDL_FillRect(fibre, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

     for( int i = ORIGINEX ; i < LARGEUR ; i += UNITEHORIZONTALE){
            position.x = i;
            position.y = (ORIGINEY - 5);
            (SDL_BlitSurface(fibre, NULL, ecran, &position));

            SDL_Flip(ecran);
    }

    for( int i = ORIGINEX ; i > 0 ; i -= UNITEHORIZONTALE){
            position.x = i;
            position.y = (ORIGINEY - 5);
            (SDL_BlitSurface(fibre, NULL, ecran, &position));

            SDL_Flip(ecran);
    }

    fibre = SDL_CreateRGBSurface(SDL_HWSURFACE, 11, 2, 32, 0, 0, 0, 0);
    SDL_FillRect(fibre, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

     for( int i = ORIGINEY ; i < HAUTEUR ; i += UNITEVERTICALE){
            position.x = ORIGINEX-5;
            position.y = (i);
            (SDL_BlitSurface(fibre, NULL, ecran, &position));

            SDL_Flip(ecran);
    }

      for( int i = ORIGINEY ; i > 0 ; i -= UNITEVERTICALE){
            position.x = ORIGINEX-5;
            position.y = (i);
            (SDL_BlitSurface(fibre, NULL, ecran, &position));

            SDL_Flip(ecran);
    }


    //-----

    double (*function)(double);

    //*
    function = f;
    plot(ecran, function);
    //*/
    /*
    function = g;
    plot(ecran, function);
    //*/
    /*
    function = h;
    plot(ecran, function);
    //*/
    //--

    pause();
    SDL_FreeSurface(fibre);
    SDL_FreeSurface(rectangle);
    SDL_Quit();

    return EXIT_SUCCESS;
}

void plot(SDL_Surface *ecran, double (*function)(double)){

    SDL_Rect position;
    SDL_Rect positionPrecedente;
    SDL_Surface *point = NULL;
    point = SDL_CreateRGBSurface(SDL_HWSURFACE, 2, 2, 32, 0, 0, 0, 0);
    SDL_FillRect(point, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));

    double v = 0;


    for(int i = 0 ; i < LARGEUR ; i++){

            position.x = i;
            v = (double)i - ORIGINEX;
            v = (int)(ORIGINEY - function(v*(double)PAS)*(ORIGINEY / YMAX));
            position.y = v;

            if(v < HAUTEUR && v >= 0 && positionPrecedente.y < HAUTEUR && positionPrecedente.y >= 0 ){
                //(SDL_BlitSurface(point, NULL, ecran, &position));
                //*
                if( i != 0 )
                    drawLine(ecran, positionPrecedente.x, positionPrecedente.y, position.x, position.y,255, 0, 0);//*/
            }

            SDL_Flip(ecran);

            positionPrecedente.x = position.x;
            positionPrecedente.y = position.y;
    }
        SDL_FreeSurface(point);
}

void pause()
{
    int continuer = 1;
    SDL_Event event;

    while (continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = 0;
        }
    }
}



unsigned long Color(int R, int G, int B)
{//Convertit RGB en long
	return 65536*R + 256*G + B;
}

void PutPixel(SDL_Surface *surface, int x, int y, unsigned long pixel)
{
	int bpp = surface->format->BytesPerPixel;
	unsigned char *p = (unsigned char *)surface->pixels + y * surface->pitch + x * bpp;
	if (bpp==4)
		*(unsigned long*)p = pixel;
}

void drawLine(SDL_Surface* surf,int x1,int y1, int x2,int y2, int R, int G, int B)  // Bresenham
{
	unsigned long couleur = Color(R, G, B);
	int x,y;
	int Dx,Dy;
	int xincr,yincr;
	int erreur;
	int i;

	Dx = abs(x2-x1);
	Dy = abs(y2-y1);
	if(x1<x2)
		xincr = 1;
	else
		xincr = -1;
	if(y1<y2)
		yincr = 1;
	else
		yincr = -1;

	x = x1;
	y = y1;
	if(Dx>Dy)
	{
		erreur = Dx/2;
		for(i=0;i<Dx;i++)
		{
			x += xincr;
			erreur += Dy;
			if(erreur>Dx)
			{
				erreur -= Dx;
				y += yincr;
			}
			PutPixel(surf,x, y,couleur);
		}
	}
	else
	{
		erreur = Dy/2;
		for(i=0;i<Dy;i++)
		{
			y += yincr;
			erreur += Dx;

			if(erreur>Dy)
			{
				erreur -= Dy;
				x += xincr;
			}
			PutPixel(surf,x, y,couleur);
		}
	}
}
