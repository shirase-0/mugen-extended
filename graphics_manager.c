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
		SDL_WINDOW_FULLSCREEN_DESKTOP
	);
	if(graphics_manager->window == NULL)
	{
		debug_print("Could not create window: %s\n", SDL_GetError());
	}

	//graphics_manager->screen_surface = SDL_GetWindowSurface(graphics_manager->window);
	graphics_manager->renderer = SDL_CreateRenderer(graphics_manager->window, -1, 0);
	if(graphics_manager->renderer == NULL)
	{
		debug_print("Could not create renderer: %s\n", SDL_GetError());
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // Make the scaled rendering look smoother.
	SDL_RenderSetLogicalSize(graphics_manager->renderer, XMAX, YMAX);
	// Not really necessary in fullscreen mode, but in the future, users should have an option to switch to windowed
	SDL_SetWindowIcon(graphics_manager->window, SDL_LoadBMP("icon.bmp")); 

	SDL_SetRenderDrawColor(graphics_manager->renderer, 0, 0, 0, 255);
	mu_clear_screen(graphics_manager);

	// Set the framerate to 60
	init_framerate(&graphics_manager->fps_manager);
	set_framerate(&graphics_manager->fps_manager, 60);

	// Load raster font
	mu_load_font(graphics_manager);

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

	SDL_SetColorKey(font_surface, SDL_TRUE, SDL_MapRGB(font_surface->format, 0, 0, 0));

	// TODO: Implement the font using textures instead of surfaces, change the MU_Graphics_Manager to reflect this
	graphics_manager->font_texture = SDL_CreateTextureFromSurface(graphics_manager->renderer, font_surface);
	SDL_FreeSurface(font_surface);
}

void mu_draw_text(MU_Graphics_Manager *graphics_manager, int x, int y, char *text, ...)
{
	char temp[255];
	// TODO: Remove these?
	// int row = 0;
	// int col = 0;
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
			SDL_RenderCopy(graphics_manager->renderer, graphics_manager->font_texture, &font_rect, &screen_rect);
			SDL_RenderPresent(graphics_manager->renderer);

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
	SDL_RenderClear(graphics_manager->renderer);
	SDL_RenderPresent(graphics_manager->renderer);
}

