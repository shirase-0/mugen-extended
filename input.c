#include "mu_headers.h"

Key *keyboard_data_init()
{
	Key *keyboard = (Key*) malloc(sizeof(Key) * KEY_COUNT);
	for(int k = 0; k < KEY_COUNT; k++)
	{
		keyboard[k].is_pressed = NOT_PRESSED;
		keyboard[k].sdl_keycode = 0;
	}

	// TODO: implement player 2 controls
	// Maybe have the player number passed in as a parameter to this function?
	keyboard[0].sdl_keycode = SDL_SCANCODE_W;
	keyboard[1].sdl_keycode = SDL_SCANCODE_S;
	keyboard[2].sdl_keycode = SDL_SCANCODE_A;
	keyboard[3].sdl_keycode = SDL_SCANCODE_D;
	keyboard[4].sdl_keycode = SDL_SCANCODE_U;
	keyboard[5].sdl_keycode = SDL_SCANCODE_I;
	keyboard[6].sdl_keycode = SDL_SCANCODE_O;
	keyboard[7].sdl_keycode = SDL_SCANCODE_J;
	keyboard[8].sdl_keycode = SDL_SCANCODE_K;
	keyboard[9].sdl_keycode = SDL_SCANCODE_L;
	keyboard[10].sdl_keycode = SDL_SCANCODE_P;
	keyboard[11].sdl_keycode = SDL_SCANCODE_TAB;

	return keyboard;
}

void mu_process_input(Key *keyboard, SDL_Event event)
{
	const uint8_t *keystate = SDL_GetKeyboardState(NULL);

	for(int k = 0; k < KEY_COUNT; k++)
	{
		// This whole if-else block could be done in one line, but it's less confusing this way
		if(keystate[keyboard[k].sdl_keycode] == 1)
		{
			keyboard[k].is_pressed = PRESSED; // 1
		}
		else
		{
			keyboard[k].is_pressed = NOT_PRESSED; // 0
		}
	}
}