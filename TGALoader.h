#ifndef	_TGA_LOADER_H_
#define	_TGA_LOADER_H_


/**> HEADER FILES <**/
#ifdef __APPLE_CC__	// Include things correctly for ProjectBuilder
    #include <Carbon/Carbon.h>
    #include <OpenGL/gl.h>
#else
    #include <Carbon.h>
    #include <gl.h>
#endif
#include <stdlib.h>
#include <stdio.h>


/**> DATA STRUCTURES <**/
typedef struct TGAImageRec
{
	GLubyte	*data;		// Image Data (Up To 32 Bits)
	GLuint	bpp;		// Image Color Depth In Bits Per Pixel.
	GLuint	sizeX;		// Image Width
	GLuint	sizeY;		// Image Height
        GLuint	texID;		// Texture ID Used To Select A Texture
}	TGAImageRec;



/**> FUNCTION PROTOTYPES <**/
TGAImageRec*	LoadTGA( char *filename );


#endif
