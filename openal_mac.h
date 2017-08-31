/*
 *  openal_mac.h
 *  PIGE
 *
 *  Created by Chad Armstrong on Sun Apr 06 2003.
 *  Copyright (c) 2003 Edenwaith. All rights reserved.
 *
 */

// ===================================================================
// Global variables
// ===================================================================
#define NUM_BUFFERS 2
#define NUM_SOURCES 2
#define NUM_ENVIRONMENTS 1

ALfloat listenerPos[]={0.0,0.0,4.0};
ALfloat listenerVel[]={0.0,0.0,0.0};
ALfloat	listenerOri[]={0.0,0.0,1.0, 0.0,1.0,0.0};

ALfloat source0Pos[]={ -2.0, 0.0, 0.0};
ALfloat source0Vel[]={ 0.0, 0.0, 0.0};

ALfloat source1Pos[]={ 2.0, 0.0, 0.0};
ALfloat source1Vel[]={ 0.0, 0.0, 0.0};

ALuint	buffer[NUM_BUFFERS];
ALuint	source[NUM_SOURCES];
ALuint  environment[NUM_ENVIRONMENTS];

ALsizei size,freq;
ALenum 	format;
ALvoid 	*data;

// ===================================================================
// void openal_init()
// ===================================================================
void openal_init(void)
{

    alListenerfv(AL_POSITION,listenerPos);
    alListenerfv(AL_VELOCITY,listenerVel);
    alListenerfv(AL_ORIENTATION,listenerOri);
    
    alGetError(); // clear any error messages
    
    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating buffers !!\n");
        exit(1);
    }
    else
    {
        printf("init() - No errors yet.\n");
    }
    
    // Generate buffers, or else no sound will happen!
    alGenBuffers(NUM_BUFFERS, buffer);
    
    alutLoadWAVFile("Footsteps.wav",&format,&data,&size,&freq);
    alBufferData(buffer[0],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);


    alutLoadWAVFile("a.wav",&format,&data,&size,&freq);
    alBufferData(buffer[1],format,data,size,freq);
    alutUnloadWAV(format,data,size,freq);

    alGetError(); /* clear error */
    alGenSources(NUM_SOURCES, source);

    if(alGetError() != AL_NO_ERROR) 
    {
        printf("- Error creating sources !!\n");
        exit(2);
    }
    else
    {
        printf("init - no errors after alGenSources\n");
    }

    alSourcef(source[0],AL_PITCH,1.0f);
    alSourcef(source[0],AL_GAIN,1.0f);
    alSourcefv(source[0],AL_POSITION,source0Pos);
    alSourcefv(source[0],AL_VELOCITY,source0Vel);
    alSourcei(source[0],AL_BUFFER,buffer[0]);
    alSourcei(source[0],AL_LOOPING,AL_TRUE);


    alSourcef(source[1],AL_PITCH,1.0f);
    alSourcef(source[1],AL_GAIN,1.0f);
    alSourcefv(source[1],AL_POSITION,source1Pos);
    alSourcefv(source[1],AL_VELOCITY,source1Vel);
    alSourcei(source[1],AL_BUFFER,buffer[1]);
    alSourcei(source[1],AL_LOOPING,AL_TRUE);
   
}