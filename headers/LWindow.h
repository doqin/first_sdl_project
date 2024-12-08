//
// Created by đỗ quyên on 5/12/24.
//

#ifndef LWINDOW_H
#define LWINDOW_H
#include <SDL.h>
#include <sstream>

extern int gTotalDisplays;
extern SDL_Rect* gDisplayBounds;

class LWindow {
public:
    //Initializes internals
    LWindow();

    //Creates window
    bool init(int windowWidth, int windowHeight);

    //Handles window events
    void handleEvent(const SDL_Event& e);

    //Focuses on window
    void focus();

    //Clear window contents
    void clear();

    //Shows windows contents
    void render();

    //Deallocates internals
    void free();

    //Window dimensions
    int getWidth();
    int getHeight();

    //Window's renderer
    SDL_Renderer* getRenderer();

    //window focii
    bool hasMouseFocus();
    bool hasKeyboardFocus();
    bool isMinimized();
    bool isShown();
private:
    //Window data
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    int mWindowID;
    int mWindowDisplayID;

    //Window dimensions
    int mWidth;
    int mHeight;

    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullscreen;
    bool mMinimized;
    bool mShown;
};

/*--------*
 Functions
 ---------*/

inline LWindow::LWindow() {
    //Initialize non-existent window
    mWindow = nullptr;
    mRenderer = nullptr;
    mMouseFocus = false;
    mKeyboardFocus = false;
    mFullscreen = false;
    mMinimized = false;
    mShown = false;
    mWindowID = 0;
    mWindowDisplayID = 0;
    mWidth = 0;
    mHeight = 0;
}

inline bool LWindow::init(const int windowWidth, const int windowHeight) {
    //Create window
    mWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (mWindow != nullptr) {
        mMouseFocus = true;
        mKeyboardFocus = true;
        mWidth = windowWidth;
        mHeight = windowHeight;

        //Create renderer for window
        mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (mRenderer == nullptr) {
            printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
        }
        else {
            //Initialize renderer color
            SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

            //Grab window identifiers
            mWindowID = SDL_GetWindowID(mWindow);
            mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);

            //Flag as opened
            mShown = true;
        }
    }
    else {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
    }

    return mWindow != nullptr && mRenderer != nullptr;
}

inline void LWindow::handleEvent(const SDL_Event& e) {
    //Caption update flag
    bool updateCaption = false;

    //Window event occurred
    if (e.type == SDL_WINDOWEVENT && e.window.windowID == mWindowID) {
        switch (e.window.event) {
            //Window moved
            case SDL_WINDOWEVENT_MOVED:
                mWindowDisplayID = SDL_GetWindowDisplayIndex(mWindow);
                updateCaption = true;
                break;
            //Window appeared
            case SDL_WINDOWEVENT_SHOWN:
                mShown = true;
                break;
            //Window disappeared
            case SDL_WINDOWEVENT_HIDDEN:
                mShown = false;
                break;
            //Get new dimensions and repaint on window size change
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                mWidth = e.window.data1;
                mHeight = e.window.data2;
                SDL_RenderPresent(mRenderer);
                break;
            //Repaint on exposure
            case SDL_WINDOWEVENT_EXPOSED:
                SDL_RenderPresent(mRenderer);
                break;
            //Mouse entered window
            case SDL_WINDOWEVENT_ENTER:
                mMouseFocus = true;
                updateCaption = true;
                break;
            //Mouse left window
            case SDL_WINDOWEVENT_LEAVE:
                mMouseFocus = false;
                updateCaption = true;
                break;
            //Window has keyboard focus
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                mKeyboardFocus = true;
                updateCaption = true;
                break;
            //Window lost keyboard focus
            case SDL_WINDOWEVENT_FOCUS_LOST:
                mKeyboardFocus = false;
                updateCaption = true;
                break;
            //Window minimized
            case SDL_WINDOWEVENT_MINIMIZED:
                mMinimized = true;
                break;
            //Window maximized
            case SDL_WINDOWEVENT_MAXIMIZED:
                mMinimized = false;
                break;
            //Window restored
            case SDL_WINDOWEVENT_RESTORED:
                mMinimized = false;
                break;
            //Hide on close
            case SDL_WINDOWEVENT_CLOSE:
                SDL_HideWindow(mWindow);
                break;
        }
    }
    //Enter exit full screen on return key
    else if (e.type == SDL_KEYDOWN && e.window.windowID == mWindowID) {
        //Display change flag
        bool switchDisplay = false;

        switch (e.key.keysym.sym) {
            case SDLK_RETURN:
                if (mFullscreen) {
                    SDL_SetWindowFullscreen(mWindow, 0);
                    mFullscreen = false;
                }
                else {
                    SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    mFullscreen = true;
                    mMinimized = false;
                }
                break;
            case SDLK_EQUALS:
                ++mWindowDisplayID;
                switchDisplay = true;
                break;
            case SDLK_MINUS:
                --mWindowDisplayID;
                switchDisplay = true;
                break;
        }
        //Display needs to be updated
        if (switchDisplay) {
            //Bound display index
            if (mWindowDisplayID < 0) {
                mWindowDisplayID = gTotalDisplays - 1;
            }
            else if (mWindowDisplayID >= gTotalDisplays) {
                mWindowDisplayID = 0;
            }
            //Move window to center of next display
            SDL_SetWindowPosition(mWindow, gDisplayBounds[mWindowDisplayID].x + (gDisplayBounds[mWindowDisplayID].w - mWidth) / 2, gDisplayBounds[mWindowDisplayID].y + (gDisplayBounds[mWindowDisplayID].h - mHeight) / 2);
            updateCaption = true;
        }
    }
    //Update window caption with new data
    if (updateCaption) {
        std::stringstream caption;
        caption << "SDL Tutorial - ID: " << mWindowID << " Display: " << mWindowDisplayID << " MouseFocus: " << ((mMouseFocus) ? "On" : "Off") << " KeyboardFocus: " << ((mKeyboardFocus) ? "On" : "Off");
        SDL_SetWindowTitle(mWindow, caption.str().c_str());
    }
}

inline void LWindow::focus() {
    //Restore window if needed
    if (!mShown) {
        SDL_ShowWindow(mWindow);
    }
    //Move window forward
    SDL_RaiseWindow(mWindow);
}

inline void LWindow::clear() {
    if (!mMinimized) {
        //Clear screen
        SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(mRenderer);
    }
}

inline void LWindow::render() {
    if (!mMinimized) {
        //Update screen
        SDL_RenderPresent(mRenderer);
    }
}

inline void LWindow::free() {
    if (mWindow != nullptr) {
        SDL_DestroyWindow(mWindow);
        mWindow = nullptr;
    }
    if (mRenderer != nullptr) {
        SDL_DestroyRenderer(mRenderer);
        mRenderer = nullptr;
    }
}

inline int LWindow::getWidth() {
    return mWidth;
}

inline int LWindow::getHeight() {
    return mHeight;
}

inline SDL_Renderer* LWindow::getRenderer() {
    return mRenderer;
}

inline bool LWindow::hasMouseFocus() {
    return mMouseFocus;
}

inline bool LWindow::hasKeyboardFocus() {
    return mKeyboardFocus;
}

inline bool LWindow::isMinimized() {
    return mMinimized;
}

inline bool LWindow::isShown() {
    return mShown;
}


/*-----*
Objects
-------*/

//The window we're rendering to
extern LWindow gWindow;

#endif //LWINDOW_H
