#include "stdafx.h"
#include "color.h"

using namespace Graphics;

Color::Color(float f)
: r_(f), g_(f), b_(f)
{
}

Color::Color(float r, float g, float b)
: r_(r), g_(g), b_(b)
{
}

Color::Color(const Color& color)
: r_(color.r_), g_(color.g_), b_(color.b_)
{
}
