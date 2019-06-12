// gcc -o a main.cpp -lSDL -L/usr/lib -lGL -lSDL_image -lSDL_gfx -lGLU

#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"
#include "SDL/SDL_opengl.h"

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_mixer.lib")

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

SDL_Surface *mWIN; //メインウィンドウ
Uint32 cntA,cntB,now,lev,wit;//FPS用

const int WIW=480; //ウィンドウ-幅
const int WIH=320; //ウィンドウ-高さ
#define REC 64     //図形の大きさ
GLuint texA=0;//テクスチャ

int Pot(int inSize)
{
int outSize=1;
while(outSize<inSize) outSize<<=1;
return outSize;
}

void creatTex(char *file,GLuint *tex)
{
GLuint textures;
SDL_Surface *imgSrc[2];
SDL_Rect area;

//画像読み込み
imgSrc[0]=SDL_LoadBMP(file);
if(imgSrc[0]==NULL)SDL_Quit();//読み込まれなかったら終了

//imgSrc[0]を変換
imgSrc[1]=SDL_CreateRGBSurface
  (
  SDL_SWSURFACE,
  Pot(imgSrc[0]->w), Pot(imgSrc[0]->h),
  32,
  #if SDL_BYTEORDER==SDL_LIL_ENDIAN /* OpenGL RGBA masks */
    0x000000FF, 
    0x0000FF00, 
    0x00FF0000, 
    0xFF000000
  #else
    0xFF000000,
    0x00FF0000, 
    0x0000FF00, 
    0x000000FF
  #endif
  );
if(imgSrc[1]==NULL)SDL_Quit();

area.x=0;  area.y=0;
area.w=imgSrc[0]->w;
area.h=imgSrc[0]->h;

SDL_BlitSurface(imgSrc[0], &area, imgSrc[1], &area);

//テクスチャ作成
glGenTextures(1, &textures);
glBindTexture(GL_TEXTURE_2D, textures);
//拡大時
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//縮小時
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexImage2D
  (
  GL_TEXTURE_2D,//2Dテクスチャ
  0,
  GL_RGBA,//GL_RGBA
  imgSrc[1]->w, imgSrc[1]->h,//テクスチャ幅・高さ
  0,//ボーダー
  GL_RGBA,//pixelsの渡すデータ形式
  GL_UNSIGNED_BYTE,//pixelsのデータ型
  imgSrc[1]->pixels
  );
*tex=textures; if(*tex==0)SDL_Quit();

int num;//Surface解放
for(num=0; num<2; num++) SDL_FreeSurface(imgSrc[num]);
}
 
void modelD(GLdouble alp,GLint tex)
{
glBindTexture(GL_TEXTURE_2D, tex);
glColor4d(1, 1, 1, alp);
glBegin(GL_QUADS);
  glTexCoord2i(0, 0);  glVertex2i(-REC*0.5,-REC*0.5);
  glTexCoord2i(1, 0);  glVertex2i( REC*0.5,-REC*0.5);
  glTexCoord2i(1, 1);  glVertex2i( REC*0.5, REC*0.5);
  glTexCoord2i(0, 1);  glVertex2i(-REC*0.5, REC*0.5);
glEnd();
}

void modelD_test()
{
glLoadIdentity();
glEnable(GL_TEXTURE_2D);//テクスチャON

static GLdouble alp,vec;
if     ( (vec==0)&&(alp+=0.01)>1){alp=1; vec=1;}//透過計算
else if( (vec==1)&&(alp-=0.01)<0){alp=0; vec=0;}

glTranslated(WIW/2+96, WIH/2+96, 0.0);
modelD(alp,texA);//modelD

glDisable(GL_TEXTURE_2D);//テクスチャOFF
}

void inits()
{
if(SDL_Init(SDL_INIT_VIDEO)==-1) SDL_Quit();//初期化失敗で終了
//メインウィンドウ作成
SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
mWIN=SDL_SetVideoMode(WIW,WIH,32,SDL_OPENGL);
SDL_WM_SetCaption("OpenGL Test Window", NULL);//windowの名前

glClearColor( 0.0, 0.0, 0.0, 1.0 );//バックの色
glShadeModel(GL_FLAT);//シェーディングモード
glMatrixMode(GL_PROJECTION);
glOrtho(0.0, WIW, WIH, 0.0, 0.0, 1.0);//2D表示
glMatrixMode(GL_MODELVIEW);
//テクスチャ作成
creatTex("ya2.bmp", &texA);
}


int endKeys()
{
SDL_Event Qevents;
if(SDL_PollEvent(&Qevents))
switch(Qevents.type)
  {
  case SDL_QUIT:      //ウィンドウのボタン
    return 0;
  case SDL_KEYDOWN:   //escキー
    if(Qevents.key.keysym.sym==SDLK_ESCAPE)  return 0;
  }
return 1;
}


void back()
{
glClear(GL_COLOR_BUFFER_BIT);//バック地を黒で塗り潰し
glLoadIdentity();
//ウィンドウの中心線の描画
glBegin(GL_LINES);
glColor3d(0.0, 0.0, 1.0);
  glVertex2i(0,WIH/2);  glVertex2i(WIW,WIH/2);
  glVertex2i(WIW/2,0);  glVertex2i(WIW/2,WIH);
glEnd();
}

void modelDrow()
{
//ここに、描画したいモノを置く
modelD_test();
}

int main(){
inits();//初期化
while(endKeys()!=0)//メインループ
  {
  back();     //バック地＆中心線
  modelDrow();//色々描画
  
  //フレームレート調整
  now=SDL_GetTicks();
  wit=now-lev;
  if(wit<16)SDL_Delay((16-wit));
  lev=SDL_GetTicks();

  if(now-cntB>=5000)//FPS確認用
    {
    printf("%d\n",cntA/5);
    cntA=1;  cntB=now;
    }  else cntA+=1;

  SDL_GL_SwapBuffers();//画面更新
  }
SDL_Quit();
return 0;
}
