#include "common.h"

int cellect_flag = 0;	//
int stopper = 1;	//
int col = 0;		//矢印の位置設定


/*
装備の変更とそれに連なる画像の変更
*/
int wepon_collect(){
	SDL_GL_SwapBuffers();//画面更新
	
	/*セレクト系処理*/
	SDL_Event Qevents;
	if(SDL_PollEvent(&Qevents))
		//更新がはやいためぶれる
		switch(Qevents.type){
		case SDL_KEYDOWN: 	
			cellect_flag = 1;
		case SDL_JOYAXISMOTION:
			if(Qevents.jaxis.axis==1){
				if(Qevents.jaxis.value > 0){
					col++;
					if(col > 1){
						col = 1;
					}
				}
				if(Qevents.jaxis.value < 0){
					col--;
					if(col < 0){
						col = 0;
					}
				}

			}//printf("%d\n",col);
			break;
		case SDL_JOYBUTTONDOWN:
			if(Qevents.jbutton.button == 3) cellect_flag = 1;
			break;
	}
	modelDrow2(col);//描画
	
	
	if(stopper){
		SDL_GL_SwapBuffers();	//入力されたときのみ更新
		stopper=0;
		sleep(1);
	}
	if(cellect_flag) return 1;	//セレクト完了最大次に移動
	return 0;
}
