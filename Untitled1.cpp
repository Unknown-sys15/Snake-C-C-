#include <SDL.h>
#include <stdio.h>
#include <cstdlib>
#include <SDL_image.h>
#include <string>
#include <cmath>
#include <SDL_ttf.h>

//Screen dimension
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;

const int gDebelinaKvadratka = 32;
//NOTE SCREEN_WIDTH in SCREEN_HEIGHT morata biti deljiva z gDebelinaKvadratka

class LTexture
{
	public:
		//Initializes variables
		LTexture();
		//Deallocates memory
		~LTexture();
		//Loads image at specified path
		bool loadFromFile( std::string path );
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		//Deallocates texture
		void free();
		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		//Set blending
		void setBlendMode( SDL_BlendMode blending );
		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		//Gets image dimensions
		int getWidth();
		int getHeight();
	private:
		//The actual hardware texture
		SDL_Texture* mTexture;
		//Image dimensions
		int mWidth;
		int mHeight;
};

//Starts up SDL create window
bool init();
//Loads media
bool loadMedia();
//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//The window renderer
SDL_Renderer* gRenderer = NULL;
//Globally used font
TTF_Font *gFont = NULL;
//Rendered texture
LTexture gTextTexture;


///////////////////////////////////////////////////////////////////////////////////////////////////////
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}
LTexture::~LTexture()
{
	//Deallocate
	free();
}
bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();
	//The final texture
	SDL_Texture* newTexture = NULL;
	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}
	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface == NULL )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}
		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	//Return success
	return mTexture != NULL;
}
void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}
void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}
void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}
void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}
int LTexture::getWidth()
{
	return mWidth;
}
int LTexture::getHeight()
{
	return mHeight;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

int razmerjex = SCREEN_WIDTH/gDebelinaKvadratka;
int razmerjey = SCREEN_HEIGHT/gDebelinaKvadratka;

struct Food {
	int x, y;
	void move() {
		x = rand() % (SCREEN_WIDTH / gDebelinaKvadratka);
		y = rand() % (SCREEN_HEIGHT / gDebelinaKvadratka);
	}

	void draw(SDL_Renderer* gRenderer) {
		SDL_Rect r{ gDebelinaKvadratka * x, gDebelinaKvadratka * y, gDebelinaKvadratka, gDebelinaKvadratka };
		SDL_SetRenderDrawColor(gRenderer, 0, 255, 0, 255);
		SDL_RenderFillRect(gRenderer, &r);
	}
};
struct Vector{
    int x,y;
};

int score = 0;

//Overloading operators - definiranje operatorja za neko strukturo
//sprejme 2 strukturi in vrne vsoto - s tem lahko sestevam struct
Vector operator+ (const Vector& a, const Vector& b){
    return Vector {a.x + b.x , a.y + b.y};
}
//pristeva aju
void operator+= (Vector& a, const Vector& b) {
	a.x += b.x;
	a.y += b.y;
}

const int tail_max = 625;

struct Snake {
	Vector pos;//position
	Vector vel;//velocity
	Vector acc;//changeOfDirection

    bool hasDied = false;

	Vector tail[tail_max];//array struktur za rep
	int tail_start, tail_end;
	int tail_len ;

	uint32_t counter;

	void update(uint32_t delta_time, struct Food& food) {
		counter += delta_time;
		if (counter > 70 ) {// velocity
			counter = 0;

			vel = acc;
			printf(" \n ");
			printf("tail lenght: %d \n ", tail_len);
            printf("vel.x : %d \n ",vel.x);
            printf("vel.y : %d \n ",vel.y);

			tail[tail_end % tail_max] = pos;
            printf("tail_end ostanek tail_max : %d \n ",tail_end % tail_max);

			tail_start++;
			tail_end++;

			pos.x += vel.x;
			pos.y += vel.y;
            printf("pos.x : %d \n ",pos.x);
            printf("pos.y : %d \n ",pos.y);

            printf("tail_start: %d \n ",tail_start);
            printf("tail_end: %d \n ",tail_end);

            //resetning (v primeru ce gre na eni strani predalec se mirrora na zacetek)
			pos.x = (pos.x + razmerjex) % razmerjex;
			pos.y = (pos.y + razmerjey) % razmerjey;


            //food detection
			if (pos.x == food.x && pos.y == food.y) {
				//podalsamo kaco, pristejemo score in premaknemo hrano na novo lokacijo
				tail_len += 1;
				tail_start -= 1;
				score += 1;
				while(1){
                    //v primeru da se food spawna v telesu kace ga premikamo dokler ni vredu
                    food.move();
                    printf("\n v loopu \n");
                    printf("food x pos %d \n",food.x);
                    printf("food y pos %d \n",food.y);
                    for (int i = 0; i < tail_len; i++) {
                        Vector& tail_pos = tail[(tail_start + i) % tail_max];
                        if (tail_pos.x == food.x && tail_pos.y == food.y) {
                            food.move();
                            i = 0;
                        }
                    }
                    break;
                }
			}

            //collision
			for (int i = 0; i < tail_len; i++) {
				Vector& tail_pos = tail[(tail_start + i) % tail_max];
				if (tail_pos.x == pos.x && tail_pos.y == pos.y) {
					//we died
					tail_len = 0;
					tail_start = tail_end;
					hasDied = true;
				}
			}
		}
	}

	void draw(SDL_Renderer* gRenderer) {
		SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);

        //drawing the body
		for (int i = 0; i < tail_len; i++) {
			Vector& tail_pos = tail[(tail_start + i) % tail_max];
			SDL_Rect r{ gDebelinaKvadratka * tail_pos.x, gDebelinaKvadratka * tail_pos.y, gDebelinaKvadratka, gDebelinaKvadratka };
			SDL_RenderFillRect(gRenderer, &r);
		}

		//drawing the head
		SDL_SetRenderDrawColor(gRenderer, 0, 191, 255, 255);
		SDL_Rect r{ gDebelinaKvadratka * pos.x, gDebelinaKvadratka * pos.y, gDebelinaKvadratka, gDebelinaKvadratka };
		SDL_RenderFillRect(gRenderer, &r);
	}
};

int main( int argc, char* args[] )
{

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
		    //Event handler
			SDL_Event e;
			//Main loop flag
			bool running = true;
            Snake snake = {};
            Food food = {};
			food.move();
			//food.draw(gRenderer);

            uint32_t current_time = 0, previous_time, delta_time;


            while(running){
                //Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )//buffer
				{
					//User wants to quit
					if( e.type == SDL_QUIT )
					{
						running = false;
					}
					else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                        case SDLK_r:
                            goto label;
                            break;
                        }
                    }
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Render current frame
				gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );
                SDL_RenderPresent( gRenderer );

            }

            label:
            running = true;

			//GAME LOOP
			while( running )
			{
			    previous_time = current_time;
                current_time = SDL_GetTicks();
                delta_time = current_time - previous_time;

				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ) //buffer
				{
					//User wants to quit
					if( e.type == SDL_QUIT )
					{
						running = false;
					}
					else if (e.type == SDL_KEYDOWN) {
                        switch (e.key.keysym.sym) {
                        case SDLK_ESCAPE:
                            running = false;
                            break;

                        case SDLK_w: if (snake.vel.y != 1) snake.acc = { 0, -1 }; break;
                        case SDLK_s: if (snake.vel.y != -1) snake.acc = { 0, 1 }; break;
                        case SDLK_a: if (snake.vel.x != 1) snake.acc = { -1, 0 }; break;
                        case SDLK_d: if (snake.vel.x != -1) snake.acc = { 1, 0 }; break;
                        }
                    }
				}

				snake.update(delta_time, food);

                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                SDL_RenderClear(gRenderer);

                snake.draw(gRenderer);
                food.draw(gRenderer);

                SDL_RenderPresent(gRenderer);


                if(snake.hasDied){
                    printf("\n YOUR SCORE WAS: %d\n", (score*10));

                    break;
                }
			}
		}
	}

	//Free memory space and close SDL
	close();

	return 0;
}


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Snake", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );//present V-Sync -> capping fps
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}

			}
		}
	}

	return success;
}

bool loadMedia()
{
    //Loading success flag
	bool success = true;

    //Open the font
	gFont = TTF_OpenFont( "lazy.ttf", 28 );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		//Render text
		SDL_Color textColor = { 0, 0, 0 };

		if( !gTextTexture.loadFromRenderedText( "Welcome: use A W S D to move -> Press r to continue", textColor ) )
		{
			printf( "Failed to render text texture!\n" );
			success = false;
		}
	}

	return success;
}


void close()
{
    //Free loaded images
	gTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
    TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
