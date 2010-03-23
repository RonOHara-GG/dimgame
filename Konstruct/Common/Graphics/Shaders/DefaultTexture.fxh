#ifndef _DEFAULT_TEXTURE_FXH_
#define _DEFAULT_TEXTURE_FXH_

texture g_DefaultTexture	: DEFAULTTEXTURE;

sampler DefaultTextureSampler = sampler_state
{
    Texture = <g_DefaultTexture>;
    MinFilter = LINEAR;  
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};


#endif // _DEFAULT_TEXTURE_FXH_