#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <string>
#include <sstream>

/**Constant variables*/

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

//Maximum number of supported recording devices
constexpr int MAX_RECORDING_DEVICES = 10;

//Maximum recording time
constexpr int MAX_RECORDING_SECONDS = 5;

//Maximum recording time plus padding
constexpr int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;

//The various recording actions we can take
enum RecordingState {
    SELECTING_DEVICE,
    STOPPED,
    RECORDING,
    RECORDED,
    PLAYBACK,
    ERROR
};

/**Non-constant variables*/

//Number of available devices
int gRecordingDeviceCount = 0;

//Recording data buffer
Uint8* gRecordingBuffer = nullptr;

//Size of data buffer
Uint32 gBufferByteSize = 0;

//Position in data buffer
Uint32 gBufferBytePosition = 0;

//Maximum position in data buffer for recording
Uint32 gBufferByteMaxPosition = 0;

//Text color
SDL_Color gTextColor = {0, 0, 0, 0xFF};

/**Important Components*/

//The window we'll be rendering to
SDL_Window* gWindow = nullptr;

//The window renderer
SDL_Renderer* gRenderer = nullptr;

//Globally used font
TTF_Font* gFont = nullptr;

//Received audio spec
SDL_AudioSpec gReceivedRecordingSpec;
SDL_AudioSpec gReceivedPlaybackSpec;

/**Structures*/

/**Classes*/

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

/**Objects*/

//Prompt Texture
LTexture gPromptTexture;

//The text textures that specify recording device names
LTexture gDeviceTextures[MAX_RECORDING_DEVICES];

/**Function prototypes below*/

//Load prototypes
bool init();
bool loadMedia();
void close();

/**LTexture functions*/

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

/**LTimer functions*/

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

/**Recording functions*/

//Recording/playback callbacks
void audioRecordingCallBack(void* userdata, Uint8* stream, const int len) {
    //Copy audio from stream
    memcpy(&gRecordingBuffer[gBufferBytePosition], stream, len);

    //Move along buffer
    gBufferBytePosition += len;
}
void audioPlaybackCallBack(void* userdata, Uint8* stream, const int len) {
    //Copy audio to stream
    memcpy(stream, &gRecordingBuffer[gBufferBytePosition], len);

    //Move along buffer
    gBufferBytePosition += len;
}

/**Main functions*/

//Starts up the SDL and creates window
bool init() {
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
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
                if (constexpr int imgFlags = IMG_INIT_PNG; !(IMG_Init(imgFlags) & imgFlags)) {
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

    //Open the font
    gFont = TTF_OpenFont("../assets/fonts/lazy.ttf", 28);
    if (gFont == nullptr) {
        printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else {
        //Set starting prompt
        gPromptTexture.loadFromRenderedText("Select your recording device:", gTextColor);

        //Get capture device count
        gRecordingDeviceCount = SDL_GetNumAudioDevices(SDL_TRUE);

        //No recording devices
        if (gRecordingDeviceCount < 0) {
            printf("Unable to get audio capture device! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else {
            //Cap recording device count
            if (gRecordingDeviceCount > MAX_RECORDING_DEVICES) {
                gRecordingDeviceCount = MAX_RECORDING_DEVICES;
            }

            //Render device names
            std::stringstream promptText;
            for (int i = 0; i < gRecordingDeviceCount; i++) {
                //Get capture device name
                promptText.str("");
                promptText << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);

                //Set texture from name
                gDeviceTextures[i].loadFromRenderedText(promptText.str(), gTextColor);
            }
        }
    }

    return success;
}

//Frees media and shuts down SDL
void close() {
    //Free loaded image
    gPromptTexture.free();
    for (int i = 0; i < MAX_RECORDING_DEVICES; ++i) {
        gDeviceTextures[i].free();
    }

    //Free global font
    TTF_CloseFont(gFont);
    gFont = nullptr;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = nullptr;
    gRenderer = nullptr;

    //Free playback audio
    if (gRecordingBuffer != nullptr) {
        delete[] gRecordingBuffer;
        gRecordingBuffer = nullptr;
    }

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

            //Set the default recording state
            RecordingState currentState = SELECTING_DEVICE;

            //Audio device IDs
            SDL_AudioDeviceID recordingDeviceId = 0;
            SDL_AudioDeviceID playbackDeviceId = 0;

            //While application is running
            while(!quit) {
                //Handle events on queue
                while(SDL_PollEvent(&e) != 0) {

                    //User requests quit
                    if(e.type == SDL_QUIT)
                        quit = true;

                    //Do current state event handling
                    switch (currentState) {
                        //User is selecting recording device
                        case SELECTING_DEVICE:
                            //On key press
                            if (e.type == SDL_KEYDOWN) {
                                //Handle key press from 0 to 9
                                if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
                                    //Get selection index
                                    int index = e.key.keysym.sym - SDLK_0;

                                    //Index is valid
                                    if (index < gRecordingDeviceCount) {
                                        //Default audio spec
                                        SDL_AudioSpec desiredRecordingSpec;
                                        SDL_zero(desiredRecordingSpec);
                                        desiredRecordingSpec.freq = 44100;
                                        desiredRecordingSpec.format = AUDIO_F32;
                                        desiredRecordingSpec.channels = 2;
                                        desiredRecordingSpec.samples = 4096;
                                        desiredRecordingSpec.callback = audioRecordingCallBack;

                                        //Open recording device
                                        recordingDeviceId = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(index, SDL_TRUE), SDL_TRUE, &desiredRecordingSpec, &gReceivedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);

                                        //Device failed to open
                                        if (recordingDeviceId == 0) {
                                            //Report error
                                            printf("Failed to open recording device! SDL Error: %s\n", SDL_GetError());
                                            gPromptTexture.loadFromRenderedText("Failed to open recording device!", gTextColor);
                                            currentState = ERROR;
                                        }
                                        //Device opened successfully
                                        else {
                                            //Default audio spec
                                            SDL_AudioSpec desiredPlaybackSpec;
                                            SDL_zero(desiredPlaybackSpec);
                                            desiredPlaybackSpec.freq = 44100;
                                            desiredPlaybackSpec.format = AUDIO_F32;
                                            desiredPlaybackSpec.channels = 2;
                                            desiredPlaybackSpec.samples = 4096;
                                            desiredPlaybackSpec.callback = audioPlaybackCallBack;

                                            //Open playback device
                                            playbackDeviceId = SDL_OpenAudioDevice(nullptr, SDL_FALSE, &desiredPlaybackSpec, &gReceivedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
                                            if (playbackDeviceId == 0) {
                                                //Report error
                                                printf("Failed to open playback device! SDL Error: %s\n", SDL_GetError());
                                                gPromptTexture.loadFromRenderedText("Failed to open playback device!", gTextColor);
                                                currentState = ERROR;
                                            }
                                            else {
                                                //Calculate per sample bytes
                                                const int bytesPerSample = gReceivedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(gReceivedRecordingSpec.format) / 8);

                                                //Calculate bytes per second
                                                const int bytesPerSecond = gReceivedRecordingSpec.freq * bytesPerSample;

                                                //Calculate buffer size
                                                gBufferByteSize = RECORDING_BUFFER_SECONDS * bytesPerSecond;

                                                //Calculate max buffer use
                                                gBufferByteMaxPosition = MAX_RECORDING_SECONDS * bytesPerSecond;

                                                //Allocate and initialize byte buffer
                                                gRecordingBuffer = new Uint8[gBufferByteSize];
                                                memset(gRecordingBuffer, 0, gBufferByteSize);

                                                //Go on to next state
                                                gPromptTexture.loadFromRenderedText("Press 1 to record for 5 seconds.", gTextColor);
                                                currentState = STOPPED;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        //User getting ready to record
                        case STOPPED:
                            //On key press
                            if (e.type == SDL_KEYDOWN) {
                                //Start recording
                                if (e.key.keysym.sym == SDLK_1) {
                                    //Go back to beginning of buffer
                                    gBufferBytePosition = 0;

                                    //Start recording
                                    SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);

                                    //Go on to next state
                                    gPromptTexture.loadFromRenderedText("Recording...", gTextColor);
                                    currentState = RECORDING;
                                }
                            }
                            break;
                        //User has finished recording
                        case RECORDED:
                            //On key press
                            if (e.type == SDL_KEYDOWN) {
                                //Start playback
                                if (e.key.keysym.sym == SDLK_1) {
                                    //Go back to beginning of buffer
                                    gBufferBytePosition = 0;

                                    //Start playback
                                    SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);

                                    //Go on to next state
                                    gPromptTexture.loadFromRenderedText("Playing...", gTextColor);
                                    currentState = PLAYBACK;
                                }
                                //Record again
                                if (e.key.keysym.sym == SDLK_2) {
                                    //Reset the buffer
                                    gBufferBytePosition = 0;
                                    memset(gRecordingBuffer, 0, gBufferByteSize);

                                    //Start recording
                                    SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);

                                    //Go on to the next state
                                    gPromptTexture.loadFromRenderedText("Recording...", gTextColor);
                                    currentState = RECORDING;
                                }
                            }
                            break;
                        default:
                            break;
                    }
                }
                //Updating recording
                if (currentState == RECORDING) {
                    //Lock callback
                    SDL_LockAudioDevice(recordingDeviceId);

                    //Finished recording
                    if (gBufferBytePosition > gBufferByteMaxPosition) {
                        //Stop recording audio
                        SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);

                        //Go on to next state
                        gPromptTexture.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", gTextColor);
                        currentState = RECORDED;
                    }

                    //Unlock callback
                    SDL_UnlockAudioDevice(recordingDeviceId);
                }
                else if (currentState == PLAYBACK) {
                    //Lock callback
                    SDL_LockAudioDevice(playbackDeviceId);

                    //Finished playback
                    if (gBufferBytePosition > gBufferByteMaxPosition) {
                        //Stop playing audio
                        SDL_PauseAudioDevice(playbackDeviceId, SDL_TRUE);

                        //Go on to next state
                        gPromptTexture.loadFromRenderedText("Press 1 to play back. Press 2 to record again.", gTextColor);
                        currentState = RECORDED;
                    }

                    //Unlock callback
                    SDL_UnlockAudioDevice(playbackDeviceId);
                }

                //Clear screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //Render prompt centered at the top of the screen
                gPromptTexture.render((SCREEN_WIDTH - gPromptTexture.getWidth()) / 2, 0);

                //User is selecting
                if (currentState == SELECTING_DEVICE) {
                    //Render device names
                    int yOffset = gPromptTexture.getHeight() * 2;
                    for (int i = 0; i < gRecordingDeviceCount; ++i) {
                        gDeviceTextures[i].render(0, yOffset);
                        yOffset += gDeviceTextures[i].getHeight() + 1;
                    }
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

/**Secondary functions*/
