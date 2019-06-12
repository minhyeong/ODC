#include "common.h"

/*
オブジェクト関連の初期化
*/
void init_object(Object *object){
	static int i;
	double r[3]={0,0,0};
	for(i=0; i<100; i++){
		object[i].type = 1;
		object[i].status = 1;
		//受け取り debris[3]={x,y,z}
		receive_debris(r,sizeof(double));
		printf("(%4f,%4f,%4f)\n",r[0],r[1],r[2]);
		object[i].x = r[0];
		object[i].y = r[1];
		object[i].z = r[2];
		printf("x=%f\n",object[i].x);
	}
	
}



/*
ウィンドウ関連の初期化
*/
void init_window(SDL_Surface* window)
{
    /* SDLを初期化する */
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    printf("failed to initialize SDL.\n");
    exit(-1);
  }

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);	// 赤色の確保するサイズ
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);	// 緑色の確保するサイズ
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);	// 青色の確保するサイズ
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);	// 深度の確保するサイズ
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);	// ダブルバッファを使用する
  
  /* 画面のタイトルを変更する */
  SDL_WM_SetCaption("My SDL Sample Game", NULL);
  /* 画面を初期化する */
  if((window = SDL_SetVideoMode(800, 450, 32, SDL_OPENGL|SDL_RESIZABLE)) == NULL) {
    printf("failed to initialize videomode.\n");
    exit(-1);
  }
  
  
  /* TODO: ここで画像や音声を読み込む */
 
}
/*
機体の座標の初期化
*/
void init_Airframe(Airframe data){
	data.x = 0;
	data.y = 0;
	data.z = 0;
	data.angle_a =0;
	data.angle_e =0;
}


/*
ジョイスティック関連の初期化
*/
void init_joystick(SDL_Joystick *joystick)
{
  static int i;
  
	// SDL初期化（ビデオとジョイスティック）
	if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0) {
		printf("failed to initialize SDL.\n");
		exit(-1);
	}
	
  	// 接続されているジョイスティックの名前を表示
  	for(i=0;i<SDL_NumJoysticks();i++){
    		printf("%s\n", SDL_JoystickName(i));
  	}
  	joystick=SDL_JoystickOpen(0);	// ジョイスティックを開いて構造体に割り当てる（有効にする）．ここではジョイスティックは1つとして引数に0を指定
  	SDL_JoystickEventState(SDL_ENABLE);	// ジョイスティック関連のイベントを取得可能にする
  
  	// ジョイスティックが開けなかったら
  	if(!joystick) {
    		printf("failed to open joystick.\n");
    		exit(-1);
  	}
  	else{
    		printf("The found joystick ID (index) is %d.\n", SDL_JoystickIndex(joystick));	// 接続されたジョイスティックのID番号（1つしか接続されていない場合は0
    		printf("The found joystick has %d axses.\n",SDL_JoystickNumAxes(joystick));	// ジョイスティックの方向キー数を取得
    		printf("The found joystick has %d Hat keys.\n",SDL_JoystickNumHats(joystick));	// ジョイスティックのHatキー数を取得（電算室のジョイスティックでは0?）
    		printf("The found joystick has %d balls.\n",SDL_JoystickNumBalls(joystick));	// ジョイスティックのボール数を取得（電算室のジョイスティックでは0?） */
    		printf("The found joystick has %d buttons.\n",SDL_JoystickNumButtons(joystick));	// ジョイスティックのボタン数を取得
  }
}
