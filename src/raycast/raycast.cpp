#include "raycast.h"
#include <cmath>
#include <cstdlib>
#include "bitmap.h"

#ifndef M_PI
# define M_PI 3.14159265358979323846
#endif

#ifndef M_2_PI
# define M_2_PI (M_PI*2.0)
#endif

#ifndef M_PI_2
# define M_PI_2 (M_PI/2.0)
#endif

#ifndef M_PI_4
# define M_PI_4 (M_PI/4.0)
#endif

#ifndef M_PI_8
# define M_PI_8 (M_PI/8.0)
#endif

double floatMod(double a, double b){
	int q = floor(a / b);
	return a - q * b;
}

double angleToRadians(double a){
	return a * M_PI / 180.0;
}

double radiansToAngle(double r){
	return floatMod(360.0 + (r * 180.0 / M_PI),360.0);
}

Vector2D::Vector2D() : x(0.0), y(0.0) {}

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D::Vector2D(const Vector2D &value) : x(value.x), y(value.y) {}

bool Vector2D::operator==(const Vector2D &value) const{
	return x == value.x && y == value.y;
}

bool Vector2D::operator!=(const Vector2D &value) const{
	return !(*this == value);
}

Vector2D &Vector2D::operator+=(const Vector2D &value){
	x += value.x;
	y += value.y;
	return *this;
}

Vector2D &Vector2D::operator-=(const Vector2D &value){
	x -= value.x;
	y -= value.y;
	return *this;
}

Vector2D &Vector2D::operator*=(const Vector2D &value){
	x *= value.x;
	y *= value.y;
	return *this;
}

Vector2D &Vector2D::operator/=(const Vector2D &value){
	x /= value.x;
	y /= value.y;
	return *this;
}

Vector2D Vector2D::operator+(const Vector2D &value) const{
	return Vector2D(x+value.x, y+value.y);
}
	
Vector2D Vector2D::operator-(const Vector2D &value) const{
	return Vector2D(x-value.x, y-value.y);
}

Vector2D Vector2D::operator*(const Vector2D &value) const{
	return Vector2D(x*value.x, y*value.y);
}

Vector2D Vector2D::operator/(const Vector2D &value) const{
	return Vector2D(x/value.x, y/value.y);
}

void Vector2D::rotate(double r){
	double s = sin(r);
	double c = cos(r);
	x = x*c+y*s;
	y = x*-s+y*c;
}

