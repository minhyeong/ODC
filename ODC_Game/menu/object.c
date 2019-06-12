#include "common.h"
int number;
void draw_cube();
void draw_airframe();

//テスト用の立方体の辺
int edge[][2] = {
  { 0, 1 },
  { 1, 2 },
  { 2, 3 },
  { 3, 0 },
  { 4, 5 },
  { 5, 6 },
  { 6, 7 },
  { 7, 4 },
  { 0, 4 },
  { 1, 5 },
  { 2, 6 },
  { 3, 7 }
};

//テスト用の立方体の面
int face[][4] = {
  { 0, 1, 2, 3 },
  { 1, 5, 6, 2 },
  { 5, 4, 7, 6 },
  { 4, 0, 3, 7 },
  { 4, 5, 1, 0 },
  { 3, 2, 6, 7 }
};

//テスト用の立方体の頂点
GLdouble cube_vertex[][3] = {
  { 0.0, 0.0, 0.0 },
  { 1.0, 0.0, 0.0 },
  { 1.0, 0.0, 1.0 },
  { 0.0, 0.0, 1.0 },
  { 0.0, 1.0, 0.0 },
  { 1.0, 1.0, 0.0 },
  { 1.0, 1.0, 1.0 },
  { 0.0, 1.0, 1.0 }
};

//ワールド端描画用頂点
GLdouble end_vertex[][3] = {
    { 255.0, -256.0, -256.0 }, /* A */
    { -256.0, -256.0, -256.0 }, /* B */
    { -256.0, 255.0, -256.0 }, /* C */
    { 255.0, 255.0, -256.0 }, /* D */
    { 255.0, -256.0, 255.0 }, /* E */
    { -256.0, -256.0, 255.0 }, /* F */
    { -256.0, 255.0, 255.0 }, /* G */
    { 255.0, 255.0, 255.0 }  /* H */
};

//テスト用立方体の色
GLdouble color[][3] = {
  { 1.0, 0.0, 0.0 },
  { 0.0, 1.0, 0.0 },
  { 0.0, 0.0, 1.0 },
  { 1.0, 1.0, 0.0 },
  { 1.0, 0.0, 1.0 },
  { 0.0, 1.0, 1.0 }
};

//オブジェクト座標を受け取ってオブジェクトを描画
void draw_object(Object *debris)
{
  static int i;
  
  for(i = 0; i < Debri_MAX; i++){
    glPushMatrix();
    glDisable(GL_LIGHTING);
    glTranslated(debris[i].x, debris[i].y,debris[i].z);
    switch(debris[i].type){
    case 1:
     if(debris[i].status == 1){
        glColor3f(1.0, 0.0, 0.0);
        object1();
     }
     else if(debris[i].status == 2){
        glColor3f(0.0, 1.0, 0.0);
        object1();
     }
      break;
    case 2:
     if(debris[i].status == 1){
        glColor3f(0.0, 0.0, 1.0);
        object2();
     }
     else if(debris[i].status == 2){
        glColor3f(0.0, 1.0, 0.0);
        object2();
     }
      break;
    case 3:
     if(debris[i].status == 1){
        glColor3f(1.0, 1.0, 1.0);
        object3();
     }
     else if(debris[i].status == 2){
        glColor3f(0.0, 1.0, 0.0);
        object3();
     }
     break;
    case 4:
     if(debris[i].status == 1){
        glColor3f(1.0, 1.0, 1.0);
        object4();
     }
     else if(debris[i].status == 2){
        glColor3f(0.0, 1.0, 0.0);
        object4();
     }
      break;  
      
    }
    glPopMatrix();
  }
}

//機体座標を受け取って機体を描画
void draw_airframe(Airframe data)
{
  static int i;
  
  
  glPushMatrix();
  glColor3d(1.0, 1.0, 0.0);
  glTranslated(data.x, data.y, data.z);
  glRotated(data.angle_a * 180 / PI, 0.0, 1.0, 0.0);
  glRotated(-data.angle_e * 180 / PI, 1.0, 0.0, 0.0);
  draw_airframe1();
  glPopMatrix();
  
}

//テスト用立方体の描画
void draw_cube()
{
    int i,j;
  
  
  glBegin(GL_QUADS);
  for (j = 0; j < 6; ++j) {
        glColor3dv(color[j]);
    for (i = 0; i < 4; ++i) {
      glVertex3dv(cube_vertex[face[j][i]]);
    }
  }
  glEnd();

  
}

//ワールド限界の描画
void draw_end()
{
    int i,j;
    glBegin(GL_QUADS);
    for (j = 0; j < 6; ++j) {
        glColor3dv(color[j]);
        for (i = 0; i < 4; ++i) {
            glVertex3dv(end_vertex[face[j][i]]);
        }
    }
    glEnd();
}

//機体,オブジェクトの座標を受け取って比較,当たっているかどうかを確認
int hit(Airframe data, Object *debris, int num)
{
    GLdouble d;
    
    if(debris[num].status == 0){
        return 0;
    }

    d = sqrt(pow((data.x - debris[num].x), 2.0) + pow((data.y - debris[num].y), 2.0)+ pow((data.z - debris[num].z), 2.0));
    
    //printf("%.3f", d);

    if(fabs(d) < 1.5){
        return 1;
    }

    return 0;
}

//機体,オブジェクトの座標を受け取って機体前方の一定位置にオブジェクトがあれば破壊
int attack(Airframe data, Object *debris)
{
    static GLdouble d;
    static int i;
    static GLdouble x = 0,y=0,z=0;


    x = -1 * cos(data.angle_e) * sin (data.angle_a) + data.x;
    y = -1 * sin(data.angle_e) + data.y;
    z = -1 * cos(data.angle_e) * cos(data.angle_a) + data.z;
    //printf("%.3f, %.3f,%.3f\n", x, y, z);
    
    for(i = 0; i < Debri_MAX; i++){
        d = sqrt(pow((x - debris[i].x), 2.0) + pow((y - debris[i].y), 2.0)+ pow((z - debris[i].z), 2.0));
    
        //printf("%.3f", d);
        
        if(fabs(d) < 1.5 && debris[i].status != 0){
            debris[i].status = 0;
	    number = i;
	    printf("no = %d\n",number);
	    switch(debris[i].type){
            case 1:
                return 100;
                break;
	    case 2:
		return 200;
		break;
            }
            //printf("%d", debris[i].status);
        }
    }
	return 0;
}

//機体,オブジェクトの座標を受け取ってオブジェクトを破壊できるなら色を変える
void able(Airframe data, Object *debris)
{
    static GLdouble d;
    static int i;
    static GLdouble x = 0,y=0,z=0;

    x = -1 * cos(data.angle_e) * sin (data.angle_a) + data.x;
    y = -1 * sin(data.angle_e) + data.y;
    z = -1 * cos(data.angle_e) * cos(data.angle_a) + data.z;

    for(i = 0; i < Debri_MAX; i++){
        d = sqrt(pow((x - debris[i].x), 2.0) + pow((y - debris[i].y), 2.0)+ pow((z - debris[i].z), 2.0));
    
        //printf("%.3f", d);
        
        if(fabs(d) < 1.5 && debris[i].status == 1){
            debris[i].status = 2;
        }
        else if(fabs(d) >= 1.5 && debris[i].status == 2){
            debris[i].status = 1;
        }
        else if(debris[i].status == 0){
            debris[i].status = 0;
        }
    }
}

