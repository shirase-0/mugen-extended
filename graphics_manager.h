#ifndef __MUGRAPHICS__
#define __MUGRAPHICS__

// TODO: Make these changeable settings the user can set in-game
// For now they're set to my screen's native resolution
#define SCREENX_LIMIT 1679
#define SCREENY_LIMIT 1049
#define XMAX 1680
#define YMAX 1050

MU_Graphics_Manager *mu_init_graphics_manager();
void mu_load_font(MU_Graphics_Manager *graphics_manager);
void mu_draw_text(MU_Graphics_Manager *graphics_manager, int x, int y, const char *text, ...);
void mu_clear_screen(MU_Graphics_Manager *graphics_manager);
// TODO: Remove the next two functions unless a use is found for them
// SDL_Surface *mu_create_surface(MU_Graphics_Manager *graphics_manager, int x, int y);
uint32_t mu_map_rgb(MU_Graphics_Manager *graphics_manager, int red, int green, int blue);

void mu_normal_blt(MU_Graphics_Manager *graphics_manager, SFF_Sprite *sprite_list, int16_t x, int16_t y, bool has_mask);
void mu_normal_flip_h(MU_Graphics_Manager *graphics_manager, SFF_Sprite *sprite_list, int16_t x, int16_t y, bool has_mask);

void mu_draw(MU_Graphics_Manager *graphics_manager, SDL_Texture *sdl_texture);

// These functions either don't have any code, or have commented out code
// void mu_cleanup_graphics_manager();
// void mu_draw_rect(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b); // Note to self: data types may need to be changed, especially u8!
// void mu_filter_image();

//void mu_normal_flip_v(SFFSPRITE *lp_sprite, s16 x, s16 y, bool b_mask);
//void mu_normal_flip_hv(SFFSPRITE *lp_sprite, s16 x, s16 y, bool b_mask);

#endif