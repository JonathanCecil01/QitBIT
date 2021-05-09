//
//  Texture.hpp
//  QitBit
//
//  Created by Jonathan Cecil on 09/05/21.
//

#ifndef Texture_hpp
#define Texture_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

//Screen dimension constants
const int SCREEN_WIDTH = 500;
const int SCREEN_HEIGHT = 300;

class LTexture
{
    public:
        LTexture();
        ~LTexture();
        bool loadFromFile( std::string path );
        #if defined(SDL_TTF_MAJOR_VERSION)
        bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
        #endif
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

#endif /* Texture_hpp */
