cmake_minimum_required(VERSION 3.18.4)

if(LINUX)
    set(CMAKE_C_COMPILER /usr/bin/gcc)
endif()

project("Bruhs" LANGUAGES C)

# Main Programin
add_executable(${PROJECT_NAME}
	src/objects/object.c  src/objects/object.h
	src/objects/player.c  src/objects/player.h
	src/objects/goomba.c src/objects/goomba.h
    src/globals.c src/globals.h
	src/levels.c  src/levels.h
    src/main.c
)

# Libs
add_subdirectory(thirdparty/raylib)
target_include_directories(${PROJECT_NAME} 
    PRIVATE thirdparty/raylib/src
)

target_link_directories(${PROJECT_NAME} 
    PRIVATE thirdparty/raylib/src
)

target_link_libraries(${PROJECT_NAME} raylib)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
