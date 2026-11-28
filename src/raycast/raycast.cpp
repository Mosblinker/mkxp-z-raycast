#include "raycast.h"
#include <cmath>
#include <cstdlib>
#include "bitmap.h"
#include <vector>
#include <variant>

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

Bitmap* getMissingTexture(int width, int height){
	Bitmap *img = new Bitmap(width,height);
	Vec4 black(0,0,0,255);
	img->fillRect(0,0,width,height,black);
	Vec4 magenta(255,0,255,255);
	int squareW = (int) ceil(width / 8.0);
	int squareH = (int) ceil(height / 8.0);
	for (int i = 0; i < 8; i++){
		int y = i*squareH;
		if (y >= height)
			break;
		for (int x = i % 2; x < 8; x+=2){
			img->fillRect(x*squareW, y, squareW, squareH, magenta);
		}
	}
	return img;
}

Vec4 changeBrightness(const Vec4 &color, double brightness, bool hasAlpha){
	float a = (hasAlpha) ? color.w : 255.0f;
	if (brightness >= 1.0)
		return Vec4(color.x, color.y, color.z, a);
	if (brightness <= 0.0)
		return Vec4(0,0,0,a);
	return Vec4(float(color.x*brightness),float(color.y*brightness),float(color.z*brightness),a);
}

Color changeBrightness(const Color &color, double brightness, bool hasAlpha){
	double a = (hasAlpha) ? color.alpha : 255.0;
	if (brightness >= 1.0)
		return Color(color.red, color.green, color.blue, a);
	if (brightness <= 0.0)
		return Color(0.0,0.0,0.0,a);
	return Color(color.red*brightness,color.green*brightness,color.blue*brightness,a);
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

Vec2 rotateVector(const Vec2 &vector, double r){
	double s = sin(r);
	double c = cos(r);
	return Vec2(float(vector.x*c+vector.y*s),float(vector.x*-s+vector.y*c));
}

Line2D::Line2D(double x1, double y1, double x2, double y2) : x1(x1),y1(y1),x2(x2),y2(y2) {}

Line2D::Line2D(const Line2D &line) : x1(line.x1),y1(line.y1),x2(line.x2),y2(line.y2) {}

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Vec4 &color){
	if (x2 < x1){
		double temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
	}
    double dx = x2 - x1;
    double dy = y2 - y1;
    if (dx != 0 || dy != 0){
    	if (dx == 0)
    		bitmap.fillRect(int(x1),int(std::min(y1,y2)),1,int(dy),color);
    	else if (dy == 0)
    		bitmap.fillRect(int(x1),int(y1),int(dx),1,color);
    	else{
    		double m = dy/dx;
    		double y0 = y1;
    		int x2 = int(ceil(x2));
    		double y3 = std::max(std::min(y1,y2), 0.0);
    		double y4 = std::max(std::min(y1,y2), double(bitmap.height()));
    		for (int x = int(floor(x1)); x1 <= x2; x++){
    			double y = m * (x - x1) + y1;
    			int yMin = int(floor(std::max(std::min(y0,y),y3)));
    			int yMax = int(ceil(std::min(std::max(y0,y), y4)));
    			bitmap.fillRect(x,yMin,1,std::max(yMax-yMin,1),color);
			}
		}
	}
}

void drawLine(Bitmap &bitmap, double x1, double y1, double x2, double y2, const Color &color){
	drawLine(bitmap,x1,y1,x2,y2,Vec4(color.red,color.green,color.blue,color.alpha));
}

void drawLine(Bitmap &bitmap, const Line2D &line, const Vec4 &color){
	drawLine(bitmap,line.x1,line.y1,line.x2,line.y2,color);
}

void drawLine(Bitmap &bitmap, const Line2D &line, const Color &color){
	drawLine(bitmap,line.x1,line.y1,line.x2,line.y2,color);
}



RaycastObject::RaycastObject(double x,double y,double dir,double radius){
	this -> x = x;
	this -> y = y;
	this -> dir = Vector2D(1.0,0.0);
	if (dir != 0.0 && floatMod(dir, M_2_PI) != 0.0)
		this->dir.rotate(dir);
	active = true;
	this -> radius = radius;
}

RaycastObject::RaycastObject() : RaycastObject(0.0,0.0) {}

RaycastObject::RaycastObject(const RaycastObject &obj): RaycastObject(obj.getX(),obj.getY(),0.0,obj.getRadius()){
	dir.x = obj.getDirX();
	dir.y = obj.getDirY();
	this -> active = obj.getActive();
}

RaycastObject::~RaycastObject(){
	
}

void RaycastObject::setX(double x){
	this -> x = x;
}

double RaycastObject::getX() const{
	return x;
}

void RaycastObject::setY(double y){
	this -> y = y;
}

double RaycastObject::getY() const{
	return y;
}

void RaycastObject::setPosition(double x, double y){
	this -> x = x;
	this -> y = y;
}

int RaycastObject::mapX(){
	return int(x);
}

int RaycastObject::mapY(){
	return int(y);
}

void RaycastObject::setDirX(double x){
	dir.x = x;
}

double RaycastObject::getDirX() const{
	return dir.x;
}

void RaycastObject::setDirY(double y){
	dir.y = y;
}

double RaycastObject::getDirY() const{
	return dir.y;
}

void RaycastObject::setDirRad(double rad){
	rotate(getDirRad() - rad);
}

double RaycastObject::getDirRad() const{
	return atan2(dir.y,dir.x);
}

void RaycastObject::setDirAngle(double angle){
	setDirRad(angleToRadians(angle));
}

double RaycastObject::getDirAngle() const{
	return radiansToAngle(getDirRad());
}

virtual void RaycastObject::rotate(double r){
	dir.rotate(r);
}

void RaycastObject::setActive(bool value){
	active = value;
}

bool RaycastObject::getActive() const{
	return active;
}

void RaycastObject::setRadius(double value){
	radius = value;
}

double RaycastObject::getRadius() const{
	return radius;
}

virtual RaycastObject::RaycastObjectType type(){
	return RAYCAST_OBJECT;
}

double RaycastHit::dirRad() const{
	return atan2(dirY,dirX);
}

double RaycastHit::dirAngle() const{
	return radiansToAngle(dirRad());
}

RaycastTriggerLineData::RaycastTriggerLineData(Line2D line, int type, int tag, double cooldown){
	this -> line = line;
	this -> type = type;
	this -> tag = tag;
	this -> cooldown = cooldown;
	interactable = 0;
	timer = 0.0;
}

RaycastTriggerLineData::RaycastTriggerLineData(double x1, double y1, double x2, double y2, int type, int tag, double cooldown){
	line = Line2D(x1,y1,x2,y2);
	this -> type = type;
	this -> tag = tag;
	this -> cooldown = cooldown;
	interactable = 0;
	timer = 0.0;
}

void RaycastTriggerLineData::setLine(Line2D line){
	this -> line = line;
}

void RaycastTriggerLineData::setLine(double x1, double y1, double x2, double y2){
	line = Line2D(x1,y1,x2,y2);
}

Line2D RaycastTriggerLineData::getLine() const{
	return line;
}

void RaycastTriggerLineData::setX1(double x1){
	line.x1 = x1;
}

double RaycastTriggerLineData::getX1() const{
	return line.x1;
}

void RaycastTriggerLineData::setY1(double y1){
	line.y1 = y1;
}

double RaycastTriggerLineData::getY1() const{
	return line.y1;
}

void RaycastTriggerLineData::setX2(double x2){
	line.x2 = x2;
}

double RaycastTriggerLineData::getX2() const{
	return line.x2;
}

void RaycastTriggerLineData::setY2(double y2){
	line.y2 = y2;
}

double RaycastTriggerLineData::getY2() const{
	return line.y2;
}

void RaycastTriggerLineData::setType(int type){
	this -> type = type;
}

int RaycastTriggerLineData::getType() const{
	return type;
}

void RaycastTriggerLineData::setTag(int tag){
	this -> tag = tag;
}

int RaycastTriggerLineData::getTag() const{
	return tag;
}

void RaycastTriggerLineData::setInteractable(int value){
	interactable = value;
}

int RaycastTriggerLineData::getInteractable() const{
	return interactable;
}

void RaycastTriggerLineData::setCooldownDuration(double duration){
	cooldown = duration;
}

double RaycastTriggerLineData::getCooldownDuration() const{
	return cooldown;
}

void RaycastTriggerLineData::setCooldownTimer(double value){
	timer = value;
}

double RaycastTriggerLineData::getCooldownTimer() const{
	return timer;
}

void RaycastTriggerLineData::reset(){
	interactable = 0;
	timer = 0.0;
}

RaycastLevelData::RaycastLevelData(int width, int height, int floorColor, int ceilingColor){
	w = width;
	h = height;
	fColor = floorColor;
	cColor = ceilingColor;
	for (int y = 0; y < h; y++){
		std::vector<int> mapRow;
		mapRow.insert(mapRow.end(), w, 0);
		map.push_back(mapRow);
}

int RaycastLevelData::width() const{
	return w;
}

int RaycastLevelData::height() const{
	return h;
}

bool RaycastLevelData::contains(int x, int y) const{
	return x >= 0 && x < w && y >= 0 && y < h;
}

int RaycastLevelData::get(int x, int y) const{
	if (!contains(x,y))
		return 1;
	return map.at(y).at(x);
}

void RaycastLevelData::set(int x, int y, int value){
	if (contains(x,y))
		map.at(y).at(x) = value;
}

int RaycastLevelData::getFloorColor() const{
	return fColor;
}

void RaycastLevelData::setFloorColor(int color){
	fColor = color;
}

void RaycastLevelData::setFloorColor(unsigned char r, unsigned char g, unsigned char b){
	setFloorColor((int(r) << 16) | (int(g) << 8) | int(b));
}

int RaycastLevelData::getCeilingColor() const{
	return cColor;
}

void RaycastLevelData::setCeilingColor(int color){
	cColor = color;
}

void RaycastLevelData::setCeilingColor(unsigned char r, unsigned char g, unsigned char b){
	setCeilingColor((int(r) << 16) | (int(g) << 8) | int(b));
}

void RaycastLevelData::reset(){
}

