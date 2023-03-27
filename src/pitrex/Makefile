# VEXXON makefile (PiTrex ONLY)

# Just change root dir if your repo is located in another folder
# This relative path works if the Makefile is in ~/{project}/src,
# otherwise you must change here.
ROOT_DIR := ../../pitrex/pitrex

# This is the standard folder structure used in the PiTrex repo
SDK_DIR := $(ROOT_DIR)/pitrex
INC_DIR := $(SDK_DIR)
PITREX_DIR := $(SDK_DIR)/pitrex/
VECTREX_DIR := $(SDK_DIR)/vectrex/

# Source path for the different modules
SRC_ROOT_DIR := ../
SRC_BASE_DIR := $(SRC_ROOT_DIR)base/
SRC_ENGINE3D_DIR := $(SRC_ROOT_DIR)engine3d/
SRC_GAME_DIR := $(SRC_ROOT_DIR)game/

# Build and compiler flags
SETTINGS := /opt/pitrex/settings
BUILD_DIR := build/
CFLAGS := -g -I$(INC_DIR) -I$(SRC_BASE_DIR) -I$(SRC_ENGINE3D_DIR) -I$(SRC_GAME_DIR) -DSETTINGS_DIR="\"$(SETTINGS)\"" -DAVOID_TICKS -DPIZERO -DRPI0 -DPITREX -lpthread -lm
CC := gcc

.PHONY: dirCheck vexxon

dirCheck:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi

clean:
	$(RM) $(BUILD_DIR)*.*
	$(RM) vexxon

# Project files (Game)
$(BUILD_DIR)game_vexxon.o: $(SRC_GAME_DIR)game_vexxon.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)game_vexxon.o -c $(SRC_GAME_DIR)game_vexxon.cpp

# Project files (Engine3D)
$(BUILD_DIR)rb_engine.o: $(SRC_ENGINE3D_DIR)rb_engine.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_engine.o -c $(SRC_ENGINE3D_DIR)rb_engine.cpp
$(BUILD_DIR)rb_file.o: $(SRC_ENGINE3D_DIR)rb_file.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_file.o -c $(SRC_ENGINE3D_DIR)rb_file.cpp
$(BUILD_DIR)rb_level.o: $(SRC_ENGINE3D_DIR)rb_level.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_level.o -c $(SRC_ENGINE3D_DIR)rb_level.cpp
$(BUILD_DIR)rb_math.o: $(SRC_ENGINE3D_DIR)rb_math.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_math.o -c $(SRC_ENGINE3D_DIR)rb_math.cpp
$(BUILD_DIR)rb_mesh.o: $(SRC_ENGINE3D_DIR)rb_mesh.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_mesh.o -c $(SRC_ENGINE3D_DIR)rb_mesh.cpp
$(BUILD_DIR)rb_object.o: $(SRC_ENGINE3D_DIR)rb_object.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_object.o -c $(SRC_ENGINE3D_DIR)rb_object.cpp

# Project files (Base)
$(BUILD_DIR)rb_log.o: $(SRC_BASE_DIR)rb_log.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_log.o -c $(SRC_BASE_DIR)rb_log.c

# Project files (PiTrex)
$(BUILD_DIR)rb_pitrex_main.o: rb_pitrex_main.cpp
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_pitrex_main.o -c rb_pitrex_main.cpp
$(BUILD_DIR)rb_pitrex_platform.o: rb_pitrex_platform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_pitrex_platform.o -c rb_pitrex_platform.c
$(BUILD_DIR)rb_pitrex_window.o: rb_pitrex_window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_pitrex_window.o -c rb_pitrex_window.c

# PiTrex lib files
$(BUILD_DIR)bcm2835.o: $(PITREX_DIR)bcm2835.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bcm2835.o -c $(PITREX_DIR)bcm2835.c

$(BUILD_DIR)pitrexio-gpio.o: $(PITREX_DIR)pitrexio-gpio.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrexio-gpio.o -c $(PITREX_DIR)pitrexio-gpio.c

# Vectrex lib files
$(BUILD_DIR)vectrexInterface.o: $(VECTREX_DIR)vectrexInterface.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vectrexInterface.o -c $(VECTREX_DIR)vectrexInterface.c
$(BUILD_DIR)osWrapper.o: $(VECTREX_DIR)osWrapper.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)osWrapper.o -c $(VECTREX_DIR)osWrapper.c
$(BUILD_DIR)baremetalUtil.o: $(VECTREX_DIR)baremetalUtil.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)baremetalUtil.o -c $(VECTREX_DIR)baremetalUtil.c


# Build executable
vexxon:	$(BUILD_DIR)game_vexxon.o \
		$(BUILD_DIR)rb_engine.o $(BUILD_DIR)rb_file.o $(BUILD_DIR)rb_rb_level.o $(BUILD_DIR)rb_math.o $(BUILD_DIR)rb_mesh.o $(BUILD_DIR)rb_object.o \
		$(BUILD_DIR)rb_log.o \
		$(BUILD_DIR)rb_pitrex_main.o $(BUILD_DIR)rb_pitrex_platform.o $(BUILD_DIR)rb_pitrex_window.o \
		$(BUILD_DIR)bcm2835.o $(BUILD_DIR)pitrexio-gpio.o $(BUILD_DIR)vectrexInterface.o $(BUILD_DIR)osWrapper.o $(BUILD_DIR)baremetalUtil.o

	$(RM) vexxon
	$(CC) $(CFLAGS) -o vexxon \
	$(BUILD_DIR)game_vexxon.o \
	$(BUILD_DIR)rb_engine.o $(BUILD_DIR)rb_file.o $(BUILD_DIR)rb_rb_level.o $(BUILD_DIR)rb_math.o $(BUILD_DIR)rb_mesh.o $(BUILD_DIR)rb_object.o \
	$(BUILD_DIR)rb_log.o \
	$(BUILD_DIR)rb_pitrex_main.o \
	$(BUILD_DIR)rb_pitrex_platform.o \
	$(BUILD_DIR)rb_pitrex_window.o \
	$(BUILD_DIR)bcm2835.o \
	$(BUILD_DIR)pitrexio-gpio.o \
	$(BUILD_DIR)vectrexInterface.o \
	$(BUILD_DIR)osWrapper.o \
	$(BUILD_DIR)baremetalUtil.o
