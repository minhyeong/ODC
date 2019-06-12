#include "common.h"

//テクスチャ
GLuint texA=0;// 0 タイトル
GLuint texB=0;// 1 簡易説明
//GLuint texC=0;// 2 装備選択
//GLuint texD=0;// 3 Result 個人
GLuint texE=0;// 4 Result 全員
//GLuint texF=0;// 5 やじるし
//GLuint texG=0;// 6 攻撃装備
//GLuint texH=0;// 7 収集装備
GLuint texI=0;// 8 攻撃装備 背景統合
GLuint texJ=0;// 9 収集装備 背景統合
GLuint texK=0;// 10 キー

SDL_Surface *mWIN; //メインウィンドウ

const int Wide=800; //ウィンドウ-幅
const int High=450; //ウィンドウ-高さ

//	modelD は 全画面用
//	modelE は 矢印の位置調整用

// arm 		(-456,-143)(-430,-143)(-430,-120)(-456,-120) <--方向から上から記述
// booster	(-456,-101)(-430,-101)(-430,-78)(-456,-78)
// capacity	(-456,-59)(-430,-59)(-430,-36)(-456,-36)
// weapon	(-456,-17)(-430,-17)(-430,6)(-456,6)

int cellect[2][8]={
	 {-456,-143,-430,-143,-430,-120,-456,-120}
	,{-456,-101,-430,-101,-430,-78,-456,-78}
};

int cellect2[2][8]={
	 {356,-143,-130,-143,-130,143,356,143}
	,{356,-143,-130,-143,-130,143,356,143}
};

//描画
void modelDrow(int changer){
	//ここに、描画したいモノを置く
	modelD_A(changer);
}

void modelDrow2(int col){
	//ここに描画したいモノを置く
	modelD_B(col);//訂正済素材描画
	//modelD_A(2);// 移動用上書き
	//modelE_A(col);//矢印
	//modelF_A(col);//機体
}

//テクスチャ生成用 2^n
int Pot(int inSize){
	int outSize=1;
	while(outSize<inSize) outSize<<=1;
	return outSize;
}

//テクスチャ貼り付け用図形
void modelD(GLdouble alp,GLint tex){
	glBindTexture(GL_TEXTURE_2D, tex);
	glColor4d(1, 1, 1, alp);
	glBegin(GL_QUADS);		// wide = 510 	high = 257
		glTexCoord2i(0, 0);  glVertex2i(-wide,-high);
		glTexCoord2i(1, 0);  glVertex2i( wide,-high);
		glTexCoord2i(1, 1);  glVertex2i( wide, high);
		glTexCoord2i(0, 1);  glVertex2i(-wide, high);
	glEnd();
}
/*
	0    0				800 0
	-510 -257			510 -257
			中心 400 225
			     0   0
	0 450				800 450
	-510 257			510 257
*/	

/*void modelE(GLdouble alp,GLint tex, int col){	// 横/縦 800/450
	glBindTexture(GL_TEXTURE_2D, tex);	// -510~x~510 -257~y~257
	glColor4d(1, 1, 1, alp);		// W/w=1.275 H/h=1.142222 W:Hの移動pxにかければおｋ
	glBegin(GL_QUADS);			
						// (26,23)間隔 下へ42p
						// arm 		(-456,-143)(-430,-143)(-430,-120)(-456,-120) <--方向から上から記述
						// booster	(-456,-101)(-430,-101)(-430,-78)(-456,-78)
						// capacity	(-456,-59)(-430,-59)(-430,-36)(-456,-36)
						// weapon	(-456,-17)(-430,-17)(-430,6)(-456,6)
						// cellect[4][8]に格納中 colでウェポンの位置を設定する
		glTexCoord2i(0, 0);  glVertex2i(cellect[col][0],cellect[col][1]);		//左上	? 23
		glTexCoord2i(1, 0);  glVertex2i(cellect[col][2],cellect[col][3]);		//右上
		glTexCoord2i(1, 1);  glVertex2i(cellect[col][4],cellect[col][5]);		//右下
		glTexCoord2i(0, 1);  glVertex2i(cellect[col][6],cellect[col][7]);		//左下
	glEnd();
}*/

/*void modelF(GLdouble alp,GLint tex, int col){	// 横/縦 800/450
	glBindTexture(GL_TEXTURE_2D, tex);	// -510~x~510 -257~y~257
	glColor4d(1, 1, 1, alp);		// W/w=1.275 H/h=1.142222 W:Hの移動pxにかければおｋ
	glBegin(GL_QUADS);			
		glTexCoord2i(0, 0);  glVertex2i(cellect2[col][0],cellect2[col][1]);		//左上	? 23
		glTexCoord2i(1, 0);  glVertex2i(cellect2[col][2],cellect2[col][3]);		//右上
		glTexCoord2i(1, 1);  glVertex2i(cellect2[col][4],cellect2[col][5]);		//右下
		glTexCoord2i(0, 1);  glVertex2i(cellect2[col][6],cellect2[col][7]);		//左下
	glEnd();
}*/

void modelD_A(int changers){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//テクスチャON
	static GLdouble alp,vec;

	glTranslated(wide, high, 0.0);
	switch(changers){
	case 1:
		modelD(alp,texA);
		break;
	case 2:
		modelD(alp,texB);
		break;
	case 0:
		modelD(alp,texK);
		break;
	/*case 2:
		modelD(alp,texC);
		break;
	case 3:
		modelD(alp,texD);
		break;
	case 4:
		modelD(alp,texE);
		break;*/
	}
	
	glDisable(GL_TEXTURE_2D);//テクスチャOFF
}

void modelD_B(int changers){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//テクスチャON
	static GLdouble alp,vec;

	glTranslated(wide, high, 0.0);
	switch(changers){
	case 0:
		modelD(alp,texI);
		break;
	case 1:
		modelD(alp,texJ);
		break;
	}
	
	glDisable(GL_TEXTURE_2D);//テクスチャOFF
}

/*void modelE_A(int col){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//テクスチャON
	static GLdouble alp,vec;

	glTranslated(wide, high, 0.0);
		modelE(alp,texF,col);
	glDisable(GL_TEXTURE_2D);//テクスチャOFF
}*/

/*void modelF_A(int col){
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//テクスチャON
	static GLdouble alp,vec;

	glTranslated(wide, high, 0.0);
		switch(col){
		case 0:
			modelF(alp,texG,col);
			break;
		case 1:
			modelF(alp,texH,col);
			break;
		}
	glDisable(GL_TEXTURE_2D);//テクスチャOFF
}*/

void creatTex(char *file,GLuint *tex){
	GLuint textures;
	SDL_Surface *imgSrc[2];
	SDL_Rect area;
	
	//画像読み込み
	imgSrc[0]=SDL_LoadBMP(file);
	if(imgSrc[0]==NULL) SDL_Quit();//読み込まれなかったら終了
	
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
	if(imgSrc[1]==NULL) SDL_Quit();
	
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

//初期化の関数	
void init(){
	//メインウィンドウ作成
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	mWIN=SDL_SetVideoMode(Wide,High,32,SDL_OPENGL);
	SDL_WM_SetCaption("OpneGL Mapping test", NULL);//windowの名前

        glClearColor(0.0, 0.0, 0.0,0.0);

	glMatrixMode(GL_PROJECTION);
	glOrtho(0.0, Wide, High, 0.0, 0.0, 1.0);//2D表示
	glMatrixMode(GL_MODELVIEW);
	//テクスチャ作成
	creatTex("./sozai/title.bmp", &texA);
	creatTex("./sozai/txt.bmp", &texB);
	//creatTex("./sozai/menu9.bmp", &texC);
	//creatTex("./sozai/result-1.bmp", &texD);
	creatTex("./sozai/result.bmp", &texE);
	//creatTex("./sozai/ya2.bmp",&texF);
	//creatTex("./sozai/odc1.bmp",&texG);
	//creatTex("./sozai/odc2.bmp",&texH);
	creatTex("./sozai/menu1.bmp",&texI);
	creatTex("./sozai/menu2.bmp",&texJ);
	creatTex("./sozai/key.bmp",&texK);
}












