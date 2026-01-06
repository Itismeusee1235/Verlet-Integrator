#include "../include/Vector.hpp"
#include <iostream>

Vec2 Vec2::operator+(const Vec2& other) const
{
  return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const
{
  return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(double value) const
{
  return Vec2(x * value, y * value);
}

Vec2 Vec2::operator/(double value) const
{
  return Vec2(x / value, y / value);
}

double Vec2::operator*(const Vec2& other) const
{
  return x * other.x + y * other.y;
}

Vec2& Vec2::operator+=(const Vec2& other)
{
  x += other.x;
  y += other.y;
  return *this;
}

Vec2& Vec2::operator-=(const Vec2& other)
{
  x -= other.x;
  y -= other.y;
  return *this;
}

Vec2& Vec2::operator*=(double value)
{
  x *= value;
  y *= value;
  return *this;
}

Vec2& Vec2::operator/=(double value)
{
  x /= value;
  y /= value;
  return *this;
}

void Vec2::normalize()
{
  double mag = this->norm();
  x /= mag;
  y /= mag;
}

Vec2 Vec2::normalized() const
{
  Vec2 out = *this;
  out.normalize();
  return out;
}

Vec2 Vec2::unit() const
{
  Vec2 copy(x, y);
  copy.normalize();
  return copy;
}

Vec2 Vec2::rotate(double rads) const
{

  double costheta = std::cos(rads);
  double sintheta = std::sin(rads);

  double dx = x * costheta - y * sintheta;
  double dy = y * costheta + x * sintheta;

  return Vec2(dx, dy);
}

void Vec2::setMag(double mag)
{
  this->normalize();
  *this *= mag;
}

void Vec2::print()
{
  printf("X: %f, Y: %f, Mag: %f\n", x, y, this->norm());
}
