# Vexxon makefile
# Just change root dir if your repo is located in another folder
ROOT_DIR := ../pitrex/pitrex

# This is teh standard folder structure used in the PiTrex repo
SDK_DIR := $(ROOT_DIR)/pitrex
INC_DIR := $(SDK_DIR)
PITREX_DIR := $(SDK_DIR)/pitrex/
VECTREX_DIR := $(SDK_DIR)/vectrex/

# Build and compiler flags
SETTINGS := /opt/pitrex/settings
BUILD_DIR := build/
CFLAGS := -g -I$(SDK_DIR) -DSETTINGS_DIR="\"$(SETTINGS)\"" -DAVOID_TICKS -DPIZERO -DRPI0 -DPITREX -lpthread
CC := gcc

.PHONY: dirCheck

all: vexxon
	echo All up to date

dirCheck:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi

clean:
	$(RM) $(BUILD_DIR)*.*


# pitrex lib files
$(BUILD_DIR)bcm2835.o: $(PITREX_DIR)bcm2835.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)bcm2835.o -c $(PITREX_DIR)bcm2835.c

$(BUILD_DIR)pitrexio-gpio.o: $(PITREX_DIR)pitrexio-gpio.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)pitrexio-gpio.o -c $(PITREX_DIR)pitrexio-gpio.c


# vectrex lib files
$(BUILD_DIR)vectrexInterface.o: $(VECTREX_DIR)vectrexInterface.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vectrexInterface.o -c $(VECTREX_DIR)vectrexInterface.c
$(BUILD_DIR)osWrapper.o: $(VECTREX_DIR)osWrapper.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)osWrapper.o -c $(VECTREX_DIR)osWrapper.c
$(BUILD_DIR)baremetalUtil.o: $(VECTREX_DIR)baremetalUtil.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)baremetalUtil.o -c $(VECTREX_DIR)baremetalUtil.c


# project files
$(BUILD_DIR)game_main.o: game_main.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)game_main.o -c game_main.cpp

$(BUILD_DIR)rb_engine.o: rb_engine.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_engine.o -c rb_engine.cpp

$(BUILD_DIR)rb_object.o: rb_object.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_object.o -c rb_object.cpp

$(BUILD_DIR)rb_mesh.o: rb_mesh.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_mesh.o -c rb_mesh.cpp

$(BUILD_DIR)rb_math.o: rb_math.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_math.o -c rb_math.cpp

$(BUILD_DIR)rb_level.o: rb_level.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_level.o -c rb_level.cpp

$(BUILD_DIR)platform.o: platform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)platform.o -c platform.c

$(BUILD_DIR)window.o: window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)window.o -c window.c

# server files (not yet used)
$(BUILD_DIR)rb_webserver.o: rb_webserver.cpp
	g++ $(CFLAGS) -o $(BUILD_DIR)rb_webserver.o -c rb_webserver.cpp

$(BUILD_DIR)rb_server.o: rb_server.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_server.o -c rb_server.c


# build
vexxon: $(BUILD_DIR)game_main.o \
	$(BUILD_DIR)rb_engine.o $(BUILD_DIR)rb_object.o $(BUILD_DIR)rb_level.o $(BUILD_DIR)rb_mesh.o $(BUILD_DIR)rb_math.o $(BUILD_DIR)platform.o \
	$(BUILD_DIR)rb_webserver.o $(BUILD_DIR)rb_server.o \
	$(BUILD_DIR)bcm2835.o $(BUILD_DIR)pitrexio-gpio.o $(BUILD_DIR)vectrexInterface.o $(BUILD_DIR)osWrapper.o $(BUILD_DIR)baremetalUtil.o $(BUILD_DIR)window.o

	$(RM) vexxon
	g++ $(CFLAGS) -o vexxon \
	$(BUILD_DIR)bcm2835.o \
	$(BUILD_DIR)pitrexio-gpio.o \
	$(BUILD_DIR)vectrexInterface.o \
	$(BUILD_DIR)osWrapper.o \
	$(BUILD_DIR)baremetalUtil.o \
	$(BUILD_DIR)window.o \
	$(BUILD_DIR)game_main.o \
	$(BUILD_DIR)rb_engine.o \
	$(BUILD_DIR)rb_object.o \
	$(BUILD_DIR)rb_math.o \
	$(BUILD_DIR)rb_mesh.o \
	$(BUILD_DIR)rb_level.o \
	$(BUILD_DIR)rb_webserver.o \
	$(BUILD_DIR)rb_server.o \
	$(BUILD_DIR)platform.o

install:
	install -o root -g games -m 6555 vexxon /opt/pitrex/bin
