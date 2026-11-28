/**
	Classes and methods related to processing and rendering a 3D raycasted 
	world.
	
	By Mosblinker
*/

#ifndef RAYCAST_H
#define RAYCAST_H
#pragma once
#include "bitmap.h"
#include <vector>
#include <variant>

double floatMod(double a, double b);

double angleToRadians(double a);

double radiansToAngle(double r);

Bitmap* getMissingTexture(int width, int height);

Vec4 changeBrightness(const Vec4 &color, double brightness, bool hasAlpha = false);

Color changeBrightness(const Color &color, double brightness, bool hasAlpha = false);

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

Vec2 rotateVector(const Vec2 &vector, double r);

struct Line2D{
	
	double x1;
	double y1;
	double x2;
	double y2;
	
	Line2D() : x1(0.0),y1(0.0),x2(0.0),y2(0.0) {}
	
	Line2D(double x1, double y1, double x2, double y2);
	
	Line2D(const Line2D &line);
};

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Vec4 &color);

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Color &color);

void drawLine(Bitmap &bitmap, const Line2D &line, const Vec4 &color);

void drawLine(Bitmap &bitmap, const Line2D &line, const Color &color);

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
	RaycastWallHit(int x, int y,int side, int type) : mapX(x),mapY(y),side(side) {}
	RaycastWallHit() : RaycastWallHit(0,0,0) {}
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
/**
	
*/
class RaycastLevelData{
	public:
		RaycastLevelData(int width, int height, int floorColor = 0, int ceilingColor = 0);
		
		int width() const;
		int height() const;
		
		// Map data
		bool contains(int x, int y) const;
		int get(int x, int y) const;
		void set(int x, int y, int value);
		DECL_ATTR(FloorColor, int);
		void setFloorColor(unsigned char r, unsigned char g, unsigned char b);
		DECL_ATTR(CeilingColor, int);
		void setCeilingColor(unsigned char r, unsigned char g, unsigned char b);
		
		void reset();
	
	private:
		int w;
		int h;
		std::vector<std::vector<int>> map;
		int fColor;
		int cColor;
};



#endif