
cmake_minimum_required(VERSION 3.0)
project(Playground)

# ==============================================================================
# Include and library pathes
# ==============================================================================

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

# ==============================================================================
# Headers and sources
# ==============================================================================

include_directories(src/sdl ../base ../engine3d  ../game)

set(BASE_SOURCES
    ../base/rb_base.h
    ../base/rb_platform.h
    ../base/rb_log.c
    ../base/rb_log.h
    ../base/rb_keys.h
    ../base/rb_vtext.c
    ../base/rb_vtext.h
)

set(ENGINE3D_SOURCES
    ../engine3d/rb_engine.cpp
    ../engine3d/rb_engine.hpp
    ../engine3d/rb_level.cpp
    ../engine3d/rb_level.hpp
    ../engine3d/rb_math.cpp
    ../engine3d/rb_math.hpp
    ../engine3d/rb_mesh.cpp
    ../engine3d/rb_mesh.hpp
    ../engine3d/rb_object.cpp
    ../engine3d/rb_object.hpp
    ../engine3d/rb_types.hpp
    ../engine3d/rb_file.cpp
    ../engine3d/rb_file.hpp
)

set(GAME_SOURCES
    ../game/game_vexxon.cpp
)

set(PLAYGROUND_SOURCES
    src/rb_sdl.cpp
    src/platform_win.c
    src/platform_mac.c
)

# ==============================================================================
# Resource definition
# ==============================================================================

set(ASSET_FILES
    ../game/assets/bullet.obj
    ../game/assets/jet.obj
    ../game/assets/rocket.obj
    ../game/assets/spaceship.obj
    ../game/assets/tank.obj
    ../game/assets/level1.txt
)

source_group(
    "Assets" FILES ${ASSET_FILES}
)

#list(APPEND
#    ENGINE3D_SOURCES
#    ${ASSET_FILES}
#) 

# ==============================================================================
# Target properties and definitions
# ==============================================================================

set(CMAKE_CXX_STANDARD 17) 
#set_target_properties(${TARGET} PROPERTIES C_STANDARD 90)
#set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS ON)
#set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

set(TARGET Playground)

if(MSVC)
    add_executable(${TARGET} ${PLAYGROUND_SOURCES} ${BASE_SOURCES} ${ENGINE3D_SOURCES} ${GAME_SOURCES})
endif()

if(APPLE)
    add_executable(${TARGET} ${PLAYGROUND_SOURCES} ${BASE_SOURCES} ${ENGINE3D_SOURCES} ${GAME_SOURCES} ${ASSET_FILES})
    set_target_properties(Playground PROPERTIES MACOSX_BUNDLE TRUE MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget RESOURCE "${ASSET_FILES}" )
endif()

if(UNIX AND NOT APPLE)
add_executable(${TARGET} ${PLAYGROUND_SOURCES} ${BASE_SOURCES} ${ENGINE3D_SOURCES} ${GAME_SOURCES} ${ASSET_FILES})
endif()

# ==============================================================================
# Compile options
# ==============================================================================

if(MSVC)
    target_compile_options(${TARGET} PRIVATE /W4 /D_CRT_SECURE_NO_WARNINGS)
    target_compile_options(${TARGET} PRIVATE /fp:fast)
else()
#    target_compile_options(${TARGET} PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(${TARGET} PRIVATE -ffast-math)
	#target_compile_features(${TARGET} PUBLIC c_std_99)
endif()

if(UNIX AND NOT APPLE)
    target_compile_options(${TARGET} PRIVATE -D_POSIX_C_SOURCE=200809L -std=c++11)
endif()

# ==============================================================================
# Link libraries
# ==============================================================================

if(WIN32)
    target_link_libraries(Playground ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
elseif(APPLE)
    target_link_libraries(${TARGET} PRIVATE "-framework Cocoa")
    target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES})
else()
    target_link_libraries(${TARGET} PRIVATE ${SDL2_LIBRARIES} m X11)
endif()

# ==============================================================================
# IDE support
# ==============================================================================

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT ${TARGET})

source_group("Playground" FILES ${PLAYGROUND_SOURCES})
source_group("Engine" FILES ${ENGINE3D_SOURCES})
source_group("Game" FILES ${GAME_SOURCES})
source_group("Base" FILES ${BASE_SOURCES})

# ==============================================================================
# macOS apps bundle support
# ==============================================================================

set_target_properties(${TARGET} PROPERTIES
	MACOSX_BUNDLE TRUE
)
