#include "mu_headers.h"

MU_SFF_Manager *sff_manager_init()
{
	MU_SFF_Manager *sff_manager = (MU_SFF_Manager*) malloc(sizeof(MU_SFF_Manager));
	sff_manager->sff_video_system = NULL;
	sff_manager->sff_allocator = NULL;
	sff_manager->x_scale_value = 1.0;
	sff_manager->y_scale_value = 1.0;

	return sff_manager;
}

void mu_set_sff_pointers(MU_SFF_Manager *sff_manager, MU_Video_System *video_system, MU_Allocator *allocator, MU_Timer *timer, MU_Air_Manager *air_manager)
{
	sff_manager->sff_video_system = video_system;
	sff_manager->sff_allocator = allocator;
	sff_manager->sff_timer = timer;
	sff_manager->sff_air_manager = air_manager;
}

void reset_sff_manager(MU_SFF_Manager *sff_manager)
{
	sff_manager->b_pallet_loaded = false;
	sff_manager->lp_sff_file = 0;
	sff_manager->n_total_images = 0;
	sff_manager->n_current_image = 0;
	sff_manager->n_image_list_size = 100;
	sff_manager->lp_sprite_list = (SFF_Sprite*) mu_alloc(sff_manager->sff_allocator, sizeof(SFF_Sprite) * sff_manager->n_image_list_size);
	//memset(sff_manager->colour_palette, 0, sizeof(u16) * 256);
	// NORMAL BLIT
	sff_manager->n_flags = BLT_NORMAL;
}

u8 *decode_pcx(MU_SFF_Manager *sff_manager, u8 *pcx_byte, PCX_Header header)
{
	u8 *by_image_buffer = 0;
	u8 bpp;
	u8 by_data;
	u16 size;
	s16 x;
	s16 y;
	s16 width;
	u32 pos = 0;

	u32 n_count_byte = 0; // unused?
	u32 n_total_byte = header.bytes_per_line * header.n_planes; // unused?
	u32 n_how_many_bytes = 0; // unused?
	u32 n_how_many_blank = 0;

	bpp = header.n_planes * 8;

	// Allocate memory
	by_image_buffer = (u8*) mu_alloc(sff_manager->sff_allocator, sizeof(u8) * (header.width * header.n_planes * header.height + 1));

	width = header.width;

	if(width < header.bytes_per_line * header.n_planes)
	{
		width = header.bytes_per_line * header.n_planes;
	}

	// We don't support 24 bit pcx images
	if(bpp > 8)
	{
		mu_log_message("SFF Manager: 24 bit pcx file is not supported");
		return by_image_buffer;
	}

	size = 0;
	y = 0;
	x = 0;

	for(y = 0; y < header.height; y++)
	{
		x = 0;
		while(x < width)
		{
			by_data = pcx_byte[pos++];

			if((by_data & 0xC0) == 0xC0)
			{
				size = by_data & 0x3F;
				by_data = pcx_byte[pos++];
			}
			else
			{
				size = 1;
			}

			while(size-- > 0)
			{
				if(x <= header.width)
				{
					by_image_buffer[x + (y * header.width)] = by_data;
				}
				//this it to Skip blank data on PCX image wich are on the right side
				//TODO:OK? Skip two bytes
				if(x == width && width != header.width)
				{
					n_how_many_blank = width - header.width;
					for(u32 i = 0; i < n_how_many_blank; i++)
					{
						pos +=2;
					}
				}
				x++;
			}
		}
	}

	return by_image_buffer;
}

void decode_sff_file(MU_SFF_Manager *sff_manager)
{
	SFF_Subheader subheader;
	u8 *temp_byte;

	// Read the first subheader
	fread(&subheader, sizeof(subheader), 1, sff_manager->lp_sff_file);

	while(!feof(sff_manager->lp_sff_file))
	{
		// Increase array size if needed
		if(sff_manager->n_total_images > sff_manager->n_image_list_size - 1)
		{
			sff_manager->n_image_list_size += 100;
			sff_manager->lp_sprite_list = (SFF_Sprite*) mu_realloc(sff_manager->sff_allocator, sff_manager->lp_sprite_list, sizeof(SFF_Sprite) * sff_manager->n_image_list_size);
		}

		// Copy the information contained in the subheader
		sff_manager->lp_sprite_list[sff_manager->n_total_images].group_number = subheader.group_number;
		sff_manager->lp_sprite_list[sff_manager->n_total_images].image_number = subheader.image_number;
		sff_manager->lp_sprite_list[sff_manager->n_total_images].x = subheader.x;
		sff_manager->lp_sprite_list[sff_manager->n_total_images].y = subheader.y;

		// Is it a linked sprite?
		// Note to self: this section can never be reached because length_of_subheader is never set
		// This should probably at least be zeroed out at the start of this function to prevent undefined behaviour
		if(subheader.length_of_subheader != 0)
		{
			// Read the pcx header
			fread(&sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header, sizeof(PCX_Header), 1, sff_manager->lp_sff_file);

			// Correct the image dimension
			sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.width = sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.width - sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.x + 1;
			sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.height = sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.height - sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.y + 1;

			// Now read the pcx data
			//TODO:check subheader.LenghtOfSubheader
			temp_byte = (u8*) mu_alloc(sff_manager->sff_allocator, sizeof(u8) * (subheader.length_of_subheader - 127));
			fread(temp_byte, sizeof(u8) * (subheader.length_of_subheader - 127), 1, sff_manager->lp_sff_file);

			sff_manager->lp_sprite_list[sff_manager->n_total_images].by_pcx_file = decode_pcx(sff_manager, temp_byte, sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header);

			// Free temp_byte
			mu_free(sff_manager->sff_allocator, temp_byte);

			// Move the file pointer to the colour palette of the pcx file
			fseek(sff_manager->lp_sff_file, -768L, SEEK_CUR);

			// Eat empty 8 bits
			fgetc(sff_manager->lp_sff_file);

			if(fgetc(sff_manager->lp_sff_file) == 12 && !subheader.palette_same && !sff_manager->b_pallet_loaded && sff_manager->lp_sprite_list[sff_manager->n_total_images].pcx_header.n_planes <=1)
			{
				for(int j = 0; j < 256; j++)
				{
					int r, g, b;
					r = fgetc(sff_manager->lp_sff_file);
					g = fgetc(sff_manager->lp_sff_file);
					b = fgetc(sff_manager->lp_sff_file);
					
					sff_manager->colour_palette[j] = mu_map_rgb(sff_manager->sff_video_system, r, g, b);
				}
			}

			// Copy the colour palette to the SFF_Sprite struct
			memcpy(&sff_manager->lp_sprite_list[sff_manager->n_total_images].colour_palette, sff_manager->colour_palette, sizeof(unsigned) * 256); // was sizeof(u16)
		}
		else
		{
			// subheader.IndexOfPrevious-1 if the first element is 1 and not 0
          	// we have a linked image here

          	// Note to self: there was a problem here where images linked to index_of_previous 0 were causing segfaults
          	// I'm not sure if the following code actually fixed the problem or not though, since the problematic image in question was a 9000,0 image
          	// mu_log_message("subheader.index_of_previous: %i", subheader.index_of_previous);
          	if(subheader.index_of_previous == 0)
          	{
          		memcpy(&sff_manager->lp_sprite_list[sff_manager->n_total_images], &sff_manager->lp_sprite_list[subheader.index_of_previous], sizeof(SFF_Sprite));
          	}
          	else
          	{
          		memcpy(&sff_manager->lp_sprite_list[sff_manager->n_total_images], &sff_manager->lp_sprite_list[subheader.index_of_previous - 1], sizeof(SFF_Sprite));
          	}
          	sff_manager->lp_sprite_list[sff_manager->n_total_images].group_number = subheader.group_number;
          	sff_manager->lp_sprite_list[sff_manager->n_total_images].image_number = subheader.image_number;
		}
		sff_manager->n_total_images++;

		fseek(sff_manager->lp_sff_file, subheader.next_subheader_file_offset, SEEK_SET);

		// Read the next subheader
		fread(&subheader, sizeof(subheader), 1, sff_manager->lp_sff_file);
	}
	mu_log_message("SFF Manager: %d images have been decoded", sff_manager->n_total_images);
}

int find_sprite(MU_SFF_Manager *sff_manager, s16 n_group_number, s16 n_image_number)
{
	for(int i = 0; i < sff_manager->n_image_list_size; i++)
	{
		if(sff_manager->lp_sprite_list[i].group_number == n_group_number && sff_manager->lp_sprite_list[i].image_number == n_image_number)
		{
			return i;
		}
	}

	// Else if no image found
	return -1;
}

bool load_sff_file(MU_SFF_Manager *sff_manager, const char *str_sff_file)
{
	SFF_Header header;

	mu_log_message("SFF Manager: Loading %s", str_sff_file);
	sff_manager->lp_sff_file = fopen(str_sff_file, "rb");

	if(sff_manager->lp_sff_file == 0)
	{
		mu_log_message("SFF Manager: Failed to find %s", str_sff_file);
		return false;
	}

	// Read the header of the file
	fread(&header, sizeof(header), 1, sff_manager->lp_sff_file);

	// Check file signature
	if(strcmp((char*) header.signature, "ElecbyteSpr") != 0)
	{
		mu_log_message("SFF Manager: %s is not an SFF file", str_sff_file);
		return false;
	}
	mu_log_message("SFF Manager: SFF file contains %d images", header.number_of_image);

	// Move the file to the first subheader
	fseek(sff_manager->lp_sff_file, header.subheader_file_offset, SEEK_SET);
	decode_sff_file(sff_manager);
	fclose(sff_manager->lp_sff_file);

	// Save memory
	sff_manager->lp_sprite_list = (SFF_Sprite*) mu_realloc(sff_manager->sff_allocator, sff_manager->lp_sprite_list, sizeof(SFF_Sprite) * sff_manager->n_total_images);

	return true;
}

bool load_act_to_sff(MU_SFF_Manager *sff_manager, const char *str_act_file)
{
	sff_manager->b_pallet_loaded = true;
	FILE *act = fopen(str_act_file, "rb");

	if(act == NULL)
	{
		mu_log_message("SFF Manager: %s file not found", str_act_file);
	}

	for(int j = 255; j >=0; j--)
	{
		int r, g, b;
		// This order has been flipped to BGR, as the palette has blue and red values reversed
		r = fgetc(act); 
		g = fgetc(act);
		b = fgetc(act);
		
		

		sff_manager->colour_palette[j] = mu_map_rgb(sff_manager->sff_video_system, r, g, b);

		// This was a workaround for a problem that has seemingly disappeared??
		//mu_log_message("Mapped RGB Value: %i", sff_manager->colour_palette[j]);
		// if(b == 0 && g == 0 && r == 0)
		// {
		// 	for(; j > 0; j--)
		// 	{
		// 		//mu_log_message("%i", fgetc(act));
		// 		r = fgetc(act); 
		// 		g = fgetc(act);
		// 		b = fgetc(act);

		// 		sff_manager->colour_palette[j] = mu_map_rgb(sff_manager->sff_video_system, r, g, b);
		// 		//mu_log_message("Mapped RGB Value: %i", sff_manager->colour_palette[j]);
		// 	}
		// }
	}
	fclose(act);

	return true;
}

void blit_sprite(MU_SFF_Manager *sff_manager, s16 n_group_number, s16 n_image_number, s16 x, s16 y)
{
	// Search for the sprite in the list
	int i = find_sprite(sff_manager, n_group_number, n_image_number);

	if(i == -1)
	{
		return;
	}

	if(sff_manager->n_flags == BLT_NORMAL)
	{
		mu_normal_blt(sff_manager->sff_video_system, &sff_manager->lp_sprite_list[i], x, y, false);
	}

	if(sff_manager->n_flags == BLT_NORMALMASKED)
	{
		mu_normal_blt(sff_manager->sff_video_system, &sff_manager->lp_sprite_list[i], x, y, true);
	}

	if(sff_manager->n_flags == BLT_FLIPH)
	{
		mu_normal_flip_h(sff_manager->sff_video_system, &sff_manager->lp_sprite_list[i], x, y, false);
	}

	if(sff_manager->n_flags == BLT_FLIPHMASKED)
	{
		mu_normal_flip_h(sff_manager->sff_video_system, &sff_manager->lp_sprite_list[i], x, y, true);
	}

	// It's my guess that we still need to fill in the flip_v and flip_hv functions here, but they weren't written in the original code, so I'll have to write those from scratch later
}

void prepare_anim(MU_SFF_Manager *sff_manager, s32 n_anim)
{
	sff_manager->anim = get_action(sff_manager->sff_air_manager, n_anim);

	sff_manager->anim->n_during_time = get_game_time(sff_manager->sff_timer) + sff_manager->anim->animation_element[0].n_during_time;
	sff_manager->anim->n_current_image = 0;
	sff_manager->anim->b_looped = false;
	sff_manager->anim->n_current_image_time = 0;
	sff_manager->anim->n_current_time = 0;
}

void blit_anim(MU_SFF_Manager *sff_manager, s16 x, s16 y)
{
	blit_sprite(sff_manager, sff_manager->anim->animation_element[sff_manager->anim->n_current_image].n_group_number, sff_manager->anim->animation_element[sff_manager->anim->n_current_image].n_image_number, x, y);

	// Is the current image in time? && do not check if during time of the current image is -1
	if(sff_manager->anim->n_during_time <= get_game_time(sff_manager->sff_timer) && sff_manager->anim->animation_element[sff_manager->anim->n_current_image].n_during_time != -1)
	{
		sff_manager->anim->n_current_image++;

		if(sff_manager->anim->n_current_image >= sff_manager->anim->n_number_of_elements)
		{
			sff_manager->anim->n_current_image = 0;
			sff_manager->anim->n_current_time = 0;

			// If we have a loop start, go to it
			if(sff_manager->anim->loop_start != -1)
			{
				sff_manager->anim->n_current_image = sff_manager->anim->loop_start;
			}
		}

		// Calculate the new during time
		sff_manager->anim->n_during_time = get_game_time(sff_manager->sff_timer) + sff_manager->anim->animation_element[sff_manager->anim->n_current_image].n_during_time;
	}
	sff_manager->anim->n_current_time++;
}