#include "stdafx.h"
#include "texture.h"
#include "../Utility/logmacros.h"
#include "../Utility/exception.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gl/gl.h>

using namespace Graphics;

Texture::Texture()
: LogContext("Graphics::Texture")
, id_(0)
{
}

Texture::~Texture()
{
    Delete();
}

void Texture::Create(int w, int h, boost::shared_array<unsigned char> pixels)
{
    if(!id_)
    {
        LOG("Binding...");

        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)pixels.get());
        switch(glGetError())
        {
        case GL_NO_ERROR: LOG("glTexImage2D returned GL_NO_ERROR"); break;
        case GL_INVALID_ENUM: LOG("glTexImage2D returned GL_INVALID_ENUM"); break;
        case GL_INVALID_VALUE: LOG("glTexImage2D returned GL_INVALID_VALUE"); break;
        case GL_INVALID_OPERATION: LOG("glTexImage2D returned GL_INVALID_OPERATION"); break;
        case GL_STACK_OVERFLOW: LOG("glTexImage2D returned GL_STACK_OVERFLOW"); break;
        case GL_STACK_UNDERFLOW: LOG("glTexImage2D returned GL_STACK_UNDERFLOW"); break;
        case GL_OUT_OF_MEMORY: LOG("glTexImage2D returned GL_OUT_OF_MEMORY"); break;
        }

        LOG("Bound as OpenGL image [" << id_ << "]");
    }
}

void Texture::Delete()
{
    if(id_)
    {
        LOG("Unbinding OpenGL image [" << id_ << "]");
        glDeleteTextures(1, &id_);
        id_ = 0;
    }
}

int Texture::GetWidth() const
{
    int width = 0;
    if(id_)
    {
        glBindTexture(GL_TEXTURE_2D, id_);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    }
    return width;
}

int Texture::GetHeight() const
{
    int height = 0;
    if(id_)
    {
        glBindTexture(GL_TEXTURE_2D, id_);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    }
    return height;
}
