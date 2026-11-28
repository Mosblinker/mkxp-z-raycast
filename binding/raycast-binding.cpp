#include "binding-types.h"
#include "binding-util.h"
#include "raycast/raycast.h"
#include "exception.h"
#include "sharedstate.h"
#include <cmath>

#if RAPI_FULL > 187
DEF_TYPE(Line2D);
#else
DEF_ALLOCFUNC(Line2D);
#endif

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

RB_METHOD(vectorRotate){
	RB_UNUSED_PARAM;
	
	VALUE arr;
	double rad;
	
	rb_get_args(argc, argv, "of", &arr, &rad RB_ARG_END);
	
	Check_Type(arr, T_ARRAY);
	
	long len = RARRAY_LEN(arr);
	
	double x,y;
	GUARD_EXC(
		if (len != 2)
			throw Exception(Exception::ArgumentError, "Array must have two elements");
		x = rb_ary_entry(arr, 0);
		y = rb_ary_entry(arr, 1);
		
		double s = sin(rad);
		double c = cos(rad);
		
		x = x*c+y*s;
		y = x*-s+y*c;
	)
	
	VALUE out = rb_ary_new_capa(2);
	rb_ary_push(out,rb_float_new(x));
	rb_ary_push(out,rb_float_new(y));
	
	return out;
}

ATTR_DOUBLE_RW(Line2D, X1)
ATTR_DOUBLE_RW(Line2D, Y1)
ATTR_DOUBLE_RW(Line2D, X2)
ATTR_DOUBLE_RW(Line2D, Y2)

EQUAL_FUN(Line2D)

INIT_FUN(Line2D, double, "ffff", 0)

SET_FUN(Line2D, double, "ffff", 0)






void raycastBindingInit(){
	rb_define_global_function("angle_to_radians", RUBY_METHOD_FUNC(angleToRads), -1);
	rb_define_global_function("radians_to_angle", RUBY_METHOD_FUNC(radsToAngle), -1);
	rb_define_global_function("get_missing_texture", RUBY_METHOD_FUNC(raycastGetMissingTexture), -1);
	rb_define_global_function("rotate_vector", RUBY_METHOD_FUNC(vectorRotate), -1);
	
	
	
}