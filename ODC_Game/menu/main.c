#include "common.h"
#include "constants.h"

SDL_Joystick *joystick;     
int title_flag = 1; 
Object debris[Debri_MAX];
//server
extern void setup_server(int, u_short);
extern int control_requests_server();
extern void terminate_server();

//client
extern int setup_client(char *, u_short);
extern void terminate_client(); 

int myident;

//終了イベントの関数 
int endKeys(){     
	SDL_Event Qevents;
	if(SDL_PollEvent(&Qevents))  
		switch(Qevents.type){
  			case SDL_QUIT:      //ウィンドウのボタン
    		return 0; 
  			case SDL_KEYDOWN:   //escキー
    			if(Qevents.key.keysym.sym==SDLK_ESCAPE)  
				return 0;      
  		}         
	return 1; 
}    

//コールバック関数   
void callbackfunc(SDL_Event event)     
{
  /* TODO: ここにゲームの状態を更新する処理を書く */

}          

//メイン関数                     
  
//	ホスト名 ポート番号 
int main(int argc, char *argv[]){
	SDL_Joystick *joystick;
	SDL_Event event;
	Airframe myself; 
	Airframe other;
	
	Mix_Music *music;
 	Mix_Chunk *chunk,*chunk2;
	
	SDL_Init(SDL_INIT_AUDIO);		// SDL初期化
	Mix_OpenAudio(22050,AUDIO_S16,2,4096);	// SDL_Mixerオープン
	
	double next_frame = SDL_GetTicks(); 
  	double wait = 1000.0 / 60;
	
	   
	u_short port = DEFAULT_PORT;     
 	char server_name[MAX_LEN_NAME];
  	sprintf(server_name, "localhost"); 
 
  	switch(argc) {    
  	case 1:
    		break;
  	case 2:
    		sprintf(server_name, "%s", argv[1]);
    		break;
  	case 3:
    		sprintf(server_name, "%s", argv[1]);
    		port = (u_short)atoi(argv[2]);
    		break;
  	default:
    		fprintf(stderr, "Usage: %s [server name] [port number]\n", argv[0]);
    		return 1;
  	}

  	myident = setup_client(server_name, port);
	
	//printf("myid = %d",myident);
	init_joystick(joystick);
	init_window(window);
  	init_Airframe(myself);
	
	//for(int i = 0; i<4; i++){
	  init_Airframe(other);
	  //}

	init();
	music = Mix_LoadMUS("./sound/menu.mp3");	// 曲ファイルをロード
	Mix_AllocateChannels(16);			// 同時発声数を設定
	Mix_PlayMusic(music,-1);			// 曲の再生スタート(無限ループ)
	
	while(endKeys()!=0){
            if(Loop_Title()) 
                break;
                }			// タイトル＆説明
	while(endKeys()!=0){
            if(wepon_collect()) 
                break;
        }
        
        init_model();	// セレクト画面
	
	Mix_HaltMusic();			// 曲の再生停止
	Mix_FreeMusic(music);			// ロードした曲を開放
	music = Mix_LoadMUS("./sound/game.mp3");// 曲ファイルをロード
	Mix_AllocateChannels(16);		// 同時発声数を設定
	Mix_PlayMusic(music,-1);		// 曲の再生スタート(無限ループ)
	
        while(endKeys()!=0){
            if(Loop_Game(myident)) 
                break;
        }
	Mix_HaltMusic();	// 曲の再生停止
        Mix_FreeMusic(music);	// ロードした曲を開放
 	Mix_CloseAudio();	// SDL_Mixerクローズ
 	SDL_Quit();		// SDL使用終了

        //init();	//ゲーム開始 initは画面表示の切り替えのために使用
	
        //while(endKeys()!=0)
        //{
        //    if(Loop_Result()) 
        //        break;
        //}			//ゲーム結果画面
	Finalize();
	
	return 0;
}

