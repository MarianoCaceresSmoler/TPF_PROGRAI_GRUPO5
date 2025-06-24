
CC := gcc
OPTIONS := -O2 -g -Wall # -g for debug, -O2 for optimize and -Wall additional messages

# Detectar plataforma
UNAME_M := $(shell uname -m)

ifeq ($(UNAME_M),x86_64)
    PLATFORM := pc
    PLATFORM_FLAG := -DPLATFORM_PC
    OUTPUT := game_pc

else
    PLATFORM := rpi
    PLATFORM_FLAG := -DPLATFORM_RPI
    OUTPUT := game_rpi
endif

$(info Compilando para: $(PLATFORM))

# DEFINIR BIEN TODOS LOS ARCHIVOS OBJETO 
# USAR flag de platform para concatenarla y compilar archivos dependiendo de la plataforma

OBJS := physics.o entities.o

hello: ${OBJS}
    ${CC} ${OBJS} ${OPTIONS} -o hello
hello.o: hello.c print.h
    ${CC} hello.c -c ${OPTIONS}
print.o: print.c print.h
    ${CC} print.c -c ${OPTIONS}
#Limpiar archivos objeto
clean:
    rm *.o

