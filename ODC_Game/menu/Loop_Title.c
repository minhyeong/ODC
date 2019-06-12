#include "common.h"

Uint32 cntA,cntB,now,lev,wit;//FPS用
int title_changer = 0;
/*効果音
*/
void soundbt(){
	Mix_Music    *musicx;
	musicx = Mix_LoadMUS("menu.mp3");
	Mix_PlayMusic(musicx,1);
	SDL_Delay(850);
}

/*
タイトル画面に関するメイン関数
*/
int Loop_Title(){
	SDL_Joystick *joystick;
	SDL_Event event;
	Airframe myself;
	
	double next_frame = SDL_GetTicks();
  	double wait = 1000.0 / 60;

	modelDrow(title_changer); //色々描画
		
	if(SDL_PollEvent(&event)){
		switch (event.type) {
		case SDL_JOYBUTTONDOWN:
			if(event.jbutton.button == 3){
				title_changer++;
				soundbt();
			}
			break;
		}
	}
	
 	//フレームレート調整
 	now=SDL_GetTicks();
  	wit=now-lev;
		
  	if(wit<16)SDL_Delay((16-wit));
  	lev=SDL_GetTicks();
	
  	if(now-cntB>=5000){//FPS確認用
 	   	printf("%d\n",cntA/5);
 		cntA=1;  cntB=now;
 	}else{
		cntA+=1;
	}
	SDL_GL_SwapBuffers();//画面更新
	if(title_changer == 3) return 1;
	return 0;
}











