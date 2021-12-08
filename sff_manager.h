#ifndef __MUSFFMANAGER__
#define __MUSFFMANAGER__

MU_SFF_Manager *sff_manager_init();
void mu_set_sff_pointers(MU_SFF_Manager *sff_manager, MU_Video_System *video_system, MU_Allocator *allocator, MU_Timer *timer, MU_Air_Manager *air_manager);
void reset_sff_manager(MU_SFF_Manager *sff_manager);
void free_manager();

u8 *decode_pcx(MU_SFF_Manager *sff_manager, u8 *pcx_byte, PCX_Header header);
void decode_sff_file(MU_SFF_Manager *sff_manager);
int find_sprite(MU_SFF_Manager *sff_manager, s16 n_group_number, s16 n_image_number);
bool load_sff_file(MU_SFF_Manager *sff_manager, const char *str_sff_file);
bool load_act_to_sff(MU_SFF_Manager *sff_manager, const char *str_act_file);
void prepare_anim(MU_SFF_Manager *sff_manager, s32 n_anim);

//x and y value are the axis value not the left corner of the image
void blit_sprite(MU_SFF_Manager *sff_manager, s16 n_group_number, s16 n_image_number, s16 x, s16 y);
void blit_anim(MU_SFF_Manager *sff_manager, s16 x, s16 y);

// These may not be necessary, as they're simple getters/setters
Action_Element *get_current_animation();
void set_blt_flags(u16 n_flag);

#endif