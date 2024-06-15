#ifndef RECT_H
#define RECT_H

#include "Vector2.h"

struct Recti
{
    static Recti Zero() { return Recti( 0, 0, 0, 0 ); };

    Recti()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    Recti(int x, int y, int w, int h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    Recti(const Recti& r)
    {
        this->x = r.x;
        this->y = r.y;
        this->w = r.w;
        this->h = r.h;
    }

    void operator = (const Recti& rhs)
    {   x = rhs.x; 
        y = rhs.y;
        w = rhs.w;
        h = rhs.h;
    }

    bool operator == (const Recti& rhs)
    { 
        return (x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h);
    }

    bool operator != (const Recti& rhs)
    {
        return (x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h);
    }

    int Top() const { return y; }

    int Left() const { return x; }

    int Bottom() const { return y + h; }

    int Right() const { return x + w; }

    Vector2i TopLeft() const { return Vector2i(Left(), Top()); }

    Vector2i TopRight() const { return Vector2i(Right(), Top()); }

    Vector2i BottomLeft() const { return Vector2i(Left(), Bottom()); }

    Vector2i BottomRight() const { return Vector2i(Right(), Bottom()); }

    Vector2i RelCenter() const { return Vector2i(w / 2, h / 2); }

    Vector2i AbsCenter() const { return Vector2i(x + (w / 2), y + (h / 2)); }

    int x;
    int y;
    int w;
    int h;
};

struct Rectf
{
    static Rectf Zero() { return Rectf( 0, 0, 0, 0 ); };

    Rectf()
    {
        x = 0;
        y = 0;
        w = 0;
        h = 0;
    }

    Rectf(double x, double y, double w, double h)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    Rectf(const Rectf& r)
    {
        this->x = r.x;
        this->y = r.y;
        this->w = r.w;
        this->h = r.h;
    }

    void operator = (const Rectf& rhs)
    {   x = rhs.x; 
        y = rhs.y;
        w = rhs.w;
        h = rhs.h;
    }

    bool operator == (const Rectf& rhs)
    { 
        return (x == rhs.x && y == rhs.y && w == rhs.w && h == rhs.h);
    }

    bool operator != (const Rectf& rhs)
    {
        return (x != rhs.x || y != rhs.y || w != rhs.w || h != rhs.h);
    }

    double Top() const { return y; }

    double Left() const { return x; }

    double Bottom() const { return y + h; }

    double Right() const { return x + w; }

    Vector2f TopLeft() const { return Vector2f(Left(), Top()); }

    Vector2f TopRight() const { return Vector2f(Right(), Top()); }

    Vector2f BottomLeft() const { return Vector2f(Left(), Bottom()); }

    Vector2f BottomRight() const { return Vector2f(Right(), Bottom()); }

    Vector2f RelCenter() const { return Vector2f(w / 2, h / 2); }

    Vector2f AbsCenter() const { return Vector2f(x + (w / 2), y + (h / 2)); }

    double x;
    double y;
    double w;
    double h;
};

#endif
