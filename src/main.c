#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <SDL_image.h>

#define WIDTH 39
#define HEIGHT 260
#define ICON_SIZE 40
#define MARGIN (HEIGHT-ICON_SIZE*4)/3

typedef struct {
    unsigned int scancode;
    bool state;
    SDL_Texture *image_on;
    SDL_Texture *image_off;
    SDL_Rect rect;
}Button;

// KRC5 user buttons scancodes
#define SCANCODE1 129//65//129
#define SCANCODE2 130//83//130
#define SCANCODE3 131//68//131
#define SCANCODE4 132//70//132

static Button buttons[4];

HHOOK hHook;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *kbd = (KBDLLHOOKSTRUCT*)lParam;

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            for(int i = 0; i < 4; i++) {
                if(kbd->vkCode == buttons[i].scancode) buttons[i].state = true;
            }
            // printf("Key pressed: %lu\n", kbd->vkCode);
        }

        if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
            for(int i = 0; i < 4; i++) {
                if(kbd->vkCode == buttons[i].scancode) buttons[i].state = false;
            }
            // printf("Key released: %lu\n", kbd->vkCode);
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

int main (int argc, char *argv[]) {
    buttons[0].scancode = SCANCODE1;
    buttons[1].scancode = SCANCODE2;
    buttons[2].scancode = SCANCODE3;
    buttons[3].scancode = SCANCODE4;

    // keyboard hook
    HINSTANCE hInstance = GetModuleHandle(NULL);
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);

    if(!hHook) {
        printf("Failed to install hook\n");
        return EXIT_FAILURE;
    }

    printf("Hook installed. Press ESC to exit.\n");
    MSG msg;

	// SDL declarations
	SDL_Window *win = NULL;
	SDL_Renderer *renderer = NULL;
	int w, h; // texture width & height
	
	// SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL\n");
        return EXIT_FAILURE;
    }
	
	// create the window and renderer
	// note that the renderer is accelerated
    // and the windows is always on top
	win = SDL_CreateWindow("TEC KRC Overlay", 0, 486, WIDTH, HEIGHT, SDL_WINDOW_BORDERLESS);
	renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetWindowAlwaysOnTop(win, 1);
	
    // load button images
    buttons[0].image_off = IMG_LoadTexture(renderer, "img/icon1_off.png");
    buttons[0].image_on = IMG_LoadTexture(renderer, "img/icon1_on.png");
    buttons[1].image_off = IMG_LoadTexture(renderer, "img/icon2_off.png");
    buttons[1].image_on = IMG_LoadTexture(renderer, "img/icon2_on.png");
    buttons[2].image_off = IMG_LoadTexture(renderer, "img/icon3_off.png");
    buttons[2].image_on = IMG_LoadTexture(renderer, "img/icon3_on.png");
    buttons[3].image_off = IMG_LoadTexture(renderer, "img/icon4_off.png");
    buttons[3].image_on = IMG_LoadTexture(renderer, "img/icon4_on.png");
	
	//SDL_QueryTexture(img1, NULL, NULL, &w, &h); // get image width and height

    // define size and position of the 4 icons
    for(int i = 0; i < 4; i++) {
        buttons[i].rect.x = (WIDTH-ICON_SIZE)/2;
        buttons[i].rect.y = ICON_SIZE*i+MARGIN*i;
        buttons[i].rect.w = ICON_SIZE;
        buttons[i].rect.h = ICON_SIZE;
    }

    // open krc interface file
    FILE *fh = fopen("C:/KRC/ROBOTER/UserFiles/krc-overlay", "r");
    // FILE *fh = fopen("krc-overlay", "r");
    if (!fh) {
        printf("Failed to open KRC file\n");
        // return EXIT_FAILURE;
    }
    char krc_var_status;
	
	// main loop
	while(true) {
		
		// SDL event handling
		SDL_Event e;
		if ( SDL_PollEvent(&e) ) {
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYUP && e.key.keysym.sym == SDLK_ESCAPE)
				break;
		}

        // windows hook event handling
        // if (GetMessage(&msg, NULL, 0, 0)) {
        //     if (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE)
        //         break;
        //     TranslateMessage(&msg);
        //     DispatchMessage(&msg);
        // }

        // krc interface file reading
        if (fread(&krc_var_status, 1, 1, fh) == 1) {
            buttons[0].state = krc_var_status - '0';
            fseek(fh, 0, SEEK_SET);
        }
		
        // graphics rendering
		SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 160, 165, 170, 255);
		
        for(int i = 0; i < 4; i++)
        {
            if(buttons[i].state) SDL_RenderCopy(renderer, buttons[i].image_on, NULL, &buttons[i].rect);
            else SDL_RenderCopy(renderer, buttons[i].image_off, NULL, &buttons[i].rect);
        }
        
		SDL_RenderPresent(renderer);
	}
	
    // clean up
    UnhookWindowsHookEx(hHook);

    for(int i = 0; i < 4; i++) {
        SDL_DestroyTexture(buttons[i].image_on);
        SDL_DestroyTexture(buttons[i].image_off);
    }
    
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

    fclose(fh);
	
	return EXIT_SUCCESS;
}