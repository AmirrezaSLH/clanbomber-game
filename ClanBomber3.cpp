#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <SDL/SDL.h>
#include <cstdlib>
#include <fstream>

using namespace std ;

SDL_Surface * screen = SDL_SetVideoMode ( 680 , 520 , 32 , SDL_SWSURFACE ) ;
SDL_Surface * title ;
string s = "" ;
SDL_Event e ;
int j = 0 , model = 1 , playernum = 0 , LR = 0 , ELR = 0 ;
int map[100][100] ;
int bon_x , bon_y , bon_m ;
bool bon_f = false ;

struct Bomb 
{
	int x , y ;
	int t ;
	int r ;	
	int num ;
	Bomb () ;
	void Explosion () ;
};

Bomb :: Bomb ()
{
	x = 0 ;
	y = 0 ;
	t = 0 ;
	r = 0 ;
	num = 0 ;
}

struct location
{
	bool f ;
	int x , y ;
	int crs ;
	int dx , dy ; 
	int bomb ;
	int r ;
	int num ;
	int LR ;
	location () ;
	void move() ;
	void Run () ;
};

location :: location ()
{
	r = 2 ;
	LR = 0 ;
	bomb = 5 ;
	crs = 0 ;
	f = true ;
}

location player [4] ;
Bomb Bomb [100] ;

void copyImage ( SDL_Surface * src , int x , int y , SDL_Surface * scr , int xx , int yy , int i = 0 , int j = 0 )
{
        SDL_Rect rect ;
        rect.x = x * 40  ;
        rect.y = y * 40 ;
        rect.w = 40 ;
        rect.h = 40 ;
        
        SDL_Rect rect2 ;
        rect2.x = ( xx * 40 ) + i ;
        rect2.y = ( yy * 40 ) + j ;
        rect2.w = 40 ;
        rect2.h = 40 ;
        
        SDL_BlitSurface ( src , &rect , scr , &rect2 ) ;
}

void Pic ( int x , int y , SDL_Surface * scr , int xx , int yy , int k , int i = 0 , int j = 0 )
{
        SDL_Surface * player ;
	char c[100] ;
	sprintf ( c , "pics/%d.bmp" , k ) ;
	player = SDL_LoadBMP(c) ;
	
	SDL_SetColorKey ( player , SDL_SRCCOLORKEY , SDL_MapRGB ( player -> format , 255 , 255 , 255 ) ) ;
	
	copyImage ( player , x , y , scr , xx , yy , i , j ) ;
}

void getmap ( string s ) 
{
	for ( int i = 0 ; i < s.length() ; i++ )
	{
		title = SDL_LoadBMP ("pics/maptiles.bmp") ;
		if ( s[i] == '0' )
	        {
			Pic ( 0 , 0 , screen , i , j , 0 ) ;
			playernum++ ;
			map[i][j] = -3 ;
			player[0].x = i ;
			player[0].y = j ;
			player[0].num = 0 ;
		}
	}
	j++ ;
	s = "" ;
}

void MoveTo ( int n , int i , int j , int crs )
{
	for ( int k = 0 ; k < 40 ; k++ )
	{
		player[n].dx += i ;
		player[n].dy += j ;
		player[n].crs = crs ;
		player[n].LR++ ;
		player[n].LR %= 2 ;
		Pic ( player[n].LR , player[n].crs , screen , player[n].x , player[n].y , n , player[n].dx , player[n].dy ) ;
		SDL_Delay( 15 );
		SDL_Flip ( screen ) ;
	}
	player[n].x += i ;
	player[n].y += j ;
	player[n].dx = 0 ;
	player[n].dy = 0 ;
}

void location :: move ()
{
	if ( f == true )
	{
		if ( num == 0 )
		{
			if ( e.key.keysym.sym == SDLK_a ) 
			{
				MoveTo (num,-1,0,1) ;
			}
			if ( e.key.keysym.sym == SDLK_w )
			{
				MoveTo (num,0,-1,2) ;
			}
			if ( e.key.keysym.sym == SDLK_d )
			{
				MoveTo (num,1,0,3) ;
			}
			if ( e.key.keysym.sym == SDLK_s )
			{
				MoveTo (num,0,1,0) ;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//								   ///
//								   ///
//	()	    \\                               ()		   ///
//		   \\\\\                              		   ///
//	\\  \\\\\   \\	        \\\\\\\\\    \\\     \\  \\\\\	   ///
//	\\  \\  \\  \\   	\\  \\  \\  \\  \\   \\  \\  \\	   ///
//	\\  \\  \\  \\		\\  \\  \\   \\\\\   \\  \\  \\	   ///
//								   ///
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main()
{
	SDL_Init ( SDL_INIT_EVERYTHING ) ;
	int rand = 40 ;
	srand ( time( NULL ) ) ;
        ifstream fin ( "maps/Football.map" ) ;
        	getline ( fin , s ) ;
       		getline ( fin , s ) ;
		while ( fin.eof() == 0 )
                {
        		getline ( fin , s ) ;
        		getmap ( s ) ;   
                }
        fin.close();
        char z ;
        SDL_Flip ( screen ) ;
        while ( e.type != SDL_QUIT )
        {
        	SDL_PollEvent( &e ) ;
		SDL_Flip ( screen ) ;
		SDL_PollEvent( &e ) ;
		player[0].move () ;
                if ( e.type == SDL_QUIT ) break ;
        }
        SDL_Delay ( 2000 ) ;
        SDL_Quit () ;
        return 0 ; 
}




















