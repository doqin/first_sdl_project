//
// Created by đỗ quyên on 6/12/24.
//

#ifndef LTEXTURE_H
#define LTEXTURE_H
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

//Texture wrapper class
class LTexture {
public:
    //Initializes variables
    LTexture();
    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(SDL_Renderer* mRenderer, const std::string& path);

    //Creates image from font string
    bool loadFromRenderedText(SDL_Renderer* mRenderer, TTF_Font* mFont, const std::string& textureText, SDL_Color textColor);

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(SDL_Renderer* mRenderer, int x, int y, const SDL_Rect* clip = nullptr, double angle = 0.0, const SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    void renderOnce(SDL_Renderer *mRenderer, Uint8 alpha, int x, int y, const SDL_Rect *clip = nullptr, double angle = 0.0, const SDL_Point *center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //Gets image dimensions
    int getWidth() const;
    int getHeight() const;

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
    Uint8 mAlpha;
};


/*----------------*
LTexture functions
------------------*/

//Initializes variable
inline LTexture::LTexture() {
    //Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
    mAlpha = 255;
}

//Deallocates memory
inline LTexture::~LTexture() {
    //Deallocate
    free();
}

//Loads image at specified path
inline bool LTexture::loadFromFile(SDL_Renderer* mRenderer, const std::string& path) {
    //Get rid of preexisting texture
    free();

    //The final texture
    SDL_Texture* newTexture = nullptr;

    //Load image at specified path
    if (SDL_Surface* loadedSurface = IMG_Load(path.c_str()); loadedSurface == nullptr) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else {
        //Color key image
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(mRenderer, loadedSurface);
        if (newTexture == nullptr) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else {
            //Get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }
        //Ged rif of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //Return success
    mTexture = newTexture;
    return mTexture != nullptr;
}

//Creates image from font string
inline bool LTexture::loadFromRenderedText(SDL_Renderer* mRenderer, TTF_Font* mFont, const std::string& textureText, const SDL_Color textColor) {
    //Get rid of preexisting texture
    free();

    //Render text surface
    if (SDL_Surface* textSurface = TTF_RenderText_Solid(mFont, textureText.c_str(), textColor); textSurface == nullptr) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(mRenderer, textSurface);
        if (mTexture == nullptr) {
            printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else {
            //Get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //Return success
    return mTexture != nullptr;
}

//Deallocates texture
inline void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

//Set color modulation
inline void LTexture::setColor(const Uint8 red, const Uint8 green, const Uint8 blue) {
    //Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

//Set blending
inline void LTexture::setBlendMode(const SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

//Set alpha modulation
inline void LTexture::setAlpha(const Uint8 alpha) {
    //Set alpha value
    mAlpha = alpha;
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

//Renders texture at given point
inline void LTexture::render(SDL_Renderer* mRenderer, const int x, const int y, const SDL_Rect* clip, const double angle, const SDL_Point* center, const SDL_RendererFlip flip) {

    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

//Renders texture at given point
inline void LTexture::renderOnce(SDL_Renderer* mRenderer, const Uint8 alpha ,const int x, const int y, const SDL_Rect* clip, const double angle, const SDL_Point* center, const SDL_RendererFlip flip) {

    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_SetTextureAlphaMod(mTexture, alpha);
    //Render to screen
    SDL_RenderCopyEx(mRenderer, mTexture, clip, &renderQuad, angle, center, flip);
    SDL_SetTextureAlphaMod(mTexture, mAlpha);
}

//Gets image dimensions
inline int LTexture::getWidth() const {
    return mWidth;
}

inline int LTexture::getHeight() const {
    return mHeight;
}

/*-----*
Objects
-------*/

//Text texture
extern LTexture gFPSTextTexture;

//Tile sprite sheet
extern LTexture gTileTexture;

//Shimmer texture
extern LTexture gShimmerTexture;

#endif //LTEXTURE_H
