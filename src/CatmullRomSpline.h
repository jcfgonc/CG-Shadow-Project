/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
* started on 09-04-2007
*
* adapted from Catmull-Rom Splines
* Llew Mason 
* comp.graphics.algorithms, 30 Jun 1994
*/

#ifndef CATMULL_ROM_SPLINE_HEADER
#define CATMULL_ROM_SPLINE_HEADER

float catmullRomSpline (float p1, float p2, float p3, float p4, float t) {
	return(0.5f*((-p1 + 3*p2 -3*p3 + p4)*t*t*t
		+ (2*p1 -5*p2 + 4*p3 - p4)*t*t
		+ (-p1+p3)*t
		+ 2*p2));
}

#endif
