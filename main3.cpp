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
// #include "TGALoader.h"

// evil global variables
// TGAImageRec textures[1]; 	// for the cursor icon
// unsigned long 	texture[1];	// for the cursor icon (newer)
typedef struct				// Create A Structure
{
	GLubyte	*imageData;		// Image Data (Up To 32 Bits)
	GLuint	bpp;			// Image Color Depth In Bits Per Pixel.
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID Used To Select A Texture
} TextureImage;				// Structure Name

TextureImage textures[1];	// Storage For One Texture

// prototypes
void draw_cursor(int, int);
// void LoadGLTextures();
bool LoadGLTGA(TextureImage *texture, char *filename);
void draw_lines(float, float, float, float);
void PassiveMouseFunc(int, int);
void specialKeys(int key, int x, int y);
//void InitGL();
int InitGL();

// global variables
enum cursor_types {DO, LOOK, TALK, INVENTORY};
enum cursor_types cursor_type = DO;

// int frame = 0;
// int time = 1;
// int timebase = 0;
static int mouse_x = 0, mouse_y = 0;
float cursor_x = 0.0, cursor_y = 0.0;




/********************> LoadTGA() <*****/
bool LoadTGA(TextureImage *texture, char *filename)			// Loads A TGA File Into Memory
{    
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};	// Uncompressed TGA Header
	GLubyte		TGAcompare[12];								// Used To Compare TGA Header
	GLubyte		header[6];									// First 6 Useful Bytes From The Header
	GLuint		bytesPerPixel;								// Holds Number Of Bytes Per Pixel Used In The TGA File
	GLuint		imageSize;									// Used To Store The Image Size When Setting Aside Ram
	GLuint		temp;										// Temporary Variable
	GLuint		type=GL_RGBA;								// Set The Default GL Mode To RBGA (32 BPP)

	FILE *file = fopen(filename, "rb");						// Open The TGA File

	if(	file==NULL ||										// Does File Even Exist?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// Are There 12 Bytes To Read?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0				||	// Does The Header Match What We Want?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// If So Read Next 6 Header Bytes
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->width  = header[1] * 256 + header[0];			// Determine The TGA Width	(highbyte*256+lowbyte)
	texture->height = header[3] * 256 + header[2];			// Determine The TGA Height	(highbyte*256+lowbyte)
    
 	if(	texture->width	<=0	||								// Is The Width Less Than Or Equal To Zero
		texture->height	<=0	||								// Is The Height Less Than Or Equal To Zero
		(header[4]!=24 && header[4]!=32))					// Is The TGA 24 or 32 Bit?
	{
		fclose(file);										// If Anything Failed, Close The File
		return false;										// Return False
	}

	texture->bpp	= header[4];							// Grab The TGA's Bits Per Pixel (24 or 32)
	bytesPerPixel	= texture->bpp/8;						// Divide By 8 To Get The Bytes Per Pixel
	imageSize		= texture->width*texture->height*bytesPerPixel;	// Calculate The Memory Required For The TGA Data

	texture->imageData=(GLubyte *)malloc(imageSize);		// Reserve Memory To Hold The TGA Data

	if(	texture->imageData==NULL ||							// Does The Storage Memory Exist?
		fread(texture->imageData, 1, imageSize, file)!=imageSize)	// Does The Image Size Match The Memory Reserved?
	{
		if(texture->imageData!=NULL)						// Was Image Data Loaded
			free(texture->imageData);						// If So, Release The Image Data

		fclose(file);										// Close The File
		return false;										// Return False
	}

	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)		// Loop Through The Image Data
	{														// Swaps The 1st And 3rd Bytes ('R'ed and 'B'lue)
		temp=texture->imageData[i];							// Temporarily Store The Value At Image Data 'i'
		texture->imageData[i] = texture->imageData[i + 2];	// Set The 1st Byte To The Value Of The 3rd Byte
		texture->imageData[i + 2] = temp;					// Set The 3rd Byte To The Value In 'temp' (1st Byte Value)
	}

	fclose (file);											// Close The File

	// Build A Texture From The Data
	glGenTextures(1, &texture[0].texID);					// Generate OpenGL texture IDs

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);			// Bind Our Texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered
	
	if (texture[0].bpp==24)									// Was The TGA 24 Bits
	{
		type=GL_RGB;										// If So Set The 'type' To GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;											// Texture Building Went Ok, Return True
}

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
   // LoadGLTextures();
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
    glBindTexture(GL_TEXTURE_2D, textures[0].texID);
    
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
        glTexCoord2f(0.0f, 0.0f); glVertex3f(cursor_x, cursor_y, 0.0);
        glTexCoord2f(1.0f, 0.0f); glVertex3f(cursor_x, cursor_y - 0.5, 0.0);
        glTexCoord2f(1.0f, 1.0f); glVertex3f(cursor_x + 0.5, cursor_y - 0.5, 0.0);
        glTexCoord2f(0.0f, 1.0f); glVertex3f(cursor_x + 0.5, cursor_y, 0.0);
    glEnd();
    glutPostRedisplay();
}

// ===================================================================
// void LoadTextures()
// May want to move most of this code into the LoadTGA function
// Refer to Lesson25.cpp for example
// =================================================================== 
/*
void LoadGLTextures()
{
    GLuint type = GL_RGBA;
    
    TGAImageRec *texture1;
    // textures[0] = LoadTGA("LOOK_cursor_alpha.tga");

    // texture1 = LoadTGA("LOOK_cursor_alpha.tga");
    texture1 = LoadTGA("weirdbrick.tga");

    glGenTextures(1, &texture[0]);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
    if ( texture1->bpp == 24 )
	type = GL_RGB;
    else
	type = GL_RGBA;
			
    glTexImage2D(GL_TEXTURE_2D, 0, type, texture1->sizeX, texture1->sizeY, 0, type, GL_UNSIGNED_BYTE, texture1->data);
    
    cout << "Texture was supposedly loaded." << endl;       
        
}
*/

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


// ===================================================================
// void display()
// Draw to the display
// =================================================================== 
void display(void)
{
/*
   frame++;
	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		printf("FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;		
		frame = 0;
	}
*/

   glClear (GL_COLOR_BUFFER_BIT);
   // triangle3D ();
   cube();
   // pyramid(15.0, 5.0, 5.0); 
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
        // cout << "x: " << x << " y: " << y << endl;
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
      //case 'f':
      //case 'F': printf("%d Hertz\n", glutGameModeGet(GLUT_GAME_MODE_REFRESH_RATE)); break;

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

/*
GLvoid InitGL2(GLvoid)
{
	
	LoadGLTextures();							// Load The Texture(s) ( NEW )
	glEnable(GL_TEXTURE_2D);					// Enable Texture Mapping ( NEW )
	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
	glClearDepth(1.0);							// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);						// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);					// Enables Depth Testing 
	glShadeModel(GL_SMOOTH);					// Enables Smooth Color Shading

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();							// Reset The Projection Matrix

//	gluPerspective(45.0f, (GLfloat) kWindowWidth / (GLfloat) kWindowHeight, 0.1f, 100.0f);	
												// Calculate The Aspect Ratio Of The Window

	glMatrixMode(GL_MODELVIEW);

}
*/

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	if (!LoadGLTGA(&textures[0], "weirdbrick.tga"))								// Jump To Texture Loading Routine
	{
		return false;									// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
        
        return true;
}

int main(int argc, char *argv[])
{

  int WindW = 400;
  int WindH = 300;

  glutInit(&argc, argv);
  glutInitWindowSize(WindW, WindH);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);


  // InitGL();
  // init();
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

