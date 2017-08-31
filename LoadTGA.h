/*
 *  LoadTGA.h
 *  PIGE
 *
 *  Created by admin on Tue Oct 30 2001.
 *  Copyright (c) 2001 Edenwaith. All rights reserved.
 *
 */


 typedef struct TGAImageRec
{
	GLubyte	*data;		// Image data (Up to 32 bits)
	GLuint	bpp;		// Image color depth in bits per pixel
        GLuint	sizeX;		// Image width
	GLuint	sizeY;		// Image height
}	TGAImageRec;


TGAImageRec* LoadTGA( char *filename )
{    
        int i = 0;
	GLubyte			TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0}; // Uncompressed TGA header
	GLubyte			TGAcompare[12];	// Used to compare TGA header
        GLubyte			header[6];	// First 6 Useful bytes from header
	GLuint			bytesPerPixel;	// Holds number of bytes per pixel used in TGA file
	GLuint			imageSize;	// Used to store image size when setting aside room
	GLuint			temp;		// Temporary variable
	// int			type = GL_RGBA;	// Set default GL mode to RGBA (32 BPP)	
	TGAImageRec		*texture;	// Texture structure to return
	FILE			*file;		// Image file to be opened
	
        // Differences in the Mac OS X version.  bytesPerPixel, imageSize, and temp
        // are 'int' instead of GLUint.  Also type is commented out, but it is
        // never used.
	
	file = fopen( filename, "rb" );

	if( ( file == NULL ) ||	
		( fread( TGAcompare, 1, sizeof( TGAcompare ), file ) != sizeof( TGAcompare ) ) ||
		( memcmp( TGAheader, TGAcompare, sizeof( TGAheader ) ) != 0 ) ||	
		( fread( header, 1, sizeof( header ), file ) != sizeof( header ) ) )
        {
		printf("Error: could not find or open file %s\n", filename);
                fclose( file );
		return NULL;
	}
	
	texture = ( TGAImageRec* )malloc( sizeof( TGAImageRec ) );
	
	texture->sizeX  = header[1] * 256 + header[0];
	texture->sizeY = header[3] * 256 + header[2];
    
 	if(	( texture->sizeX <= 0 ) || ( texture->sizeY <= 0 ) || ( ( header[4] != 24 ) && ( header[4] != 32 ) ) )
	{
		fclose( file );
		free( texture );
		return NULL;
        }
	
	texture->bpp = header[4];							
	bytesPerPixel = texture->bpp/8;	
	
	imageSize = texture->sizeX * texture->sizeY * bytesPerPixel;
	
	texture->data = ( GLubyte* )malloc( imageSize );		
	
	if(	( texture->data == NULL ) || ( fread( texture->data, 1, imageSize, file ) != imageSize ) )
	{

		if( texture->data != NULL )
                {
			free( texture->data );
                }

		fclose( file );
		free( texture );
		return NULL;
	}
        
        // For Linux and Windows, imageSize may need to be
        // casted as an int.
	for ( i = 0; i < imageSize ; i += bytesPerPixel )
	{

		temp = texture->data[i];		
		texture->data[i] = texture->data[i + 2];	
                texture->data[i + 2] = temp;				

        }
	
	fclose( file );
	
	return texture;
}

