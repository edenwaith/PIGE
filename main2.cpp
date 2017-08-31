/*
 *  main.cpp
 *  PIGE
 *
 *  Created by admin on Thu Oct 18 2001.
 *  Copyright (c) 2001 Edenwaith. All rights reserved.
 *
 */

// include files
#include <iostream.h>
#include <GLUT/glut.h>
#include <stdio.h>
#include <stdlib.h>

// prototypes
void draw_lines(float, float, float, float);
void pyramid(float, float, float);
void PassiveMouseFunc(int, int);
void specialKeys(int key, int x, int y);

// global variables
enum cursor_types {DO, LOOK, TALK, INVENTORY};
enum cursor_types cursor_type = DO;

int frame = 0;
int time = 1;
int timebase = 0;
static int mouse_x = 0, mouse_y = 0;
float cursor_x = 0.0, cursor_y = 0.0;

void choice_selected(int value)
{
	if(value == 1) glEnable(GL_LIGHTING);
	if(value == 2) glDisable(GL_LIGHTING);
	if(value == 3) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if(value == 4) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if(value == 666) exit(0);
	glutPostRedisplay();
}

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
   glutCreateMenu(choice_selected);
   glutAddMenuEntry("Enable lighting", 1);
   glutAddMenuEntry("Disable lighting", 2);
   glutAddMenuEntry("Fill polygons", 3);
   glutAddMenuEntry("Wire frames", 4);
   glutAddMenuEntry("Quit", 666);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// ===================================================================
// void draw_cursor(int x, int y)
// =================================================================== 
void draw_cursor(int x, int y)
{
    
    glBegin (GL_QUADS);
        if (cursor_type == DO)
        {
            glColor3f(1.0, 0.0, 0.0);
        }
        else if (cursor_type == LOOK)
        {
            glColor3f(1.0, 1.0, 0.0); 
        }
        else if (cursor_type == TALK)
        {
            glColor3f(1.0, 0.0, 1.0);
        }
        else
        {
            glColor3f(0.0, 0.0, 1.0);
        }
        glVertex3f(cursor_x, cursor_y, 0.0);
        //glColor3f(1.0, 0.0, 0.0);
        glVertex3f(cursor_x, cursor_y - 0.5, 0.0);
        //glColor3f(1.0, 0.0, 0.0);
        glVertex3f(cursor_x + 0.5, cursor_y - 0.5, 0.0);
        glVertex3f(cursor_x + 0.5, cursor_y, 0.0);
    glEnd();
    glutPostRedisplay();
}


void triangle(float x, float y)
{
   glBegin (GL_TRIANGLES);
   glColor3f (1.0, 0.0, 0.0);
   glVertex2f (x+5.0, y+15.0);
   /* glColor3f (0.0, 1.0, 0.0); */
   glVertex2f (x+15.0, y+15.0);
   glColor3f (0.0, 0.0, 1.0); 
   glVertex2f (x+5.0, y+25.0);
   glEnd();
}

void triangle3D(void)
{
   triangle(1.0, 1.0);
   triangle(0.0, 0.0);
   /* draw_lines(5.0, 15.0, 6.0, 16.0); */
   draw_lines(6.0, 26.0, 16.0, 16.0);
   draw_lines(5.0, 25.0, 6.0, 26.0);
   draw_lines(15.0, 15.0, 16.0, 16.0);
}

void squares(float x, float y, float z, float red)
{
	glBegin (GL_QUADS);
	glColor3f(red, 0.0, 0.0);
	glVertex3f(5.0 + x, 5.0 + y, z);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(10.0 + x, 5.0 + y, z);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(10.0 + x, 10.0 + y, z);
	glVertex3f(5.0 + x, 10.0 + y, z);
	glEnd();
}

void draw_lines(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex3f(x1, y1, 0.0);
	glVertex3f(x2, y2, 0.0);
	glEnd();
}

void wire_square(float x, float y, int new_color)
{
	glBegin(GL_LINES);

	if (new_color == 1)
		glColor3f(1.0, 0.0, 0.0);
	else if (new_color == 2)
		glColor3f(0.0, 1.0, 0.0);
	else if (new_color == 3)
		glColor3f(0.0, 0.0, 1.0);
	else
		glColor3f(1.0, 1.0, 1.0);

	draw_lines(5.0 + x, 5.0 + y, 5.0 + x, 10.0 + y);
	draw_lines(5.0 + x, 10.0 + y, 10.0 + x, 10.0 + y);
	draw_lines(10.0 + x, 10.0 + y, 10.0 + x, 5.0 + y);
	draw_lines(10.0 + x, 5.0 + y, 5.0 + x, 5.0 + y);
	glEnd();
}

void cube(void)
{
	/* wire_square(2.5, 2.5, 2); */
	squares(2.5, 2.5, 0.0, .5);
	/* wire_square(0.0, 0.0, 1); */
	squares(0.0, 0.0, 0.0, 1);

	glColor3f(0.0, 0.5, 0.5);
	draw_lines(5.0, 5.0, 7.5, 7.5);
	draw_lines(5.0, 10.0, 7.5, 12.5);
	draw_lines(10.0, 10.0, 12.5, 12.5);
	draw_lines(10.0, 5.0, 12.5, 7.5);
}

void pyramid(float x, float y, float base)
{
	float heightx = x + 3.0;
	float heighty = y + 7.0;

	/* base of pyramid */
	draw_lines(x, y, x + 2.5, y + 2.5);
	draw_lines(x + 2.5, y + 2.5, x + 2.5 + base, y + 2.5);
	draw_lines(x+2.5+base, y+2.5, x+base, y);
	draw_lines(x+base, y, x, y);
	/* cone part */
	glColor3f(0.0, 0.0, 1.0);
	draw_lines(x, y, heightx, heighty);
	draw_lines(x+base, y, heightx, heighty);
	draw_lines(x+2.5+base, y+2.5, heightx, heighty);
	draw_lines(x+2.5, y+2.5, heightx, heighty);
}

void display(void)
{
   frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		printf("FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}


   glClear (GL_COLOR_BUFFER_BIT);
   triangle3D ();
   cube();
   pyramid(15.0, 5.0, 5.0); 
   draw_cursor(mouse_x, mouse_y);
   glFlush ();
   glutSwapBuffers();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   if (w <= h)
      gluOrtho2D (0.0, 30.0, 0.0, 30.0 * (GLfloat) h/(GLfloat) w);
   else
      gluOrtho2D (0.0, 30.0 * (GLfloat) w/(GLfloat) h, 0.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
   

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
// =================================================================== 
void PassiveMouseFunc(int x, int y)
{
    
    // set the global x and y mouse coordinates -- find a better solution
    mouse_x = x;
    mouse_y = y;
        
    cursor_x = (float)x / 25.06;
    
    // for some odd reason, this slight modification was necessary so when 
    // the mouse is pushed forward, the cursor goes up, and when the 
    // mouse is pulled down, the cursor goes down.  Otherwise, w/o
    // this algorithm, the cursor goes in opposite expected directions
    // This is intended for a screen that is 1024x768.  
    cursor_y = (float)(768 - mouse_y) / 25.06;

}

// ===================================================================
// void keyboard(unsigned char key, int x, int y)
// =================================================================== 
void keyboard(unsigned char key, int x, int y)
{
   switch (key) 
   {
      case 27:
         exit(0);
         break;
      case 'f':
      case 'F': printf("%d Hertz\n", glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE)); break;

      case 'q':
      case 'Q': glutLeaveGameMode(); exit(0); break;
      case 100:
		  choice_selected(1); break;
		  /* 101 is 'e' */
      case 101:
		  choice_selected(2); break;
		 
   }
}

// ===================================================================
// void specialKeys(int key, int x, int y)
// =================================================================== 
void specialKeys(int key, int x, int y)
{
    switch(key)
    {
	case GLUT_KEY_RIGHT :
		//c_hor = c_hor + 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_LEFT :
		//c_hor = c_hor - 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP :
		//c_ver = c_ver + 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN :
		//c_ver = c_ver - 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_HOME :
		//zoomer = zoomer - 0.05;
		glutPostRedisplay();
		break;
	case GLUT_KEY_END :
		//zoomer = zoomer + 0.05;
		glutPostRedisplay();
		break;
	default:
		break;
    }
}

int main(int argc, char *argv[])
{

  int WindW = 400;
  int WindH = 300;

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

/*
  glutInitWindowSize (500, 500); 
  glutInitWindowPosition (100, 100);
  glutCreateWindow ("Psychomancer");
*/


  // Enter to game mode: 800x600, 32 bit color, 72 Hz refresh rate
  //glutGameModeString("800x600:32@72"); // For full screen game mode
  glutGameModeString("1024x768:32&72"); // full screen game mode
  glutEnterGameMode(); 
  glutSetCursor(GLUT_CURSOR_NONE);

  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(processMouse);
  glutPassiveMotionFunc(PassiveMouseFunc);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(specialKeys);
  glClearColor(0, 0, 0, 0);
  glutMainLoop();

  return 0;

}

