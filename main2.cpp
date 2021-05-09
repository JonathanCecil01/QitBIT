//
//  main2.cpp
//  QitBit
//
//  Created by Jonathan Cecil on 09/05/21.
//

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include "Texture.hpp"
//#include "Button.hpp"

using namespace std;

const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 25;
const int TOTAL_BUTTONS = 1;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 2
};
class LButton
{
    public:
        //Initializes internal variables
        LButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e , string);

        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        LButtonSprite mCurrentSprite;
};
const int BUTTON2_WIDTH = 150;
const int BUTTON2_HEIGHT = 50;
const int TOTAL2_BUTTONS = 4;

enum RButtonSprite
{
    BUTTON2_SPRITE_MOUSE1_OUT = 0,
    BUTTON2_SPRITE_MOUSE1_OVER_MOTION = 1,
    BUTTON2_SPRITE_MOUSE1_DOWN = 2,
    BUTTON2_SPRITE_MOUSE2_OUT = 3,
    BUTTON2_SPRITE_MOUSE2_OVER_MOTION = 4,
    BUTTON2_SPRITE_MOUSE2_DOWN = 5,
    BUTTON2_SPRITE_MOUSE3_OUT = 6,
    BUTTON2_SPRITE_MOUSE3_OVER_MOTION = 7,
    BUTTON2_SPRITE_MOUSE3_DOWN = 8,
    BUTTON2_SPRITE_MOUSE4_OUT = 9,
    BUTTON2_SPRITE_MOUSE4_OVER_MOTION = 10,
    BUTTON2_SPRITE_MOUSE4_DOWN = 11,
    
//    BUTTON2_SPRITE_MOUSE_UP = 1,
    BUTTON2_SPRITE_TOTAL = 12
};
class RButton
{
    public:
        //Initializes internal variables
        RButton();

        //Sets top left position
        void setPosition( int x, int y );

        //Handles mouse event
        void handleEvent( SDL_Event* e, int i);

        //Shows button sprite
        void render();

    private:
        //Top left position
        SDL_Point mPosition;

        //Currently used global sprite
        RButtonSprite mCurrentSprite;
};


//Starts up SDL and creates window
bool init();
bool initv2();
//Loads media
bool loadMedia();
bool loadMediav2(string);
//Frees media and shuts down SDL
void close();
void closev2();
void open_Window_Input(string);
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Scene textures
LTexture gPromptTextTexture;
LTexture gInputTextTexture;
SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL], lSpriteClips[BUTTON2_SPRITE_TOTAL];
LTexture gButtonSpriteSheetTexture;
LTexture lButtonSpriteSheetTexture;
LButton gButtons[TOTAL_BUTTONS];
RButton lButtons[TOTAL2_BUTTONS];

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

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface != NULL )
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
    else
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }

    
    //Return success
    return mTexture != NULL;
}
#endif

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
LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent( SDL_Event* e, string nickName )
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        //Mouse is outside button
        if( !inside )
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //Mouse is inside button
        else
        {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                    SDL_DestroyRenderer(gRenderer);
                    cout<<nickName<<endl;
                    open_Window_Input(nickName);
                break;
                
                case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}
    
void LButton::render()
{
    //Show current button sprite
    gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}
RButton::RButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON2_SPRITE_MOUSE1_OUT;
}

void RButton::setPosition( int x, int y )
{
    mPosition.x = x;
    mPosition.y = y;
}

void RButton::handleEvent( SDL_Event* e , int i)
{
    //If mouse event happened
    if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
    {
        //Get mouse position
        int x, y;
        SDL_GetMouseState( &x, &y );

        //Check if mouse is in button
        bool inside = true;

        //Mouse is left of the button
        if( x < mPosition.x )
        {
            inside = false;
        }
        //Mouse is right of the button
        else if( x > mPosition.x + BUTTON_WIDTH )
        {
            inside = false;
        }
        //Mouse above the button
        else if( y < mPosition.y )
        {
            inside = false;
        }
        //Mouse below the button
        else if( y > mPosition.y + BUTTON_HEIGHT )
        {
            inside = false;
        }

        //Mouse is outside button
        if( !inside &&i==0)
        {
            mCurrentSprite = BUTTON2_SPRITE_MOUSE1_OUT;
        }
        else if( !inside &&i==1)
        {
            mCurrentSprite = BUTTON2_SPRITE_MOUSE2_OUT;
        }
        else if( !inside &&i==2)
        {
            mCurrentSprite = BUTTON2_SPRITE_MOUSE3_OUT;
        }
        else if( !inside &&i==3)
        {
            mCurrentSprite = BUTTON2_SPRITE_MOUSE4_OUT;
        }
        //Mouse is inside button
        else
        {
            if(i==0)
            {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE1_OVER_MOTION;
                break;
            
                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE1_DOWN;
                    
                    cout<<"HELLLOOOOO"<<endl;
                    //open_Window_Input(nickName);
                break;
                
            }
            }
            else if(i==1)
            {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE2_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE2_DOWN;


                    //open_Window_Input(nickName);
                break;

            }
            }
            else if(i==2)
            {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE3_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE3_DOWN;


                    //open_Window_Input(nickName);
                break;

            }
            }
            else if(i==3)
            {
            //Set mouse over sprite
            switch( e->type )
            {
                case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE4_OVER_MOTION;
                break;

                case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON2_SPRITE_MOUSE4_DOWN;
                    closev2();

                    //open_Window_Input(nickName);
                break;


            }
            }
        }
    }
}
    
void RButton::render()
{
    //Show current button sprite
    lButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &lSpriteClips[ mCurrentSprite ] );
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
        gWindow = SDL_CreateWindow( "QitBIT LOGIN", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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
    gFont = TTF_OpenFont( "/Users/jonathancecil/Downloads/lato/Lato-Heavy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render the prompt
        SDL_Color textColor = { 0, 0, 0, 0xFF };
        if( !gPromptTextTexture.loadFromRenderedText( "Enter NickName:", textColor ) )
        {
            printf( "Failed to render prompt text!\n" );
            success = false;
        }
    }
    //Load sprites
    if( !gButtonSpriteSheetTexture.loadFromFile( "/Users/jonathancecil/Downloads/example/test.png" ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
        {
            gSpriteClips[ i ].x = 0;
            gSpriteClips[ i ].y = i * 50;
            gSpriteClips[ i ].w = BUTTON_WIDTH;
            gSpriteClips[ i ].h = BUTTON_HEIGHT;
        }

        //Set buttons in corners
          gButtons[ 0 ].setPosition( 200, 150 );
//        gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
//        gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
//        gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
    }
    return success;
}
void close()
{
    //Free loaded images
    gPromptTextTexture.free();
    gInputTextTexture.free();
    gButtonSpriteSheetTexture.free();

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
bool init2(string nickname)
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
        gWindow = SDL_CreateWindow("QITBIT", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
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
bool loadMediav2(string nickname)
{
    //Loading success flag
    bool success = true;

    //Open the font
    gFont = TTF_OpenFont( "/Users/jonathancecil/Downloads/lato/Lato-Heavy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render the prompt
        SDL_Color textColor = { 0, 0, 0, 0xFF };
        string prompt = "hello "+nickname+" !";
        if( !gPromptTextTexture.loadFromRenderedText( prompt, textColor ) )
        {
            printf( "Failed to render prompt text!\n" );
            success = false;
        }
    }
    //Load sprites
    if( !lButtonSpriteSheetTexture.loadFromFile( "/Users/jonathancecil/Downloads/17_mouse_events/button.png" ) )
    {
        printf( "Failed to load button sprite texture!\n" );
        success = false;
    }
    else
    {
        //Set sprites
        for( int i = 0; i < BUTTON2_SPRITE_TOTAL; ++i )
        {
            lSpriteClips[ i ].x = 0;
            lSpriteClips[ i ].y = i * 50;
            lSpriteClips[ i ].w = BUTTON2_WIDTH;
            lSpriteClips[ i ].h = BUTTON2_HEIGHT;
        }

        //Set buttons in corners
          lButtons[ 0 ].setPosition( 180, 50 );
          lButtons[ 1 ].setPosition( 180, 100 );
          lButtons[ 2 ].setPosition( 180, 150 );
          lButtons[ 3 ].setPosition( 180, 200 );
    }
    return success;
}
void closev2()
{
    //Free loaded images
    gPromptTextTexture.free();
    gInputTextTexture.free();
    gButtonSpriteSheetTexture.free();

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

void open_Window_Input(string nickName)
{
    //Start up SDL and create window
    if(!init2(nickName))
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMediav2(nickName) )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Set text color as black
            SDL_Color textColor = { 0, 0, 0, 0 };

            //The current input text.
            std::string inputText = "";
            gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );

            //Enable text input
            SDL_StartTextInput();

            //While application is running
            while( !quit )
            {
                //The rerender text flag
                bool renderText = false;

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //Special key input
                    else if( e.type == SDL_KEYDOWN )
                    {
                        //Handle backspace
                        if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                        {
                            //lop off character
                            inputText.pop_back();
                            renderText = true;
                        }
                        //Handle copy
                        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                        {
                            SDL_SetClipboardText( inputText.c_str() );
                        }
                        //Handle paste
                        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                        {
                            inputText = SDL_GetClipboardText();
                            renderText = true;
                        }
                    }
                    //Special text input event
                    else if( e.type == SDL_TEXTINPUT )
                    {
                        //Not copy or pasting
                        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
                        {
                            //Append character
                            inputText += e.text.text;
                            renderText = true;
                        }
                    }
                    nickName = inputText;
                    //Handle button events
                    for( int i = 0; i < TOTAL2_BUTTONS; ++i )
                    {
                        lButtons[ i ].handleEvent( &e, i);
                    }
                }
                
                //Rerender text if needed
                if(renderText )
                {
                    //Text is not empty
                    if( inputText != "" )
                    {
                        //Render new text
                        gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        gInputTextTexture.loadFromRenderedText( " ", textColor );
                    }
                }
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 0);
                SDL_RenderClear( gRenderer );

                //Render text textures
                gPromptTextTexture.render( 200, 0 );
                gInputTextTexture.render( /*( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2*/0, gPromptTextTexture.getHeight() );
                //Render buttons
                for( int i = 0; i < TOTAL2_BUTTONS; ++i )
                {
                    lButtons[ i ].render();
                }

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
            
            //Disable text input
            SDL_StopTextInput();
        }
    }
    
    //Free resources and close SDL
    closev2();
    //cout<<nickName<<endl;
    return;
}

int main( int argc, char* args[] )
{
    string nickName;
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
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Set text color as black
            SDL_Color textColor = { 0, 0, 0, 0 };

            //The current input text.
            std::string inputText = "";
            gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );

            //Enable text input
            SDL_StartTextInput();

            //While application is running
            while( !quit )
            {
                //The rerender text flag
                bool renderText = false;

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //Special key input
                    else if( e.type == SDL_KEYDOWN )
                    {
                        //Handle backspace
                        if( e.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
                        {
                            //lop off character
                            inputText.pop_back();
                            renderText = true;
                        }
                        //Handle copy
                        else if( e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
                        {
                            SDL_SetClipboardText( inputText.c_str() );
                        }
                        //Handle paste
                        else if( e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
                        {
                            inputText = SDL_GetClipboardText();
                            renderText = true;
                        }
                    }
                    //Special text input event
                    else if( e.type == SDL_TEXTINPUT )
                    {
                        //Not copy or pasting
                        if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
                        {
                            //Append character
                            inputText += e.text.text;
                            renderText = true;
                        }
                    }
                    nickName = inputText;
                    //Handle button events
                    for( int i = 0; i < TOTAL_BUTTONS; ++i )
                    {
                        gButtons[ i ].handleEvent( &e, nickName);
                    }
                }
                
                //Rerender text if needed
                if(renderText )
                {
                    //Text is not empty
                    if( inputText != "" )
                    {
                        //Render new text
                        gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
                    }
                    //Text is empty
                    else
                    {
                        //Render space texture
                        gInputTextTexture.loadFromRenderedText( " ", textColor );
                    }
                }
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 255, 255, 255, 0);
                SDL_RenderClear( gRenderer );

                //Render text textures
                gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
                gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );
                //Render buttons
                for( int i = 0; i < TOTAL_BUTTONS; ++i )
                {
                    gButtons[ i ].render();
                }

                //Update screen
                SDL_RenderPresent( gRenderer );
            }
            
            //Disable text input
            SDL_StopTextInput();
        }
    }
    
    //Free resources and close SDL
    close();
    cout<<nickName<<endl;
    return 0;
}
