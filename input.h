#ifndef __MUINPUT__
#define __MUINPUT__

Key_Element *key_element_init();
Keyboard_Data *keyboard_data_init();

void mu_process_input(Keyboard_Data *lp_keyboard, SDL_Event event);

#endif