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

#include <GLUT/glut.h>		// Mac OS X specification
#include <OpenAL/alut.h>

#include "LoadTGA.h"
#include "LoadGLTextures.h"
//#include "TGALoader.h"
#include "get_random.h"
#include "openal_mac.h"

// evil global variables -----------------------------------------
TGAImageRec textures[32]; 	// for the cursor icon
unsigned long 	texture[32];	// for the cursor icon (newer)

enum cursor_types {DO, LOOK, TALK, INVENTORY};
enum cursor_types cursor_type = LOOK;

int		light1_on	= 0; // 0 is on, 1 is off
int		window;
int		mouse_entry	= 1;
int		kWindowWidth	= 1024;
int		kWindowHeight	= 768;
static int 	mouse_x 	= 0; // kWindowWidth/2;
static int	mouse_y 	= kWindowHeight;
int		fullscreen	= 0;
int		isDirectionalKeyDown = false;
int		directional_key;

float 		cursor_x 	= 0; // kWindowWidth/2;
float		cursor_y 	= 0; // kWindowHeight/2;
float		cursor_z	= 0.0;
float		old_x 		= 0; //kWindowWidth/2;
float		old_y		= kWindowHeight;
float		mouse_sensitivity=    0.0009799849; // 0.00125; // this was 0.0075
float		mouse_sensitivity_y=   0.0013;
float 		cursor_size 	= 0.10;

float		x_camera	= 0.0;
float		y_camera	= -0.5;
float		z_camera	= -3.0;

int		x_angle		= 0;
int		y_angle		= 0;
int		z_angle		= 0;

int		lights_on	= FALSE;

int		isSphereNoisy	= FALSE;

int 		objectID	= 0;

int		random_red	= get_random(0, 255);
int		random_green	= get_random(0, 255);
int		random_blue	= get_random(0, 255);

int		sphere_red	= 255; // The three RGB components of the
int		sphere_green	= 255; // floating sphere which will change
int		sphere_blue	= 255; // colors randomly
int		sphere_direct	= 1; // 1 the sphere is going up
                                     // 0 the sphere is going down
float		sphere_height	= 0.0;


float		l1z_pos		= 2.0;

GLfloat 	LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat 	LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat		LightPosition[] = { 0.0f, 0.0f, l1z_pos, 1.0f };

GLfloat		xrot;						// X Rotation
GLfloat		yrot;						// Y Rotation
GLfloat		xspeed;						// X Rotation Speed
GLfloat 	yspeed;						// Y Rotation Speed

GLfloat 	walkbias = 0;
GLfloat 	walkbiasangle = 0;

GLfloat 	lookupdown = 0.0f;
const float 	piover180 = 0.0174532925f;

float 		heading = 0.0, xpos = 0.0, zpos = 3.0;


// ===================================================================
// Prototypes
// ===================================================================
// void LoadGLTextures(unsigned long []);
// drawing functions
void draw_cursor(int, int);
// display & initialize
void display();
GLvoid drawShapes(GLenum mode);
void InitGL();
void reshape(int w, int h);
void Idle();
// input functions
void processMouse(int button, int state, int x, int y) ;
int retrieveObjectID(int x, int y);
void PassiveMouseFunc(int, int);
void MovingMouseFunc(int x, int y);
void EntryFunc(int state);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void directionalKeys2(int key);
void specialKeysUp(int key, int x, int y);


// ===================================================================
// int main(int, char *[])
// ===================================================================
int main(int argc, char *argv[])
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(kWindowWidth, kWindowHeight);
    
    alutInit(&argc, argv) ;
    
    window = glutCreateWindow("Psychomancer");

    glutGameModeString("1024x768:24@72");
    glutEnterGameMode(); 

    glutSetCursor(GLUT_CURSOR_NONE); // make the default cursor disappear

    InitGL();
    openal_init();


    glutReshapeFunc(reshape);
    glutIdleFunc(Idle);
    glutDisplayFunc(display);
    glutMouseFunc(processMouse);
    glutPassiveMotionFunc(PassiveMouseFunc);
    glutMotionFunc(MovingMouseFunc);  
    glutEntryFunc(EntryFunc);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);
    glutSpecialUpFunc(specialKeysUp);
    glClearColor(0, 0, 0, 0);
    glutMainLoop();

    return 0;

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
        glBindTexture(GL_TEXTURE_2D, texture[2]);
    }
    else // INVENTORY ITEM
    {
        glBindTexture(GL_TEXTURE_2D, texture[1]); // 3
    }


glMatrixMode(GL_PROJECTION);			// Select Projection
    glPushMatrix();				// Push The Matrix
    glLoadIdentity();				// Reset The Matrix
    glOrtho( 0, 1 , 0 , 1, -1, 1 );		// Select Ortho Mode (640x480)
    glMatrixMode(GL_MODELVIEW);			// Select Modelview Matrix
    glPushMatrix();				// Push The Matrix
    glLoadIdentity();
    
    glBegin (GL_QUADS);

        glColor4f(1.0, 1.0, 1.0, 0.5);
        glTexCoord2f(0.0f, 0.0f); glVertex3f(cursor_x, cursor_y, 1.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(cursor_x + cursor_size, cursor_y, 1.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cursor_x + cursor_size, cursor_y + cursor_size, 1.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(cursor_x, cursor_y + cursor_size, 1.0);
        
    glEnd();
   
    glMatrixMode( GL_PROJECTION );		// Select Projection
    glPopMatrix();				// Pop The Matrix
    glMatrixMode( GL_MODELVIEW );		// Select Modelview
    glPopMatrix();
    
    glutPostRedisplay();
}


// ===================================================================
// void check_colors()
// -------------------------------------------------------------------
// ===================================================================
void check_colors()
{
    int correct_color = 0;
    
    if (sphere_red < random_red)
    {
        sphere_red++;
    }
    else if (sphere_red > random_red)
    {
        sphere_red--;
    }
    else
    {
        correct_color++;
    }
    
    if (sphere_green < random_green)
    {
        sphere_green++;
    }
    else if (sphere_green > random_green)
    {
        sphere_green--;
    }
    else
    {
        correct_color++;
    }
    
    if (sphere_blue < random_blue)
    {
        sphere_blue++;
    }
    else if (sphere_blue > random_blue)
    {
        sphere_blue--;
    }
    else
    {
        correct_color++;
    }
    
    // new color as been met.  Make a new random color
    if (correct_color >= 3)
    {
        random_red   = get_random(0, 255);
        random_blue  = get_random(0, 255);
        random_green = get_random(0, 255);
    }
      
}

// ===================================================================
// void display()
// Call non-transparent items first, then set GL_BLEND, then call
// the transparent/translucent items
// =================================================================== 
void display(void)
{

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
    
    drawShapes(GL_RENDER);
    
    // The following part cannot be in drawShapes() or the objectID
    // function will not work properly and return the properly
    // selected item
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);
    
      draw_cursor(mouse_x, mouse_y);

    glDepthMask( GL_TRUE );
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    
    glutSwapBuffers();
    glFlush();
}


// ===================================================================
// drawShapes(GLenum mode)
// ===================================================================
GLvoid drawShapes(GLenum mode)
{
    glInitNames();
    glPushName(0);
    
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    /*
    glLoadIdentity();
	glTranslatef(0.5f,0.0f, 0.5f);
        
        glTranslatef(-xpos, y_camera, -zpos);
        
        if (GL_SELECT == mode)
        {
            glLoadName(3);
        }
        
        glColor3f(0.0f,0.0f,1.0f);
        glutSolidTorus(0.5, 3.0, 20, 20);
    */

    glLoadIdentity();

    glPushMatrix();
        
        glRotatef((float)x_angle, 1.0, 0.0, 0.0);
        glRotatef(360.0f - yrot, 0.0, 1.0, 0.0);
        glRotatef((float)z_angle, 0.0, 0.0, 1.0);

        glTranslatef(-xpos, y_camera, -zpos);

    // draw objects which do not utilize textures
    
    glPushMatrix();
    
        if (GL_SELECT == mode)
        {
            glLoadName(1);
        }

        check_colors();
        glColor3f((float)sphere_red/255.0, (float)sphere_green/255.0, (float)sphere_blue/255.0);
        
        glTranslatef(0.0, 0.5 + sphere_height, 0.5);
        
        glutSolidSphere(0.1, 20, 20);
        
        if (1 == sphere_direct)
        {
            if (sphere_height < 0.25)
            {
                sphere_height += 0.005;
            }
            else
            {
                sphere_direct = 0;
            }
        }
        else
        {
            if (sphere_height > 0.0)
            {
                sphere_height -= 0.005;
            }
            else
            {
                sphere_direct = 1;
            }
        }
        
    glPopMatrix();

    if (lights_on == FALSE)
    {
        glColor3f(1.0f, 1.0f, 1.0f);
    }
    
    if (GL_SELECT == mode)
    {
        glLoadName(2);
    }

    // now draw objects which utilize textures
    glEnable(GL_TEXTURE_2D); 

    glBindTexture(GL_TEXTURE_2D, texture[4]);

        
        glBegin (GL_QUADS);

        //glColor4f(1.0, 1.0, 1.0, 0.5);
        //glTranslatef(x_camera, y_camera, z_camera);
        // (0,1)--------(1,1)
        //   |            |
        // (0,0)--------(1,0)
        
        // floor
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
            
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5 + i, 0.0, 0.0 + j);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + i, 0.0, 0.0 + j );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5 + i, 0.0, -1.0 + j);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i, 0.0, -1.0 + j);
            
            }
        }
        
        
        
        glEnd();
        
        // bind the ceiling texture
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        
        glBegin(GL_QUADS);
        // ceiling
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 5; j++)
            {
            
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5 + i, 1.0, 0.0 + j);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + i, 1.0, 0.0 + j );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5 + i, 1.0, -1.0 + j);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i, 1.0, -1.0 + j);
            
            }
        }
        glEnd();
       
       // Remember: this call needs to be outside the glBegin - glEnd
       glBindTexture(GL_TEXTURE_2D, texture[3]);
        
        glBegin(GL_QUADS);
        // left & right walls
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 5; j++)
            {
            //glPushMatrix();
            //glRotatef(90.0, 0.0, 0.0, 1.0);
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5 + i*5, 1.0, 0.0 + j);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5 + i*5, 0.0, 0.0 + j );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5 + i*5, 0.0, -1.0 + j);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i*5, 1.0, -1.0 + j);  
            //glPopMatrix();              
            }
        }
        
        // front and back walls
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 5; j++)
            {
            // Don't try and concentrate too much how things got screwed
            // up here so the texture would align correctly, just be happy
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5 + j, 1.0, -1.0 + i*5);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + j, 0.0, -1.0 + i*5);
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5 + j, 0.0, -1.0 + i*5);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + j, 1.0, -1.0 + i*5); 
            }
        }
        
        // new code added 5. March 2003
        // raised floor
        for (int i = 0; i < 5; i++)
        {
            for (int j = 0; j < 2; j++)
            {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5 + i, 0.25, 0.0 + j);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + i, 0.25, 0.0 + j );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.5 + i, 0.25, -1.0 + j);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i, 0.25, -1.0 + j);
            }
        }
        
        // front panels to raised area
        for (int i = 2; i < 5; i++)
        {
            
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5 + i, 0.25, 1.0);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + i, 0.0, 1.0 );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5 + i, 0.0, 1.0 );
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i, 0.25, 1.0); 
            
        }
        
        // front part of bedroom wall
        for (int i = 0; i < 2; i++)
        {
            
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.5 + i, 1.0, 1.0);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.5 + i, 0.0, 1.0 );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5 + i, 0.0, 1.0 );
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5 + i, 1.0, 1.0); 
            
        }
        
        // side of bedroom wall
        for (int i = 0; i < 2; i++)
        {
            glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5, 1.0, 0.0 + i);
            glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5, 0.0, 0.0 + i );
            glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5, 0.0, -1.0 + i);
            glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5, 1.0, -1.0 + i);  
        }
                                            
        glEnd();
             
    glPopMatrix();  



     
    //glutSwapBuffers();
    //glFlush();
}

// ===================================================================
// void InitGL()
// =================================================================== 
void InitGL()
{
    	
    LoadGLTextures(texture);
        
    // move the cursor to the set (x,y) coordinates
    glutWarpPointer(kWindowWidth/2, kWindowHeight/2); 
        
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
    glClearDepth(1.0);	
    glDepthFunc(GL_LESS);	
    glEnable(GL_DEPTH_TEST);	// Enables Depth Testing 
    glShadeModel(GL_SMOOTH);	

    glViewport(0, 0, kWindowWidth, kWindowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();	

    gluPerspective(45.0f, (GLfloat) kWindowWidth / (GLfloat) kWindowHeight, 0.1f, 100.0f);	
                                                                                                        

    // Calculate the Aspect Ratio of the window
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
        objectID = retrieveObjectID(x, y);
        cout << "objectID: " << objectID << endl;
        
        if ( (1 == objectID) && (FALSE == isSphereNoisy) && (DO == cursor_type) )
        {
            cout << "Let there be music!" << endl;
            alSourcePlay(source[1]);
            isSphereNoisy = TRUE;
        }
        else if ( (1 == objectID) && (TRUE == isSphereNoisy) && (DO == cursor_type) )
        {
            alSourceStop(source[1]);
            isSphereNoisy = FALSE;
        }
        
        
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
// int retrieveObjectID(int x, int y)
// -------------------------------------------------------------------
// Version: 23. January 2003 22:51
// Created: 23. January 2003 22:51
// ===================================================================
int retrieveObjectID(int x, int y)
{
    int objectsFound = 0;
    GLint viewportCoords[4] = {0};
    GLuint selectBuffer[32] = {0};
    
    glSelectBuffer(32, selectBuffer);
    glGetIntegerv(GL_VIEWPORT, viewportCoords);
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
        glRenderMode(GL_SELECT);
        glLoadIdentity();
        gluPickMatrix(x, viewportCoords[3] - y, 2, 2, viewportCoords);
    
        gluPerspective(45.0f, (float)kWindowWidth/(float)kWindowHeight, 0.1f, 150.0f);
        glMatrixMode(GL_MODELVIEW);
    
        //DrawGLScene(); // ??
        drawShapes(GL_SELECT);
    
        objectsFound = glRenderMode(GL_RENDER);
    
        glMatrixMode(GL_PROJECTION);
        
    glPopMatrix();
    
    glMatrixMode(GL_MODELVIEW);
    
    if (objectsFound > 0)
    {
        GLuint lowestDepth = selectBuffer[1];
        
        int selectedObject = selectBuffer[3];
        
        for (int i = 1; i < objectsFound; i++)
        {
            if (selectBuffer[(i*4)+1] < lowestDepth)
            {
                lowestDepth = selectBuffer[(i*4)+1];
                selectedObject = selectBuffer[(i*4)+3];
            }
        }
        
        return selectedObject;
    
    }
    
    
    return 0;
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
        cursor_y += -mouse_sensitivity_y * (y - old_y);
        
    
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
      case 'x':
                x_angle = (x_angle + 5) % 360;
		glutPostRedisplay();
                break;
      case 'X':
                x_angle = (x_angle - 5) % 360;
		glutPostRedisplay();
                break;
      case '\t':
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
            break;
      case '\n': cout << "Saw ENTER" << endl;
                break;
      case '\r': cout << "Saw Mac ENTER" << endl; // '\r' is the Mac version of ENTER
                break;
                
		 
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
        case GLUT_KEY_F3:
            if (lights_on == TRUE)
            {
                lights_on = FALSE;
            }
            else
            {
                lights_on = TRUE;
            }
            
            break;
        case GLUT_KEY_UP: z_camera += 1.0; 
                          xpos -= (float)sin(heading*piover180) * 0.05f;
		zpos -= (float)cos(heading*piover180) * 0.05f;
		
		if (walkbiasangle >= 359.0f)
			walkbiasangle = 0.0f;
		else 
			walkbiasangle+= 10;

		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;  
                
                // Play the footsteps wave 
                if (isDirectionalKeyDown == false)
                {
                    alSourcePlay(source[0]);
                    isDirectionalKeyDown = true;
                }

                break;
        case GLUT_KEY_DOWN: z_camera -= 1.0; 
                            xpos += (float)sin(heading*piover180) * 0.05f;
		zpos += (float)cos(heading*piover180) * 0.05f;
		if (walkbiasangle <= 1.0f)
			walkbiasangle = 359.0f;
		else 
			walkbiasangle-= 10;
		
		walkbias = (float)sin(walkbiasangle * piover180)/20.0f;
                
                if (isDirectionalKeyDown == false)
                {
                    alSourcePlay(source[0]);
                    isDirectionalKeyDown = true;
                }
                
                break;
        case GLUT_KEY_RIGHT:  y_angle = (y_angle + 5) % 360;
                              heading -= 2.0f;
                              yrot = heading;
                              //camera_x += 1.0;
                              // camera_x =
                              // camera_z = 
        break;
        case GLUT_KEY_LEFT:  y_angle = (y_angle - 5) % 360;
                             heading += 2.0f;	
                             yrot = heading;
                             //camera_x -= 1.0;
                             break;
        case GLUT_KEY_HOME:
                x_angle = (x_angle + 5) % 360;
		glutPostRedisplay();
                break;
      case GLUT_KEY_END:
                x_angle = (x_angle - 5) % 360;
		glutPostRedisplay();
                break;
	default:
		break;
    }
}


// ===================================================================
// void specialKeysUp(int key, int x, int y)
// set the isDirectionalKeyUp to false if necessary
// Also it is possible to create an array of the special keys, so
// info can be contained in an array instead of just relying on
// one or two boolean type values
// =================================================================== 
void specialKeysUp(int key, int x, int y)
{
    switch (key)
    {
        case GLUT_KEY_RIGHT :
		glutPostRedisplay();
		break;
        case GLUT_KEY_LEFT :
		glutPostRedisplay();
		break;
        case GLUT_KEY_UP :
                alSourceStop(source[0]);
                isDirectionalKeyDown = false;
                glutPostRedisplay();
		break;
        case GLUT_KEY_DOWN :
                alSourceStop(source[0]);
                isDirectionalKeyDown = false;
                glutPostRedisplay();
		break;
        }
}

