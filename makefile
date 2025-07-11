
CC := gcc
OPTIONS := -O2 -g -Wall # -g for debug, -O2 for optimize and -Wall additional messages

# Automatically detects platform
UNAME_M := $(shell uname -m)

BACKEND_DIR := backend

ifeq ($(UNAME_M),armv7l)
    PLATFORM := rpi
    PLATFORM_FLAG := -DPLATFORM_RPI
    FRONTEND_DIR := frontend_rpi
    OUTPUT := game_rpi

	OBJS := entities.o game.o physics.o scores.o audio_${PLATFORM}.o input_${PLATFORM}.o main_${PLATFORM}.o render_${PLATFORM}.o ../../libs/audio/SDL2/libAudioSDL2.o ../../libs/joydisp/joydrv.o ../../libs/joydisp/disdrv.o
	INCLUDES := -I../../libs/joydisp -I../../libs/audio/SDL2/src -I/usr/local/include
	LINKS := -L/usr/local/lib -lSDL2 -lpthread
else
    PLATFORM := pc
    PLATFORM_FLAG := -DPLATFORM_PC
    FRONTEND_DIR := frontend_pc
    OUTPUT := game_pc

	OBJS := entities.o game.o physics.o scores.o audio_${PLATFORM}.o input_${PLATFORM}.o main_${PLATFORM}.o render_${PLATFORM}.o
	INCLUDES := 
	LINKS := -lallegro -lallegro_primitives -lallegro_font -lallegro_ttf -lallegro_image -lallegro_audio -lallegro_acodec -lallegro_video -lpthread
endif

$(info Compiling for: $(PLATFORM))	

# FALTA DEFINIR BIEN LAS DEPENDENCIAS PARA CADA TARGET

program: ${OBJS}
	${CC} ${OBJS} ${LINKS} ${OPTIONS} ${PLATFORM_FLAG} -o ${OUTPUT} 

game.o: ${BACKEND_DIR}/game.c ${BACKEND_DIR}/game.h ${BACKEND_DIR}/config.h ${BACKEND_DIR}/entities.h ${BACKEND_DIR}/physics.h ${BACKEND_DIR}/scores.h
	${CC} ${BACKEND_DIR}/game.c -c ${OPTIONS} ${PLATFORM_FLAG}

entities.o: ${BACKEND_DIR}/entities.c ${BACKEND_DIR}/entities.h ${BACKEND_DIR}/config.h
	${CC} ${BACKEND_DIR}/entities.c -c ${OPTIONS} ${PLATFORM_FLAG}

physics.o: ${BACKEND_DIR}/physics.c ${BACKEND_DIR}/physics.h ${BACKEND_DIR}/game.h ${BACKEND_DIR}/config.h ${BACKEND_DIR}/entities.h ${BACKEND_DIR}/scores.h
	${CC} ${BACKEND_DIR}/physics.c -c ${OPTIONS} ${PLATFORM_FLAG}

scores.o: ${BACKEND_DIR}/scores.c ${BACKEND_DIR}/scores.h ${BACKEND_DIR}/game.h ${BACKEND_DIR}/config.h
	${CC} ${BACKEND_DIR}/scores.c -c ${OPTIONS} ${PLATFORM_FLAG}

main_${PLATFORM}.o: ${FRONTEND_DIR}/main_${PLATFORM}.c ${FRONTEND_DIR}/render_${PLATFORM}.h ${FRONTEND_DIR}/input_${PLATFORM}.h ${FRONTEND_DIR}/audio_${PLATFORM}.h ${BACKEND_DIR}/game.h ${BACKEND_DIR}/config.h
	${CC} ${FRONTEND_DIR}/main_${PLATFORM}.c -c ${OPTIONS} ${PLATFORM_FLAG}

audio_${PLATFORM}.o: ${FRONTEND_DIR}/audio_${PLATFORM}.c ${FRONTEND_DIR}/audio_${PLATFORM}.h
	${CC} ${FRONTEND_DIR}/audio_${PLATFORM}.c -c ${OPTIONS} ${PLATFORM_FLAG} ${INCLUDES}

input_${PLATFORM}.o: ${FRONTEND_DIR}/input_${PLATFORM}.c ${FRONTEND_DIR}/input_${PLATFORM}.h ${BACKEND_DIR}/game.h ${FRONTEND_DIR}/render_${PLATFORM}.h
	${CC} ${FRONTEND_DIR}/input_${PLATFORM}.c -c ${OPTIONS} ${PLATFORM_FLAG} ${INCLUDES}

render_${PLATFORM}.o: ${FRONTEND_DIR}/render_${PLATFORM}.c ${FRONTEND_DIR}/render_${PLATFORM}.h ${BACKEND_DIR}/entities.h ${BACKEND_DIR}/game.h ${BACKEND_DIR}/config.h
	${CC} ${FRONTEND_DIR}/render_${PLATFORM}.c -c ${OPTIONS} ${PLATFORM_FLAG} ${INCLUDES}

clean:
	rm *.o ${OUTPUT}

