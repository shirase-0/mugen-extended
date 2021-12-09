#include "mu_headers.h"

void scale2x(SDL_Surface *src, SDL_Surface *dst); // TODO: Implement this

// Initialise the MU_Video_System
MU_Video_System *mu_init_video_system()
{
	// SDL Initialisation
	mu_log_message("Video System: Initialising");
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0)
    {
    	mu_log_message("Error initialising SDL: %s", SDL_GetError());
    	return NULL;
    }
    mu_log_message("SDL Initialisation successful!");

    // Create and render a window
	MU_Video_System *video_system = (MU_Video_System*) malloc(sizeof(MU_Video_System));
	video_system->window = NULL;
	video_system->screen_surface = NULL;
	video_system->renderer = NULL;

	video_system->window = SDL_CreateWindow(
    	"Mugen Extended",
    	SDL_WINDOWPOS_UNDEFINED,
    	SDL_WINDOWPOS_UNDEFINED,
    	XMAX,
    	YMAX,
    	SDL_WINDOW_OPENGL
    );

    if (video_system->window == NULL)
    {
    	mu_log_message("Could not create window: %s\n", SDL_GetError());
    }

    video_system->screen_surface = SDL_GetWindowSurface(video_system->window);
    video_system->renderer = SDL_CreateRenderer(video_system->window, -1, 0);

    if (video_system->renderer == NULL)
    {
    	mu_log_message("Could not create renderer: %s\n", SDL_GetError());
    }

    if(!video_system->screen_surface)
    {
    	mu_log_message("Could not create screen surface: %s\n", SDL_GetError());
    	SDL_Quit();
    	return NULL;
    }   

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // Make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(video_system->renderer, XMAX, YMAX); // XMAX and YMAX are defined in video_system.h

    SDL_FillRect(video_system->screen_surface, NULL, SDL_MapRGB(video_system->screen_surface->format, 0, 0, 0));
	SDL_UpdateWindowSurface(video_system->window);

	// Zero out initial values
	video_system->delta_ptr = 0;
	video_system->now_time = 0;
	video_system->last_time = 0;
	video_system->n_fps_count = 0;
	video_system->n_fps = 0;

	// Set the framerate to 60
	init_framerate(&video_system->fps_manager);
	set_framerate(&video_system->fps_manager, 60);

	// Load raster font
	mu_load_font(video_system);

	return video_system;
}

void mu_load_font(MU_Video_System *video_system)
{
	mu_log_message("Loading font...");
	FILE *p_file;
	char str_temp[255];
	int i = 0;
	video_system->mu_font = malloc(sizeof(MUGENFONT) * 255);
	memset(video_system->mu_font, 0, sizeof(MUGENFONT) * 255);

	video_system->font = SDL_LoadBMP("DebugFonts.bmp");

	if(video_system->font == NULL)
	{
		mu_log_message("Video System: DebugFonts not found");
	}

	p_file = fopen("font.txt", "r");

	if(p_file == NULL)
	{
		mu_log_message("Video System: font.txt not found");
	}

	while(!feof(p_file))
	{
		fgets(str_temp, 255, p_file);
		// The following line may need its formatting flags changed from %i to something more appropriate
		sscanf(str_temp, "%c %i %i", &video_system->mu_font[i].c, &video_system->mu_font[i].x, &video_system->mu_font[i].n_width);
		i++;
	}

	SDL_SetColorKey(video_system->font, SDL_TRUE, SDL_MapRGB(video_system->screen_surface->format, 0, 0, 0));
}

// Draw a string to the window
void mu_draw_text(MU_Video_System *video_system, int x, int y, char *str_text, ...)
{
	char str_temp[255];
	int n_row = 0;
	int n_col = 0;
	SDL_Rect font_rect;
	SDL_Rect screen_rect;

	// Parse any string interpolation arguments
	va_list ap;
	va_start(ap, str_text); // Parses the string for variables
	vsprintf(str_temp, str_text, ap); // Converts symbols to actual numbers
	va_end(ap);

	memset(&font_rect, 0, sizeof(SDL_Rect));
	memset(&screen_rect, 0, sizeof(SDL_Rect));

	screen_rect.w = 10;
	screen_rect.h = 16;

	font_rect = screen_rect;

	screen_rect.x = x;
	screen_rect.y = y;

	for(int i = 0; i < strlen(str_temp); i++)
	{
		for(int j = 0; j < 255; j++)
		{
			if(str_temp[i] == video_system->mu_font[j].c)
			{
				font_rect.x = video_system->mu_font[j].x;
				font_rect.w = video_system->mu_font[j].n_width;

				j = 255;
			}
		}

		if(str_temp[i] != 32)
		{
			SDL_BlitSurface(video_system->font, &font_rect, video_system->screen_surface, &screen_rect);
			screen_rect.x += font_rect.w;
		}
		else
		{
			screen_rect.x += 5;
		}
	}
}

// Render a black rectangle over the current screen composite, effectively "clearing" the screen
void mu_clear_screen(MU_Video_System *video_system)
{
	u32 colour;

	colour = SDL_MapRGB(video_system->screen_surface->format, 0, 0, 0);
	SDL_FillRect(video_system->screen_surface, NULL, colour);
}

// This function call seems unnecessary
// TODO: remove this function?
unsigned mu_map_rgb(MU_Video_System *video_system, int red, int green, int blue)
{
	return SDL_MapRGB(video_system->screen_surface->format, red, green, blue);
}

// This function is never used
// Probably no longer necessary in SDL2
// TODO: possibly remove this?
SDL_Surface *mu_create_surface(MU_Video_System *video_system, int x, int y)
{
	SDL_Surface *temp_surface = NULL;

	temp_surface = SDL_CreateRGBSurface(0, x, y, 16, video_system->screen_surface->format->Rmask
												   , video_system->screen_surface->format->Gmask
												   , video_system->screen_surface->format->Bmask
												   , video_system->screen_surface->format->Amask);

	if(temp_surface == NULL)
	{
		mu_log_message("Video System: Failed to create surface; %s", SDL_GetError());
		return NULL;
	}

	return temp_surface;
}

// The main function that draws pixels to the screen
// This is used to render any given pcx image file
// Used for rendering player 1 sprites
void mu_normal_blt(MU_Video_System *video_system, SFF_Sprite *lp_sprite, s16 x, s16 y, bool b_mask)
{
	unsigned *lp_work_data;
	unsigned pitch; // was formerly u16

	s16 width = lp_sprite->pcx_header.width;
	s16 height = lp_sprite->pcx_header.height;
	u8 *by_data = lp_sprite->by_pcx_file;
	unsigned *colour_table = lp_sprite->colour_palette; // was formerly &lp_sprite->colour_palette; // was formerly u16

	// Calculate x and y values
	y -= height - (height - lp_sprite->y);
	x -= width - (width - lp_sprite->x);

	lp_work_data = (unsigned*) video_system->screen_surface->pixels;
	pitch = video_system->screen_surface->pitch / 2; // Affects height, not width

	u16 y_clip = 0;
	u16 y_clip2 = 0;
	u16 x_clip = 0;
	u16 x_clip2 = 0;

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
		height -= y + height -YMAX;
	}

	if(y < 0)
	{
		y_clip = -y;
		y = 0;
	}

	lp_work_data += y * pitch;
	lp_work_data += x;

	if(!b_mask) // Currently unused code path?
	{
		// Iterate across each pixel on the screen
		for(int i = y_clip; i < height; i++)
		{
			for(int j = x_clip; j < width; j++)
			{
				*lp_work_data = colour_table[by_data[j + i * lp_sprite->pcx_header.width]];
				lp_work_data++;

			}
			lp_work_data -= width - x_clip; // Skip back to x = 0
			lp_work_data += pitch / 2; // Skip over remaining x pixels on the previous row to get y++
			// ^ ^ ^ ^ ^ ^ ^this shouldn't be divided by 2, change back to just pitch when the bug that caused a need for this workaround is found
		}
	}
	else // Always used when an alpha mask is required
	{
		bool blank_row = true;
		for(int i = y_clip; i < height; i++)
		{
			if(blank_row)
			{
				lp_work_data += pitch / 2;
				blank_row = true;
			}

			for(int j = x_clip; j < width; j++)
			{

				// If the current pixel (i * number of rows + j pixels across) is not masked
				if(by_data[j + i * lp_sprite->pcx_header.width] != by_data[0])
				{
					blank_row = false;
					*lp_work_data = colour_table[by_data[j + i * lp_sprite->pcx_header.width]];
				}
				lp_work_data++; // Not sure if this should be in the if statement above or not
			}
			lp_work_data -= width - x_clip;
			lp_work_data += pitch / 2; // this shouldn't be divided by 2, change back to just pitch when the bug that caused a need for this workaround is found
		}
	}
}

// Operates as the function above does, but flipped on the horizontal axis
// This is used for rendering player 2 sprites
void mu_normal_flip_h(MU_Video_System *video_system, SFF_Sprite *lp_sprite, s16 x, s16 y, bool b_mask)
{
	unsigned *lp_work_data;
	unsigned pitch;

	s16 width = lp_sprite->pcx_header.width;
	s16 height = lp_sprite->pcx_header.height;
	u8 *by_data = lp_sprite->by_pcx_file;
	unsigned *colour_table = lp_sprite->colour_palette;

	// Calculate x and y values
	y -= height - (height - lp_sprite->y);
	x -= width - lp_sprite->x; // Is this line wrong? See the function above for clarification

	lp_work_data = (unsigned*) video_system->screen_surface->pixels;
	pitch = video_system->screen_surface->pitch / 2;

	u16 y_clip = 0;
	u16 y_clip2 = 0;
	u16 x_clip = 0;
	u16 x_clip2 = 0;

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

	lp_work_data += y * pitch;
	lp_work_data += x;

	if(!b_mask)
	{
		for(int i = y_clip; i < height; i++)
		{
			for(int j = x_clip; j < width; j++)
			{
				*lp_work_data = colour_table[by_data[width - j + x_clip2 - 1 + i * lp_sprite->pcx_header.width]];
				lp_work_data++;
			}
			lp_work_data -= width - x_clip;
			lp_work_data += pitch / 2; // this shouldn't be divided by 2, change back to just pitch when the bug that caused a need for this workaround is found
		}
	}
	else // Always used when an alpha mask is required
	{
		bool blank_row = true;
		for(int i = y_clip; i < height; i++)
		{
			if(blank_row)
			{
				lp_work_data += pitch / 2;
				blank_row = true;
			}

			for(int j = x_clip; j < width; j++)
			{
				if(by_data[width - j + x_clip2 - 1 + i * lp_sprite->pcx_header.width] != by_data[0])
				{
					blank_row = false;
					*lp_work_data = colour_table[by_data[width - j + x_clip2 - 1 + i * lp_sprite->pcx_header.width]];
				}
				lp_work_data++; // Not sure if this should be in the if statement above or not
			}
			lp_work_data -= width - x_clip;
			lp_work_data += pitch / 2; // this shouldn't be divided by 2, change back to just pitch when the bug that caused a need for this workaround is found
		}
	}
}

// Not sure if this function is actually being used to render graphics or not currently
// Initially this function was only used to set the framerate
// TODO: update this function so that every tick of the game uses this function to render the screen composite
void mu_draw(MU_Video_System *video_system, SDL_Texture *sdl_texture)
{
	// First clear the screen? Maybe unnecessary
	SDL_SetRenderDrawColor(video_system->renderer, 0, 100, 0, 255);
	//SDL_RenderClear(video_system->renderer);
	//SDL_UnlockSurface(video_system->screen_surface);

	video_system->now_time = SDL_GetTicks();

	if(video_system->now_time > video_system->last_time + 500)
	{
		video_system->n_fps = (float)video_system->n_fps_count * 1000 / (video_system->now_time - video_system->last_time);
		video_system->n_fps_count = 0;
		video_system->last_time = video_system->now_time;
	}

	mu_draw_text(video_system, 0, 0, "%2.2f FPS", video_system->n_fps);

	// TODO: Implement the following functions
	// FilterImage();
	//  scale2x(work,screen);

	SDL_UpdateTexture(sdl_texture, NULL, video_system->screen_surface->pixels, XMAX * sizeof(u32));
	SDL_RenderClear(video_system->renderer);
	SDL_RenderCopy(video_system->renderer, sdl_texture, NULL, NULL);
	SDL_RenderPresent(video_system->renderer);

	// If the RenderPresent way fails, fall back to using UpdateWindowSurface
	//SDL_UpdateWindowSurface(video_system->window);

	// Limit the framerate to 60 Hz
	framerate_delay(&video_system->fps_manager);

	video_system->n_fps_count++;
}