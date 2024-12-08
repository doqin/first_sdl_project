cmake_minimum_required(VERSION 3.30)
project(first_sdl_project)

set(CMAKE_CXX_STANDARD 23)

add_executable(${PROJECT_NAME}
        #src/audioRecording.cpp
        #src/windowEvents.cpp
        headers/global.h
        headers/LDot.h
        headers/LWindow.h
        headers/LTexture.h
        headers/LTimer.h
        headers/Circle.h
        headers/DeltaTime.h
        headers/CollisionDetection.h
        headers/GetDistance.h
        headers/AllHeaders.h
        headers/LParticle.h
        src/loadMedia.cpp
        src/close.cpp
        src/init.cpp
        src/main.cpp
        src/global.cpp
        src/eventHandler.cpp
        #src/readWriteFile.cpp
        #src/textInput.cpp
)

#find libraries and packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${SDL2_INCLUDE_DIRS}
        PUBLIC ${SDL2IMAGE_INCLUDE_DIRS}
        PUBLIC ${SDL2MIXER_INCLUDE_DIRS}
        PUBLIC ${SDL2TTF_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES}
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
        SDL2_ttf::SDL2_ttf
)

#enable compile command export
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
