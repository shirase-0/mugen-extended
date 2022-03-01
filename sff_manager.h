#ifndef __MUSFFMANAGER__
#define __MUSFFMANAGER__

MU_SFF_Manager *sff_manager_init();
void mu_set_sff_pointers(MU_SFF_Manager *sff_manager, MU_Graphics_Manager *graphics_manager, MU_Allocator *allocator, MU_Timer *timer, MU_Air_Manager *air_manager);
void reset_sff_manager(MU_SFF_Manager *sff_manager);
// Do I actually need any of these free functions?
// I have a free function for the memory allocator(s), which should cover *all* the memory that's been allocated to
// every manager and every element within them
// Leaving this unimplemented until I can test whether or not I'm causing memory leaks
// TODO
//void free_sff_manager(MU_SFF_Manager *sff_manager); 

uint8_t *decode_pcx(MU_SFF_Manager *sff_manager, uint8_t *pcx_byte, PCX_Header header);
void decode_sff_file(MU_SFF_Manager *sff_manager);
int find_sprite(MU_SFF_Manager *sff_manager, int16_t group_num, int16_t image_num);
bool load_sff_file(MU_SFF_Manager *sff_manager, const char *sff_filename);
bool load_act_to_sff(MU_SFF_Manager *sff_manager, const char *act_filename);
void prepare_anim(MU_SFF_Manager *sff_manager, int anim_id);

//x and y value are the axis value not the left corner of the image
void blit_sprite(MU_SFF_Manager *sff_manager, int16_t group_num, int16_t image_num, int16_t x, int16_t y);
void blit_anim(MU_SFF_Manager *sff_manager, int16_t x, int16_t y);

#endif