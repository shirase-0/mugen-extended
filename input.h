#ifndef __MUINPUT__
#define __MUINPUT__

Key *keyboard_data_init();

void mu_process_input(Key *keyboard, SDL_Event event);

#endif