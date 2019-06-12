#include "common.h"

SDL_Surface* window;

/* 終了処理を行う */
void Finalize(void){
    	/* TODO: ここで画像や音声を解放する */
    	SDL_FreeSurface(window);

    	/* 終了する */
    	SDL_Quit();
}
