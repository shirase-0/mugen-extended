#include "mu_headers.h"

MU_SFF_Manager *sff_manager_init()
{
	MU_SFF_Manager *sff_manager = (MU_SFF_Manager*) malloc(sizeof(MU_SFF_Manager));
	sff_manager->graphics_manager = NULL;
	sff_manager->sff_allocator = NULL;
	// These don't seem to do anything yet
	// TODO: implement scale2x, and possibly remove these values?
	// In the previous version, SDL2 handled all the scaling automatically
	// That being the case, maybe I don't need to implement scale2x, and I can *still* remove these values
	sff_manager->x_scale_value = 1.0;
	sff_manager->y_scale_value = 1.0;

	return sff_manager;
}

void mu_set_sff_pointers(MU_SFF_Manager *sff_manager, MU_Graphics_Manager *graphics_manager, MU_Allocator *allocator, MU_Timer *timer, MU_Air_Manager *air_manager)
{
	sff_manager->graphics_manager = graphics_manager;
	sff_manager->sff_allocator = allocator;
	sff_manager->timer = timer;
	sff_manager->air_manager = air_manager;
}

void reset_sff_manager(MU_SFF_Manager *sff_manager)
{
	sff_manager->is_palette_loaded = false;
	sff_manager->sff_file = 0;
	sff_manager->total_images = 0;
	sff_manager->current_image = 0;
	sff_manager->image_list_size = 100;
	sff_manager->flags = BLT_NORMAL;

	MU_Allocator *allocator = sff_manager->sff_allocator;
	sff_manager->sprite_list = (SFF_Sprite*) mu_alloc(allocator, sizeof(SFF_Sprite) * sff_manager->image_list_size);

	// This was u16 in the previous version, and for some reason that worked (It shouldn't???)
	// If the colours break, change this back to u16 and see if it works
	memset(sff_manager->colour_palette, 0, sizeof(uint32_t) * COLOUR_PALETTE_SIZE);
}

// PCX files are the standard format for sprites in MUGEN, and are stored in an SFF archive file
// This function parses a PCX file and returns an image buffer
uint8_t *decode_pcx(MU_SFF_Manager *sff_manager, uint8_t *pcx_byte, PCX_Header header)
{
	uint8_t *byte_image_buffer = 0;
	uint8_t bpp = header.planes_count * BIT_DEPTH;
	uint8_t byte_data;
	uint16_t size = 0;
	int16_t x = 0; // Current position along the current line
	int16_t y = 0; // Current line in the header
	int16_t header_width = header.width;
	uint32_t pos = 0; 
	uint32_t blank_count = 0;

	MU_Allocator *allocator = sff_manager->sff_allocator;

	byte_image_buffer = (uint8_t*) mu_alloc(allocator, sizeof(uint8_t) * (header.width * header.planes_count * header.height + 1));

	if(header_width < header.bytes_per_line * header.planes_count)
	{
		header_width = header.bytes_per_line * header.planes_count;
	}

	if(bpp > 8)
	{
		debug_print("SFF Manager: 24 bit PCX files are not supported");
		return byte_image_buffer;
	}

	for(y = 0; y < header.height; y++) // Should I change this from y = 0 to just y;
	{
		x = 0;
		while(x < header_width)
		{
			byte_data = pcx_byte[pos++];

			if((byte_data & 0xC0) == 0xC0)
			{
				size = byte_data & 0x3F;
				byte_data = pcx_byte[pos++];
			}
			else
			{
				size = 1;
			}

			while(size-- > 0)
			{
				if(x <= header.width)
				{
					byte_image_buffer[x + (y * header.width)] = byte_data;
				}

				// Skip blank data
				if(x == header_width && header_width != header.width)
				{
					blank_count = header_width - header.width;
					for(uint32_t i = 0; i < blank_count; i++)
					{
						pos += 2;
					}
				}
				x++;
			}
		}
	}

	return byte_image_buffer;
}

// This function parses an SFF archive into the SFF Manager
void decode_sff_file(MU_SFF_Manager *sff_manager)
{
	SFF_Subheader subheader;
	uint8_t *temp_bytes;

	SFF_Sprite *sprite_list = &sff_manager->sprite_list[sff_manager->total_images];
	MU_Allocator *allocator = sff_manager->sff_allocator;
	FILE *sff_file = sff_manager->sff_file; // If things break, revert this change

	// Read the first subheader
	fread(&subheader, sizeof(subheader), 1, sff_file);

	while(!feof(sff_file))
	{
		// Allocate more memory if needed
		if(sff_manager->total_images > sff_manager->image_list_size - 1)
		{
			sff_manager->image_list_size += 100;

			uint16_t image_list_size = sff_manager->image_list_size;
			SFF_Sprite *spr_list = sff_manager->sprite_list;

			sff_manager->sprite_list = (SFF_Sprite*) mu_realloc(allocator, spr_list, sizeof(SFF_Sprite) * image_list_size);
			sprite_list = &sff_manager->sprite_list[sff_manager->total_images];
		}

		sprite_list->group_num = subheader.group_num;
		sprite_list->image_num = subheader.image_num;
		sprite_list->x = subheader.x;
		sprite_list->y = subheader.y;

		// Is it a linked sprite?
		// Note to self: this section can never be reached because subheader_len is never set
		// This should probably at least be zeroed out at the start of this function to prevent undefined behaviour
		// CHECK: test to ensure this can't cause undefined behaviour
		if(subheader.subheader_len != 0)
		{
			// Read the PCX header
			// This caused a crash in the previous version
			fread(&sprite_list->pcx_header, sizeof(PCX_Header), 1, sff_file);

			sprite_list->pcx_header.width = sprite_list->pcx_header.width - sprite_list->pcx_header.x + 1;
			sprite_list->pcx_header.height = sprite_list->pcx_header.height - sprite_list->pcx_header.y + 1;

			// Read the PCX data
			// TODO: check subheader.subheader_len
			temp_bytes = (uint8_t*) mu_alloc(allocator, sizeof(uint8_t) * (subheader.subheader_len - 127));
			fread(temp_bytes, sizeof(uint8_t) * (subheader.subheader_len - 127), 1, sff_file);

			// This caused a crash in the previous version
			sprite_list->pcx_file_bytes = decode_pcx(sff_manager, temp_bytes, sprite_list->pcx_header);
			mu_free(allocator, temp_bytes);

			// Skip to colour palette of the PCX file
			fseek(sff_file, -768L, SEEK_CUR);

			// Eat empty byte
			fgetc(sff_file);

			if(fgetc(sff_file) == 12 &&
			   !subheader.is_same_palette &&
			   !sff_manager->is_palette_loaded &&
			   sprite_list->pcx_header.planes_count <= 1)
			{
				for(int j = 0; j < COLOUR_PALETTE_SIZE; j++)
				{
					// TODO: test if declaring these variables inside the loop is bad for performance
					int r, g, b;
					r = fgetc(sff_file);
					g = fgetc(sff_file);
					b = fgetc(sff_file);

					sff_manager->colour_palette[j] = mu_map_rgb(sff_manager->graphics_manager, r, g, b);
				}
			}

			// This line worked in the previous version, but I'm unsure why
			// TODO: figure out why this one worked and the others ones didn't
			memcpy(&sprite_list->colour_palette, sff_manager->colour_palette, sizeof(uint32_t) * COLOUR_PALETTE_SIZE);
		}
		else
		{
			// TODO: implement this?
			// The previous version seemed to work fine without it, but maybe I didn't conduct enough testing
			// subheader.index_of_previous - 1 if the first element is 1 and not 0
			// we have a linked image here

			// Note to self: there was a problem here where images linked to index_of_previous 0 were causing segfaults
			// I'm not sure if the following code actually fixed the problem or not though, since the problematic 
			// image in question was a 9000,0 image
			if(subheader.index_of_previous == 0)
			{
				// This code path is never run?
				// TODO: test this code path
				memcpy(sprite_list, &sff_manager->sprite_list[subheader.index_of_previous], sizeof(SFF_Sprite));
			}
			else
			{
				memcpy(sprite_list, &sff_manager->sprite_list[subheader.index_of_previous - 1], sizeof(SFF_Sprite));
			}
			sprite_list->group_num = subheader.group_num;
			sprite_list->image_num = subheader.image_num;
		}
		sff_manager->total_images++;
		sprite_list = &sff_manager->sprite_list[sff_manager->total_images];

		// Skip to and read the next subheader
		fseek(sff_file, subheader.next_subheader_file_offset, SEEK_SET);
		fread(&subheader, sizeof(subheader), 1, sff_file);
	}
	debug_print("SFF Manager: %d images have been decoded", sff_manager->total_images);
}

int find_sprite(MU_SFF_Manager *sff_manager, int16_t group_num, int16_t image_num)
{
	for(int i = 0; i < sff_manager->image_list_size; i++)
	{
		if(sff_manager->sprite_list[i].group_num == group_num &&
		   sff_manager->sprite_list[i].image_num == image_num)
		{
			return i;
		}
	}

	// If no image is found
	return -1;
}

// Loads and parses an SFF archive
bool load_sff_file(MU_SFF_Manager *sff_manager, const char *sff_filename)
{
	SFF_Header header;

	debug_print("SFF Manager: Loading %s", sff_filename);
	sff_manager->sff_file = fopen(sff_filename, "rb");

	if(sff_manager->sff_file == 0)
	{
		debug_print("SFF Manager: Failed to find %s", sff_filename);
		return false;
	}

	// Read the header of the file
	fread(&header, sizeof(header), 1, sff_manager->sff_file);

	// Check file signature
	if(strcmp((char*) header.signature, "ElecbyteSpr") != 0)
	{
		debug_print("SFF Manager: %s is not an SFF file", sff_filename);
		return false;
	}
	debug_print("SFF Manager: SFF file contains %d images", header.images_count);

	// Decode the SFF file
	fseek(sff_manager->sff_file, header.subheader_file_offset, SEEK_SET);
	decode_sff_file(sff_manager);
	fclose(sff_manager->sff_file);

	// Save memory
	MU_Allocator *allocator = sff_manager->sff_allocator;
	SFF_Sprite *sprite_list = sff_manager->sprite_list;
	uint16_t total_images = sff_manager->total_images;

	sff_manager->sprite_list = (SFF_Sprite*) mu_realloc(allocator, sprite_list, sizeof(SFF_Sprite) * total_images);

	return true;
}

// Colour palettes for MUGEN are stored in ACT files
// Each sprite archive can have many different colour palettes
// This function loads the selected colour palette to the sprite archive
bool load_act_to_sff(MU_SFF_Manager *sff_manager, const char *act_filename)
{
	sff_manager->is_palette_loaded = true;
	FILE *act = fopen(act_filename, "rb");

	if(act == NULL)
	{
		debug_print("SFF Manager: %s file not found", act_filename);
	}

	// Starts at 255 so I can use it for the array index
	for(int j = 255; j >= 0; j--)
	{
		// TODO: test if declaring these variables inside the loop is bad for performance
		int r, g, b;

		r = fgetc(act);
		g = fgetc(act);
		b = fgetc(act);

		sff_manager->colour_palette[j] = mu_map_rgb(sff_manager->graphics_manager, r, g, b);
	}
	fclose(act);

	return true;
}

// If this function causes issues, revert references to anim to sff_manager->anim
void prepare_anim(MU_SFF_Manager *sff_manager, int anim_id)
{
	sff_manager->anim = get_action(sff_manager->air_manager, anim_id);
	Action *anim = sff_manager->anim;

	anim->during_time = get_game_time(sff_manager->timer) + anim->anim_element[0].during_time;
	anim->current_image = 0;
	anim->has_loop = false;
	anim->current_image_time = 0;
	anim->current_time = 0;
}

// Chooses the appropriate drawing function for the sprite
void blit_sprite(MU_SFF_Manager *sff_manager, int16_t group_num, int16_t image_num, int16_t x, int16_t y)
{
	// Search for sprite in the list
	int i = find_sprite(sff_manager, group_num, image_num);

	if(i == -1)
	{
		debug_print("SFF Manager: sprite not found in group %d, image number %d", group_num, image_num);
		return;
	}

	if(sff_manager->flags == BLT_NORMAL)
	{
		mu_normal_blt(sff_manager->graphics_manager, &sff_manager->sprite_list[i], x, y, false);
	}

	// CHECK: can these if statements be turned into else if statements?
	if(sff_manager->flags == BLT_NORMALMASKED)
	{
		mu_normal_blt(sff_manager->graphics_manager, &sff_manager->sprite_list[i], x, y, true);
	}

	if(sff_manager->flags == BLT_FLIPH)
	{
		mu_normal_flip_h(sff_manager->graphics_manager, &sff_manager->sprite_list[i], x, y, false);
	}

	if(sff_manager->flags == BLT_FLIPHMASKED)
	{
		mu_normal_flip_h(sff_manager->graphics_manager, &sff_manager->sprite_list[i], x, y, true);
	}
	// TODO: Write flip_v and flip_hv functions
}

void blit_anim(MU_SFF_Manager *sff_manager, int16_t x, int16_t y)
{
	Element *anim_element = &sff_manager->anim->anim_element[sff_manager->anim->current_image];
	blit_sprite(sff_manager, anim_element->group_num, anim_element->image_num, x, y);
	Action *anim = sff_manager->anim;

	// Is it time for the next sprite in the animation?
	if(anim->during_time <= get_game_time(sff_manager->timer) && anim_element->during_time != 1)
	{
		anim->current_image++;

		if(anim->current_image >= anim->elements_count)
		{
			anim->current_image = 0;
			anim->current_time = 0;

			// If we have a loop start, go to it
			if(anim->loop_start != -1)
			{
				anim->current_image = anim->loop_start;
			}
		}

		// Calculate the new during time
		anim->during_time = get_game_time(sff_manager->timer) + anim_element->during_time;
	}
	anim->current_time++;
}