/*
 *  LoadGLTextures.h
 *  PIGE
 *  Mac OS X version
 *
 *  Created by Chad Armstrong on Tue Mar 04 2003.
 *  Copyright (c) 2003 Edenwaith. All rights reserved.
 *
 */



// ===================================================================
// void LoadTextures(unsigned long texture[])
// May want to move most of this code into the LoadTGA function
// Refer to Lesson25.cpp for example
// =================================================================== 
void LoadGLTextures(unsigned long texture[])
{
    GLuint type = GL_RGBA;
    
    TGAImageRec *local_tex[7];

    // texture[0] is DO
    // texture[1] is LOOK
    // texture[2] is TALK
    // texture[3] is INVENTORY
    
    // alpha images should be 32 bit or it won't work!
    local_tex[0] = LoadTGA("do_alpha32.tga");
    local_tex[1] = LoadTGA("eye_alpha.tga");
    local_tex[2] = LoadTGA("talk_alpha32.tga");
    local_tex[3] = LoadTGA("wood-1.tga");
    local_tex[4] = LoadTGA("hardwoodfloor32.tga");
    local_tex[5] = LoadTGA("ceiling.tga");
    
    for (int i = 0; i < 6; i++)
    {
    
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			
        if ( local_tex[i]->bpp == 24 )
            type = GL_RGB;
        else
        {
            cout << "Texture " << i << " BPP: " << local_tex[i]->bpp << endl;
            type = GL_RGBA;
        }
	
    glTexImage2D(GL_TEXTURE_2D, 0, type, local_tex[i]->sizeX, local_tex[i]->sizeY, 0, type, GL_UNSIGNED_BYTE, local_tex[i]->data);
    
    }
    
}