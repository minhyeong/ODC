#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

//#include "constants.h"
#include "GL/glu.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_mixer.h"

/*	
**	画面素材番号一覧
**	0 タイトル
**	1 簡易説明
**	2 装備選択
**	3 Result 個人
**	4 Result 全員
*/

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_mixer.lib")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

#define PI 3.141592
#define wide 510     	//図形の幅
#define high 257	//図形の高さ
#define MAX_LEN_BUFFER 256
#define Debri_MAX 1000		// 総デブリ数
#define Portnumber 46468	//ポート

typedef struct airframe{  	//転送に使用
	//int cid;
  	//char command;
  	//char message[MAX_LEN_BUFFER];
    	double x;  		//機体x座標
    	double y;  		//機体y座標
   	double z;  		//機体z座標
    	double angle_a;  	//機体方位角
    	double angle_e;  	//機体仰俯角
    	double id;		//ID 機体識別用
	int no;		//破壊情報[]
}Airframe;

typedef struct object{
  	int type;
  
  	GLdouble x;
  	GLdouble y;
  	GLdouble z;
  
  	int status;
}Object;

typedef struct score{
	int add[5];
}Score;

//int ident;

Airframe joystickmotion(SDL_Event event, Airframe data);
Airframe joystickbutton(SDL_Event event, Airframe data);

extern SDL_Surface* window;
extern Object object[Debri_MAX];
extern int number;
extern int score_all[5];

//window
int Pot(int inSize);
extern void modelD(GLdouble alp,GLint tex);
extern void modelE(GLdouble alp,GLint tex,int col);
extern void modelF(GLdouble alp,GLint tex,int col);
void modelD_A(int i);
void modelD_B(int i);
void modelE_A(int col);
void modelF_A(int col);
void modelDrow2(int col);

void init();
void creatTex(char *file,GLuint *tex);
void modelDrow(int changer);
void display(Airframe data,int myident);

//init
void init_window(SDL_Surface* window);
void init_Airframe(Airframe data);
void init_model();

void draw_object(Object *debris);
void draw_airframe(Airframe data);
void draw_airframe1();

void Finalize(void);
void init_window(SDL_Surface* window);
void init_joystick(SDL_Joystick *joystick);
void draw_cube();
void draw_end();

//game
int hit(Airframe pos, Object *debris, int i);
int attack(Airframe data, Object *debris);
void able(Airframe data, Object *debris);

//Loop
int Loop_Title(void);
int Loop_Game(int myident);
int Loop_Result();

//finalize
void Finalize(void);

//collect
int wepon_collect();

//client
int setup_client(char *server_name, u_short port);
Airframe control_requests_client(Airframe data,double move,int no);

//server
void setup_server(int num_cl, u_short port);

//debris
void init_object(Object *object);

//server init debris

//
int receive_debris(void *r,int size);


int control_server_result();
//CONTAINER execute_command()
