#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <sstream>

/*----------------*
Constant variables
------------------*/

//The dimensions of the level
constexpr int LEVEL_WIDTH = 1280;
constexpr int LEVEL_HEIGHT = 960;

//Screen dimension constants
constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr int SCREEN_FPS = 60;
constexpr int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

//Button constants
constexpr int BUTTON_WIDTH = 300;
constexpr int BUTTON_HEIGHT = 200;
constexpr int TOTAL_BUTTONS = 4;

//Max data
constexpr int TOTAL_DATA = 10;

//Analog joystick dead zone
constexpr int JOYSTICK_DEAD_ZONE = 8000;

enum LButtonSprite {
    BUTTON_SPRITE_MOUSE_OUT,
    BUTTON_SPRITE_MOUSE_OVER_MOTION,
    BUTTON_SPRITE_MOUSE_DOWN,
    BUTTON_SPRITE_MOUSE_UP,
    BUTTON_SPRITE_TOTAL
};

/*-------------------*
Delta time's variable
---------------------*/

//Delta time
float delta = 0;
float tickPreFrame = 0;
float tickCurrentFrame = 0;

/*------------------*
Important Components
--------------------*/

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Globally used font
TTF_Font* gFont = nullptr;

//Game controller handler with force feedback
SDL_GameController* gGameController = nullptr;

//Joystick handler with haptic
SDL_Joystick* gJoystick = nullptr;
SDL_Haptic* gJoyHaptic = nullptr;

/*--------*
Structures
----------*/

//A circle structure
struct Circle {
    int x, y;
    int r;
};

/*-----*
Classes
-------*/

//Texture wrapper class
class LTexture {
public:
    //Initializes variables
    LTexture();
    //Deallocates memory
    ~LTexture();

    //Loads image at specified path
    bool loadFromFile(const std::string& path);

    //Creates image from font string
    bool loadFromRenderedText(const std::string& textureText, SDL_Color textColor);

    //Deallocates texture
    void free();

    //Set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //Set blending
    void setBlendMode(SDL_BlendMode blending);

    //Set alpha modulation
    void setAlpha(Uint8 alpha);

    //Renders texture at given point
    void render(int x, int y, const SDL_Rect* clip, double angle, const SDL_Point* center, SDL_RendererFlip flip);

    //Gets image dimensions
    int getWidth() const;
    int getHeight() const;

private:
    //The actual hardware texture
    SDL_Texture* mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

//The application time based timer
class LTimer {
public:
    //Initializes variables
    LTimer();

    //The various clock actions
    void start();

    void stop();

    void pause();

    void unpause();

    //Gets the timer's time
    Uint32 getTicks() const;

    //Checks the status of the timer
    bool isStarted() const;

    bool isPaused() const;

private:
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};

/*-----*
Objects
-------*/

//Prompt text texture
LTexture gPromptTextTexture;

//Data Texture
LTexture gDataTexture[TOTAL_DATA];

/*-------*
File data
---------*/

//Data points
Sint32 gData[TOTAL_DATA];

/*-----------------------*
Function prototypes below
-------------------------*/

//Load prototypes
bool init();
bool loadMedia();
void close();

//Calculate distance between centres of two circles
double distanceSquared(int x1, int y1, int x2, int y2);

//Circle-Circle collision detector
bool checkCollision(const Circle& a, const Circle& b);

//Circle-Box collision detector
bool checkCollision(const Circle& a, const SDL_Rect& b);

/*----------------*
LTexture functions
------------------*/

//Initializes variable
LTexture::LTexture() {
    //Initialize
    mTexture = nullptr;
    mWidth = 0;
    mHeight = 0;
}

//Deallocates memory
LTexture::~LTexture() {
    //Deallocate
    free();
}

//Loads image at specified path
bool LTexture::loadFromFile(const std::string& path) {
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
        SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));

        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
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
bool LTexture::loadFromRenderedText(const std::string& textureText, const SDL_Color textColor) {
    //Get rid of preexisting texture
    free();

    //Render text surface
    if (SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor); textSurface == nullptr) {
        printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
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
void LTexture::free() {
    if (mTexture != nullptr) {
        SDL_DestroyTexture(mTexture);
        mTexture = nullptr;
        mWidth = 0;
        mHeight = 0;
    }
}

//Set color modulation
void LTexture::setColor(const Uint8 red, const Uint8 green, const Uint8 blue) {
    //Modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

//Set blending
void LTexture::setBlendMode(const SDL_BlendMode blending) {
    //Set blending function
    SDL_SetTextureBlendMode(mTexture, blending);
}

//Set alpha modulation
void LTexture::setAlpha(const Uint8 alpha) {
    //Modulate texture alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

//Renders texture at given point
void LTexture::render(const int x, const int y, const SDL_Rect* clip = nullptr, const double angle = 0.0, const SDL_Point* center = nullptr, const SDL_RendererFlip flip = SDL_FLIP_NONE) {

    //Set rendering space and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //Set clip rendering dimensions
    if (clip != nullptr) {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

//Gets image dimensions
int LTexture::getWidth() const {
    return mWidth;
}

int LTexture::getHeight() const {
    return mHeight;
}

/*--------------*
LTimer functions
----------------*/

//Initializes variables
LTimer::LTimer() {
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

//The various clock actions
void LTimer::start() {
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void LTimer::stop() {
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void LTimer::pause() {
    //If the timer is running and isn't already paused
    if (mStarted && !mPaused) {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void LTimer::unpause() {
    //If the timer is running and paused
    if (mStarted && mPaused) {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;

    }
}

//Gets the timer's time
Uint32 LTimer::getTicks() const {
    //The actual timer time
    Uint32 time = 0;

    //If the timer is running
    if (mStarted) {
        //If the timer is paused
        if (mPaused) {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }
    return time;
}

//Checks the status of the timer
bool LTimer::isStarted() const {
    //Timer is running and paused or unpaused
    return mStarted;
}

bool LTimer::isPaused() const {
    //Timer is running and paused
    return mPaused && mStarted;
}

/**Main functions*/

//Starts up the SDL and creates window
bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else {
        //Set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //Create window
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == nullptr) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
            if (gRenderer == nullptr) {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else {
                //Initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags)) {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }

                //Initialize SDL_mixer
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                    success = false;
                }

                //Initialize SDL_ttf
                if (TTF_Init() == -1) {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }

    return success;
}

//Loads media
bool loadMedia() {
    //Loading success flag
    bool success = true;

    //Text rendering color
    SDL_Color textColor = {0, 0, 0, 0xFF};
    SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

    //Open the font
    gFont = TTF_OpenFont("../assets/fonts/lazy.ttf", 28);
    if (gFont == nullptr) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }

    //Open file for reading in binary
    SDL_RWops* file = SDL_RWFromFile("../data/nums.bin", "r+b");

    if (file == nullptr) {
        printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

        //Create file for writing
        file = SDL_RWFromFile("../data/nums.bin", "w+b");
        if (file != nullptr) {
            printf("New file created!\n");

            //Initialize data
            for (int i = 0; i < TOTAL_DATA; ++i) {
                gData[i] = 0;
                SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
            }

            //Close file handler
            SDL_RWclose(file);
        }
        else {
            printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
    }
    else {
        //Load data
        printf("Reading file...!\n");
        for (int i = 0; i < TOTAL_DATA; ++i) {
            SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
        }

        //Close file handler
        SDL_RWclose(file);
    }

    //Initialize data textures
    gDataTexture[0].loadFromRenderedText(std::to_string(gData[0]), highlightColor);
    for (int i = 1; i < TOTAL_DATA; ++i) {
        gDataTexture[i].loadFromRenderedText(std::to_string(gData[i]), textColor);
    }

    //Initialize prompt text texture
    gPromptTextTexture.loadFromRenderedText("Enter Data:", textColor);
    return success;
}

//Frees media and shuts down SDL
void close() {
    //Open data for writing
    SDL_RWops* file = SDL_RWFromFile("../data/nums.bin", "w+b");
    if (file != nullptr) {
        //Save data
        for (int i = 0; i < TOTAL_DATA; ++i) {
            SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
        }

        //Close file handler
        SDL_RWclose(file);
    }
    else {
        printf("Error: Unable to save file! SDL Error: %s\n", SDL_GetError());
    }

    //Free loaded image
    for (int i = 0; i < TOTAL_DATA; ++i) {
        gDataTexture[i].free();
    }

    //Free global font
    TTF_CloseFont(gFont);
    gFont = nullptr;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Quit SDL subsystems
    Mix_Quit();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

//Main loop
int main(int argc, char* args[]) {
    if (!init()) {
        printf("Failed to initialize!\n");
    }
    else {
        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }
        else {

            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //Text rendering color
            constexpr SDL_Color textColor = {0, 0, 0, 0xFF};
            constexpr SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

            //Current input point
            int currentData = 0;

            //While application is running
            while(!quit) {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {

                    //User requests quit
                    if(e.type == SDL_QUIT) quit = true;

                    //Handle key press
                    else if(e.type == SDL_KEYDOWN) {
                        switch(e.key.keysym.sym) {
                            case SDLK_ESCAPE:
                                quit = true;
                                break;
                            //Previous data entry
                            case SDLK_UP:
                                //Rerender previous entry input point
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), textColor);
                                --currentData;
                                if (currentData < 0) {
                                    currentData = TOTAL_DATA - 1;
                                }
                                //Rerender current entry input point
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), highlightColor);
                                break;
                            case SDLK_DOWN:
                                //Rerender previous entry input point
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), textColor);
                                ++currentData;
                                if (currentData == TOTAL_DATA) {
                                    currentData = 0;
                                }
                                //Rerender current entry input point
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), highlightColor);
                                break;
                            //Decrement input point
                            case SDLK_LEFT:
                                --gData[currentData];
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), highlightColor);
                                break;
                            //Increment input point
                            case SDLK_RIGHT:
                                ++gData[currentData];
                                gDataTexture[currentData].loadFromRenderedText(std::to_string(gData[currentData]), highlightColor);
                                break;
                            default:
                                break;
                        }
                    }
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render text textures
                gPromptTextTexture.render((SCREEN_WIDTH - gPromptTextTexture.getWidth()) / 2, 0);
                for (int i = 0; i < TOTAL_DATA; ++i) {
                    gDataTexture[i].render((SCREEN_WIDTH - gDataTexture[i].getWidth()) / 2, gPromptTextTexture.getHeight() + gDataTexture[i].getHeight() * i);
                }

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }
    //Free resources and close SDL
    close();

    return 0;
}

/*-----------------*
Secondary functions
-------------------*/

