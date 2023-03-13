#include <cmath>

class Vec2
{
public:
    float x, y;

    Vec2();
    Vec2(float, float);
    ~Vec2();

    Vec2 normalize();
    float magnitude();
    Vec2 dot(const Vec2&);
    float cross(const Vec2&);

    Vec2 operator=(const Vec2 &v){
        x = v.x;
        y = v.y;
        return *this;
    }
    Vec2 operator/(float f){
        return Vec2(x / f, y / f);
    }
    Vec2 operator/(const Vec2 &v){
        return Vec2(x / v.x, y / v.y);
    }
    Vec2 operator*(float f){
        return Vec2(x * f, y * f);
    }
    Vec2 operator+(const Vec2 &v){
        return Vec2(x + v.x, y + v.y);
    }
    Vec2 operator-(const Vec2 &v){
        return Vec2(x - v.x, y - v.y);
    }
    Vec2 operator+=(const Vec2 &v){
        x += v.x;
        y += v.y;
        return *this;
    }
    Vec2 operator-=(const Vec2 &v){
        x -= v.x;
        y -= v.y;
        return *this;
    }
    Vec2 operator*=(float f){
        x *= f;
        y *= f;
        return *this;
    }

    Vec2 operator*=(const Vec2 &v){
        x *= v.x;
        y *= v.y;
        return *this;
    }

    Vec2 operator/=(float f){
        x /= f;
        y /= f;
        return *this;
    }
    bool operator==(const Vec2 &v){
        return x == v.x && y == v.y;
    }
    bool operator!=(const Vec2 &v){
        return x != v.x || y != v.y;
    }


};

Vec2::Vec2(){
    x = 0;
    y = 0;
}

Vec2::~Vec2()
{
}

Vec2::Vec2(float x, float y){
    this->x = x;
    this->y = y;
}

Vec2 Vec2::normalize(){
    float length = sqrt(x * x + y * y);
    return Vec2(x / length, y / length);
}

float Vec2::magnitude(){
    return sqrt(x * x + y * y);
}

Vec2 Vec2::dot(const Vec2& v){
    return Vec2(x * v.x, y * v.y);
}

float Vec2::cross(const Vec2& v){
    return x * v.y - y * v.x;
}