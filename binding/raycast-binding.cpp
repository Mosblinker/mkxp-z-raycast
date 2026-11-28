#include "binding-types.h"
#include "binding-util.h"
#include "raycast/raycast.h"
#include "exception.h"
#include "sharedstate.h"

RB_METHOD(angleToRads){
	RB_UNUSED_PARAM;
	
	double value;
	
	rb_get_args(argc, argv, "f", &value RB_ARG_END);
	
	return rb_float_new(angleToRadians(value));
}

RB_METHOD(radsToAngle){
	RB_UNUSED_PARAM;
	
	double value;
	
	rb_get_args(argc, argv, "f", &value RB_ARG_END);
	
	return rb_float_new(radiansToAngle(value));
}

RB_METHOD(raycastGetMissingTexture){
	RB_UNUSED_PARAM;
	
	int width, height;
	
	rb_get_args(argc, argv, "ii", &width, &height RB_ARG_END);
	
	Bitmap *bmp = getMissingTexture(width,height);
	
	return wrapObject(bmp, BitmapType);
}







void raycastBindingInit(){
	
	rb_define_global_function("angle_to_radians", RUBY_METHOD_FUNC(angleToRads), -1);
	rb_define_global_function("radians_to_angle", RUBY_METHOD_FUNC(radsToAngle), -1);
	rb_define_global_function("get_missing_texture", RUBY_METHOD_FUNC(raycastGetMissingTexture), -1);
	
	
}