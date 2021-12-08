#ifndef __MUHEADERS__H
#define __MUHEADERS__H

//Standard Libraries
#include <stdio.h>
#include<stdbool.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

//SDL
#include "SDL2\SDL.h"
#include "SDL2\SDL_mixer.h"

//Project Header Files
#include "structs.h"
#include "logging.h"
#include "mem_manager.h"
#include "sdl_framerate.h"
#include "video_system.h"
#include "sdl_manager.h"
#include "timer.h"
#include "tokenizer.h"
#include "air_manager.h"
#include "sff_manager.h"
#include "cmd_manager.h"
#include "state_manager.h"
#include "state_parser.h"
#include "input.h"
#include "player.h"
#include "controller_executer.h"
#include "stack.h"
#include "vm.h"
#include "engine.h"
#include "game.h"

#endif