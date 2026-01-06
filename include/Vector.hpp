#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cmath>

class Vec2 {

  public:
  double x;
  double y;

  Vec2()
      : x(0)
      , y(0)
  {
  }
  Vec2(double x_, double y_)
      : x(x_)
      , y(y_)
  {
  }

  double norm() { return sqrt(x * x + y * y); }
  double norm2() { return x * x + y * y; }
  double rotation() { return atan2(y, x); };

  Vec2 operator+(const Vec2& other) const;
  Vec2 operator-(const Vec2& other) const;
  Vec2 operator*(const double value) const;
  Vec2 operator/(const double value) const;
  double operator*(const Vec2& other) const;

  Vec2& operator+=(const Vec2& other);
  Vec2& operator-=(const Vec2& other);
  Vec2& operator*=(double value);
  Vec2& operator/=(double value);

  void normalize();
  Vec2 normalized() const;
  Vec2 unit() const;
  Vec2 rotate(double rads) const;
  void setMag(double mag);

  void print();
};

const Vec2 ZeroVec(0, 0);

inline Vec2 operator*(double s, const Vec2& other)
{
  return other * s;
}

#endif
