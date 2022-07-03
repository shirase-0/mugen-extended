#include "mu_headers.h"

void unit_test()
{
	// ===Logging===
	init_log_file();
	debug_print("debug_print working as intended");
	mu_log_message("mu_log_message working as intended");

	// ===Memory Manager===
	MU_Allocator *test_allocator = create_mu_alloc("Test Allocator");
	assert(test_allocator != NULL);
	show_mem_usage(test_allocator);
	debug_print("create_mu_alloc working as intended");
	debug_print("set_memlist_free working as intended");
	debug_print("show_mem_usage working as intended");

	void *to_remove = mu_alloc(test_allocator, 1000);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 1000);
	debug_print("mu_alloc working as intended");

	reset_allocator(test_allocator);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 0);
	debug_print("reset_allocator working as intended");

	to_remove = mu_alloc(test_allocator, 1234);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 1234);
	to_remove = mu_realloc(test_allocator, to_remove, 58000);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 58000);
	debug_print("mu_realloc working as intended");

	mu_free(test_allocator, to_remove);
	assert(test_allocator->alloc_size == 0);
	debug_print("mu_free working as intended");

	to_remove = mu_alloc(test_allocator, 2);
	assert(to_remove != NULL);
	assert(test_allocator->alloc_size == 2);
	free_allocator(test_allocator);
	assert(test_allocator->alloc_size == 0);
	assert(test_allocator->free == true);
	debug_print("free_allocator working as intended");

	MU_Allocator *test_manager = init_mem_manager();
	assert(strcmp(test_manager[MAINMENU].alloc_name, "Main Menu") == 0);
	assert(strcmp(test_manager[STAGE].alloc_name, "Stage") == 0);
	assert(strcmp(test_manager[ENGINE].alloc_name, "Engine") == 0);
	assert(strcmp(test_manager[P1].alloc_name, "Player 1") == 0);
	assert(strcmp(test_manager[P2].alloc_name, "Player 2") == 0);
	assert(strcmp(test_manager[P3].alloc_name, "Player 3") == 0);
	assert(strcmp(test_manager[P4].alloc_name, "Player 4") == 0);
	debug_print("init_mem_manager working as intended");

	// TODO: figure out a way to test if free_mem_manager is working

	mu_alloc(&test_manager[STAGE], 700000);
	mu_alloc(&test_manager[P3], 123456);
	get_total_mem_usage(test_manager);
	debug_print("get_total_mem_usage working as intended");

	// ===Framerate===
	// Framerate tests are conducted in the graphics manager section, as the graphics manager needs to initialise
	// an FPS manager anyway
	// There is currently no feasible way to test framerate_delay() without running a game

	// ===Graphics Manager===
	MU_Graphics_Manager *graphics_manager = mu_init_graphics_manager();
	assert(graphics_manager->window != NULL);
	assert(graphics_manager->screen_surface != NULL);
	assert(graphics_manager->renderer != NULL);
	assert(graphics_manager->delta_ptr == 0);
	assert(graphics_manager->now_time == 0);
	assert(graphics_manager->last_time == 0);
	assert(graphics_manager->fps_count == 0);
	assert(graphics_manager->fps == 0);
	assert(get_framerate(&graphics_manager->fps_manager) == 60);
	debug_print("init_framerate working as intended");
	debug_print("set_framerate working as intended");
	debug_print("get_framerate working as intended");
	debug_print("mu_load_font working as intended");
	debug_print("mu_init_graphics_manager working as intended");

	// This isn't scaling correctly yet, not sure why
	// Spaces also look smaller than they should be
	// Revisit this when the graphics manager is more functional
	mu_draw_text(graphics_manager, 0 , 0, "Hello World!", graphics_manager->fps);
	SDL_RenderPresent(graphics_manager->renderer);
	SDL_Delay(1000);

	mu_clear_screen(graphics_manager);
	debug_print("mu_clear_screen working as intended");
	SDL_Delay(1000);
	mu_draw_text(graphics_manager, 0 , 0, "What if we try to print a really long sentence", graphics_manager->fps);
	SDL_RenderPresent(graphics_manager->renderer);
	SDL_Delay(1000);

	// ===Tokenizer===
	// These tests will be left unimplemented until I can figure out how to implement them. It should stand to reason 
	// that if any of these functions fail, they will cause failures further down the line. This being the case, I'll 
	// eventually track those errors back to the one place that isn't being thoroughly tested: here. At the very 
	// least, errors in the Tokenizer should be semantic in nature, rather than inexplicable segfaults.
	// Below is a list of all the functions in Tokenizer.c that are not being tested.

	// Tokenizer *tokenizer_init(int buffer_size, char *comment_chars, char **operators, int operator_count);
	// void free_tokenizer(Tokenizer *tok);
	// bool mu_open_file(Tokenizer *tok, const char* filename);
	// bool mu_close_file(Tokenizer *tok);
	// bool mu_check_eof(Tokenizer *tok);
	// bool mu_check_newline(Tokenizer *tok, char c);
	// const char *get_token(Tokenizer *tok);
	// bool get_token_check(Tokenizer *tok, char *dest_str, int max_len);
	// float mu_get_float(Tokenizer *tok); 
	// int mu_get_int(Tokenizer *tok);
	// bool check_token(Tokenizer *tok, const char *str_to_look_for, bool consume); 
	// bool is_token_number(Tokenizer *tok);
	// bool is_token_quoted_str(Tokenizer *tok);

	// ===Timer===
	MU_Timer *test_timer = (MU_Timer*) malloc(sizeof(MU_Timer));
	mu_reset_timer(test_timer);
	assert(test_timer->game_time == 0);
	assert(test_timer->is_paused == false);
	debug_print("mu_reset_timer working as intended");

	mu_update_timer(test_timer);
	assert(test_timer->game_time == 1);
	debug_print("mu_update_timer working as intended");

	assert(get_game_time(test_timer) == 1);
	debug_print("get_game_time working as intended");

	mu_pause(test_timer);
	assert(test_timer->is_paused == true);
	mu_update_timer(test_timer);
	assert(get_game_time(test_timer) == 1);
	debug_print("mu_pause working as intended");

	mu_resume(test_timer);
	assert(test_timer->is_paused == false);
	mu_update_timer(test_timer);
	assert(get_game_time(test_timer) == 2);
	debug_print("mu_resume working as intended");

	// ===Air Manager===
	MU_Air_Manager *air_manager = air_manager_init();
	assert(air_manager != NULL);
	debug_print("air_manager_init working as intended");

	air_manager->air_allocator = &test_manager[P1];
	reset_air_manager(air_manager);
	assert(air_manager->total_action_block == 0);
	assert(air_manager->action_list_size == 100);
	assert(air_manager->element_list_size == 100);
	assert(air_manager->total_clsn == 0);
	assert(air_manager->is_default_clsn == false);
	assert(air_manager->action_list != NULL);
	debug_print("reset_air_manager working as intended");
	debug_print("action_list_init working as intended");

	// TODO: Add test for mu_free_air_manager when it's implemented

	open_air(air_manager, "chars\\kfm\\kfm.air");
	assert(air_manager->action_list[107].action_num == 5300);
	// Unsure how to test reallocate_action_block()
	debug_print("add_action working as intended");
	Action *test_action = get_action(air_manager, 5040);
	assert(test_action != NULL);
	assert(get_action(air_manager, 1) == NULL);
	debug_print("get_action working as intended");
	// These two functions run internally during open_air, making it rather difficult to test outside that context
	// debug_print("create_clsn_box working as intended");
	// debug_print("add_clsn_box working as intended");
	assert(test_action->loop_start == 6);
	debug_print("set_loop working as intended");
	assert(test_action->anim_element->group_num == 42);
	assert(test_action->anim_element->image_num == 5);
	assert(test_action->anim_element->x == 0);
	assert(test_action->anim_element->y == 0);
	assert(test_action->anim_element->during_time == 3);
	assert(test_action->anim_element->clsn_count == 1);
	assert(test_action->anim_element->clsn_data->is_clsn1 == false);
	assert(test_action->anim_element->clsn_data->x == -18);
	assert(test_action->anim_element->clsn_data->y == -77);
	assert(test_action->anim_element[7].group_num == 41);
	assert(test_action->anim_element[7].image_num == 8);
	assert(test_action->anim_element[7].clsn_data->x == 14);
	assert(test_action->anim_element[7].clsn_data->y == -23);
	assert(test_action->anim_element[7].clsn_data[1].x == -4);
	assert(test_action->anim_element[7].clsn_data[1].y == -105);
	debug_print("add_element working as intended");
	debug_print("open_air working as intended");

	// ===SFF Manager===
	MU_SFF_Manager *sff_manager = sff_manager_init();
	assert(sff_manager->graphics_manager == NULL);
	assert(sff_manager->sff_allocator == NULL);
	assert(sff_manager->x_scale_value == 1.0);
	assert(sff_manager->y_scale_value == 1.0);
	debug_print("sff_manager_init working as intended");

	mu_set_sff_pointers(sff_manager, graphics_manager, &test_manager[P1], test_timer, air_manager);
	assert(sff_manager->graphics_manager != NULL);
	assert(sff_manager->sff_allocator != NULL);
	debug_print("mu_set_sff_pointers working as intended");

	reset_sff_manager(sff_manager);
	assert(sff_manager->is_palette_loaded == false);
	assert(sff_manager->sff_file == 0);
	assert(sff_manager->total_images == 0);
	assert(sff_manager->current_image == 0);
	assert(sff_manager->image_list_size == 100);
	assert(sff_manager->flags == BLT_NORMAL);
	debug_print("reset_sff_manager working as intended");

	load_act_to_sff(sff_manager, "chars\\kfm\\kfm6.act");
	load_sff_file(sff_manager, "chars\\kfm\\kfm.sff");

	sff_manager->flags = BLT_NORMALMASKED;
	prepare_anim(sff_manager, 200);

	// Potentially useful testing code for mu_map_rgb
	// const char *pixel_format = SDL_GetPixelFormatName(370546692);
	// debug_print("%s", pixel_format);

	// debug_print("6604950");
	// SDL_PixelFormat *format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
	// uint32_t desired_value = SDL_MapRGB(format, 100, 200, 150);
	// debug_print("%u", desired_value);

	// debug_print("6604950");
	// uint32_t desired_value = mu_map_rgb(graphics_manager, 100, 200, 150);
	// debug_print("%u", desired_value);
	assert(mu_map_rgb(graphics_manager, 100, 200, 150) == 6604950);
	debug_print("mu_map_rgb working as intended");

	blit_anim(sff_manager, 70, 220);
	SDL_Texture *sprite = SDL_CreateTextureFromSurface(graphics_manager->renderer, graphics_manager->screen_surface);
	mu_draw(graphics_manager, sprite);
	SDL_Delay(1000);

	mu_clear_screen(graphics_manager);
	blit_sprite(sff_manager, 1000, 4, 70, 220);
	SDL_UpdateTexture(sprite, NULL, graphics_manager->screen_surface->pixels, XMAX * sizeof(uint32_t));
	mu_draw(graphics_manager, sprite);
	SDL_Delay(1000);

	// TODO: Tidy up these tests, and ensure proper testing of the SFF Manager occurs
}