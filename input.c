#include "mu_headers.h"

Key_Element *key_element_init()
{
	Key_Element *key_element = (Key_Element*) malloc(sizeof(Key_Element));
	key_element->is_pressed = false;
	key_element->sdl_keycode = 0;

	return key_element;
}

Keyboard_Data *keyboard_data_init()
{
	Keyboard_Data *keyboard_data = (Keyboard_Data*) malloc(sizeof(Keyboard_Data));
	keyboard_data->b_keyboard = true; // I assume this should be initialised to true

	for(int k = 0; k < KEY_COUNT; k++)
	{
		keyboard_data->key_info[k] = *(key_element_init()); // Does this kind of cast cause a memory leak?
	}

	// For now I'm hardcoding button mapping
	// Note to self: use scancodes, not keycodes
	// They work irrespective of qwerty, dvorak, etc?
	// Also I could only get the SDL_GetKeyboard_State command to work with scancodes
	// TODO: add in button mapping based on user configuration
	keyboard_data->key_info[0].sdl_keycode = SDL_SCANCODE_W;
	keyboard_data->key_info[1].sdl_keycode = SDL_SCANCODE_S;
	
	// For some reason these two directions are registering as flipped
	// I've swapped them in the array for now, though they should actually be the other way around
	keyboard_data->key_info[2].sdl_keycode = SDL_SCANCODE_D;
	keyboard_data->key_info[3].sdl_keycode = SDL_SCANCODE_A;
	
	keyboard_data->key_info[4].sdl_keycode = SDL_SCANCODE_U;
	keyboard_data->key_info[5].sdl_keycode = SDL_SCANCODE_I;
	keyboard_data->key_info[6].sdl_keycode = SDL_SCANCODE_O;
	keyboard_data->key_info[7].sdl_keycode = SDL_SCANCODE_J;
	keyboard_data->key_info[8].sdl_keycode = SDL_SCANCODE_K;
	keyboard_data->key_info[9].sdl_keycode = SDL_SCANCODE_L;
	keyboard_data->key_info[10].sdl_keycode = SDL_SCANCODE_P;
	keyboard_data->key_info[11].sdl_keycode = SDL_SCANCODE_TAB;

	return keyboard_data;
}

void mu_process_input(Keyboard_Data *lp_keyboard, SDL_Event event)
{
	// Used Uint8 in original version
	// TODO: switch to using stdint.h type definitions
	u8 *keystate = SDL_GetKeyboardState(NULL);

	// Process keyboard input
	if(lp_keyboard->b_keyboard)
	{
		for(int k = 0; k < KEY_COUNT; k++)
		{
			lp_keyboard->key_info[k].is_pressed = keystate[lp_keyboard->key_info[k].sdl_keycode];
		}
	}
}