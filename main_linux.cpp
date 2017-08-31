// ===================================================================
/*
 *  main.cpp
 *  PIGE
 *
 *  Created by admin on Thu Oct 18 2001.
 *  Copyright (c) 2001 Edenwaith. All rights reserved.
 *
 */
// ===================================================================

// include files
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>

// Mac OS X header files
// #include <GLUT/glut.h>		// Mac OS X specification

// Linux header files
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string.h>

#include "LoadTGA.h"
//#include "TGALoader.h"

// evil global variables -----------------------------------------
TGAImageRec textures[2]; 	// for the cursor icon
unsigned long 	texture[2];	// for the cursor icon (newer)

enum cursor_types {DO, LOOK, TALK, INVENTORY};
enum cursor_types cursor_type = LOOK;

int		light1_on	= 0; // 0 is on, 1 is off
int		window;
int		mouse_entry	= 1;
int		kWindowWidth	= 1024;
int		kWindowHeight	= 768;
static int 	mouse_x 	= kWindowWidth/2;
static int	mouse_y 	= kWindowHeight/2;
int		fullscreen	= 0;
float 		cursor_x 	= 0.0;
float		cursor_y 	= 0.0;
float		cursor_z	= 0.0;
float		old_x 		= kWindowWidth/2;
float		old_y		= kWindowHeight/2;
float		mouse_sensitivity= 0.0075;
float 		cursor_size 	= 0.25;

float		x_camera	= 0.0;
float		y_camera	= 0.0;
float		z_camera	= -5.0;

int		x_angle		= 0;
int		y_angle		= 0;
int		z_angle		= 0;

GLuint          t_texture[2]; // Linux fix to load in textures

float		l1z_pos		= 2.0;

GLfloat 	LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat 	LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat		LightPosition[] = { 0.0f, 0.0f, l1z_pos, 1.0f };


//#define kWindowWidth	800
//#define kWindowHeight	600

// prototypes -----------------------------------------
void LoadGLTextures();
// drawing functions
void draw_cursor(int, int);
void squares(float x, float y, float z, float red);
void draw_lines(float, float, float, float);
void cube(void);
// display & initialize
void display();
void InitGL();
void reshape(int w, int h);
void Idle();
// input functions
void processMouse(int button, int state, int x, int y) ;
void PassiveMouseFunc(int, int);
void MovingMouseFunc(int x, int y);
void EntryFunc(int state);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);

// ===================================================================
// int main(int, char *[])
// ===================================================================
int main(int argc, char *argv[])
{

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(kWindowWidth, kWindowHeight);
  window = glutCreateWindow("Psychomancer");

  // have these two lines before InitGL, or textures won't load
  // glutGameModeString("1024x768:24@72"); // full screen game mode
  glutGameModeString("1024x768");
  glutEnterGameMode(); 
  glutSetCursor(GLUT_CURSOR_NONE); // make the default cursor disappear

  InitGL();
  
  // Enter to game mode: 800x600, 32 bit color, 72 Hz refresh rate
  //glutGameModeString("800x600:32@72"); // For full screen game mode
  //glutGameModeString("1024x768:24@72"); // full screen game mode
  //glutEnterGameMode(); 


  glutReshapeFunc(reshape);
  glutIdleFunc(Idle);
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutPassiveMotionFunc(PassiveMouseFunc);
  glutMotionFunc(MovingMouseFunc);  
  glutEntryFunc(EntryFunc);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glClearColor(0, 0, 0, 0);
  glutMainLoop();

  return 0;

}

// ===================================================================
// void LoadTextures()
// May want to move most of this code into the LoadTGA function
// Refer to Lesson25.cpp for example
// =================================================================== 
void LoadGLTextures()
{
  //  GLint type = GL_RGBA; // other enum types
    GLenum type = GL_RGBA; // Linux needs GLuenum type
    
    TGAImageRec *texture0, *texture1;
    // textures[0] = LoadTGA("LOOK_cursor_alpha.tga");

    // texture1 = LoadTGA("LOOK_cursor_alpha.tga");
    texture0 = LoadTGA("crate.tga");
    texture1 = LoadTGA("eye_alpha.tga");

    glGenTextures(1, &t_texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
    if ( texture0->bpp == 24 )
	type = GL_RGB;
    else
	type = GL_RGBA;
			
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture0->sizeX, texture0->sizeY, 0, type, GL_UNSIGNED_BYTE, texture0->data);
    
    glGenTextures(1, &t_texture[1]);
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
    if ( texture0->bpp == 24 )
	type = GL_RGB;
    else
	type = GL_RGBA;
			
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture1->sizeX, texture1->sizeY, 0, type, GL_UNSIGNED_BYTE, texture1->data);
    
}

// ===================================================================
// void draw_cursor(int x, int y)
// =================================================================== 
void draw_cursor(int x, int y)
{
    if (cursor_type == DO)
    {
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }
    else if (cursor_type == LOOK)
    {
        glBindTexture(GL_TEXTURE_2D, texture[1]); 
    }
    else if (cursor_type == TALK)
    {
        glBindTexture(GL_TEXTURE_2D, texture[1]); // 2
    }
    else // INVENTORY
    {
        glBindTexture(GL_TEXTURE_2D, texture[1]); // 3
    }

    glPushMatrix();

    glTranslatef(0.0f,0.0f,-5.0f);

    glRotatef(cursor_x,1.0f,0.0f,0.0f);				// Rotate On The X Axis
    glRotatef(cursor_y,0.0f,1.0f,0.0f);				// Rotate On The Y Axis
    glRotatef(cursor_z,0.0f,0.0f,1.0f);				// Rotate On The Z Axis
        
    
    glBegin (GL_QUADS);

        glColor4f(1.0, 1.0, 1.0, 0.5);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(cursor_x, cursor_y, 0.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(cursor_x + cursor_size, cursor_y, 0.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cursor_x + cursor_size, cursor_y + cursor_size, 0.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(cursor_x, cursor_y + cursor_size, 0.0);
        
    glEnd();

    glPopMatrix();
    
    
    glutPostRedisplay();
}

// ===================================================================
// void squares(float x, float y, float z, float red)
// =================================================================== 
void squares(float x, float y, float z, float red)
{
    glPushMatrix();
        glTranslatef(0.0f,0.0f,-5.0f);
	glBegin (GL_QUADS);
	glColor4f(red, 0.0, 0.0, 0.5);
	glVertex3f(5.0 + x, 5.0 + y, z);
	glColor4f(1.0, 0.0, 0.0, 0.5);
	glVertex3f(10.0 + x, 5.0 + y, z);
	glColor4f(1.0, 0.0, 0.0, 0.5);
	glVertex3f(10.0 + x, 10.0 + y, z);
	glVertex3f(5.0 + x, 10.0 + y, z);
	glEnd();
    glPopMatrix();
}

// ===================================================================
// void draw_lines(float x1, float y1, float x2, float y2)
// =================================================================== 
void draw_lines(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

// ===================================================================
// void cube(void)
// =================================================================== 
void cube(void)
{
	squares(2.5, 2.5, 0.0, .5);
	squares(0.0, 0.0, 0.0, 1);

	glColor3f(0.0, 0.5, 0.5);
	draw_lines(5.0, 5.0, 7.5, 7.5);
	draw_lines(5.0, 10.0, 7.5, 12.5);
	draw_lines(10.0, 10.0, 12.5, 12.5);
	draw_lines(10.0, 5.0, 12.5, 7.5);
}

// ===================================================================
// void display()
// Call non-transparent items first, then set GL_BLEND, then call
// the transparent/translucent items
// =================================================================== 
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer

    glLoadIdentity();

    // glutSolidTeapot(2.0);

    // cube();

    glPushMatrix();
        glTranslatef(x_camera, y_camera, z_camera);
        glRotatef((float)x_angle, 1.0, 0.0, 0.0);
        glRotatef((float)y_angle, 0.0, 1.0, 0.0);
        glRotatef((float)z_angle, 0.0, 0.0, 1.0);
        
        //glColor3f(1.0, 0.5, 0.5);
        glutSolidTeapot(1.5);
    glPopMatrix();  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    
      draw_cursor(mouse_x, mouse_y);
//    cube();
    glDepthMask( GL_TRUE );
    glDisable(GL_BLEND);
     
    glutSwapBuffers();
    glFlush();
}

// ===================================================================
// void InitGL()
// =================================================================== 
void InitGL()
{
	
	LoadGLTextures();
        
	glEnable(GL_TEXTURE_2D);
//        glEnable(GL_BLEND);
//        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	glClearDepth(1.0);	
	glDepthFunc(GL_LESS);	
	glEnable(GL_DEPTH_TEST);					// Enables Depth Testing 
	glShadeModel(GL_SMOOTH);	

        glViewport(0, 0, kWindowWidth, kWindowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();	

	gluPerspective(45.0f, (GLfloat) kWindowWidth / (GLfloat) kWindowHeight, 0.1f, 100.0f);	
												// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);
        
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	
	glEnable(GL_LIGHT1);

}

// ===================================================================
// void reshape(int w, int h)
// =================================================================== 
void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();

/*
   if (w <= h)
      gluOrtho2D (0.0, 30.0, 0.0, 30.0 * (GLfloat) h/(GLfloat) w);
   else
      gluOrtho2D (0.0, 30.0 * (GLfloat) w/(GLfloat) h, 0.0, 30.0);
*/

   gluPerspective(45.0, (GLfloat) w / (GLfloat) h, 0.1, 100.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   

}

// ===================================================================
// void Idle()
// =================================================================== 
void Idle()
{
	
	glutPostRedisplay();
}

// ===================================================================
// void processMouse(int button, int state, int x, int y)
// =================================================================== 
void processMouse(int button, int state, int x, int y) 
{

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) 
    {
        cout << "Left button" << endl;
        cout << "x: " << x << " y: " << y << endl;
        glutPostRedisplay();
    }
    else if (button == GLUT_MIDDLE_BUTTON) 
    {
        cout << "Middle button" << endl;
    }
    else if (button == GLUT_RIGHT_BUTTON) 
    {
        if (state == GLUT_DOWN)
        {
            switch (cursor_type)
            {
                case DO: 	cursor_type = LOOK; 	 break;
                case LOOK: 	cursor_type = TALK; 	 break;
                case TALK: 	cursor_type = INVENTORY; break;
                case INVENTORY:	cursor_type = DO; 	 break;
                default:	cursor_type = DO;
            }

            cout << "Cursor_type: " << cursor_type << endl;
            glutPostRedisplay();
        }
    }
}

// ===================================================================
// void PassiveMouseFunc(int x, int y)
// Find out where the mouse cursor is when buttons aren't pressed
// =================================================================== 
void PassiveMouseFunc(int x, int y)
{
    // global information to know where the cursor is
    mouse_x = x;
    mouse_y = y;
    
    // Between each 'move', need to calculate how far the cursor has moved from the
    // previous position to the new position, and then calculate how far the cursor
    // icon is to be moved.
    if(mouse_entry == 1) // if cursor is in game screen
    {
    //cursor_x += (float)(glutGet(GLUT_WINDOW_WIDTH) - x)/ 25.06;

        cursor_x +=  mouse_sensitivity * (x - old_x);
        cursor_y += -mouse_sensitivity * (y - old_y);
    
        old_x = x;
        old_y = y;

    } 


}

// ===================================================================
// void MovingeMouseFunc(int x, int y)
// Find out where the mouse cursor is when buttons aren't pressed
// Specifies the function, func, that's called when the mouse pointer 
// moves within the window while one or more mouse buttons is pressed.
// The x and y callback parameters indicate the location (in 
// window-relative coordinates) of the mouse when the event occurred.
// ===================================================================
void MovingMouseFunc(int x, int y)
{
}

// ===================================================================
// void EntryFunc(int x, int y)
// 
// ===================================================================
void EntryFunc(int state)
{
    if (state == GLUT_LEFT)
    {
        mouse_entry = 0;
        cout << "Mouse left: " << mouse_entry << endl;        
    }
    else // if (state == GLUT_ENTERED)
    {
        mouse_entry = 1;
        cout << "Mouse entered: " << mouse_entry << endl;
    }
}
// ===================================================================
// void keyboard(unsigned char key, int x, int y)
// =================================================================== 
void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         glutLeaveGameMode();
         glutDestroyWindow(window);
         exit(0);
         break;
      case 'l':
      case 'L': 
                if (light1_on == 0)
                {
                    light1_on = 1;
                    glDisable(GL_LIGHTING);
                }
                else
                {
                    light1_on = 0;
                    glEnable(GL_LIGHTING);
                }
                break;
      case 'q':
      case 'Q': glutDestroyWindow(window);
                glutLeaveGameMode(); exit(0); break;
		 
   }
}

// ===================================================================
// void specialKeys(int key, int x, int y)
// =================================================================== 
void specialKeys(int key, int x, int y)
{
    switch(key)
    {
        case GLUT_KEY_F1:
            fullscreen = !fullscreen;
            if (fullscreen) 
            {
                //Xpos = glutGet((GLenum)GLUT_WINDOW_X);    /* Save parameters */
                //Ypos = glutGet((GLenum)GLUT_WINDOW_Y);
                //Xsize = glutGet((GLenum)GLUT_WINDOW_WIDTH);
                //Ysize = glutGet((GLenum)GLUT_WINDOW_HEIGHT);
                // glutFullScreen();                /* Go to full screen */
            } 
            else 
            {
                //glutReshapeWindow(Xsize, Ysize);        /* Restore us */
                //glutPositionWindow(Xpos,Ypos);
            }
            
            break;

	case GLUT_KEY_RIGHT :
		//c_hor = c_hor + 0.05;
                y_angle = (y_angle + 5) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT :
		//c_hor = c_hor - 0.05;
                y_angle = (y_angle - 5) % 360;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP :
                z_camera += 0.05; 
                glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN :
                z_camera -= 0.05;
                glutPostRedisplay();
		break;
	case GLUT_KEY_HOME :
		//zoomer = zoomer - 0.05;
                l1z_pos -= 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_END :
		//zoomer = zoomer + 0.05;
                l1z_pos += 0.05;
		glutPostRedisplay();
		break;
	default:
		break;
    }
}


