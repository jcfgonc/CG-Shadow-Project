/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
 * started on 01-05-2007
 */
#ifndef LIGHT_MOTION_HEADER
#define LIGHT_MOTION_HEADER

#define maxlinelenght 1024

#include "Vector3f.h"
#include "Vector3fSplineInterpolator.h"
#include "Light.h"
#include <stdio.h>

class LightMotion {

private:
	Light* light;

	Vector3fSplineInterpolator* pathposition;
	Vector3fSplineInterpolator* pathdirection;
	Vector3fSplineInterpolator* pathup;

	int maxNumPoints;
	int numOfPoints;

	void initialize(Light* l, int numberofpoints) {
		numOfPoints = 0;
		maxNumPoints = numberofpoints;

		//create spline paths
		pathposition = new Vector3fSplineInterpolator(maxNumPoints);
		pathdirection = new Vector3fSplineInterpolator(maxNumPoints);
		pathup = new Vector3fSplineInterpolator(maxNumPoints);

		//associate internal light
		light = l;
		//and its vectors
		light->direction = pathdirection->getCurrentPosition();
		light->position = pathposition->getCurrentPosition();
		light->up = pathup->getCurrentPosition();
	}

public:

	LightMotion(Light* l, const char* filename) {
		FILE *file;
		file = fopen(filename, "r");
		if (file == NULL) {
			printf("Unable to read file \"%s\"\n", filename);
			system("PAUSE");
			exit(-1);
		} else {
			printf("Parsing path file \"%s\"\n", filename);
			parseDataFile(l, file);
		}
	}

	~LightMotion() {
		delete pathposition;
		delete pathdirection;
		delete pathup;

		delete light->direction;
		delete light->position;
		delete light->up;
		delete light;
	}

	void addPathPoint(Vector3f* position0, Vector3f* direction0, Vector3f* up0) {
		//add data
		pathposition->addPathPoint(position0);
		pathdirection->addPathPoint(direction0);
		pathup->addPathPoint(up0);
		numOfPoints++;
	}

	void interpolateMotion(int startingpoint, float n) {
		pathposition->interpolateVector(startingpoint, n);
		pathdirection->interpolateVector(startingpoint, n);
		pathup->interpolateVector(startingpoint, n);
	}

	void interpolateMotion(float n) {
		int pos = (int) floor(n);
		float coef = n - pos;
		interpolateMotion(pos, coef);
	}

	int getNumberOfPathPoints() {
		return numOfPoints;
	}

	void parseLine(char *line, double *arg1, double *arg2, double *arg3) {
		*arg1 = atof(strtok(line, " ,\n\t"));
		*arg2 = atof(strtok(NULL, " ,\n\t"));
		*arg3 = atof(strtok(NULL, " ,\n\t"));
		//		printf("%f\t%f\t%f\n",*arg1,*arg2,*arg3);
	}

	void parseDataFile(Light* l, FILE *file) {
		int section = 0;
		double *x = new double;
		double *y = new double;
		double *z = new double;
		Vector3f* position = NULL;
		Vector3f* direction = NULL;
		Vector3f* up = NULL;
		char *line = new char[maxlinelenght + 1];
		while (feof(file) == 0) {
			//make shure we read something usefull
			if (fgets(line, maxlinelenght, file) != NULL && strlen(line) > 1) {
				if (section == 0) {
					//get number of positions
					int numofpos = (int) atof(line);
					if (numofpos < 1) {
						printf("Invalid number of path positions, %d\n", maxNumPoints);
						exit(-1);
					} else {
						initialize(l, numofpos);
						section++;
						printf("Created path of %d points\n", numofpos);
					}
				} else if (section == 1) {
					// puts("vec [xyz] position");
					// vec [xyz] position
					parseLine(line, x, y, z);
					position = new Vector3f((float) (*x), (float) (*y), (float) (*z));
					section++;
				} else if (section == 2) {
					// puts("vec [xyz] direction");
					// vec [xyz] direction
					parseLine(line, x, y, z);
					direction = new Vector3f((float) (*x), (float) (*y), (float) (*z));
					section++;
				} else if (section == 3) {
					parseLine(line, x, y, z);
					up = new Vector3f((float) (*x), (float) (*y), (float) (*z));
					//we already got the needed stuff, add it
					addPathPoint(position, direction, up);
					section = 1;
					delete position;
					delete direction;
					delete up;
				}
			}
		}
		fclose(file);
		printf("added %d path points\n", numOfPoints);
	}

	Light* getLight() {
		return this->light;
	}
};
#endif
