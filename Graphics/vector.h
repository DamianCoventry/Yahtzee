#ifndef INCLUDED_GRAPHICS_VECTOR
#define INCLUDED_GRAPHICS_VECTOR

namespace Graphics
{

struct Vector
{
    // State
    float x_, y_;

    // Construction
    Vector(float f = 0.0f)
        : x_(f), y_(f) {}

    Vector(float x, float y)
        : x_(x), y_(y) {}

    Vector(const Vector& rhs)
        : x_(rhs.x_), y_(rhs.y_) {}

    // Assignment
    Vector operator=(float f)
    {
        x_ = f; y_ = f;
        return *this;
    }

    // Comparison
    bool operator==(float f)
    {
        return (x_ == f) && (y_ == f);
    }

    bool operator==(const Vector& rhs)
    {
        return (x_ == rhs.x_) && (y_ == rhs.y_);
    }

    // Basic math
    Vector operator+(const Vector& rhs)
    {
        return Vector(x_+rhs.x_, y_+rhs.y_);
    }

    Vector operator-(const Vector& rhs)
    {
        return Vector(x_-rhs.x_, y_-rhs.y_);
    }

    Vector operator/(const Vector& rhs)
    {
        return Vector(x_/rhs.x_, y_/rhs.y_);
    }

    Vector operator*(const Vector& rhs)
    {
        return Vector(x_*rhs.x_, y_*rhs.y_);
    }

    // Basic math and assignment
    Vector operator+=(const Vector& rhs)
    {
        x_ += rhs.x_;
        y_ += rhs.y_;
        return *this;
    }

    Vector operator-=(const Vector& rhs)
    {
        x_ -= rhs.x_;
        y_ -= rhs.y_;
        return *this;
    }

    Vector operator/=(const Vector& rhs)
    {
        x_ /= rhs.x_;
        y_ /= rhs.y_;
        return *this;
    }

    Vector operator*=(const Vector& rhs)
    {
        x_ *= rhs.x_;
        y_ *= rhs.y_;
        return *this;
    }
};

}       // namespace Graphics

#endif  // INCLUDED_GRAPHICS_VECTOR
