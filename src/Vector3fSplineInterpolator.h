/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * started on 09-04-2007
 */
#ifndef VECTOR_3F_SPLINE_INTERPOLATOR_HEADER
#define VECTOR_3F_SPLINE_INTERPOLATOR_HEADER

#include "CatmullRomSpline.h"
#include "Vector3f.h"

class Vector3fSplineInterpolator {
private:
	Vector3f* currentPosition;
	Vector3f* *path;
	int maxNumPoints;
	int numPoints;

public:
	Vector3fSplineInterpolator(int numberofpathpoints) {
		numPoints = 0;
		maxNumPoints = numberofpathpoints;
		currentPosition = new Vector3f();
		path = new Vector3f*[numberofpathpoints];
	}

	~Vector3fSplineInterpolator() {
		delete currentPosition;
		for (int i = 0; i != numPoints; ++i) {
			delete path[i];
		}
		delete[] path;
		delete path; // HUMM?
	}

	void addPathPoint(Vector3f* newposition) {
		//add data
		path[numPoints] = new Vector3f(newposition);
		numPoints++;
	}

	void interpolateVector(int startingpoint, float n) {
		// control points, s1 & s2 are the interpolated ones
		int s0, s1, s2, s3;
		// cycle path loop
		if (startingpoint >= numPoints) {
			startingpoint = startingpoint % numPoints;
		}
		// check for loop positions, each one has it's own control points
		if (startingpoint == 0) {
			s0 = numPoints - 1;
			s1 = 0;
			s2 = 1;
			s3 = 2;
		} else if (startingpoint == numPoints - 2) {
			s0 = numPoints - 3;
			s1 = numPoints - 2;
			s2 = numPoints - 1;
			s3 = 0;
		} else if (startingpoint == numPoints - 1) {
			s0 = numPoints - 2;
			s1 = numPoints - 1;
			s2 = 0;
			s3 = 1;
		} else {
			s0 = startingpoint - 1;
			s1 = startingpoint;
			s2 = startingpoint + 1;
			s3 = startingpoint + 2;
		}
		float x1 = path[s0]->x;
		float x2 = path[s1]->x;
		float x3 = path[s2]->x;
		float x4 = path[s3]->x;

		float y1 = path[s0]->y;
		float y2 = path[s1]->y;
		float y3 = path[s2]->y;
		float y4 = path[s3]->y;

		float z1 = path[s0]->z;
		float z2 = path[s1]->z;
		float z3 = path[s2]->z;
		float z4 = path[s3]->z;

		currentPosition->x = catmullRomSpline(x1, x2, x3, x4, n);
		currentPosition->y = catmullRomSpline(y1, y2, y3, y4, n);
		currentPosition->z = catmullRomSpline(z1, z2, z3, z4, n);
	}

	void interpolateVector(float n) {
		int pos = (int) floor(n);
		float coef = n - float(pos);
		interpolateVector(pos, coef);
	}

	int getNumberOfPathPoints() {
		return numPoints;
	}

	Vector3f* getCurrentPosition() {
		return currentPosition;
	}
};
#endif
