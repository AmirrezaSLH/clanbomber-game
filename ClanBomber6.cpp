#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

using namespace std ;

SDL_Surface *screen = SDL_SetVideoMode ( 1000 , 800 , 32 , SDL_SWSURFACE ) ;
SDL_Surface *title ;
SDL_Surface *screen2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 1000, 800, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
SDL_Surface *message = NULL ;
Mix_Music *music = NULL;
Mix_Music *bomb = NULL;
Mix_Music *explosion = NULL;
Mix_Music *click = NULL;
const int NP = 2 ;
SDL_Color textColor = { 0, 255, 0 } , bgColor = { 0, 0, 0 } ;
string s = "" , s1 = "" ;
SDL_Event e ;
int j = 0 , model = 1 , playernum = 0 ;
int map[100][100] ;
string map2[100] ; 	 
bool MapFlag = false ;
bool finish = false ;

struct location
{
	SDL_Event event ;
	bool f ;
	int x ,y ,crs ,b ,r ,LR ,num ,tspd ,spd ,dx ,dy ;
	int dt ;
	int Won ;
	location () ;
	void Move() ;
	void MoveTo () ;
	void Clear () ;
};

location :: location ()
{
	dx = 0 ;
	dy = 0 ;
	LR = 0 ;
	spd = 1 ;
	r = 2 ;
	b = 5 ;
	dt = 0 ;
	crs = 0 ;
	Won = 0 ;
	f = true ;
}
struct Bomb 
{
	int x ,y ,t ,r ,LR ,num ;
	Bomb () ;
	void Explosion () ;
	void Clear () ;
};

Bomb :: Bomb ()
{
	x = 0 ;
	y = 0 ;
	t = 0 ;
	r = 0 ;
	LR = 0 ;
	num = -1 ;
}

struct Prize
{
	int x ,y ,r ;
	Prize () ;
	void CheckPrize () ;
	void Clear () ;
};

Prize :: Prize ()
{
	r = 0 ;
	x = -1 ;
	y = -1 ;
}

Bomb Bomb[100] ;
location player[4] ;
Prize prz[100] ;

void Bomb :: Clear ()
{
	t = 0 ;
	x = 0 ;
	y = 0 ;
	num = -1 ;
	r = 0 ;
}

void Prize :: Clear ()
{
	x = 0 ;
	y = 0 ;
	r = 0 ;
}

void location :: Clear ()
{
	dx = 0 ;
	dy = 0 ;
	LR = 0 ;
	spd = 1 ;
	r = 2 ;
	b = 5 ;
	dt = 0 ;
	crs = 0 ;
	Won = 0 ;
	f = true ;
}

void copyImage ( SDL_Surface * src , int x , int y , SDL_Surface * scr , int xx , int yy , int i = 0 , int j = 0 )
{
        SDL_Rect rect ;
        rect.x = x ;
        rect.y = y ;
        rect.w = 40 + i ;
        rect.h = 40 + j ;
        SDL_Rect rect2 ;
	rect2.x = xx + 160 ;
	rect2.y = yy + 140 ;
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

void SetPlayer ( int i , int j )
{
	copyImage ( title , ( model * 4 + 0 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
	Pic ( 0 , 0 , screen , i * 40 , j * 40 , playernum ) ;
	map[i][j] = -3 ;
	player[playernum].x = i * 40 ;
	player[playernum].y = j * 40 ;
	player[playernum].num = playernum ;
	playernum++ ;
	return ;
}

void getmap ( char *c ) 
{
	ifstream fin ( c ) ;
		getline ( fin , s ) ;
		getline ( fin , s ) ;
		while ( fin.eof() == 0 )
		{
	 		getline ( fin , s ) ;
			for ( int i = 0 ; i < s.length() ; i++ )
			{
				title = SDL_LoadBMP ("pics/maptiles.bmp") ;
				if ( s[i] == '0' )SetPlayer ( i, j );
				else if ( s[i] == '1' ) SetPlayer ( i, j );
				else if ( s[i] == '2' ) SetPlayer ( i, j );
				else if ( s[i] == '3' ) SetPlayer ( i, j );
				else if ( s[i] >= '4' ) 
				{
				        copyImage ( title , ( model * 4 + 0 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;      
					map[i][j] = -3 ;
				}
				if ( s[i] == '-' )
				{
					map[i][j] = 405 ;
				}
				else if ( s[i] == ' ' ) //empty
				{
					copyImage ( title , ( model * 4 + 0 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
					map[i][j] = -3 ;
				}
				else if ( s[i] == '*' ) //stoneWall
				{
					copyImage ( title , ( model * 4 + 1 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
					map[i][j] = -2 ;
				}
				else if ( s[i] == '+' ) //zepertWall
				{
					copyImage ( title , ( model * 4 + 2 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
					map[i][j] = -1 ;
				}
			}
			j++ ;
			s = "" ;
	        }
	fin.close();
}

void SetPrize ( int x1 , int y1 )
{
	for ( int i = 0 ; i < 100 ; i++ )
	{
		if ( prz[i].x < 0 && prz[i].y < 0 )
		{
			prz[i].x = x1 ;
			prz[i].y = y1 ;
			prz[i].r = rand() % 8 ;
			return ;
		}
	}
}

void Prize :: CheckPrize ()
{
	for ( int i = 0 ; i < NP ; i++ )
	{
		if ( x == player[i].x/40 && y == player[i].y/40 && player[i].dt == 0 )
		{
			if ( r == 0 ) 
			{
				player[i].b++ ;
				x = -1 ;
				y = -1 ;	
			}
			if ( r == 1 && player[i].r > 0) 
			{
				player[i].r-- ;
				x = -1 ;
				y = -1 ;
			}
			if ( r == 2 && player[i].spd == 1 ) 
			{
				player[i].tspd = 350 ;
				player[i].spd *= 2 ;
				x = -1 ;
				y = -1 ;
			}
			if ( r == 6 ) 
			{
				player[i].tspd = 350 ;
				player[i].spd *= -1 ;
				x = -1 ;
				y = -1 ;
			}
		}
	}
}

void DrawAll ()
{
	Pic ( 0 , 0 , screen , 0 , 0 , 14 , 1000 - 160 , 800 - 160 );
	for ( int i = 0 ; i < 17 ; i++ )
		for ( int j = 0 ; j < 13 ; j++ )
			copyImage ( title , ( model * 4 + 3 + map[i][j] ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
	for ( int i = 0 ; i < 100 ; i++ )
		if ( prz[i].x > 0 && prz[i].y > 0 && ( prz[i].r < 3 || prz[i].r == 6 ) ) 
		{	
			Pic ( prz[i].r * 40 , 0 , screen , prz[i].x * 40 , prz[i].y * 40 , 7 ) ;
			prz[i].CheckPrize () ;
		}
	for ( int i = 0 ; i < 100 ; i++ )
		if ( Bomb[i].t > 0 ) Bomb[i].Explosion() ;
	for ( int i = 0 ; i < NP ; i++ ) 
	{
		player[i].MoveTo() ;	
	}
}

void ResetAll ()
{
	string s ;
	for ( int i = 0 ; i < 100 ; i++ )
	{
		Bomb[i].Clear () ;
		prz[i].Clear () ;
	}
	for ( int i = 0 ; i < NP ; i++ )
	{
		player[i].Clear () ;
	}
	s = "map/Kitchen.map" ;
	char c[100] ;
	for ( int i = 0 ; i < 100 ; i++ ) c[i] = s[i] ;
	getmap ( c ) ;
}

void CheckPlayer ( int x , int y , int n , bool f = true ) 
{
	if ( f == true )
	{
		for ( int i = 0 ; i < NP ; i++ )
		{
			for ( int j = -3 + n ; j <= 3 - n ; j++ )
			{
				if ( player[i].x/40 == x+j && player[i].y/40 ==  y  ) player[i].f = false ;
				if ( player[i].x/40 ==  x  && player[i].y/40 == y+j ) player[i].f = false ;
			}
		}
	}
	else
	{
		for ( int i = 0 ; i < NP ; i++ ) if ( map[ player[i].x/40 ][ player[i].y/40 ] == 405 ) player[i].f = false ;
	}
}

void SetBomb ( int n )
{
	if ( player[n].b > 0 )
	{
		for ( int i = 0 ; i < 100 ; i++ )
		{
			if ( ( Bomb[i].x != player[n].x / 40 ) && ( Bomb[i].y != player[n].y / 40 ) && ( Bomb[i].t == 0 ) ) 
			{
				Bomb[i].t = 200 ;
				Bomb[i].x = player[n].x / 40 ;
				Bomb[i].y = player[n].y / 40 ;
				for ( int j = 0 ; j < 100 ; j++ ) 
					if ( Bomb[j].x == Bomb[i].x && Bomb[j].y == Bomb[i].y && i != j ) 
					{	
						Bomb[i].x = 0 ;
						Bomb[i].y = 0 ;
						Bomb[i].t = 0 ;
						return ;
					}
				Bomb[i].r = player[n].r ;
				Bomb[i].num = n ;
				player[n].b-- ;
				return ;	
			}
		}
	}
}

void Exp ( int x , int y , int i , int j , int r , int n , int t )
{
	int r1 = 4 - r ;
	int t2 = t - 131 ;	
	int t1 = 2 ;
	if ( t2 > 0 && t2 <= 3 ) t1 = 2 ;
	else if ( t2 > 3 && t2 <= 6 ) t1 = 1 ;
	else if ( t2 > 7 && t2 <= 9 ) t1 = 0 ;
	while ( r1 > 0 )
	{
		for ( int p = 0 ; p < 100 ; p++ )
		{
			if ( Bomb[p].x == x && Bomb[p].y == y && r1 < 4 - r ) Bomb[p].t = 1 ;
		}
		if ( map[x][y] == 405 ) return ;
		if ( map[x][y] == -3 )
		{	
			for ( int k = 0 ; k < 100 ; k++ ) 
				if ( prz[k].x == x && prz[k].y == y ) 
				{
					prz[k].x = -1 ;
					prz[k].y = -1 ;
				}
			if ( r1 == 1 )
			{
				if ( i < 0 ) Pic ( 0 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( i > 0 ) Pic ( 2 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( j < 0 ) Pic ( 3 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( j > 0 ) Pic ( 5 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
				return ;
			}
			Pic ( n * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
			x += i ;
			y += j ;
		}
		else 
		{			
			if ( i < 0 ) Pic ( 0 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( i > 0 ) Pic ( 2 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( j < 0 ) Pic ( 3 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( j > 0 ) Pic ( 5 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( map[x][y] == -1 ) map[x][y] = 0 ;
			if ( t == 131 && map[x][y] == 0 ) 
			{
				int random = rand() % 3 ;
				if ( random % 3 != 0 ) SetPrize ( x , y ) ;
				map[x][y] = -3 ;
				return ;
			}
		}
		r1-- ;
	}
}

void Bomb :: Explosion ()
{
	int t2 = t - 131 ;	
	int t1 = 2 ;
	if ( t2 > 0 && t2 <= 3 ) t1 = 2 ;
	else if ( t2 > 3 && t2 <= 6 ) t1 = 1 ;
	else if ( t2 > 7 && t2 <= 9 ) t1 = 0 ;
	LR++ ;
	LR %= 4 ;
	explosion = Mix_LoadMUS( "voice/Explosion1.wav" );
	if ( t > 140 )
	{
		if ( num == 1 || num == 0 ) Pic ( ( 0 + LR ) * 40 , ( 2 + num ) * 40 , screen , x * 40 , y * 40 , 9 ) ;
		if ( num == 2 || num == 3 ) Pic ( ( 4 + LR ) * 40 , ( 2 + num ) * 40 , screen , x * 40 , y * 40 , 9 ) ;
	}
	if ( t < 140 && t >= 131 )
	{
		if ( Mix_PlayingMusic() == 0 ) Mix_PlayMusic( explosion, -1 );
		Exp ( x , y ,  1 , 0 , r , 1 , t ) ;
		Exp ( x , y , -1 , 0 , r , 1 , t ) ;
		Exp ( x , y , 0 ,  1 , r , 4 , t ) ;
		Exp ( x , y , 0 , -1 , r , 4 , t ) ;
		Pic ( 6 * 40 , t1 * 40 , screen , x * 40 , y * 40 , 8 ) ;	
		CheckPlayer ( x , y , r ) ;	
	}
	if ( t == 131 ) 
	{
		x = 0 ;
		y = 0 ;
		player[num].b++ ;
	}
	if ( t == 127 )
	{
		Mix_HaltMusic () ;
		t = 1 ;
	}
	t-- ;
}

void SetMove ( int n, int x, int y, int crs, int spd )
{
	player[n].dx = x ;
	player[n].dy = y ;
	player[n].dt = 10 / abs(spd) ;
	player[n].crs = crs ;
}

void location :: MoveTo ()
{
	if ( dt > 0 )
	{
		x += dx * 4 * spd ;
		y += dy * 4 * spd ;
		LR++ ;
		LR %= 9 ;
		dt-- ;
	}
	if ( dt == 0 )
	{
		dx = 0 ;
		dy = 0 ;
	}
	if ( f == true ) Pic ( LR * 40 , crs * 40 , screen , x , y , num ) ;
	else Pic ( 9 * 40 , crs * 40 , screen , x , y , num ) ;
	if ( tspd >= 1 ) tspd-- ;
	if ( tspd == 0 ) spd = 1 ;
}

bool MovePlayer ( int x , int y )
{
	for ( int i = 0 ; i < 100 ; i++ ) 
		if ( Bomb[i].x == x && Bomb[i].y == y && Bomb[i].t > 120 ) return false ;
	for ( int i = 0 ; i < NP ; i++ )
		if ( player[i].x/40 == x && player[i].y/40 == y && player[i].f == true ) return false ;
	return true ;
}

void location :: Move ()
{	
	if ( f == true )
	{	
		if ( num == 1 && dt <= 0 )
		{
			if ( e.key.keysym.sym == SDLK_LEFT ) 
			{
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[1].spd > 0 ) SetMove (num,-1,0,1,spd) ;
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[1].spd < 0 ) SetMove (num,-1,0,3,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_UP )
			{
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[1].spd > 0 ) SetMove (num,0,-1,2,spd) ;
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[1].spd < 0 ) SetMove (num,0,-1,0,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_RIGHT )
			{
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[1].spd > 0 ) SetMove (num,1,0,3,spd) ;
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[1].spd < 0 ) SetMove (num,1,0,1,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_DOWN )
			{
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[1].spd > 0 ) SetMove (num,0,1,0,spd) ;
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[1].spd < 0 ) SetMove (num,0,1,2,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_m )
			{
				SetBomb ( num ) ;
			}
		}
		if ( num == 0 && dt <= 0 )
		{
			if ( e.key.keysym.sym == SDLK_a ) 
			{
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[0].spd > 0 ) SetMove (num,-1,0,1,spd) ;
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[0].spd < 0 ) SetMove (num,-1,0,3,spd) ;			
			}
			else if ( e.key.keysym.sym == SDLK_w )
			{
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[0].spd > 0 ) SetMove (num,0,-1,2,spd) ;
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[0].spd < 0 ) SetMove (num,0,-1,0,spd) ;			
			}
			else if ( e.key.keysym.sym == SDLK_d )
			{
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[0].spd > 0 ) SetMove (num,1,0,3,spd) ;
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[0].spd < 0 ) SetMove (num,1,0,1,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_s )
			{
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[0].spd > 0 ) SetMove (num,0,1,0,spd) ;
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[0].spd < 0 ) SetMove (num,0,1,2,spd) ;
			}
			else if ( e.key.keysym.sym == SDLK_q )
			{
				SetBomb ( num ) ;
			}
		}     	
	}
}

void apply_surface ( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{ 
	SDL_Rect offset;
	offset.x = x ;
	offset.y = y ; 
	SDL_BlitSurface( source, NULL, destination, &offset );
}


void DrawText ( SDL_Surface *message, SDL_Color textColor, string s, int x, int y, int r = 21 )
{
	char c[50] ;
	for ( int i = 0 ; i < 50 ; i++ ) c[i] = s[i] ;
	TTF_Font *font = TTF_OpenFont ( "font/andyb.ttf", r ) ;
	message = TTF_RenderText_Solid ( font, c, textColor ) ;
	apply_surface ( x , y , message , screen ) ;
}

void DrawBox ( int r , int g , int b , int a , int x = 300 , int y = 450 , int x1 = 350 , int y1 = 200 )
{
	boxRGBA( screen2, 0, 0, x, y, r, g, b, a );
	SDL_Rect onset;
   	onset.x = x1 ;
   	onset.y = y1 ;
   	SDL_BlitSurface( screen2 , NULL , screen , &onset ) ;
   	SDL_Flip ( screen ) ;
}

void PlaySound ( Mix_Music *music, bool f )
{
	if ( f == true && Mix_PlayingMusic() == 0 ) Mix_PlayMusic( music, -1 );
	else if ( f == false ) Mix_HaltMusic () ;
}

void Winner ( string s , int l )
{
	for ( int i = 0 ; i < 300 - player[l].Won * 40 ; i++ )
	{
		for ( int j = 0 ; j < NP ; j++ ) Pic ( 0 , 3 * 40 , screen , -50 , 250 + 80 * j , j ) ;
		Pic ( 0 , 0 , screen , 0 , 0 , 13 , 760 , 560 ) ;
		DrawText ( message , textColor , s , 300 , 600 , 80 ) ;
		Pic ( 0 , 0 , screen , (300 - i)*2 , 250 + 80 * l , 15 ) ;
		SDL_Flip ( screen ) ;
	}
	SDL_Delay ( 1000 ) ;
}

bool Finish_Game ()
{
	string s = "" ;
	playernum = 0 ;
	for ( int i = 0 ; i < NP ; i++ ) if ( player[i].f == true ) playernum++ ;
	if ( playernum <= 1 ) 
	{	
		for ( int i = 0 ; i < NP ; i++ ) 
			if ( player[i].f == true )
			{
				if ( i == 0 ) s = " Player 1 Won! " ;
				if ( i == 1 ) s = " Player 2 Won! " ;
				if ( i == 2 ) s = " Player 3 Won! " ;
				if ( i == 3 ) s = " Player 4 Won! " ;
				if ( i == 4 ) s = " Player 5 Won! " ;
				Mix_HaltMusic () ;
				SDL_Delay ( 1000 ) ;
				Winner (s, i) ;
				player[i].Won++ ;
			}
		return true ;
	}
	else return false ;
}

void Map ()
{
	bool f1 = true ;			
	int m = 148 , n = 0 ;
	int x , y ;
	string str = "" ;
	ifstream fin ( "Map.txt" ) ;
		while ( fin.eof() == 0 )
		{
			getline ( fin , str ) ;
			if ( m < 740 )  
			{
				DrawText ( message , textColor , str , 600 , m ) ;
				map2[n] = str ;
				m += 37 ;
				n++ ;
		 	}   
		 	else  
		 	{
		 		DrawText ( message , textColor , str , 760 , m-740+148 ) ;	
		 		map2[n] = str ;
		 		m += 37 ;
		 		n++ ;
		 	}
		}
		DrawText ( message , textColor , "  Stone  " , 250 , 250 , 30 );
   		DrawText ( message , textColor , "  Jungle " , 250 , 350 , 30 );
   		DrawText ( message , textColor , "  Temple " , 250 , 450 , 30 );
   		DrawText ( message , textColor , "  Office " , 250 , 550 , 30 );
   		DrawText ( message , textColor , " Graveyard " , 250 , 650 , 30 );
	fin.close () ;
	while ( f1 == true )
	{	
		SDL_PollEvent ( &e ) ;
		if ( e.key.keysym.sym == SDLK_TAB ) return ;
		if ( e.type == SDL_QUIT ) SDL_Quit() ;
		if ( e.type == SDL_MOUSEBUTTONDOWN ) 
		{		
			if ( e.button.button == SDL_BUTTON_LEFT)
			{
				SDL_GetMouseState( &x , &y );
				if ( y > 250 && y < 680 )
				{
					if ( x > 250 && x < 450 ) 
					{
						PlaySound ( click, true );
						SDL_Delay ( 150 ) ;
						PlaySound ( click, false );
				 		y -= 250 ;
						y /= 100 ;
						model = y ;
						return ;
					}
				}
				if ( y > 148 && y < 740 ) 
				{
					if ( x > 600 && x < 700 ) 
					{
						PlaySound ( click, true );
						SDL_Delay ( 150 ) ;
						PlaySound ( click, false );
						s1 = map2[(y-148)/37] ;
						Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
						f1 = false ;
						MapFlag = true ;
						return ;
					}
					if ( x > 760 && x < 860 ) 
					{
						PlaySound ( click, true );
						SDL_Delay ( 150 ) ;
						PlaySound ( click, false );
						s1 = map2[15+((y-148)/37)];
						Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
						f1 = false ;
						MapFlag = true ;
						return ;
					}
				}
			}
		}	
		SDL_Flip ( screen ) ;
	}
}

void menu ()
{
	int x , y ;
	DrawText ( message , textColor , " Play" , 450 , 250 , 40 );
   	DrawText ( message , textColor , " Map " , 450 , 350 , 40 );
   	DrawText ( message , textColor , " Quit" , 450 , 450 , 40 );	
   	click = Mix_LoadMUS( "voice/Click.wav" );
	while ( e.type != SDL_QUIT )
	{
		SDL_PollEvent ( &e ) ;
		DrawBox ( 0, 0, 200 , 16 ) ;
		if ( e.type == SDL_MOUSEBUTTONDOWN ) 
		{
			SDL_GetMouseState( &x , &y );
			if (( x > 400 && x < 600 ) && ( y > 250 && y < 350 )) 
			{
				PlaySound ( click, true );
				SDL_Delay ( 150 ) ;
				PlaySound ( click, false );
				return ;
			}
			if (( x > 400 && x < 600 ) && ( y > 350 && y < 450 )) 
			{	
				PlaySound ( click, true );
				SDL_Delay ( 150 ) ;
				PlaySound ( click, false );
				Pic ( 0 , 0 , screen , 0 , 0 , 5 , 760 , 560 ) ;
				Map() ;
				break ;
				if ( MapFlag == true ) return ;
				Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 ) ;
			}
			if (( x > 400 && x < 600 ) && ( y > 450 && y < 550 )) 
			{
				PlaySound ( click, true );
				SDL_Delay ( 150 ) ;
				PlaySound ( click, false );
				SDL_Quit () ;
			}
		}
		SDL_Flip ( screen );
	}
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//								   ///
//								   ///
//	()	    \\                               ()		   ///
//		  \\\\\\                              		   ///
//     \\\  \\\\\   \\	        \\\\\\\\\    \\\    \\\  \\\\\	   ///
//	\\  \\  \\  \\   	\\  \\  \\  \\  \\   \\  \\  \\	   ///
//	\\  \\  \\  \\\ 	\\  \\  \\   \\\\\   \\  \\  \\	   ///
//								   ///
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////





int main()
{
	SDL_Init ( SDL_INIT_EVERYTHING ) ;
	TTF_Init () ;
	Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) ;
	SDL_WM_SetCaption( "ClanBomber", NULL );
	int rand = 40 ;
	srand ( time( NULL ) ) ;
	music = Mix_LoadMUS( "voice/Start.wav" );
	if ( Mix_PlayingMusic() == 0 ) Mix_PlayMusic( music, -1 );
	SDL_Delay (2000);
	for ( int i = 250 ; i > 0 ; i-- )
	{
		Pic ( 0 , 0 , screen , 0 , 0 , 16 , 760 , 560 );
		DrawBox ( 0, 0, 0, i, 1000, 800, 0, 0 );
		SDL_Flip ( screen );
		if ( i == 150 ) 
		{	
			Mix_HaltMusic () ;
			break ;
		}
	}
	SDL_Delay (500);
	DrawBox ( 0, 0, 0, 0, 1000, 800, 0, 0 );
	Pic ( 0 , 0 , screen , 0 , 0 , 14 , 860 , 660 );
	Pic ( 0 , 0 , screen , 0 , 0 , 16 , 760 , 560 );
	s1 = "Arena" ;
	menu () ;
	s1 = "maps/"+s1 ; s1 += ".map" ;
		char c[100] ;
		for ( int i = 0 ; i < 100 ; i++ ) c[i] = s1[i] ;
		getmap ( c ) ;
		DrawText ( message , textColor , " TAB: Quit " , 50 , 750 ) ;
		SDL_Flip ( screen ) ;
		while ( e.type != SDL_QUIT )
		{
			int z = 0 ;
			SDL_PollEvent( &e ) ;
			if ( e.type == SDL_KEYDOWN )
			{
				player[0].Move() ;
				player[1].Move() ;
			}	
			CheckPlayer ( 0, 0, 0, false ) ; 
			DrawAll () ;
			SDL_Delay (20/NP);
			SDL_Flip ( screen ) ;
			for ( int i = 0 ; i < 100 ; i++ ) if ( Bomb[i].t > 0 ) z++ ;
			if ( z == 0 ) if ( Finish_Game () == true ) break ;
		        if ( e.type == SDL_QUIT ) return 0 ;
		        if ( e.key.keysym.sym == SDLK_TAB ) break ; 
		} 
        Mix_Quit() ;
	TTF_Quit () ;
	SDL_Quit () ;
	return 0 ; 
}






















