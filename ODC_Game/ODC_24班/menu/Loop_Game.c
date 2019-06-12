#include "common.h"

Airframe myself;
Airframe past;
//double time;
Airframe another[4];

int debf=0;
int camera_flag = 0;

GLdouble ex = 0.0, ey = 0.0, ez = 10.0;
GLdouble ux = 0.0, uy = 1.0, uz = 0.0;
double r = 10.0;

int button = 0;
int a= 0;
int e = 0;

int number;//破壊番号
double move = 0;
int score_all[5];
Score score;
Object object[Debri_MAX];

/* 機体の描画 */
void display(Airframe data,int myident){
	static int i;	

  	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  	glLoadIdentity(); //行列リセット
  	gluLookAt(ex, ey, ez, data.x, data.y, data.z, ux, uy, uz); //視点の設定
  
    	/* TODO: ここに描画処理を書く */
	draw_end();

	draw_object(object);
	
	draw_airframe(data);//自機の描画
	for(i=0; i<4; i++)
	  {
              if(i != myident) draw_airframe(another[i]);//他機の描画
	  }

    	/* 画面を更新する */
  	SDL_GL_SwapBuffers();
	
}

/*
コールバック関数
主にジョイスティックからの入力のコールバックに使用
*/
void callbackfunc2(SDL_Event event, int myident)
{

  static int i;
  /* TODO: ここにゲームの状態を更新する処理を書く */
  
  past = myself;
  able(myself, object);

   if(a > 0){
	move = 1;
        myself.angle_a -= (1.0 / 180) * PI;
        if(myself.angle_a < 0){
	    myself.angle_a += 2 * PI;
        }
    }
    else if(a < 0){
	move = 1;
        myself.angle_a += (1.0 / 180) * PI;
        if(myself.angle_a > 2 * PI){
	    myself.angle_a -= 2 * PI;
        }
    }

    if(e > 0){
	move = 1;
        myself.angle_e += (1.0 / 180) * PI;
        if(myself.angle_e > 2 * PI){
	    myself.angle_e -= 2 * PI;
        }
    }
    else if(e < 0){
	move = 1;
        myself.angle_e -= (1.0 / 180) * PI;
        if(myself.angle_e < 0){
	    myself.angle_e += 2 * PI;
        }
    }

    if(button == 1){
	move = 1;
        if(myself.x <= 255.0 && myself.x >= -256.0){
            
            myself.x += -1 * cos(myself.angle_e) * sin (myself.angle_a);
            
        }
        else if(myself.x > 255.0){
            myself.x = 255.0;
        }
        else if(myself.x < -256.0){
            myself.x = -256.0;
        }

        if(myself.y <= 255.0 && myself.y >= -256.0){
            
            myself.y += -1 * sin(myself.angle_e);
            
        }
        else if(myself.y > 255.0){
            myself.y = 255.0;
        }
        else if(myself.y < -256.0){
            myself.y = -256.0;
        }

        if(myself.z <= 255.0 && myself.z >= -256.0){
            
            myself.z += -1 * cos(myself.angle_e) * cos(myself.angle_a);
            
        }    
        else if(myself.z > 255.0){
              myself.z = 255.0;
        }
        else if(myself.z < -256.0){
            myself.z = -256.0;
        }
        
    }

    for(i = 0; i < Debri_MAX; i++){
      if(hit(myself, object, i)){
        
	myself = past;
	//printf("\n");
      }
    }
    
  display(myself, myident);
}


/*
ゲーム関係のメインループ
デブリの破壊情報、スコアの更新などゲームの主軸
*/
int Loop_Game(int myident){
	//time = 7000;
	Airframe data;
	//サーバからデータ受信
	//他のキャラの座標
	
  	SDL_Event event;
  	double next_frame = SDL_GetTicks();
  	double wait = 1000.0 / 60;
	int camera_flag = 0;
	static int i;
	int debris_data= 0;//debri no.取得し消す
	int no=-1;//破壊番号
        double time;
	number=-1;//破壊番号
	
   
	glViewport(0, 0, 800, 450); //ビューポートの設定 (x,y):左隅の座標(左下が原点)
	glMatrixMode(GL_PROJECTION); //行列モード切替
  	glLoadIdentity();
  	gluPerspective(30.0, (double)800 / (double)450, 1.0, 256.0); //視野の設定
  	glMatrixMode(GL_MODELVIEW); //行列モードを戻す   
  
	for (;;) {
        while(SDL_PollEvent(&event)){
            
		if(event.type == SDL_JOYAXISMOTION){
                	if(event.jaxis.axis==0){
                    		a = event.jaxis.value;
                	}
                	if(event.jaxis.axis==1)
                	    	e = event.jaxis.value;
            		}
	    		if(event.type == SDL_JOYBUTTONDOWN){
	      			if(event.jbutton.button==11){
					printf("---------------\n");
					printf("ex:%.3f\ney:%.3f\nez:%.3f\n\n",ex, ey, ez);
					printf("posx:%.3f\nposy:%.3f\nposz:%.3f\n\n", myself.x, myself.y, myself.z);
					printf("uy:%.3f\n\n",uy);
					printf("angle_w:%.3f\nangle_h:%.3f\n",myself.angle_a / PI * 180, myself.angle_e / PI * 180);
					printf("---------------\n\n");
        			}
				if(event.jbutton.button==7){
                  			button = 1;
              			}
                                if(event.jbutton.button==5){
                                    score_all[myident] += attack(myself, object);
                                    score_all[myident] /= 100;
                  		    score_all[myident] *= 100;
                  		    printf("score = %d\n", score_all[myident]);
				    printf("nunmber = %d\n",number);
                                }
			}
	    		if(event.type == SDL_JOYBUTTONUP){
	      			if(event.jbutton.button==7){
					button = 0;
	      			}
            		}
            		if(event.type == SDL_QUIT){
                		SDL_Quit();
                		exit(0);
                		break;
            		}
        	}

		myself.id = myident;
		data = control_requests_client(myself,move,number);
		if(data.no != -1){
			object[data.no].status = 0;
		}
		data.no = -1;//破壊デブリ送信
		number = -1;//破壊デブリ送信
		/*if(data.id==-10){
			control_result2();
			Loop_Result();
		}*/
		//debris_data = debris_cont();
		
		move = 0;
                another[(int)data.id] = data;

        	if (SDL_GetTicks() >= next_frame) {	
			time = (double)(120000 - SDL_GetTicks()) / 1000;
		
                        if(time > 120){
                		time = 0;
				//control_result();
				control_requests_client(data,-10, 0);
				Loop_Result();
            		}
            		if(fmod(time, 1) < 0.0153 && time <=70 && time >= 0){
                		printf("Time = %.0f\n", time);
				
            		}
	  		callbackfunc2(event, myident);
	  		next_frame += 1000 / 60;	// 現在の経過時間に1000ms（1秒）をプラスして格納
        	}
		
        	if(fabs(myself.angle_e - PI / 2) < 0.001 || fabs(myself.angle_e - PI * 3 / 2) < 0.001){
            		if(camera_flag == 0){
                		uy *= -1;
                		camera_flag = 1;
            		}
            		else {
                			uy *= -1;
                			camera_flag = 0;
            		}
        	}
        ex = r * cos(myself.angle_e) * sin (myself.angle_a) + myself.x;
        ey = r * sin(myself.angle_e) + myself.y;
        ez = r * cos(myself.angle_e) * cos(myself.angle_a) + myself.z;
    	}
	
}

