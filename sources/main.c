#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 320
#define HEIGHT 200

uint32_t framebuffer[WIDTH * HEIGHT];



void put_pixel(int x, int y, uint32_t color) {
    if(x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;
    framebuffer[y*WIDTH + x] = color;
    return;
}
void clear(uint32_t color) {
    for(int i=0; i < WIDTH * HEIGHT; i++)
    framebuffer[i] = color;
}


int main(void) {

    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    SDL_Event event;

    const double target_frame = 1.0/60.0; // 0.016

    if (SDL_Init(SDL_INIT_VIDEO) == 0) {
        fprintf(stderr, "SDL init failed: %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(
        "SDL Framebuffer", 
        WIDTH * 4, 
        HEIGHT * 4, 
        0
    );
    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow failed: %s \n", SDL_GetError());
        SDL_Quit();
        return EXIT_FAILURE;
    }


    renderer = SDL_CreateRenderer(
        window, 
        NULL
    );
    if (renderer == NULL) {
        fprintf(stderr, "SDL_Create_renderer failed: %s \n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }


    texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_XRGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );
    if (texture == NULL) {
        fprintf(stderr, "SDL_CreateTexture failed: %s \n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }




    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST); 

    uint8_t is_running = 1;
    uint32_t frame = 0;


    while (is_running) {

        uint64_t start = SDL_GetPerformanceCounter();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                is_running = 0;
            }
        }

        // clear(0xFFFFFF);
        // clear(0x2A2A2A);

        // int x = frame % 320;
        // int y = HEIGHT/2;        
        // put_pixel(x, y, 0x00FF00);


        // put_pixel(WIDTH/2, HEIGHT/2, 0xFF0000);
        // framebuffer[HEIGHT*30 + 10] = 0x00FF00;
        // framebuffer[16500] = 0xFF0000;

        for (int y=0; y<HEIGHT; y++){
            for(int x=0; x<WIDTH; x++){
                if((x+y)%5 == 0){
                    put_pixel(x, y, 0x123456);
                }
            }
        }



        SDL_UpdateTexture(
            texture,
            NULL,
            framebuffer,
            WIDTH * sizeof(uint32_t) 
        );
    
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
 
 
        uint64_t end = SDL_GetPerformanceCounter();

        double elapsed = (double)(end-start) / (double)SDL_GetPerformanceFrequency();
    
        if(elapsed < target_frame) {
            SDL_Delay((target_frame - elapsed) * 1000.0); // sleep untill 60FPS
        }
        frame++;
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();


    printf("done");
    return EXIT_SUCCESS;
}