# Playground makefile (PITREX ONLY)

# Just change root dir if your repo is located in another folder
# This relative path works if the Makefile is in ~/{project}/src,
# otherwise you must change here.
ROOT_DIR := ../../pitrex/pitrex

# This is the standard folder structure used in the PiTrex repo
SDK_DIR := $(ROOT_DIR)/pitrex
INC_DIR := $(SDK_DIR)
PITREX_DIR := $(SDK_DIR)/pitrex/
VECTREX_DIR := $(SDK_DIR)/vectrex/

# Build and compiler flags
SETTINGS := /opt/pitrex/settings
BUILD_DIR := build/
CFLAGS := -g -I$(INC_DIR) -DSETTINGS_DIR="\"$(SETTINGS)\"" -DAVOID_TICKS -DPIZERO -DRPI0 -DPITREX -lpthread -lm
CC := gcc

.PHONY: dirCheck

all: playground
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
$(BUILD_DIR)vexxon.o: vexxon.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)vexxon.o -c vexxon.c

$(BUILD_DIR)rb_draw.o: rb_draw.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_draw.o -c rb_draw.c

$(BUILD_DIR)rb_draw3d.o: rb_draw3d.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_draw3d.o -c rb_draw3d.c

$(BUILD_DIR)rb_engine.o: rb_engine.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_engine.o -c rb_engine.c

$(BUILD_DIR)rb_graphics.o: rb_graphics.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_graphics.o -c rb_graphics.c

$(BUILD_DIR)rb_input.o: rb_input.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_input.o -c rb_input.c

$(BUILD_DIR)rb_level.o: rb_level.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_level.o -c rb_level.c

$(BUILD_DIR)rb_math.o: rb_math.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_math.o -c rb_math.c

$(BUILD_DIR)rb_platform.o: rb_platform.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_platform.o -c rb_platform.c

$(BUILD_DIR)rb_primitive.o: rb_primitive.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_primitive.o -c rb_primitive.c

$(BUILD_DIR)rb_vector.o: rb_vector.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_vector.o -c rb_vector.c

$(BUILD_DIR)rb_vtext.o: rb_vtext.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_vtext.o -c rb_vtext.c

$(BUILD_DIR)rb_pitrex.o: rb_pitrex.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_pitrex.o -c rb_pitrex.c

$(BUILD_DIR)rb_pitrex_window.o: rb_pitrex_window.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)rb_pitrex_window.o -c rb_pitrex_window.c


# build
playground: $(BUILD_DIR)vexxon.o \
	$(BUILD_DIR)rb_draw.o $(BUILD_DIR)rb_draw3d.o $(BUILD_DIR)rb_engine.o $(BUILD_DIR)rb_graphics.o \
	$(BUILD_DIR)rb_input.o $(BUILD_DIR)rb_level.o $(BUILD_DIR)rb_math.o $(BUILD_DIR)rb_platform.o \
	$(BUILD_DIR)rb_primitive.o $(BUILD_DIR)rb_vector.o $(BUILD_DIR)rb_vtext.o \
	 $(BUILD_DIR)rb_pitrex.o $(BUILD_DIR)rb_pitrex_window.o
	$(BUILD_DIR)bcm2835.o $(BUILD_DIR)pitrexio-gpio.o $(BUILD_DIR)vectrexInterface.o $(BUILD_DIR)osWrapper.o $(BUILD_DIR)baremetalUtil.o

	$(RM) playground
	$(CC) $(CFLAGS) -o playground \
	$(BUILD_DIR)bcm2835.o \
	$(BUILD_DIR)pitrexio-gpio.o \
	$(BUILD_DIR)vectrexInterface.o \
	$(BUILD_DIR)osWrapper.o \
	$(BUILD_DIR)baremetalUtil.o \
	$(BUILD_DIR)rb_pitrex.o \
	$(BUILD_DIR)rb_pitrex_window.o \
	$(BUILD_DIR)vexxon.o \
	$(BUILD_DIR)rb_draw.o \
	$(BUILD_DIR)rb_draw3d.o \
	$(BUILD_DIR)rb_engine.o \
	$(BUILD_DIR)rb_graphics.o \
	$(BUILD_DIR)rb_input.o \
	$(BUILD_DIR)rb_level.o \
	$(BUILD_DIR)rb_math.o \
	$(BUILD_DIR)rb_platform.o \
	$(BUILD_DIR)rb_primitive.o \
	$(BUILD_DIR)rb_vector.o \
	$(BUILD_DIR)rb_vtext.o

install:
#	install -o root -g games -m 6555 playground /opt/pitrex/bin
