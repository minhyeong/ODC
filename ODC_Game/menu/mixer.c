#include "SDL/SDL.h"
#include "SDL/SDL_mixer.h"

// 以下ソースの適当なところに書く
int main(void)
{
 Mix_Music    *music;
 Mix_Chunk    *chunk,*chunk2;

 SDL_Init(SDL_INIT_AUDIO  );                // SDL初期化

 Mix_OpenAudio(22050,AUDIO_S16,2,4096);    // SDL_Mixerオープン

 music = Mix_LoadMUS("cyberbird    - GHOST IN THE SHELL O.S.T.2 -.wav");        // 曲ファイルをロード

 Mix_AllocateChannels(16);                // 同時発声数を設定
 
 Mix_PlayMusic(music,-1);                // 曲の再生スタート(無限ループ)
 SDL_Delay(20000);
 Mix_HaltMusic();                        // 曲の再生停止
 
 Mix_FreeMusic(music);                    // ロードした曲を開放

 Mix_CloseAudio();                        // SDL_Mixerクローズ

 SDL_Quit();                                // SDL使用終了
    
}
