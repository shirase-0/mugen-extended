#include "mu_headers.h"

// This file was initially supposed to be for unit testing, but there are many areas of the code that
// are not currently being tested, for various reasons. Notably, freeing memory after it's no longer 
// needed is something that has not yet been implemented through most of the code, and as such is not
// being tested here. 

int main(int argc, char *argv[]) {
    bool unit_testing = false; // Set this to true when you want to run unit tests; This should probably be a constant

    // -----------------Tests----------------------------------
    if(unit_testing)
    {
        // =================logging================================
        init_log_file();
        mu_log_message("Unit Test: mu_log_message success");

        // =================mem_manager============================
        MU_Allocator *test_manager = init_mem_manager();
        assert(test_manager != NULL);

        MU_Allocator *test_allocator;
        for(int i = 0; i < MEM_MANAGER_NUMBER_OF_ALLOCATORS; i++)
        {
            test_allocator = get_allocator(test_manager, i);
            assert(test_allocator != NULL);
            show_mem_usage(test_allocator);
            assert(test_allocator->lp_mem_list != NULL);
        }

        void *to_be_removed = mu_alloc(test_allocator, 1000);
        void *to_stay = mu_alloc(test_allocator, 30);
        assert(to_be_removed != NULL);
        assert(to_stay != NULL);
        assert(test_allocator->lp_mem_list[0].n_type == 255);
        assert(test_allocator->lp_mem_list[1].n_type == 255);
        assert(test_allocator->lp_mem_list[0].n_size == 1000);
        assert(test_allocator->lp_mem_list[1].n_size == 30);

        // realloc MEMLIST 0 from 1000 to 500
        mu_realloc(test_allocator, to_be_removed, 500);
        assert(test_allocator->lp_mem_list[0].n_size == 500);

        mu_free(test_allocator, to_be_removed);
        assert(test_allocator->lp_mem_list[0].address == NULL);

        free_allocator(test_allocator);
        assert(test_allocator->n_alloc_size == 0);
        assert(test_allocator->n_alloc_number == 0);

        to_be_removed = mu_alloc(test_allocator, 200);
        to_stay = mu_alloc(test_allocator, 10);
        assert(test_allocator->lp_mem_list[0].n_type == 255);
        assert(test_allocator->lp_mem_list[1].n_type == 255);
        assert(test_allocator->lp_mem_list[0].n_size == 200);
        assert(test_allocator->lp_mem_list[1].n_size == 10);
        reset_allocator(test_allocator);
        assert(test_allocator->n_alloc_size == 0);
        assert(test_allocator->n_alloc_number == 0);
        assert(test_allocator->n_mem_list_size == 100);
        assert(test_allocator->b_free == true);
        assert(test_allocator->lp_mem_list != NULL);

        to_be_removed = mu_alloc(test_allocator, 500);
        to_stay = mu_alloc(test_allocator, 70000);
        assert(test_allocator->lp_mem_list[0].n_type == 255);
        assert(test_allocator->lp_mem_list[1].n_type == 255);
        assert(test_allocator->lp_mem_list[0].n_size == 500);
        assert(test_allocator->lp_mem_list[1].n_size == 70000);

        free_mem_manager(test_manager);
        assert(test_allocator->lp_mem_list[0].address == NULL);
        assert(test_allocator->n_alloc_size == 0);
        assert(test_allocator->n_alloc_number == 0);

        get_total_mem_usage(test_manager);

        // ===============sdl_manager, video_system, sdl_framerate==========================
        MU_Video_System *video_system = mu_sdl_manager_init();
        assert(video_system != NULL);
        assert(video_system->fps_manager.rate == 60);
        assert(get_framerate(&video_system->fps_manager) != -1);
        // No feasible way to test framerate_delay (sdl_framerate.c)
        // If I figure out a way to test it, it should go here
        assert(video_system->mu_font != NULL);
        mu_draw_text(video_system, 0, 20, "Unit Testing for mu_draw_text");
        mu_draw_text(video_system, 0, 40, "Our current framerate is %i", get_framerate(&video_system->fps_manager));
        SDL_UpdateWindowSurface(video_system->window);
        SDL_Delay(1000);
        mu_clear_screen(video_system);
        SDL_UpdateWindowSurface(video_system->window);
        SDL_Delay(1000);
        assert(mu_map_rgb(video_system, 100, 200, 150) == 6604950);
        // mu_create_surface (video_system.c) is never used, so no test has been included for it
        // If it ends up being used, put a test here for it

        // TODO: Add more unit tests here
    }
    else // Run the game as intended
    {
        Game *mugen = init_game();

        // Initialise SDL Mixer
        Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

        // Load audio file
        Mix_Music *backgroundSound = Mix_LoadMUS("test.mp3");

        mu_log_message("%s", SDL_GetCurrentAudioDriver());

        Mix_PlayMusic(backgroundSound, -1);

        run_game(mugen);
        quit_game();
        Mix_FreeMusic(backgroundSound);
        Mix_CloseAudio();
    }
    // ------------------------------------------------------------
    
    SDL_Quit();
    mu_log_message("Normal exit :o");
    return 0;
}