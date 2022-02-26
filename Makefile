# Made by yours truly, Shirase-0

CC   = gcc.exe
CFLAGS = -g -Wall
BIN  = bin\MugenExtended.exe
OBJ  = obj\main.o obj\logging.o obj\unit_testing.o obj\mem_manager.o obj\sdl_framerate.o obj\graphics_manager.o obj\tokenizer.o obj\timer.o obj\air_manager.o
LINKOBJ = obj/main.o obj/logging.o obj/unit_testing.o obj/mem_manager.o obj/sdl_framerate.o obj/graphics_manager.o obj/tokenizer.o obj/timer.o obj/air_manager.o
#LIBS = -I/opt/local/x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main -L/opt/local/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_mixer
LIBS = -L"lib" -lmingw32 -lSDL2_mixer -lSDL2main -lSDL2
# The mwindows library supresses/hides the console window, hence we're using the incorrect library config here
# To work out what should actually go in this field, run sdl2-config --cflags --libs
INCLUDE = -I D:\MinGW\include\SDL2

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(LINKOBJ) -o "bin/MugenExtended.exe" $(INCLUDE) $(LIBS)

obj\main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o obj/main.o

obj\logging.o: logging.c
	$(CC) $(CFLAGS) -c logging.c -o obj/logging.o

obj\unit_testing.o: unit_testing.c
	$(CC) $(CFLAGS) -c unit_testing.c -o obj/unit_testing.o

obj\mem_manager.o: mem_manager.c
	$(CC) $(CFLAGS) -c mem_manager.c -o obj/mem_manager.o

obj\sdl_framerate.o: sdl_framerate.c
	$(CC) $(CFLAGS) -c sdl_framerate.c -o obj/sdl_framerate.o

obj\graphics_manager.o: graphics_manager.c
	$(CC) $(CFLAGS) -c graphics_manager.c -o obj/graphics_manager.o

obj\tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c -o obj/tokenizer.o

obj\timer.o: timer.c
	$(CC) $(CFLAGS) -c timer.c -o obj/timer.o

obj\air_manager.o: air_manager.c
	$(CC) $(CFLAGS) -c air_manager.c -o obj/air_manager.o