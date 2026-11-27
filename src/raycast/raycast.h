/**
	Classes and methods related to processing and rendering a 3D raycasted 
	world.
	
	By Mosblinker
*/

#ifndef RAYCAST_H
#define RAYCAST_H
#include "bitmap.h"

double floatMod(double a, double b);

double angleToRadians(double a);

double radiansToAngle(double r);

Bitmap getMissingTexture(int width, int height){
	Bitmap img(width,height);
	Vec4 black(0,0,0,255);
	img.fillRect(0,0,width,height,black);
	Vec4 magenta(255,0,255,255);
	int squareW = (int) ceil(width / 8.0);
	int squareH = (int) ceil(height / 8.0);
	for (int i = 0; i < 8; i++){
		int y = i*squareH;
		if (y >= height)
			break;
		for (int x = i % 2; x < 8; x+=2){
			img.fillRect(x*squareW, y, squareW, squareH, magenta);
		}
	}
	return img;
}

Vec4 changeBrightness(const Vec4 &color, double brightness, bool hasAlpha = false){
	float a = (hasAlpha) ? color.w : 255.0f;
	if (brightness >= 1.0)
		return Vec4(color.x, color.y, color.z, a);
	if (brightness <= 0.0)
		return Vec4(0,0,0,a);
	return Vec4(float(color.x*brightness),float(color.y*brightness),float(color.z*brightness),a);
}

Color changeBrightness(const Color &color, double brightness, bool hasAlpha = false){
	double a = (hasAlpha) ? color.alpha : 255.0;
	if (brightness >= 1.0)
		return Color(color.red, color.green, color.blue, a);
	if (brightness <= 0.0)
		return Color(0.0,0.0,0.0,a);
	return Color(color.red*brightness,color.green*brightness,color.blue*brightness,a);
}
#endif