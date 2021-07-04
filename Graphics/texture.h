#ifndef INCLUDED_GRAPHICS_TEXTURE
#define INCLUDED_GRAPHICS_TEXTURE

#include <boost/shared_ptr.hpp>
#include <boost/shared_array.hpp>
#include "../Utility/logcontext.h"

namespace Graphics
{

class Texture :
    public Utility::LogContext

{
public:
    Texture();
    ~Texture();

    void Create(int w, int h, boost::shared_array<unsigned char> pixels);
    void Delete();

    unsigned int GetId() const { return id_; }
    int GetWidth() const;
    int GetHeight() const;

private:
    unsigned int id_;
};

typedef boost::shared_ptr<Texture> TexturePtr;

}       // namespace Grahics

#endif  // INCLUDED_GRAPHICS_TEXTURE
