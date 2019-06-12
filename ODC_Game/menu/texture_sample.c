#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <GL/glu.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_gfxPrimitives.h>

#define image_num 1
#define text_num 5



char *image_name[image_num] = {"./sozai/result.bmp"};
int score_all[5];
// 1 2 3 4 総合 ｽｺｱ
char text_name[text_num][7];

SDL_Surface *window, *texture_surface[image_num], *texture_surface2[image_num], *text_surface[text_num], *text_surface2[text_num]; // サーフェイスへのポインタ
GLuint texture = 0;	// テクスチャの管理番号を格納する変数（int型に相当）
GLenum texture_format;	// テクスチャのフォーマット（番号）を格納する変数（unsigned int型に相当）
TTF_Font* font;

//テクスチャ用のサーフェイスを生成
void create_surface(int surface_num)
{
  switch(texture_surface[surface_num]->format->BytesPerPixel){
	case 3:	// 24bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){	// ビッグエンディアン（連続したデータを桁の大きい方からByteごとに格納）なら
			texture_format = GL_BGR;
		}
		else{
			texture_format = GL_RGB;
		}
		break;
	case 4:	// 32bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
			texture_format = GL_BGRA;
		}
		else{
			texture_format = GL_RGBA;
		}
		break;
	}
	
	// RGBAサーフェイスを生成、テクスチャを変換（テクスチャを扱うために必要です）
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
	    	texture_surface2[surface_num] = SDL_CreateRGBSurface(SDL_SWSURFACE, texture_surface[surface_num]->w, texture_surface[surface_num]->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);	// 空のサーフェイスを生成
	}
	else{
	    	texture_surface2[surface_num] = SDL_CreateRGBSurface(SDL_SWSURFACE, texture_surface[surface_num]->w, texture_surface[surface_num]->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	}
	// サーフェイス生成に失敗した場合
	if (!texture_surface2[surface_num]) {
		printf("failed to convert to RGBA format: %s\n", SDL_GetError());
		exit(-1);
	}		
	else{
		SDL_BlitSurface(texture_surface[surface_num], NULL, texture_surface2[surface_num], NULL);	// テクスチャを読み込んだサーフェイスを新たに作成したRGBAサーフェイスに貼り付け
	}
}

void create_surface2(int surface_num)
{
  switch(text_surface[surface_num]->format->BytesPerPixel){
	case 3:	// 24bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){	// ビッグエンディアン（連続したデータを桁の大きい方からByteごとに格納）なら
			texture_format = GL_BGR;
		}
		else{
			texture_format = GL_RGB;
		}
		break;
	case 4:	// 32bit per Pixel
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
			texture_format = GL_BGRA;
		}
		else{
			texture_format = GL_RGBA;
		}
		break;
	}
	
	// RGBAサーフェイスを生成、テクスチャを変換（テクスチャを扱うために必要です）
	if(SDL_BYTEORDER == SDL_BIG_ENDIAN){
	    	text_surface2[surface_num] = SDL_CreateRGBSurface(SDL_SWSURFACE, text_surface[surface_num]->w, text_surface[surface_num]->h, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);	// 空のサーフェイスを生成
	}
	else{
	    	text_surface2[surface_num] = SDL_CreateRGBSurface(SDL_SWSURFACE, text_surface[surface_num]->w, text_surface[surface_num]->h, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	}
	// サーフェイス生成に失敗した場合
	if (!text_surface2[surface_num]) {
		printf("failed to convert to RGBA format: %s\n", SDL_GetError());
		exit(-1);
	}		
	else{
		SDL_BlitSurface(text_surface[surface_num], NULL, text_surface2[surface_num], NULL);	// テクスチャを読み込んだサーフェイスを新たに作成したRGBAサーフェイスに貼り付け
	}
}

void create_texture(int textuer_num)
{
    // テクスチャを作る
    glEnable(GL_TEXTURE_2D);	// テクスチャを有効にする
    glGenTextures(1, &texture);	// ある１つのテクスチャ名（整数値）を生成
    glBindTexture(GL_TEXTURE_2D, texture);	// 指定したテクスチャの実体（2次元テクスチャ）を作成
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// テキスチャのパラメータを指定
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

// 平面を描画する関数
void draw_background(){
    glClearColor(1.0, 1.0, 1.0, 1.0);	// 塗り潰し色を白色に設定
    glClear(GL_COLOR_BUFFER_BIT); // 塗りつぶし
    
    glColor4d(1.0, 1.0, 1.0, 0.0); // 各面の頂点に色を設定
    
    glBegin(GL_QUADS);	// 四角形の開始
    // 以下の四角形に（管理番号が指す）テクスチャを描画する		
    glTexCoord2f(0.0f, 0.0f);	// 画像の左上と次の頂点を対応させる
    glVertex3f(-2.0f, 1.08f, 0.0f);
    glTexCoord2f(0.0f, 1.0f);	// 画像の左下と次の頂点を対応させる
    glVertex3f(-2.0f, -1.08f, 0.0f);
    glTexCoord2f(1.0f, 1.0f);	// 画像の右下と次の頂点を対応させる
    glVertex3f(2.0f, -1.08f, 0.0f);
    glTexCoord2f(1.0f, 0.0f);	// 画像の右上と次の頂点を対応させる
    glVertex3f(2.0f, 1.08f, 0.0f);
    glEnd(); // 図形終了
    SDL_GL_SwapBuffers(); // 描画を反映する
}

void draw_string(int textuer_num){
    
    GLfloat p = 0;


    p = -0.20 * textuer_num;

    if(textuer_num == 4){
        p = -1.05;
    }

    glColor3d(1.0, 1.0, 1.0); // 各面の頂点に色を設定
    
    glBegin(GL_QUADS);	// 四角形の開始
    // 以下の四角形に（管理番号が指す）テクスチャを描画する		
    glTexCoord2f(0.0f, 0.0f);	// 画像の左上と次の頂点を対応させる
    glVertex3f(1.0f, 0.44+p, 0.0f);
    glTexCoord2f(0.0f, 1.0f);	// 画像の左下と次の頂点を対応させる
    glVertex3f(1.0f, 0.26+p, 0.0f);
    glTexCoord2f(1.0f, 1.0f);	// 画像の右下と次の頂点を対応させる
    glVertex3f(1.4f, 0.26+p, 0.0f);
    glTexCoord2f(1.0f, 0.0f);	// 画像の右上と次の頂点を対応させる
    glVertex3f(1.4f, 0.44+p, 0.0f);
    glEnd(); // 図形終了
    SDL_GL_SwapBuffers(); // 描画を反映する
}

// メイン関数
int Loop_Result() {
    
    
    SDL_Event event; // SDLによるイベントを検知するための構造体
    SDL_Color white = {0xFF, 0xFF, 0xFF};

    int i,j,num;
   
   

    //glViewport(0, 0, 800, 450); //ビューポートの設定 (x,y):左隅の座標(左下が原点)

    glLoadIdentity();
    //gluPerspective(30.0, (double)800 / (double)450, 1.0, 100.0);
    gluLookAt(0.0, 0.0, 4.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    //glOrtho(-1, 1, -1, 1, -1.0, 1.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glDisable(GL_DEPTH_TEST);
    
    //glClear(GL_COLOR_BUFFER_BIT);

    for(i = 0; i < 4; i++){
        score_all[4] += score_all[i];
    }

    for(i = 0; i < text_num; i++){	
	sprintf(text_name[i], "%06d", score_all[i]);
        puts(text_name[i]);
    }

    TTF_Init();	
    font = TTF_OpenFont("./sozai/fonts-japanese-gothic.ttf", 24);

    // テクスチャ画像を読み込む
    for(i = 0; i < image_num; i++){
        texture_surface[i] = SDL_LoadBMP(image_name[i]);		// テクスチャ画像の読み込み（2のn乗×2のn乗ピクセルの画像を指定）
        
	// 画像の読み込みに失敗した場合
        if (!texture_surface[i]){
            printf("failed to load the image file %d.\n", i);
            exit(-1);
        }
    }

    for(i = 0; i < text_num; i++){
        text_surface[i] = TTF_RenderUTF8_Blended(font, text_name[i], white);
        //text_surface[i] = TTF_RenderUTF8_Blended(font, text_name[i], white);		// 文字列の読み込み（2のn乗×2のn乗ピクセルの画像を指定）
        
	// 画像の読み込みに失敗した場合
        if (!text_surface[i]){
            printf("failed to load text %d.\n", i);
            exit(-1);
        }
    }

    
    for(i = 0; i < image_num; i++){
        create_surface(i);
        create_texture(i);
        SDL_FreeSurface(texture_surface[i]);	// サーフェイスの解放
    }

    for(i = 0; i < text_num; i++){
        create_surface2(i);
        create_texture(i);
        SDL_FreeSurface(text_surface[i]);	// サーフェイスの解放
    }
    
    
    
    glTexImage2D(GL_TEXTURE_2D, 0, texture_surface2[0]->format->BytesPerPixel, texture_surface2[0]->w, texture_surface2[0]->h, 0, texture_format, GL_UNSIGNED_BYTE, texture_surface2[0]->pixels);	// 画像データとテクスチャを関連付ける
    
    draw_background();	// 平面描画


    for(i = 0; i < text_num; i++){
        glTexImage2D(GL_TEXTURE_2D, 0, text_surface2[i]->format->BytesPerPixel, text_surface2[i]->w, text_surface2[i]->h, 0, texture_format, GL_UNSIGNED_BYTE, text_surface2[i]->pixels);	// 画像データとテクスチャを関連付ける
        draw_string(i);
    }
    
    
    //draw_string();

    //draw_cube();
    //SDL_GL_SwapBuffers();
    
    while(1){
        if(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_KEYDOWN:	// キーボードのキーが押された時
				// 押されたキーごとに処理
                switch(event.key.keysym.sym){
                case SDLK_ESCAPE:	// Escキーが押された時
                    SDL_FreeSurface(texture_surface2[0]);	// サーフェイスの解放
                    glDeleteTextures(1, &texture);	// テクスチャを削除する
                    SDL_Quit();
                    exit(0);
                    break;
                case SDLK_1:
                    
                    break;
                case SDLK_2:
                   
                    break;
                case SDLK_3:
                   
                    break;
                }
                
                break;
                
            case SDL_QUIT:
                SDL_Quit();
                exit(0);
                break;
		
		
            }
        }
    }
    
    return 0;
}
