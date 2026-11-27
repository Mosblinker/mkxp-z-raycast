/**
	Classes and methods related to processing and rendering a 3D raycasted 
	world.
	
	By Mosblinker
*/

#ifndef RAYCAST_H
#define RAYCAST_H
#include "bitmap.h"
#include <variant>

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

struct Vector2D{
	
	double x;
	double y;
	
	Vector2D();
	
	Vector2D(double x, double y);
	
	Vector2D(const Vector2D &value);
	
	bool operator==(const Vector2D &value) const;
	
	bool operator!=(const Vector2D &value) const;
	
	Vector2D &operator+=(const Vector2D &value);
	
	Vector2D &operator-=(const Vector2D &value);
	
	Vector2D &operator*=(const Vector2D &value);
	
	Vector2D &operator/=(const Vector2D &value);
	
	Vector2D operator+(const Vector2D &value) const;
	
	Vector2D operator-(const Vector2D &value) const;
	
	Vector2D operator*(const Vector2D &value) const;
	
	Vector2D operator/(const Vector2D &value) const;
	
	void rotate(double r);
};

Vec2 rotateVector(const Vec2 &vector, double r){
	double s = sin(r);
	double c = cos(r);
	return Vec2(float(vector.x*c+vector.y*s),float(vector.x*-s+vector.y*c));
}

struct Line2D{
	
	double x1;
	double y1;
	double x2;
	double y2;
	
	Line2D() : x1(0.0),y1(0.0),x2(0.0),y2(0.0) {}
	
	Line2D(double x1, double y1, double x2, double y2) : x1(x1),y1(y1),x2(x2),y2(y2) {}
	
	Line2D(const Line2D &line) : x1(line.x1),y1(line.y1),x2(line.x2),y2(line.y2) {}
};

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Vec4 &color);

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Color &color){
	drawLine(bitmap,x1,y1,x2,y2,Vec4(color.red,color.green,color.blue,color.alpha));
}

void drawLine(Bitmap &bitmap, const Line2D &line, const Vec4 &color){
	drawLine(bitmap,line.x1,line.y1,line.x2,line.y2,color);
}

void drawLine(Bitmap &bitmap, const Line2D &line, const Color &color){
	drawLine(bitmap,line.x1,line.y1,line.x2,line.y2,color);
}

enum RaycastObjectType{
	RAYCAST_OBJECT
};
/**
	Base class for all objects and entities in a raycasted world.
*/
class RaycastObject{
	public:
		RaycastObject();
		RaycastObject(double x,double y,double dir = 0.0,double radius = 0.0);
		RaycastObject(const RaycastObject &obj);
		~RaycastObject();
		
		DECL_ATTR(X, double);
		DECL_ATTR(Y, double);
		DECL_ATTR(DirX, double);
		DECL_ATTR(DirY, double);
		DECL_ATTR(DirRad, double);
		DECL_ATTR(DirAngle, double);
		DECL_ATTR(Active, bool);
		DECL_ATTR(Radius, double);
		
		void setPosition(double x, double y);
		int mapX() const;
		int mapY() const;
		
		virtual void rotate(double r);
		
		virtual RaycastObjectType type();
		
	private:
		double x;
		double y;
		Vector2D dir;
		bool active;
		double radius;
};

struct RaycastObjectHit{
	RaycastObject* obj;
	virtual ~RaycastObjectHit() {}
}

struct RaycastWallHit{
	int mapX;
	int mapY;
	int side;
	int wallType;
	RaycastWallHit(int x, int y,int side, int type) : mapX(x),mapY(y),side(side),wallType(type) {}
	RaycastWallHit() : RaycastWallHit(0,0,0,1) {}
};
/**
	Struct for representing a wall or object that was hit by a raycast.
*/
struct RaycastHit{
	double dirX;
	double dirY;
	double distance;
	RaycastHit(double dirX, double dirY, double distance) : dirX(dirX),dirY(dirY),distance(distance) {}
	RaycastHit() : RaycastHit(1.0,0.0,0.0) {}
	std::variant<RaycastObjectHit,RaycastWallHit> target;
	double dirRad();
	double dirAngle();
};
/**

*/
class RaycastTriggerLineData{
	public:
		RaycastTriggerLineData(Line2D line, int type, int tag, double cooldown = 0.0);
		RaycastTriggerLineData(double x1, double y1, double x2, double y2, int type, int tag, double cooldown = 0.0);
		
		DECL_ATTR(Line, Line2D);
		DECL_ATTR(X1, double);
		DECL_ATTR(Y1, double);
		DECL_ATTR(X2, double);
		DECL_ATTR(Y2, double);
		void setLine(double x1, double y1, double x2, double y2);
		DECL_ATTR(Type, int);
		DECL_ATTR(Tag, int);
		DECL_ATTR(Interactable, int);
		DECL_ATTR(CooldownDuration, double);
		DECL_ATTR(CooldownTimer, double);
		
		void reset();
	private:
		Line2D line;
		int type;
		int tag;
		int interactable;
		double cooldown;
		double timer;
};


#endif