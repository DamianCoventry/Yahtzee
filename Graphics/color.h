#ifndef INCLUDED_GRAPHICS_COLOR
#define INCLUDED_GRAPHICS_COLOR

namespace Graphics
{

class Color
{
public:
    Color(float f);
    Color(float r, float g, float b);
    Color(const Color& color);

    void operator=(const Color& rhs)
    {
        r_ = rhs.GetRed();
        g_ = rhs.GetGreen();
        b_ = rhs.GetBlue();
    }

    void SetRed(float r)    { r_ = r; }
    void SetGreen(float g)  { g_ = g; }
    void SetBlue(float b)   { b_ = b; }

    float GetRed() const    { return r_; }
    float GetGreen() const  { return g_; }
    float GetBlue() const   { return b_; }

private:
    float r_, g_, b_;
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_SPRITE
