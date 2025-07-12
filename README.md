# TP_FINAL_PROGRAI_GRUPO5

POR HACER:

- Ver de mejorar toda la validacion de datos en funciones, y comentarios si hace falta

Back+Front:
- Cambiar funciones de game para que el front solo tenga que llamar a gameUpdate(meter gameReset, gamePause, gameResume, etc. en gameUpdate y sacarlas del main)

Backend:
- Revisar formato, y prolijidad del codigo (constantes, macros, etc)
- Validacion en funciones

Frontend:
- Arreglar error en pantalla de gameover cuando no se hace ningun punto
- Mostrar que hace cada powerUp en menu de inicio
- Mostrar powerUps en hud (los muestra pero es medio confuso, tambien podria haber alguna animacion cuando lo agarras)
- Agregar sonido de 1-UP (suena la explosion cuando agarras powerUp de 1-UP)

Raspi:
- Arreglar fisica de las balas (chocan aunque no esten en el mismo espacio de LED) // revisar y definir bien checkCollision y hitbox para raspi
- Arreglar movimiento mothership (se mueve demasiado rapido) // % algo
- Arreglar movimiento aliens (no llegan a los bordes) // LINEA 425 de game.c hay que definir constante de limite lateral izq y derecho en config para cada dispositivo
- Editar audio mothership para que entre mejor de tiempo // bajar velocidad de mothership
- Arreglar que la musica del juego arranque de cero cuando se hace restart 
- Hacer menu de inicio y pausa, y pantalla gameover (en gameover mostrar score final)
- Dibujar HUD en gameplay (con vidas y nivel) // capaz en menu de pausa para que entre bien 
- Arreglar todo el makefile con dependencias de pc y raspi
