#ifndef __MUGAME__
#define __MUGAME__

Game *init_game();

void check_sdl(Game *game);

void run_game(Game *game);
void quit_game();

void crash_game(); // Not implemented in original code

#endif