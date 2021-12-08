#ifndef __SDLFRAMERATE__
#define __SDLFRAMERATE__

// Rates are in Hz
// In the original code, these were preprocessor #defines, not sure why
#define FPS_UPPER_LIMIT 200
#define FPS_LOWER_LIMIT 1
#define FPS_DEFAULT 30 // I'd prefer this to be 60 but idk if that'd work with MUGEN
// const int FPS_UPPER_LIMIT = 200;
// const int FPS_LOWER_LIMIT = 1;
// const int FPS_DEFAULT = 30; // I'd prefer this to be 60 but idk if that'd work with MUGEN


void init_framerate(MU_FPS_Manager *fps_manager);
int set_framerate(MU_FPS_Manager *fps_manager, int rate);
int get_framerate(MU_FPS_Manager *fps_manager);
void framerate_delay(MU_FPS_Manager *fps_manager);

#endif