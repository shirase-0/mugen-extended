# Made by yours truly, Shirase-0

CC   = gcc.exe
CFLAGS = -g -Wall
BIN  = bin\MugenUltimate.exe
OBJ  = obj\test.o obj\logging.o obj\mem_manager.o obj\sdl_framerate.o obj\video_system.o obj\sdl_manager.o obj\timer.o obj\tokenizer.o obj\air_manager.o obj\sff_manager.o obj\cmd_manager.o obj\state_manager.o obj\state_parser.o obj\input.o obj\player.o obj\controller_executer.o obj\stack.o obj\vm.o obj\engine.o obj\game.o
LINKOBJ = obj/test.o obj/logging.o obj/mem_manager.o obj/sdl_framerate.o obj/video_system.o obj/sdl_manager.o obj/timer.o obj/tokenizer.o obj/air_manager.o obj/sff_manager.o obj/cmd_manager.o obj/state_manager.o obj/state_parser.o obj/input.o obj/player.o obj/controller_executer.o obj/stack.o obj/vm.o obj/engine.o obj/game.o
#LIBS = -I/opt/local/x86_64-w64-mingw32/include/SDL2 -Dmain=SDL_main -L/opt/local/x86_64-w64-mingw32/lib -lmingw32 -lSDL2main -lSDL2 -mwindows -lSDL2_mixer
LIBS = -L"lib" -lmingw32 -lSDL2_mixer -lSDL2main -lSDL2
# The mwindows library supresses/hides the console window, hence we're using the incorrect library config here
# To work out what should actually go in this field, run sdl-config --cflags --libs
INCLUDE = -I D:\MinGW\include\SDL2

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $(LINKOBJ) -o "bin/MugenUltimate.exe" $(INCLUDE) $(LIBS)

obj\test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o obj/test.o

obj\logging.o: logging.c
	$(CC) $(CFLAGS) -c logging.c -o obj/logging.o

obj\mem_manager.o: mem_manager.c
	$(CC) $(CFLAGS) -c mem_manager.c -o obj/mem_manager.o

obj\sdl_framerate.o: sdl_framerate.c
	$(CC) $(CFLAGS) -c sdl_framerate.c -o obj/sdl_framerate.o

obj\video_system.o: video_system.c
	$(CC) $(CFLAGS) -c video_system.c -o obj/video_system.o

obj\sdl_manager.o: sdl_manager.c
	$(CC) $(CFLAGS) -c sdl_manager.c -o obj/sdl_manager.o

obj\timer.o: timer.c
	$(CC) $(CFLAGS) -c timer.c -o obj/timer.o

obj\tokenizer.o: tokenizer.c
	$(CC) $(CFLAGS) -c tokenizer.c -o obj/tokenizer.o

obj\air_manager.o: air_manager.c
	$(CC) $(CFLAGS) -c air_manager.c -o obj/air_manager.o

obj\sff_manager.o: sff_manager.c
	$(CC) $(CFLAGS) -c sff_manager.c -o obj/sff_manager.o

obj\cmd_manager.o: cmd_manager.c
	$(CC) $(CFLAGS) -c cmd_manager.c -o obj/cmd_manager.o

obj\state_manager.o: state_manager.c
	$(CC) $(CFLAGS) -c state_manager.c -o obj/state_manager.o

obj\state_parser.o: state_parser.c
	$(CC) $(CFLAGS) -c state_parser.c -o obj/state_parser.o

obj\input.o: input.c
	$(CC) $(CFLAGS) -c input.c -o obj/input.o

obj\player.o: player.c
	$(CC) $(CFLAGS) -c player.c -o obj/player.o

obj\controller_executer.o: controller_executer.c
	$(CC) $(CFLAGS) -c controller_executer.c -o obj/controller_executer.o

obj\stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c -o obj/stack.o

obj\vm.o: vm.c
	$(CC) $(CFLAGS) -c vm.c -o obj/vm.o

obj\engine.o: engine.c
	$(CC) $(CFLAGS) -c engine.c -o obj/engine.o

obj\game.o: game.c
	$(CC) $(CFLAGS) -c game.c -o obj/game.o