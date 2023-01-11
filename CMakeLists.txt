# 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

cmake_minimum_required(VERSION 3.10)
project(Playground)

# ==============================================================================
# Include and library pathes
# ==============================================================================

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS})

# ==============================================================================
# Headers and sources
# ==============================================================================

include_directories(src/engine src/sdl)

set(ENGINE_SOURCES
    src/engine/rb_sdl.c
    src/engine/rb_sdl.h
    src/engine/rb_base.h
    src/engine/rb_draw.c
    src/engine/rb_draw.h
    src/engine/rb_draw3d.c
    src/engine/rb_draw3d.h
    src/engine/rb_engine.c
    src/engine/rb_engine.h
    src/engine/rb_graphics.c
    src/engine/rb_graphics.h
    src/engine/rb_input.c
    src/engine/rb_input.h
    src/engine/rb_level.c
    src/engine/rb_level.h
    src/engine/rb_math.c
    src/engine/rb_math.h
    src/engine/rb_primitive.c
    src/engine/rb_primitive.h
    src/engine/rb_vector.c
    src/engine/rb_vector.h
    src/engine/rb_vtext.c
    src/engine/rb_vtext.h
    src/engine/rb_keys.h
    src/engine/rb_platform.c
    src/engine/rb_platform.h
)

set(GAME_SOURCES
	src/game/vexxon.c
)

# ==============================================================================
# Resource definition
# ==============================================================================

set(RESOURCE_FILES
    assets/level1.txt
)

source_group(
    "Resources" FILES ${RESOURCE_FILES}
)

list(APPEND
    GAME_SOURCES
    ${RESOURCE_FILES}
) 

# ==============================================================================
# Target properties and definitions
# ==============================================================================

set(CMAKE_CXX_STANDARD 17) 
#set_target_properties(${TARGET} PROPERTIES C_STANDARD 90)
#set_target_properties(${TARGET} PROPERTIES C_EXTENSIONS ON)
#set_target_properties(${TARGET} PROPERTIES INTERPROCEDURAL_OPTIMIZATION ON)

set(TARGET Playground)

add_executable(${TARGET} ${GAME_SOURCES} ${ENGINE_SOURCES} ${RESOURCE_FILES})

if(APPLE)
    set_target_properties(Playground PROPERTIES MACOSX_BUNDLE TRUE MACOSX_FRAMEWORK_IDENTIFIER org.cmake.ExecutableTarget RESOURCE "${RESOURCE_FILES}" )
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
	target_compile_features(${TARGET} PUBLIC c_std_99)
endif()

if(UNIX AND NOT APPLE)
    target_compile_options(${TARGET} PRIVATE -D_POSIX_C_SOURCE=200809L)
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

source_group("Engine" FILES ${ENGINE_SOURCES})
source_group("Platform" FILES ${PLATFORM_SOURCES})
source_group("Vexxon" FILES ${GAME_SOURCES})

# ==============================================================================
# macOS apps bundle support
# ==============================================================================

set_target_properties(${TARGET} PROPERTIES
	MACOSX_BUNDLE TRUE
)
