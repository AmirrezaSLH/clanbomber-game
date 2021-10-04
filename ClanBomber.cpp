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
int j = 0 , model = 2 , playernum = 0 , LR = 0 ;
int map[100][100] ;
int bon_x = 0 , bon_y = 0 , bon_m = 0 ;
bool bon_f = false ;

struct bomb 
{
	int x , y ;
	int t ;
	int r ;	
	bomb () ;
};

bomb :: bomb ()
{
	x = 0 ;
	y = 0 ;
	t = 0 ;
	r = 2 ;
}

struct location
{
	bool f ;
	int x , y ;
	int crs ;
	bomb b[5] ;
	void bmb () ;
	location () ;
};

location :: location ()
{
	crs = 0 ;
	f = true ;
}

location map2 [4] ;

/////////////////////////////////////////////////////

void copyImage ( SDL_Surface * src , int x , int y , SDL_Surface * scr , int xx , int yy )
{
        SDL_Rect rect ;
        rect.x = x * 40 ;
        rect.y = y * 40 ;
        rect.w = 40 ;
        rect.h = 40 ;
        
        SDL_Rect rect2 ;
        rect2.x = xx * 40 ;
        rect2.y = yy * 40 ;
        rect2.w = 40 ;
        rect2.h = 40 ;
        
        SDL_BlitSurface ( src , &rect , scr , &rect2 ) ;
}

void player ( int x , int y , SDL_Surface * scr , int xx , int yy , int k )
{
        SDL_Surface * player ;
	char c[10] ;
	sprintf ( c , "pics/%d.bmp" , k ) ;
	player = SDL_LoadBMP(c) ;
	
	SDL_SetColorKey ( player , SDL_SRCCOLORKEY , SDL_MapRGB ( player -> format , 255 , 255 , 255 ) ) ;
	
	copyImage ( player , x , y , scr , xx , yy ) ;
}

void CheckPlayer ( int x , int y , int n ) 
{
	for ( int i = 0 ; i < 4 ; i++ )
	{
		for ( int j = -3 + n ; j <= 3 - n ; j++ )
		{
			if ( map2[i].x == x + j && map2[i].y == y ) map2[i].f = false ;
			if ( map2[i].x == x && map2[i].y == y + j ) map2[i].f = false ;
		}
		if ( map2[i].f == false ) player ( 9 , map2[i].crs , screen , map2[i].x , map2[i].y , i ) ;
	}
}

/*void Explosion ( int x , int y , int i , int j , int r , int n , string s )
{
	int r1 = r ;
	while ( r1 != 0 )
	{
		if ( map[x][y] % 2 != 0 )
		{
			if ( s == "First" ) player ( n , r  , screen , x , y , 8 ) ;
			if ( s == "Last" ) 
			{
				if ( map[ x + i ][ y + j ] == -1 ) map[ x + i ][ y + j ] = 0 ; 
				copyImage ( title , model * 4 + 3 + map[x][y] , 0 , screen , x , y ) ;
			}
			x += i ;
			y += j ;
		}
		else return ;
		r1-- ;
	}
}*/

void location :: bmb ()
{
	for ( int i = 0 ; i < 5 ; i++ )
	{
		if ( b[i].t > 0 )
		{
			b[i].t-- ; 
			
			if ( b[i].t > 150 )
			{
				player ( LR , b[i].r , screen , b[i].x , b[i].y , 9 ) ;
			}
			
			if ( b[i].t < 150 && b[i].t > 145 ) 
			{
				copyImage ( title , model * 4 + 3 + map[ b[i].x ][ b[i].y ] , 0 , screen , b[i].x , b[i].y ) ;
				for ( int j = -3 + b[i].r + 1 ; j <= 3 - b[i].r - 1 ; j++ )
				{
					player ( 4 , b[i].r , screen , b[i].x , b[i].y + j , 8 ) ;
					player ( 1 , b[i].r , screen , b[i].x + j , b[i].y , 8 ) ;
				}
				player ( 6 , b[i].r , screen , b[i].x , b[i].y , 8 ) ;
				
				player ( 3 , b[i].r , screen , b[i].x , b[i].y - 3 + b[i].r , 8 ) ;
				player ( 5 , b[i].r , screen , b[i].x , b[i].y + 3 - b[i].r , 8 ) ;
				player ( 0 , b[i].r , screen , b[i].x - 3 + b[i].r , b[i].y , 8 ) ;
				player ( 2 , b[i].r , screen , b[i].x + 3 - b[i].r , b[i].y , 8 ) ;
			}
		
			
			for ( int j = -3 + b[i].r ; j <= 3 - b[i].r ; j++ )
			{ 
				if ( b[i].t <= 145 && b[i].t > 141 )
				{
					
					copyImage ( title , model * 4 + 3 + map[ b[i].x ][ b[i].y + j ] , 0 , screen , b[i].x , b[i].y + j ) ;
					copyImage ( title , model * 4 + 3 + map[ b[i].x + j ][ b[i].y ] , 0 , screen , b[i].x + j , b[i].y ) ;
					
					if ( map[ b[i].x ][ b[i].y + j ] == -1 ) 
					{
						map[ b[i].x ][ b[i].y + j ] = 0 ;
						copyImage ( title , model * 4 + 3 + map[ b[i].x ][ b[i].y + j ] , 0 , screen , b[i].x , b[i].y + j ) ;
					}
					if ( map[ b[i].x + j ][ b[i].y ] == -1 ) 
					{
						map[ b[i].x + j ][ b[i].y ] = 0 ;
						copyImage ( title , model * 4 + 3 + map[ b[i].x + j ][ b[i].y ] , 0 , screen , b[i].x + j , b[i].y ) ;
					}
					
					CheckPlayer ( b[i].x , b[i].y , b[i].r ) ;
				}
				if ( b[i].t == 130 ) copyImage ( title , model * 4 + 3 + map[x][y] , 0 , screen , x , y ) ;
				if ( b[i].t <= 120 )
				{
					if ( map[ b[i].x ][ b[i].y + j ] == 0 ) 
					{
						map[ b[i].x ][ b[i].y + j ] = -3 ;
						copyImage ( title , model * 4 + 0 , 0 , screen , b[i].x , b[i].y + j ) ;
					}
					if ( map[ b[i].x + j ][ b[i].y ] == 0 ) 
					{
						map[ b[i].x + j ][ b[i].y ] = -3 ;
						copyImage ( title , model * 4 + 0 , 0 , screen , b[i].x + j , b[i].y ) ;
					}
					b[i].t = 10 ;	
				}
			}	
		}
	}
}

void SetBomb ( int n )
{
	int h ;
	for ( int i = 0 ; i < 5 ; i++ )
	{
		if ( map2[n].x == map2[n].b[i].x && map2[n].y == map2[n].b[i].y ) 
		{
			if ( map2[n].b[i].t > 140 ) h = 1 ;
		}
	}
	if ( h != 1 )
	{
		for ( int i = 0 ; i < 5 ; i++ )
		{
			if ( map2[n].b[i].t == 0 )
			{
				map2[n].b[i].t = 200 ;
				map2[n].b[i].x = map2[n].x ;
				map2[n].b[i].y = map2[n].y ;
				return ;
			}
		}
	}
}

void getmap ( string s ) 
{
	for ( int i = 0 ; i < s.length() ; i++ )
	{
		title = SDL_LoadBMP ("pics/maptiles.bmp") ;
		
		if ( s[i] == '0' )
	        {
	                copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			player ( 0 , 0 , screen , i , j , 0 ) ;
			playernum++ ;
			
			map[i][j] = -3 ;
			
			map2[0].x = i ;
			map2[0].y = j ;
		}
		else if ( s[i] == '1' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			player ( 0 , 0 , screen , i , j , 1 ) ;
			playernum++ ;
			
			map[i][j] = -3 ;
			
			map2[1].x = i ;
			map2[1].y = j ;
		}
		else if ( s[i] == '2' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			player ( 0 , 0 , screen , i , j , 2 ) ;
			playernum++ ;
			
			map[i][j] = -3 ;
			
			map2[2].x = i ;
			map2[2].y = j ;
		}
		else if ( s[i] == '3' ) 
		{
		        copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;      
			player ( 0 , 0	 , screen , i , j , 3 ) ;
			playernum++ ;
			
			map[i][j] = -3 ;
			
			map2[3].x = i ;
			map2[3].y = j ;
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
		else if ( s[i] == ' ' ) 
		{
			copyImage ( title , model * 4 + 0 , 0 , screen , i , j ) ;
			map[i][j] = -3 ;
		}
		else if ( s[i] == '*' ) 
		{
			copyImage ( title , model * 4 + 1 , 0 , screen , i , j ) ;
			map[i][j] = -2 ;
		}
		else if ( s[i] == '+' ) 
		{
			copyImage ( title , model * 4 + 2 , 0 , screen , i , j ) ;
			map[i][j] = -1 ;
		}
		 
	}
	j++ ;
	s = "" ;
}

void vibrate ()
{
	LR++ ;
	LR %= 2 ;
        for ( int i = 0 ; i < 4 ; i++ )
        {
        	if ( map2[i].f == true ) player ( LR , map2[i].crs , screen , map2[i].x , map2[i].y , i ) ;
               	SDL_Delay ( 20 ) ;
	}
}

void MoveLeft ( int n )
{
	copyImage ( title , model * 4 + 3 + map[ map2[n].x ][ map2[n].y ] , 0 , screen , map2[n].x , map2[n].y ) ;
	player ( 0 , 0 , screen , map2[n].x - 1 , map2[n].y , n ) ;
	map2[n].x -- ;
	map2[n].crs = 1 ;
}

void MoveUp ( int n )
{
	copyImage ( title , model * 4 + 3 + map[ map2[n].x ][ map2[n].y ] , 0 , screen , map2[n].x , map2[n].y ) ;
	player ( 0 , 0 , screen , map2[n].x , map2[n].y - 1 , 0 ) ;
	map2[n].y -- ;
	map2[n].crs = 2 ;
}

void MoveRight ( int n )
{
	copyImage ( title , model * 4 + 3 + map[ map2[n].x ][ map2[n].y ] , 0 , screen , map2[n].x , map2[n].y ) ;
	player ( 0 , 0 , screen , map2[n].x + 1 , map2[n].y , 0 ) ;
	map2[n].x ++ ;
	map2[n].crs = 3 ;
}

void MoveDown ( int n )
{
	copyImage ( title , model * 4 + 3 + map[ map2[n].x ][ map2[n].y ] , 0 , screen , map2[n].x , map2[n].y ) ;
	player ( 0 , 0 , screen , map2[n].x , map2[n].y + 1 , 0 ) ;
	map2[n].y ++ ;
	map2[n].crs = 0 ;
}

void Extras ()
{
	do
	{
		bon_y = rand() % ( screen -> w / 40 ) ;
		bon_x = rand() % ( screen -> h / 40 ) ;
	}
	while ( map[bon_x][bon_y] != -3 ) ;
	bon_m = rand() % 2 ;
	player ( bon_m , 0 , screen , bon_x , bon_y , 6 ) ;
}

void GetExtras ()
{
	for ( int i = 0 ; i < 5 ; i++ )
	{
		if ( map2[i].x == bon_x && map2[i].y == bon_y )
		{
			if ( bon_m == 1 && map2[i].b[i].r != 0 ) map2[i].b[i].r-- ;
		}
	}
}

void move ()
{
	if ( map2[0].f == true )
	{
		if ( e.key.keysym.sym == SDLK_a )
		{
			if ( map[ map2[0].x - 1 ][ map2[0].y ] % 3 == 0 ) MoveLeft (0) ;
		}
		if ( e.key.keysym.sym == SDLK_w )
		{
			if ( map[ map2[0].x ][ map2[0].y - 1 ] % 3 == 0 ) MoveUp (0) ;
		}
		if ( e.key.keysym.sym == SDLK_d )
		{
			if ( map[ map2[0].x + 1 ][ map2[0].y ] % 3 == 0 ) MoveRight (0) ;
		}
		if ( e.key.keysym.sym == SDLK_s )
		{
			if ( map[ map2[0].x ][ map2[0].y + 1 ] % 3 == 0 ) MoveDown (0) ;
		}
		if ( e.key.keysym.sym == SDLK_q )
		{
			SetBomb ( 0 ) ;
		}
	}	
	
/////////////////////////////////////////////////////

	if ( map2[1].f == true )
	{
		if ( e.key.keysym.sym == SDLK_LEFT )
		{
			if ( map[ map2[1].x - 1 ][ map2[1].y ] % 3 == 0 ) MoveLeft (1) ;
		}
		if ( e.key.keysym.sym == SDLK_UP )
		{
			if ( map[ map2[1].x ][ map2[1].y - 1 ] % 3 == 0 ) MoveUp (1) ;
		}
		if ( e.key.keysym.sym == SDLK_RIGHT )
		{
			if ( map[ map2[1].x + 1 ][ map2[1].y ] % 3 == 0 ) MoveRight (1) ;
		}
		if ( e.key.keysym.sym == SDLK_DOWN )
		{
			if ( map[ map2[1].x ][ map2[1].y + 1 ] % 3 == 0 ) MoveDown (1) ;
		}
		if ( e.key.keysym.sym == SDLK_m )
		{
			SetBomb ( 1 ) ;
		}
	}	
	
/////////////////////////////////////////////////////

	if ( map2[2].f == true )
	{	
		if ( e.key.keysym.sym == SDLK_j )
		{
			if ( map[ map2[2].x - 1 ][ map2[2].y ] % 3 == 0 ) MoveLeft (2) ;
		}
		if ( e.key.keysym.sym == SDLK_i )
		{
			if ( map[ map2[2].x ][ map2[2].y - 1 ] % 3 == 0 ) MoveUp (2) ;
		}
		if ( e.key.keysym.sym == SDLK_l )
		{
			if ( map[ map2[2].x + 1 ][ map2[2].y ] % 3 == 0 ) MoveRight (2) ;
		}
		if ( e.key.keysym.sym == SDLK_k )
		{
			if ( map[ map2[2].x ][ map2[2].y + 1 ] % 3 == 0 ) MoveDown (2) ;
		}
		if ( e.key.keysym.sym == SDLK_o )
		{
			SetBomb ( 2 ) ;
		}
	}	
/////////////////////////////////////////////////////

}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//								   ///
//								   ///
//	()	    \\                               ()		   ///
//		   \\\\\                              		   ///
//	\\  \\\\\   \\	        \\\\\\\\\    \\\     \\  \\\\\	   ///
//	\\  \\  \\  \\   	\\  \\  \\  \\  \\   \\  \\  \\	   ///
//	\\  \\  \\  \\		\\  \\  \\   \\\\\\  \\  \\  \\	   ///
//								   ///
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

int main()
{
        SDL_Init ( SDL_INIT_EVERYTHING ) ;
	int rand = 40 ;
	srand ( time( NULL ) ) ;
        ifstream fin ( "maps/Arena.map" ) ;
        ///
        	getline ( fin , s ) ;
       		getline ( fin , s ) ;
		while ( fin.eof() == 0 )
                {
        		getline ( fin , s ) ;
        		getmap ( s ) ;   
                }
        ///
        fin.close();
        SDL_Flip ( screen ) ;
        
        while ( e.type != SDL_QUIT )
        {
        	rand++ ;
        	rand %= 100 ;
        	SDL_PollEvent( &e ) ;
        	if ( e.type == SDL_KEYDOWN ) move () ;
                map2[0].bmb () ;
                map2[1].bmb () ;
                map2[2].bmb () ;
                vibrate () ;
                if ( rand % 100 == 0 ) Extras() ; 
                if ( rand % 100 == 40 ) 
                {
			copyImage ( title , model * 4 + 0 , 0 , screen , bon_x , bon_y ) ;
			bon_x = 0 ;
			bon_y = 0 ;
                }
                SDL_Flip ( screen ) ;
        }
                
        SDL_Delay ( 2000 ) ;
        SDL_Quit () ;
        return 0 ; 
}











