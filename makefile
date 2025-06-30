
CC := gcc
OPTIONS := -O2 -g -Wall # -g for debug, -O2 for optimize and -Wall additional messages

# Automatically detect platform
UNAME_M := $(shell uname -m)

BACKEND_DIR := backend

ifeq ($(UNAME_M),x86_64)
    PLATFORM := pc
    PLATFORM_FLAG := -DPLATFORM_PC
    FRONTEND_DIR := frontend_pc
    OUTPUT := game_pc
else
    PLATFORM := rpi
    PLATFORM_FLAG := -DPLATFORM_RPI
    FRONTEND_DIR := frontend_rpi
    OUTPUT := game_rpi
endif

$(info Compiling for: $(PLATFORM))

OBJS := entities.o game.o physics.o  scores.o audio_${PLATFORM}.o input_${PLATFORM}.o main_${PLATFORM}.o render_${PLATFORM}.o

# FALTA DEFINIR BIEN LAS DEPENDENCIAS

program: ${OBJS}
	${CC} ${OBJS} ${OPTIONS} -o ${OUTPUT}
game.o: ${BACKEND_DIR}/game.c ${BACKEND_DIR}/game.h
	${CC} ${BACKEND_DIR}/game.c -c ${OPTIONS}
entities.o: ${BACKEND_DIR}/entities.c ${BACKEND_DIR}/entities.h
	${CC} ${BACKEND_DIR}/entities.c -c ${OPTIONS}
entities.o: ${BACKEND_DIR}/entities.c ${BACKEND_DIR}/entities.h
	${CC} ${BACKEND_DIR}/entities.c -c ${OPTIONS}
physics.o: ${BACKEND_DIR}/physics.c ${BACKEND_DIR}/physics.h
	${CC} ${BACKEND_DIR}/physics.c -c ${OPTIONS}
scores.o: ${BACKEND_DIR}/scores.c ${BACKEND_DIR}/scores.h
	${CC} ${BACKEND_DIR}/scores.c -c ${OPTIONS}
main_${PLATFORM}.o: ${FRONTEND_DIR}/main_${PLATFORM}.c
	${CC} ${FRONTEND_DIR}/main_${PLATFORM}.c -c ${OPTIONS}
audio_${PLATFORM}.o: ${FRONTEND_DIR}/audio_${PLATFORM}.c ${FRONTEND_DIR}/audio_${PLATFORM}.h
	${CC} ${FRONTEND_DIR}/audio_${PLATFORM}.c -c ${OPTIONS}
input_${PLATFORM}.o: ${FRONTEND_DIR}/input_${PLATFORM}.c ${FRONTEND_DIR}/input_${PLATFORM}.h
	${CC} ${FRONTEND_DIR}/input_${PLATFORM}.c -c ${OPTIONS}
render_${PLATFORM}.o: ${FRONTEND_DIR}/render_${PLATFORM}.c ${FRONTEND_DIR}/render_${PLATFORM}.h
	${CC} ${FRONTEND_DIR}/render_${PLATFORM}.c -c ${OPTIONS}

clean:
	rm *.o ${OUTPUT}
