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
