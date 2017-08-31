// ===================================================================
// LoadGLTextures_win_linux.h
// Windows & Linux version to load in textures
// ===================================================================


// ===================================================================
// void LoadTextures(unsigned long texture[])
// =================================================================== 
void LoadGLTextures(unsigned int texture[])
{
    GLuint type = GL_RGBA;
    TGAImageRec *local_tex[7];
    
    // alpha images should be 32 bit or it won't work!
    local_tex[0] = LoadTGA("do_alpha32.tga");      // DO icon
    local_tex[1] = LoadTGA("eye_alpha.tga");       // LOOK icon
    local_tex[2] = LoadTGA("talk_alpha32.tga");    // TALK icon
    local_tex[3] = LoadTGA("wood-1.tga"); 	   // wall texture
    local_tex[4] = LoadTGA("hardwoodfloor32.tga"); // floor texture
    local_tex[5] = LoadTGA("ceiling.tga"); 	   // ceiling texture
    
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
