#ifndef __MUVIDEO__
#define __MUVIDEO__

// Note to self: in the original version, these were consts. For some reason, the linker hates this, so I had to change them to #defines
#define SCREENX_LIMIT 639
#define SCREENY_LIMIT 479
#define XMAX 640
#define YMAX 480

MU_Video_System *mu_init_video_system();
void mu_load_font(MU_Video_System *video_system);
void mu_draw_text(MU_Video_System *video_system, int x, int y, char *str_text, ...);
void mu_clear_screen(MU_Video_System *video_system);
SDL_Surface *mu_create_surface(MU_Video_System *video_system, int x, int y);
unsigned mu_map_rgb(MU_Video_System *video_system, int red, int green, int blue);

void mu_normal_blt(MU_Video_System *video_system, SFF_Sprite *lp_sprite, s16 x, s16 y, bool b_mask);
void mu_normal_flip_h(MU_Video_System *video_system, SFF_Sprite *lp_sprite, s16 x, s16 y, bool b_mask);

void mu_draw(MU_Video_System *video_system, SDL_Texture *sdl_texture);

// These functions either don't have any code, or have commented out code
// void mu_cleanup_video_system();
// void mu_draw_rect(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b); // Note to self: data types may need to be changed, especially u8!
// void mu_filter_image();

//void mu_normal_flip_v(SFFSPRITE *lp_sprite, s16 x, s16 y, bool b_mask);
//void mu_normal_flip_hv(SFFSPRITE *lp_sprite, s16 x, s16 y, bool b_mask);

#endif