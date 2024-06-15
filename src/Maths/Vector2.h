#ifndef VECTOR2_H
#define VECTOR2_H

#include <cmath>

struct Vector2i
{
    static Vector2i Zero() { return Vector2i(0, 0); }

    static Vector2i North() { return Vector2i(0, -1); }
    static Vector2i East() { return Vector2i(1, 0); }
    static Vector2i South() { return Vector2i(0, 1); }
    static Vector2i West() { return Vector2i(-1, 0); }

    Vector2i()
    {
        x = 0;
        y = 0;
    }

    Vector2i(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2i(const Vector2i& p)
    {
        x = p.x;
        y = p.y;
    }

    void operator = (const Vector2i& rhs)
    {  
        x = rhs.x; 
        y = rhs.y;
    }

    bool operator == (const Vector2i& rhs)
    { 
        return (x == rhs.x && y == rhs.y);
    }

    bool operator != (const Vector2i& rhs)
    {
        return (x != rhs.x || y != rhs.y);
    }

    Vector2i operator + (const Vector2i& rhs)
    {
        Vector2i res;
        res.x = x + rhs.x;
        res.y = y + rhs.y;
        return res;
    }

    Vector2i& operator += (const Vector2i& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2i operator - (const Vector2i& rhs)
    {
        Vector2i res;
        res.x = x - rhs.x;
        res.y = y - rhs.y;
        return res;
    }

    Vector2i operator * (const int& rhs)
    {
        Vector2i res;
        res.x = x * rhs;
        res.y = y * rhs;
        return res;
    }

    int Length()
    {
        return sqrt((x * x) + (y * y));
    }

    int LengthSquared()
    {
        return (x * x) + (y * y);
    }

    Vector2i Normal()
    {
        double m = Length();
        return Vector2i(round(x / m), round(y / m));
    }

    int x;
    int y;
};

struct Vector2f
{
    static Vector2f Zero() { return Vector2f(0, 0); }

    static Vector2f North() { return Vector2f(0, -1.0); }
    static Vector2f East() { return Vector2f(1.0, 0); }
    static Vector2f South() { return Vector2f(0, 1.0); }
    static Vector2f West() { return Vector2f(-1.0, 0); }

    Vector2f()
    {
        x = 0;
        y = 0;
    }

    Vector2f(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    Vector2f(const Vector2f& p)
    {
        x = p.x;
        y = p.y;
    }

    void operator = (const Vector2f& rhs)
    {  
        x = rhs.x; 
        y = rhs.y;
    }

    bool operator == (const Vector2f& rhs)
    { 
        return (x == rhs.x && y == rhs.y);
    }

    bool operator != (const Vector2f& rhs)
    {
        return (x != rhs.x || y != rhs.y);
    }

    Vector2f operator + (const Vector2f& rhs)
    {
        Vector2f res;
        res.x = x + rhs.x;
        res.y = y + rhs.y;
        return res;
    }

    Vector2f& operator += (const Vector2f& rhs)
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }

    Vector2f operator - (const Vector2f& rhs)
    {
        Vector2f res;
        res.x = x - rhs.x;
        res.y = y - rhs.y;
        return res;
    }

    Vector2f operator * (const double& rhs)
    {
        Vector2f res;
        res.x = x * rhs;
        res.y = y * rhs;
        return res;
    }

    double Length()
    {
        return sqrt((x * x) + (y * y));
    }

    double LengthSquared()
    {
        return (x * x) + (y * y);
    }

    Vector2f Normal()
    {
        double m = Length();
        return Vector2f(round(x / m), round(y / m));
    }

    double x;
    double y;
};

#endif
