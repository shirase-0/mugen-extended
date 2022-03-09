#include "mu_headers.h"

// Initialise the MU_Graphics_Manager
MU_Graphics_Manager *mu_init_graphics_manager()
{
	// SDL Initialisation
	debug_print("Graphics Manager: Initialising");
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
	{
		debug_print("Error initialising SDL: %s", SDL_GetError());
		return NULL;
	}
	debug_print("SDL Initialisation successful!");

	// Create and render a window
	MU_Graphics_Manager *graphics_manager = (MU_Graphics_Manager*) malloc(sizeof(MU_Graphics_Manager));
	graphics_manager->delta_ptr = 0;
	graphics_manager->now_time = 0;
	graphics_manager->last_time = 0;
	graphics_manager->fps_count = 0;
	graphics_manager->fps = 0;

	graphics_manager->window = SDL_CreateWindow(
		"Mugen Extended",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		XMAX,
		YMAX,
		SDL_WINDOW_BORDERLESS//SDL_WINDOW_BORDERLESS // SDL_WINDOW_FULLSCREEN_DESKTOP
	);
	if(graphics_manager->window == NULL)
	{
		debug_print("Could not create window: %s\n", SDL_GetError());
		return NULL;
	}

	graphics_manager->screen_surface = SDL_GetWindowSurface(graphics_manager->window);
	graphics_manager->renderer = SDL_CreateRenderer(graphics_manager->window, -1, 0);
	if(graphics_manager->renderer == NULL)
	{
		debug_print("Could not create renderer: %s\n", SDL_GetError());
		return NULL;
	}
	graphics_manager->colour_format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);

	//SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best"); // Make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(graphics_manager->renderer, XMAX, YMAX);
	SDL_RenderSetScale(graphics_manager->renderer, 2.0, 2.0);
	// Not really necessary in fullscreen mode, but in the future, users should have an option to switch to windowed
	SDL_SetWindowIcon(graphics_manager->window, SDL_LoadBMP("icon.bmp")); 
	mu_clear_screen(graphics_manager);

	// Set the framerate to 60
	init_framerate(&graphics_manager->fps_manager);
	set_framerate(&graphics_manager->fps_manager, 60);

	// Load raster font
	mu_load_font(graphics_manager);

	// Set up rectangles for rendering
	graphics_manager->destination_rect.x = 0;
	graphics_manager->destination_rect.y = 100;
	graphics_manager->destination_rect.w = XMAX / 2;
	graphics_manager->destination_rect.h = (YMAX / 2) - 101;

	graphics_manager->src_rect.x = 0;
	graphics_manager->src_rect.y = 0;
	graphics_manager->src_rect.w = XMAX / 2;
	graphics_manager->src_rect.h = YMAX / 2;

	return graphics_manager;
}

void mu_load_font(MU_Graphics_Manager *graphics_manager)
{
	debug_print("Graphics Manager: Loading font...");
	FILE *file;
	char temp[255];
	int i = 0;
	graphics_manager->raster_font = calloc(255, sizeof(MUGENFONT));
	SDL_Surface *font_surface = SDL_LoadBMP("DebugFonts.bmp");

	if(font_surface == NULL)
	{
		debug_print("Graphics Manager: DebugFonts not found");
	}

	file = fopen("font.txt", "r");
	if(file == NULL)
	{
		debug_print("Graphics Manager: font.txt not found");
	}

	while(!feof(file))
	{
		fgets(temp, 255, file);
		sscanf(temp, "%c %i %i", &graphics_manager->raster_font[i].c, &graphics_manager->raster_font[i].x, &graphics_manager->raster_font[i].width);
		i++;
	}

	// Switching font_surface->format to screen_surface->format fixes the text rendering badly
	SDL_SetColorKey(font_surface, SDL_TRUE, SDL_MapRGB(graphics_manager->screen_surface->format, 0, 0, 0)); 

	graphics_manager->font_texture = SDL_CreateTextureFromSurface(graphics_manager->renderer, font_surface);
	SDL_FreeSurface(font_surface);
}

void mu_draw_text(MU_Graphics_Manager *graphics_manager, int x, int y, const char *text, ...)
{
	char temp[255];
	SDL_Rect font_rect;
	SDL_Rect screen_rect;

	// Parse any string interpolation arguments
	va_list ap;
	va_start(ap, text); // Parses the string for variables
	vsprintf(temp, text, ap); // Converts symbols to actual numbers
	va_end(ap);

	// Why do we not need to malloc space for these SDL_Rects first?
	memset(&font_rect, 0, sizeof(SDL_Rect));
	memset(&screen_rect, 0, sizeof(SDL_Rect));

	screen_rect.w = 10;
	screen_rect.h = 16;
	font_rect = screen_rect;
	screen_rect.x = x;
	screen_rect.y = y;

	// For every character in the temp string (255)
	for(int i = 0; i < strlen(temp); i++)
	{
		// For every character supported by the font
		for(int j = 0; j < 255; j++)
		{
			// If the character in the string matches one supported by the font
			if(temp[i] == graphics_manager->raster_font[j].c)
			{
				font_rect.x = graphics_manager->raster_font[j].x;
				font_rect.w = graphics_manager->raster_font[j].width;

				j = 255; // Move onto the next character in the string
			}
		}

		// Draw character, or skip some space if the character is whitespace
		if(temp[i] != 32)
		{
			// TODO: abstract this into a generalised render method
			// Tidy this up once the scaling issue has been fixed
			//SDL_UpdateTexture(graphics_manager->font_texture, NULL, graphics_manager->screen_surface->pixels, XMAX * sizeof(uint32_t));
			// The following two lines were working code, commented out now to try and fix the rendering pipeline
			SDL_RenderCopy(graphics_manager->renderer, graphics_manager->font_texture, &font_rect, &screen_rect);
			screen_rect.x += font_rect.w;
		}
		else
		{
			screen_rect.x += 5;
		}
	}
}

void mu_clear_screen(MU_Graphics_Manager *graphics_manager)
{
	SDL_SetRenderDrawColor(graphics_manager->renderer, 0, 0, 0, 255);
	SDL_RenderClear(graphics_manager->renderer);
}

uint32_t mu_map_rgb(MU_Graphics_Manager *graphics_manager, int red, int green, int blue)
{
	uint32_t rgb = SDL_MapRGB(graphics_manager->colour_format, red, green, blue);
	return rgb;
}

void mu_normal_blt(MU_Graphics_Manager *graphics_manager, SFF_Sprite *sprite_list, int16_t x, int16_t y, bool has_mask)
{
	uint32_t *work_data;
	uint32_t pitch;
	int16_t width = sprite_list->pcx_header.width;
	int16_t height = sprite_list->pcx_header.height;
	uint8_t *byte_data = sprite_list->pcx_file_bytes;
	uint32_t *colour_palette = sprite_list->colour_palette;

	// Calculate x and y values
	y -= height - (height - sprite_list->y);
	x -= width - (width - sprite_list->x);

	work_data = (uint32_t*) graphics_manager->screen_surface->pixels; 
	pitch = graphics_manager->screen_surface->pitch / 2;

	uint16_t y_clip = 0;
	uint16_t x_clip = 0;

	// Ensure that sprites render exactly where they're supposed to
	// i.e. if a sprite is half off screen, render only the half that is on screen
	if(x + width > XMAX)
	{
		width -= x + width - XMAX;
	}

	if(x < 0)
	{
		x_clip = -x;
		x = 0;
	}

	if(y + height > YMAX)
	{
		height -= y + height - YMAX;
	}

	if(y < 0)
	{
		y_clip = -y;
		y = 0;
	}

	work_data += y * pitch;
	work_data += x;

	if(has_mask)
	{
		bool blank_row = true;
		for(int i = y_clip; i < height; i++)
		{
			if(blank_row)
			{
				work_data += pitch / 2;
				blank_row = true;
			}

			for(int j = x_clip; j < width; j++)
			{
				// CHECK: If this doesn't work, switch references to width back to sprite_data->pcx_header.width
				// If the current pixel (i * number of rows + j pixels across) is not masked
				if(byte_data[j + i * width] != byte_data[0])
				{
					blank_row = false;
					*work_data = colour_palette[byte_data[j + i * width]];
				}
				work_data++;
			}
			work_data -= width - x_clip;
			work_data += pitch / 2;
		}
	}
	else
	{
		for(int i = y_clip; i < height; i++)
		{
			for(int j = x_clip; j < width; j++)
			{
				*work_data = colour_palette[byte_data[j + i * width]];
				work_data++;
			}
			work_data -= width - x_clip;
			work_data += pitch / 2; 
		}
	}
}

void mu_normal_flip_h(MU_Graphics_Manager *graphics_manager, SFF_Sprite *sprite_list, int16_t x, int16_t y, bool has_mask)
{
	uint32_t *work_data;
	uint32_t pitch;
	int16_t width = sprite_list->pcx_header.width;
	int16_t height = sprite_list->pcx_header.height;
	uint8_t *byte_data = sprite_list->pcx_file_bytes;
	uint32_t *colour_palette = sprite_list->colour_palette;

	// Calculate x and y values
	y -= height - (height - sprite_list->y);
	x -= width - sprite_list->x; // Is this line wrong? See the function above for clarification

	work_data = (uint32_t*) graphics_manager->screen_surface->pixels; 
	pitch = graphics_manager->screen_surface->pitch / 2;

	uint16_t y_clip = 0;
	uint16_t x_clip = 0;
	uint16_t x_clip2 = 0;

	// Ensure that sprites render exactly where they're supposed to
	// i.e. if a sprite is half off screen, render only the half that is on screen
	if(x + width > XMAX)
	{
		x_clip2 = width;
		width -= x + width - XMAX;
		// Needed for horizontal flip
		x_clip2 = x_clip2 - width;
	}

	if(x < 0)
	{
		x_clip = -x;
		x = 0;
	}

	if(y + height > YMAX)
	{
		height -= y + height - YMAX;
	}

	if(y < 0)
	{
		y_clip = -y;
		y = 0;
	}

	work_data += y * pitch;
	work_data += x;

	if(has_mask)
	{
		bool blank_row = true;
		for(int i = y_clip; i < height; i++)
		{
			if(blank_row)
			{
				work_data += pitch / 2;
				blank_row = true;
			}

			for(int j = x_clip; j < width; j++)
			{
				if(byte_data[width - j + x_clip2 - 1 + i * width] != byte_data[0])
				{
					blank_row = false;
					*work_data = colour_palette[byte_data[width - j + x_clip2 - 1 + i * width]];
				}
				work_data++;
			}
			work_data -= width - x_clip;
			work_data += pitch / 2; 
		}
	}
	else
	{
		for(int i = y_clip; i < height; i++)
		{
			for(int j = x_clip; j < width; j++)
			{
				*work_data = colour_palette[byte_data[width - j + x_clip2 - 1 + i * width]];
				work_data++;
			}
			work_data -= width - x_clip;
			work_data += pitch / 2; 
		}
	}
}

// TODO: Profile this function and speed it up as much as possible
void mu_draw(MU_Graphics_Manager *graphics_manager, SDL_Texture *texture)
{
	SDL_Renderer *renderer = graphics_manager->renderer;
	SDL_Rect destination = graphics_manager->destination_rect;
	SDL_Rect src = graphics_manager->src_rect;

	graphics_manager->now_time = SDL_GetTicks();
	uint32_t now_time = graphics_manager->now_time;
	uint32_t last_time = graphics_manager->last_time;

	if(now_time > last_time + HALF_SECOND)
	{
		graphics_manager->fps = (double) graphics_manager->fps_count * ONE_SECOND / (now_time - last_time);
		graphics_manager->fps_count = 0;
		graphics_manager->last_time = now_time;
	}

	mu_draw_text(graphics_manager, 0, 0, "%2.2f FPS", graphics_manager->fps);

	// TODO: Implement the following functions
	// FilterImage();
	//  scale2x(work,screen);

	SDL_UpdateTexture(texture, NULL, graphics_manager->screen_surface->pixels, XMAX * sizeof(uint32_t));
	SDL_RenderCopy(renderer, texture, &src, &destination);
	// Test code to render a boundary around the destination rectangle
	// SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	// SDL_RenderClear(renderer);
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_RenderDrawRect(renderer, &destination);
	SDL_RenderPresent(renderer);

	// Limit the framerate to 60 Hz
	framerate_delay(&graphics_manager->fps_manager);
	graphics_manager->fps_count++;

	// Clear screen before next frame renders
	SDL_RenderClear(graphics_manager->renderer);
	SDL_FillRect(graphics_manager->screen_surface, NULL, 0x000000);
}