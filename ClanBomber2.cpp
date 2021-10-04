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
	num = -1 ;
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
	location () ;
	void move() ;
};

location :: location ()
{
	r = 2 ;
	bomb = 5 ;
	crs = 0 ;
	f = true ;
}

location player [4] ;
Bomb Bomb [100] ;

void copyImage ( SDL_Surface * src , int x , int y , SDL_Surface * scr , int xx , int yy , int p = 0 , int q = 0 )
{
        SDL_Rect rect ;
        rect.x = x * 40  ;
        rect.y = y * 40 ;
        rect.w = 40 ;
        rect.h = 40 ;
        
        SDL_Rect rect2 ;
        rect2.x = ( xx * 40 ) + p ;
        rect2.y = ( yy * 40 ) + q ;
        rect2.w = 40 ;
        rect2.h = 40 ;
        
        SDL_BlitSurface ( src , &rect , scr , &rect2 ) ;
}

void Pic ( int x , int y , SDL_Surface * scr , int xx , int yy , int k , int p = 0 , int q = 0 )
{
        SDL_Surface * player ;
	char c[100] ;
	sprintf ( c , "pics/%d.bmp" , k ) ;
	player = SDL_LoadBMP(c) ;
	
	SDL_SetColorKey ( player , SDL_SRCCOLORKEY , SDL_MapRGB ( player -> format , 255 , 255 , 255 ) ) ;
	
	copyImage ( player , x , y , scr , xx , yy , p , q ) ;
}

void getmap ( string s ) 
{
	for ( int i = 0 ; i < s.length() ; i++ )
	{
		title = SDL_LoadBMP ("pics/maptiles.bmp") ;
		if ( s[i] == '0' )
	        {
	                copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			Pic ( 0 , 0 , screen , i , j , 0 ) ;
			playernum++ ;
			map[i][j] = -3 ;
			player[0].x = i ;
			player[0].y = j ;
			player[0].num = 0 ;
		}
		else if ( s[i] == '1' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			Pic ( 0 , 0 , screen , i , j , 1 ) ;
			playernum++ ;
			map[i][j] = -3 ;
			player[1].x = i ;
			player[1].y = j ;
			player[1].num = 1 ;
		}
		else if ( s[i] == '2' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			Pic ( 0 , 0 , screen , i , j , 2 ) ;
			playernum++ ;
			map[i][j] = -3 ;
			player[2].x = i ;
			player[2].y = j ;
			player[2].num = 2 ;
		}
		else if ( s[i] == '3' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;      
			Pic ( 0 , 0	 , screen , i , j , 3 ) ;
			playernum++ ;
			map[i][j] = -3 ;
			player[3].x = i ;
			player[3].y = j ;
			player[3].num = 3 ;
		}
		else if ( s[i] >= '4' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;      
			map[i][j] = -3 ;
		}
/////////////////////////////////////////////////////
		if ( s[i] == '-' )
		{
		        map[i][j] = 404 ;
		}
		else if ( s[i] == ' ' ) //empty
		{
			copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			map[i][j] = -3 ;
		}
		else if ( s[i] == '*' ) //stoneWall
		{
			copyImage ( title , model * 4 + 1 , 0 , screen , i , j ) ;
			map[i][j] = -2 ;
		}
		else if ( s[i] == '+' ) //zepertWall
		{
			copyImage ( title , model * 4 + 2 , 0 , screen , i , j ) ;
			map[i][j] = -1 ;
		}
	}
	j++ ;
	s = "" ;
}

void SetBomb ( int n )
{
	if ( player[n].bomb > 0 )
	{
		for ( int i = 0 ; i < 100 ; i++ )
		{
			if ( Bomb[i].t == 0 )
			{
				Bomb[i].t = 200 ;
				Bomb[i].x = player[n].x ;
				Bomb[i].y = player[n].y ;
				Bomb[i].r = player[n].r ;
				Bomb[i].num = n ;
				player[n].bomb-- ;
				return ;
			}
		}
	}
}

void Exp ( int x , int y , int i , int j , int r , int n )
{
	int r1 = r ;
	while ( r1 >= 0 )
	{
		if ( map[x][y] == -3 )
		{
			Pic ( n , r , screen , x , y , 8 ) ;
			if ( map[x][y] == -1 ) map[x][y] = 0 ;
			x += i ;
			y += j ;
			
		}
		else 
		{	
			Pic ( n , r , screen , x , y , 8 ) ;
			if ( map[x][y] == -1 ) map[x][y] = 0 ;
			return ;
		}
		r1-- ;
	}
}

void Bomb :: Explosion ()
{
	if ( t > 0 )
	{
		t-- ;
	}
	if ( t < 140 && t > 135 )
	{
		Exp ( x , y ,  1 , 0 , 4 - r , 1 ) ;
		Exp ( x , y , -1 , 0 , 4 - r , 1 ) ;
		Exp ( x , y , 0 ,  1 , 4 - r , 4 ) ;
		Exp ( x , y , 0 , -1 , 4 - r , 4 ) ;
		Pic ( 6 , r , screen , x , y , 8 ) ;		
	}
	if ( t == 136 ) 
	{
		player[num].bomb++ ;
		for ( int j = -3 + r ; j <= 3 - r ; j++ )
		{
			if ( map[x][y+j] == 0 ) map[x][y+j] = -3 ;
			if ( map[x+j][y] == 0 ) map[x+j][y] = -3 ;
		}
	}
}

void MoveTo ( int n , int i , int j , int crs )
{
	player[n].x += i ;
	player[n].y += j ;
	player[n].crs = crs ;
}


void location :: move ()
{
	if ( f == true )
	{
		if ( num == 0 )
		{
			if ( e.key.keysym.sym == SDLK_a ) 
			{
				if ( map[x - 1][y] % 3 == 0 ) MoveTo (num,-1,0,1) ;
			}
			if ( e.key.keysym.sym == SDLK_w )
			{
				if ( map[x][y - 1] % 3 == 0 ) MoveTo (num,0,-1,2) ;
			}
			if ( e.key.keysym.sym == SDLK_d )
			{
				if ( map[x + 1][y] % 3 == 0 ) MoveTo (num,1,0,3) ;
			}
			if ( e.key.keysym.sym == SDLK_s )
			{
				if ( map[x][y + 1] % 3 == 0 ) MoveTo (num,0,1,0) ;
			}
			if ( e.key.keysym.sym == SDLK_q )
			{
				SetBomb ( num ) ;
			}
		}
		if ( num == 1 )
		{
			if ( e.key.keysym.sym == SDLK_LEFT )
			{
				if ( map[x - 1][y] % 3 == 0 ) MoveTo (num,-1,0,1) ;
			}
			if ( e.key.keysym.sym == SDLK_UP )
			{
				if ( map[x][y - 1] % 3 == 0 ) MoveTo (num,0,-1,2) ;
			}
			if ( e.key.keysym.sym == SDLK_RIGHT )
			{
				if ( map[x + 1][y] % 3 == 0 ) MoveTo (num,1,0,3) ;
			}
			if ( e.key.keysym.sym == SDLK_DOWN )
			{
				if ( map[x][y + 1] % 3 == 0 ) MoveTo (num,0,1,0) ;
			}
			if ( e.key.keysym.sym == SDLK_m )
			{
				SetBomb ( num ) ;
			}
		}
	}
}

void DrawAll ()
{
	for ( int i = 0 ; i < 17 ; i++ )
	{
		for ( int j = 0 ; j < 13 ; j++ )
		{
			copyImage ( title , model * 4 + 3 + map[i][j] , 0 , screen , i , j ) ;
		}
	}
	if ( bon_f == true ) Pic ( bon_m , 0 , screen , bon_x , bon_y , 6 ) ;
	LR++ ;
	LR %= 2 ;
	for ( int i = 0 ; i < 100 ; i++ )
	{
		if ( Bomb[i].t > 140 ) 
		{
			if ( Bomb[i].num == 1 || Bomb[i].num == 0 ) Pic ( 0 + LR , 2 + Bomb[i].num , screen , Bomb[i].x , Bomb[i].y , 9 ) ;
			if ( Bomb[i].num == 2 || Bomb[i].num == 3 ) Pic ( 4 + LR , 2 + Bomb[i].num , screen , Bomb[i].x , Bomb[i].y , 9 ) ;
		}
		Bomb[i].Explosion() ;
	}
	for ( int i = 0 ; i < 4 ; i++ )
	{
		if ( player[i].f == true ) 
		{
			Pic ( LR , player[i].crs , screen , player[i].x , player[i].y , player[i].num ) ;
			SDL_Delay ( 20 ) ;
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
        ifstream fin ( "maps/Arena.map" ) ;
        	getline ( fin , s ) ;
       		getline ( fin , s ) ;
		while ( fin.eof() == 0 )
                {
        		getline ( fin , s ) ;
        		getmap ( s ) ;   
                }
        fin.close();
        SDL_Flip ( screen ) ;
        while ( e.type != SDL_QUIT )
        {
        	ELR++ ;
        	ELR %= 50 ;
		if ( e.type == SDL_KEYDOWN ) 
			for ( int i = 0 ; i < 4 ; i++ ) player[i].move() ;
		DrawAll () ;
		SDL_Flip ( screen ) ;
                SDL_PollEvent( &e ) ;
                if ( e.type == SDL_QUIT ) break ;
        }
        SDL_Quit () ;
        return 0 ; 
}




















