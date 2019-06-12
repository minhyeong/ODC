/*  
    smooth.c
    Nate Robins, 1996

    Model viewer program.  Excercises the glm library.
 */

#include "../common.h"
#include <assert.h>
#include <stdarg.h>
#include "tb.h"

#include "glm.h"

GLuint     model_list = 0;		/* display list for object */
GLuint     model_list2 = 0;
GLuint     model_list3 = 0;
GLuint     model_list4 = 0;
GLuint     model_list5 = 0;
char*      model_file = NULL;		/* name of the obect file */
char*      model_file2 = NULL;
char*      model_file3 = NULL;
char*      model_file4 = NULL;
char*      model_file5 = NULL;
GLboolean  facet_normal = GL_FALSE;	/* draw with facet normal? */
GLMmodel*  model;
GLMmodel*  model2;
GLMmodel*  model3;
GLMmodel*  model4;
GLMmodel*  model5;
GLfloat    smoothing_angle = 90.0;	/* smoothing angle */
GLfloat    scale;			/* scaling factor */
GLfloat    scale2;
GLfloat    scale3;
GLfloat    scale4;
GLfloat    scale5;
GLdouble   pan_x = 0.0;
GLdouble   pan_y = 0.0;
GLdouble   pan_z = 0.0;
GLint      mouse_state = -1;
GLint      mouse_button = -1;
GLboolean  bounding_box = GL_FALSE;
GLboolean  performance = GL_FALSE;
GLboolean  stats = GL_FALSE;
GLfloat    weld_distance = 0.00001;
GLuint     material_mode = 0;


/* text: general purpose text routine.  draws a string according to
 * format in a stroke font at x, y after scaling it by the scale
 * specified (scale is in window-space (lower-left origin) pixels).  
 *
 * x      - position in x (in window-space)
 * y      - position in y (in window-space)
 * scale  - scale in pixels
 * format - as in printf()
 */
void 
text(GLuint x, GLuint y, GLfloat scale, char* format, ...)
{
  va_list args;
  char buffer[255], *p;
  GLfloat font_scale = 119.05 + 33.33;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));

  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  glPushAttrib(GL_ENABLE_BIT);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glDisable(GL_DEPTH_TEST);
  glTranslatef(x, y, 0.0);

  glScalef(scale/font_scale, scale/font_scale, scale/font_scale);

  for(p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  
  glPopAttrib();

  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}

void
lists(void)
{
  GLfloat ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  GLfloat diffuse[] = { 0.8, 0.8, 0.8, 1.0 };
  GLfloat specular[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat shininess = 65.0;

  if (model_list)
  {
    glDeleteLists(model_list, 1);
  }
  else if (model_list2)
  {
    glDeleteLists(model_list2, 1);
  }
  else if (model_list3)
  {
    glDeleteLists(model_list3, 1);
  }
  else if (model_list4)
  {
    glDeleteLists(model_list4, 1);
  }
  else if (model_list5)
  {
    glDeleteLists(model_list5, 1);
  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

  /* generate a list */
  if (material_mode == 0) { 
    if (facet_normal)
    {
      model_list = glmList(model, GLM_FLAT);
      model_list2 = glmList(model2, GLM_FLAT);
      model_list3 = glmList(model3, GLM_FLAT);
      model_list4 = glmList(model4, GLM_FLAT);
      model_list5 = glmList(model5, GLM_FLAT);
    }
    else
    {
      model_list = glmList(model, GLM_SMOOTH);
      model_list2 = glmList(model2, GLM_SMOOTH);
      model_list3 = glmList(model3, GLM_SMOOTH);
      model_list4 = glmList(model4, GLM_SMOOTH);
      model_list5 = glmList(model5, GLM_SMOOTH);
    }
  } 
  else if (material_mode == 1)
  {
    if (facet_normal)
    {
      model_list = glmList(model, GLM_FLAT | GLM_COLOR);
      model_list2 = glmList(model2, GLM_FLAT | GLM_COLOR);
      model_list3 = glmList(model3, GLM_FLAT | GLM_COLOR);
      model_list4 = glmList(model4, GLM_FLAT | GLM_COLOR);
      model_list5 = glmList(model5, GLM_FLAT | GLM_COLOR);
    }
    else
    {
      model_list = glmList(model, GLM_SMOOTH | GLM_COLOR);
      model_list2 = glmList(model2, GLM_SMOOTH | GLM_COLOR);
      model_list3 = glmList(model3, GLM_SMOOTH | GLM_COLOR);
      model_list4 = glmList(model4, GLM_SMOOTH | GLM_COLOR);
      model_list5 = glmList(model5, GLM_SMOOTH | GLM_COLOR);
    }
  }
  else if (material_mode == 2)
  {
    if (facet_normal)
    {
      model_list = glmList(model, GLM_FLAT | GLM_MATERIAL);
      model_list2 = glmList(model2, GLM_FLAT | GLM_MATERIAL);
      model_list3 = glmList(model3, GLM_FLAT | GLM_MATERIAL);
      model_list4 = glmList(model4, GLM_FLAT | GLM_MATERIAL);
      model_list5 = glmList(model5, GLM_FLAT | GLM_MATERIAL);
    }
    else
    {
      model_list = glmList(model, GLM_SMOOTH | GLM_MATERIAL);
      model_list2 = glmList(model2, GLM_SMOOTH | GLM_MATERIAL);
      model_list3 = glmList(model3, GLM_SMOOTH | GLM_MATERIAL);
      model_list4 = glmList(model4, GLM_SMOOTH | GLM_MATERIAL);
      model_list5 = glmList(model5, GLM_SMOOTH | GLM_MATERIAL);
    }
  }
}

void
init_model(void)
{
  model_file = "./smooth/cockpit2.obj";
  model_file2 = "./smooth/deburi.obj";
  model_file3 = "./smooth/deburi2.obj";
  model_file4 = "./smooth/cockpit2.obj";
  model_file5 = "./smooth/cockpit.obj";
  if (!model_file) {
    fprintf(stderr, "usage: smooth model_file.obj\n");
    exit(1);
  }
  if (!model_file2) {
    fprintf(stderr, "usage: smooth model_file2.obj\n");
    exit(1);
  }
  if (!model_file3) {
    fprintf(stderr, "usage: smooth model_file2.obj\n");
    exit(1);
  }
  if (!model_file4) {
    fprintf(stderr, "usage: smooth model_file2.obj\n");
    exit(1);
  }
  if (!model_file5) {
    fprintf(stderr, "usage: smooth model_file2.obj\n");
    exit(1);
  }

  tbInit(GLUT_MIDDLE_BUTTON);
  
  /* read in the model */
  model = glmReadOBJ(model_file); //機体
  scale = glmUnitize(model);
  glmFacetNormals(model);
  glmVertexNormals(model, smoothing_angle);

  model2 = glmReadOBJ(model_file2); //ココからデブリ
  scale2 = glmUnitize(model2);
  glmFacetNormals(model2);
  glmVertexNormals(model2, smoothing_angle);

  model3 = glmReadOBJ(model_file3);
  scale3 = glmUnitize(model3);
  glmFacetNormals(model3);
  glmVertexNormals(model3, smoothing_angle);

  model4 = glmReadOBJ(model_file4);
  scale4 = glmUnitize(model4);
  glmFacetNormals(model4);
  glmVertexNormals(model4, smoothing_angle);

  model5 = glmReadOBJ(model_file5);
  scale5 = glmUnitize(model5);
  glmFacetNormals(model5);
  glmVertexNormals(model5, smoothing_angle);

  if (model->nummaterials > 0)
      material_mode = 2;

  /* create new display lists */
  lists();

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
}

void draw_airframe1()
{   
    tbMatrix();
    
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glColor3f(0.5, 0.5, 0.5);
    glCallList(model_list);
    
    if (bounding_box) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0, 0.0, 0.0, 0.25);
        glutSolidCube(2.0);
    }
}

void object1()
{   
    tbMatrix();
    
    glEnable(GL_COLOR_MATERIAL);

    glCallList(model_list2);
    
    if (bounding_box) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0, 0.0, 0.0, 0.25);
        glutSolidCube(2.0);
    }
}

void object2()
{   
    tbMatrix();
    
    glEnable(GL_COLOR_MATERIAL);

    glCallList(model_list3);
    
    if (bounding_box) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0, 0.0, 0.0, 0.25);
        glutSolidCube(2.0);
    }
}

void object3()
{   
    tbMatrix();
    
    glEnable(GL_COLOR_MATERIAL);

    glCallList(model_list4);
    
    if (bounding_box) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0, 0.0, 0.0, 0.25);
        glutSolidCube(2.0);
    }
}

void object4()
{   
    tbMatrix();
    

    glEnable(GL_COLOR_MATERIAL);

    glCallList(model_list5);
    
    if (bounding_box) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_CULL_FACE);
        glColor4f(1.0, 0.0, 0.0, 0.25);
        glutSolidCube(2.0);
    }
}
