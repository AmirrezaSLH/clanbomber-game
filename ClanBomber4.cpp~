#include <string.h>
#include <iostream>
#include <string>
#include <cstring>
#include <SDL/SDL.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>

using namespace std ;

SDL_Surface *screen = SDL_SetVideoMode ( 1000 , 800 , 32 , SDL_SWSURFACE ) ;
SDL_Surface *title ;
SDL_Surface *screen2 = SDL_CreateRGBSurface(SDL_SWSURFACE, 1000, 800, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
SDL_Surface *message = NULL ;
int NP = 2 ;
SDL_Color textColor = { 0 , 255 , 0 } , bgColor = { 0, 0, 0 } ;
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
	int z ;
	location () ;
	void move() ;
};

location :: location ()
{
	dx = 0 ;
	dy = 0 ;
	LR = 0 ;
	spd = 1 ;
	r = 2 ;
	b = 5 ;
	z = 0 ;
	crs = 0 ;
	f = true ;
}
struct Bomb 
{
	int x ,y ,t ,r ,LR ,num ;
	Bomb () ;
	void Explosion () ;
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

struct Prise
{
	int x ,y ,r ;
	void CheckPrise () ;
	Prise () ;
};

Prise :: Prise ()
{
	r = 0 ;
	x = -1 ;
	y = -1 ;
}

Bomb Bomb[100] ;
location player[4] ;
Prise prs[100] ;

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

void getmap ( string s ) 
{
	for ( int i = 0 ; i < s.length() ; i++ )
	{
		title = SDL_LoadBMP ("pics/maptiles.bmp") ;
		if ( s[i] == '0' )SetPlayer ( i, j );
		else if ( s[i] == '1' ) SetPlayer ( i, j );
		//else if ( s[i] == '2' ) SetPlayer ( i, j );
		//else if ( s[i] == '3' ) SetPlayer ( i, j );
		else if ( s[i] >= '2' ) 
		{
		        copyImage ( title , ( model * 4 + 0 ) * 40 , 0 , screen , i * 40 , j * 40 ) ;      
			map[i][j] = -3 ;
		}
/////////////////////////////////////////////////////
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

void SetPrise ( int x1 , int y1 )
{
	for ( int i = 0 ; i < 100 ; i++ )
	{
		if ( prs[i].x < 0 && prs[i].y < 0 )
		{
			prs[i].x = x1 ;
			prs[i].y = y1 ;
			prs[i].r = rand() % 8 ;
			return ;
		}
	}
}

void Prise :: CheckPrise ()
{
	for ( int i = 0 ; i < NP ; i++ )
	{
		if ( x == player[i].x/40 && y == player[i].y/40 )
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
				player[i].spd = -2 ;
				player[i].tspd = 350 ;
				x = -1 ;
				y = -1 ;
			}
			if ( r == 7 ) 
			{
				player[i].spd = 10 ;
				player[i].tspd = 350 ;
				x = -1 ;
				y = -1 ;
			}
		}
	}
}

void DrawAll ()
{
	Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
	for ( int i = 0 ; i < 17 ; i++ )
		for ( int j = 0 ; j < 13 ; j++ )
			copyImage ( title , ( model * 4 + 3 + map[i][j] ) * 40 , 0 , screen , i * 40 , j * 40 ) ;
	for ( int i = 0 ; i < 100 ; i++ )
		if ( prs[i].x > 0 && prs[i].y > 0 && ( prs[i].r < 3 || prs[i].r == 7 || prs[i].r == 6 ) ) 
		{	
			Pic ( prs[i].r * 40 , 0 , screen , prs[i].x * 40 , prs[i].y * 40 , 7 ) ;
			prs[i].CheckPrise () ;
		}
	for ( int i = 0 ; i < 100 ; i++ )
		if ( Bomb[i].t > 0 ) Bomb[i].Explosion() ;
	for ( int i = 0 ; i < NP ; i++ ) 
		{
			if ( player[i].f == true ) Pic ( player[i].LR * 40 , player[i].crs * 40 , screen , player[i].x , player[i].y , player[i].num ) ;
			else Pic ( 9 * 40 , player[i].crs * 40 , screen , player[i].x , player[i].y , player[i].num ) ;
			if ( player[i].tspd >= 1 ) player[i].tspd-- ;
			if ( player[i].tspd == 0 ) player[i].spd = 1 ;
		}
	SDL_Delay ( 10 ) ;
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
	while ( r1 > 0 )
	{
		if ( map[x][y] == 405 ) return ;
		if ( map[x][y] == -3 )
		{	
			if ( r1 == 1 )
			{
				if ( i < 0 ) Pic ( 0 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( i > 0 ) Pic ( 2 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( j < 0 ) Pic ( 3 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
				if ( j > 0 ) Pic ( 5 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
				return ;
			}
			Pic ( n * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
			x += i ;
			y += j ;
		}
		else 
		{			
			if ( i < 0 ) Pic ( 0 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( i > 0 ) Pic ( 2 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( j < 0 ) Pic ( 3 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( j > 0 ) Pic ( 5 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;
			if ( map[x][y] == -1 ) map[x][y] = 0 ;
			if ( t == 121 && map[x][y] == 0 ) 
			{
				int random = rand() % 3 ;
				if ( random % 3 != 0 ) SetPrise ( x , y ) ;
				map[x][y] = -3 ;
				return ;
				
			}
		}
		r1-- ;
	}
}

void Bomb :: Explosion ()
{
	LR++ ;
	LR %= 2 ;
	if ( t < 120 ) return ;
	if ( t > 140 )
	{
		if ( num == 1 || num == 0 ) Pic ( ( 0 + LR ) * 40 , ( 2 + num ) * 40 , screen , x * 40 , y * 40 , 9 ) ;
		if ( num == 2 || num == 3 ) Pic ( ( 4 + LR ) * 40 , ( 2 + num ) * 40 , screen , x * 40 , y * 40 , 9 ) ;
	}
	if ( t < 140 && t >= 121 )
	{
		Exp ( x , y ,  1 , 0 , r , 1 , t ) ;
		Exp ( x , y , -1 , 0 , r , 1 , t ) ;
		Exp ( x , y , 0 ,  1 , r , 4 , t ) ;
		Exp ( x , y , 0 , -1 , r , 4 , t ) ;
		Pic ( 6 * 40 , r * 40 , screen , x * 40 , y * 40 , 8 ) ;	
		CheckPlayer ( x , y , r ) ;	
	}
	if ( t == 120 ) 
	{
		x = 0 ;
		y = 0 ;
		player[num].b++ ;
		t = 1 ;
	}
	t-- ;
}

void MoveTo ( int n , int x , int y , int crs , int spd )
{
	for ( int i = 0 ; i < 10 / abs(spd) ; i++ )
	{
		player[n].x += x * 4 * spd ;
		player[n].y += y * 4 * spd ;
		player[n].crs = crs ;
		player[n].LR++ ;
		player[n].LR %= 4 ;
		if ( player[n].LR < 2 ) player[n].LR = 1 ;
		else player[n].LR = 0 ;
		DrawAll () ;
		SDL_Flip ( screen ) ;
	}
}

bool MovePlayer ( int x , int y )
{
	for ( int i = 0 ; i < 100 ; i++ ) 
		if ( Bomb[i].x == x && Bomb[i].y == y && Bomb[i].t > 120 ) return false ;
	for ( int i = 0 ; i < NP ; i++ )
		if ( player[i].x/40 == x && player[i].y/40 == y && player[i].f == true ) return false ;
	return true ;
}

void location :: move ()
{	
	if ( f == true )
	{	
		if ( num == 0 )
		{
			SDL_PollEvent ( &e ) ;
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_a ) 
			{
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[0].spd > 0 ) MoveTo (num,-1,0,1,spd) ;
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[0].spd < 0 ) MoveTo (num,-1,0,3,spd) ;			
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_w )
			{
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[0].spd > 0 ) MoveTo (num,0,-1,2,spd) ;
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[0].spd < 0 ) MoveTo (num,0,-1,0,spd) ;			
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_d )
			{
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[0].spd > 0 ) MoveTo (num,1,0,3,spd) ;
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[0].spd < 0 ) MoveTo (num,1,0,1,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_s )
			{
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[0].spd > 0 ) MoveTo (num,0,1,0,spd) ;
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[0].spd < 0 ) MoveTo (num,0,1,2,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q )
			{
				SetBomb ( num ) ;
			}
		}
		//-------------------------------------------------//
	       //-------------------------------------------------//
	      //-------------------------------------------------//
		if ( num == 1 )
		{
			SDL_PollEvent ( &e ) ;
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_LEFT ) 
			{
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[1].spd > 0 ) MoveTo (num,-1,0,1,spd) ;
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[1].spd < 0 ) MoveTo (num,-1,0,3,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_UP )
			{
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[1].spd > 0 ) MoveTo (num,0,-1,2,spd) ;
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[1].spd < 0 ) MoveTo (num,0,-1,0,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RIGHT )
			{
				if ( map[x/40 + 1][y/40] % 3 == 0 && MovePlayer ( x/40+1, y/40 ) == true && player[1].spd > 0 ) MoveTo (num,1,0,3,spd) ;
				if ( map[x/40 - 1][y/40] % 3 == 0 && MovePlayer ( x/40-1, y/40 ) == true && player[1].spd < 0 ) MoveTo (num,1,0,1,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_DOWN )
			{
				if ( map[x/40][y/40 + 1] % 3 == 0 && MovePlayer ( x/40, y/40+1 ) == true && player[1].spd > 0 ) MoveTo (num,0,1,0,spd) ;
				if ( map[x/40][y/40 - 1] % 3 == 0 && MovePlayer ( x/40, y/40-1 ) == true && player[1].spd < 0 ) MoveTo (num,0,1,2,spd) ;
			}
			if ( e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_m )
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
	TTF_Font * font = TTF_OpenFont ( "andyb.ttf", r ) ;
	message = TTF_RenderText_Solid ( font , c , textColor ) ;
	apply_surface ( x , y , message , screen ) ;
}

void Winner ( string s )
{
	Pic ( 0 , 0 , screen , 0 , 0 , 13 , 760 , 560 ) ;
	DrawText ( message , textColor , s , 100 , 300 , 80 ) ;
	SDL_Flip ( screen ) ;
	SDL_Delay ( 2000 ) ;
}

bool Finish_Game ()
{
	playernum = 0 ;
	for ( int i = 0 ; i < NP ; i++ ) if ( player[i].f == true ) playernum++ ;
	if ( playernum <= 1 ) 
	{	
		for ( int i = 0 ; i < NP ; i++ ) 
			if ( player[i].f == true )
			{
				if ( i == 0 ) string s = " Player 0 Won! " ;
				if ( i == 1 ) string s = " Player 1 Won! " ;
				if ( i == 2 ) string s = " Player 2 Won! " ;
				if ( i == 3 ) string s = " Player 3 Won! " ;
				SDL_Delay ( 1000 ) ;
				cout << s << endl ;
				Winner (s) ;
			}
		return true ;
	}
	else return false ;
}

void DrawBox ()
{
	boxRGBA( screen2, 0, 0, 300, 450, 0, 0, 200, 16 );
	SDL_Rect onset;
   	onset.x = 350 ;
   	onset.y = 200 ;
   	SDL_BlitSurface( screen2 , NULL , screen , &onset ) ;
   	SDL_Flip ( screen ) ;
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
	fin.close () ;
	while ( f1 == true )
	{	
		SDL_PollEvent ( &e ) ;
		if ( e.key.keysym.sym == SDLK_q ) return ;
		if ( e.type == SDL_QUIT ) SDL_Quit() ;
		if ( e.type == SDL_MOUSEBUTTONDOWN ) 
		{
			SDL_PollEvent ( &e ) ;
			SDL_GetMouseState( &x , &y );
			if ( y > 148 && y < 740 ) 
			{	
				if ( x > 600 && x < 700 ) 
				{
					s1 = map2[(y-148)/37] ;
					Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
					f1 = false ;
					MapFlag = true ;
					return ;
				}
				if ( x > 760 && x < 860 ) 
				{
					s1 = map2[15+((y-148)/37)];
					Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
					f1 = false ;
					MapFlag = true ;
					return ;
				}
			}
		}	
		SDL_Flip ( screen ) ;
	}
}

void Option ()
{
	Pic ( 0 , 0 , screen , 0 , 0 , 11 , 335 , 800 ) ;
}

void menu ()
{
	int x , y ;
	DrawText ( message , textColor , " Play" , 450 , 250 , 40 );
   	DrawText ( message , textColor , " Map " , 450 , 350 , 40 );
   	DrawText ( message , textColor , "Option" , 450 , 450 , 40 );
   	DrawText ( message , textColor , " Quit" , 450 , 550 , 40 );
	while ( e.type != SDL_QUIT )
	{
		SDL_PollEvent ( &e ) ;
		DrawBox () ;
		SDL_GetMouseState( &x , &y );
		if ( e.type == SDL_MOUSEBUTTONDOWN ) 
		{
			if (( x > 400 && x < 600 ) && ( y > 250 && y < 350 )) return ;
			if (( x > 400 && x < 600 ) && ( y > 350 && y < 450 )) 
			{
				Pic ( 0 , 0 , screen , 0 , 0 , 5 , 760 , 560 ) ;
				Map() ;
				Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 ) ;
				if ( MapFlag == true ) return ;
			}
			if (( x > 400 && x < 600 ) && ( y > 450 && y < 550 )) 
			{
				Pic ( 0 , 0 , screen , 0 , 0 , 6 , 760 , 560 ) ;
				Option () ;
			}
			if (( x > 400 && x < 600 ) && ( y > 550 && y < 650 )) SDL_Quit () ;
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
	SDL_WM_SetCaption( "ClanBomber", NULL );
	int rand = 40 ;
	srand ( time( NULL ) ) ;
	Pic ( 0 , 0 , screen , 0 , 0 , 4 , 760 , 560 );
	SDL_Flip ( screen ) ;
	SDL_Delay ( 1000 ) ;
	s1 = "Arena" ;
	menu () ;
	s1 = "maps/"+s1 ; s1 += ".map" ;
	char c[100] ;
	for ( int i = 0 ; i < 100 ; i++ ) c[i] = s1[i] ;
	ifstream fin ( c ) ;
		getline ( fin , s ) ;
		getline ( fin , s ) ;
		while ( fin.eof() == 0 )
		{
	 		getline ( fin , s ) ;
        		getmap ( s ) ;   
                }
        fin.close();
	DrawText ( message , textColor , " TAB: Quit " , 50 , 750 ) ;
        SDL_Flip ( screen ) ;
        while ( e.type != SDL_QUIT )
        {
        	SDL_PollEvent( &e ) ;
		for ( int i = 0 ; i < NP ; i++ ) 
			player[i].move() ;
		CheckPlayer ( 0, 0, 0, false ) ; 
		DrawAll () ;
		SDL_Flip ( screen ) ;
		if ( Finish_Game () == true ) break ;
                if ( e.type == SDL_QUIT ) break ;
                if ( e.key.keysym.sym == SDLK_TAB ) break ; 
        }
	TTF_Quit () ;
	SDL_Quit () ;
	return 0 ; 
}






















