/*
* Author      : Joao Carlos jcfgonc@gmail.com
* License     : MIT License
* started on 09-04-2007
* checked on 14-04-2007: OK, clean
*/
#ifndef CAMERA_PATH_FOLLOWER_HEADER
#define CAMERA_PATH_FOLLOWER_HEADER

#define maxlinelenght 1024

#include "Vector3f.h"
#include "CatmullRomSpline.h"
#include "Vector3fSplineInterpolator.h"
#include "Camera.h"
#include <stdio.h>

class CameraPathFollower {
public:
	Camera* cam;
	Vector3fSplineInterpolator* pathposition;
	Vector3fSplineInterpolator* pathdirection;
	Vector3fSplineInterpolator* pathup;
	int maximumNumberPoints;
	int numberPoints;

	void initialize(int arraylength_)
	{
		numberPoints = 0;
		maximumNumberPoints = arraylength_;

		pathposition = new Vector3fSplineInterpolator(arraylength_);
		pathdirection = new Vector3fSplineInterpolator(arraylength_);
		pathup = new Vector3fSplineInterpolator(arraylength_);

		cam = new Camera(true);
		cam->direction = pathdirection->getCurrentPosition();
		cam->position = pathposition->getCurrentPosition();
		cam->up = pathup->getCurrentPosition();
	}

	CameraPathFollower(int numberofarraylength) {
		initialize(numberofarraylength);
	}

	~CameraPathFollower() {
		delete pathposition;
		delete pathdirection;
		delete pathup;
		delete cam;
	}

	CameraPathFollower(const char* filename)
	{
		FILE* file = fopen(filename, "r");
		if (file == NULL)
		{
			printf("Unable to read file \"%s\"\n", filename);
			system("PAUSE");
			exit(-1);
		}
		else
		{
			printf("Parsing path file \"%s\"\n", filename);
			parseDataFile(file);
		}
	}

	void parseLine(char* line, double* arg1, double* arg2, double* arg3)
	{
		*arg1 = atof(strtok(line, " ,\n\t"));
		*arg2 = atof(strtok(NULL, " ,\n\t"));
		*arg3 = atof(strtok(NULL, " ,\n\t"));
		//		printf("%f\t%f\t%f\n",*arg1,*arg2,*arg3);
	}

	void parseDataFile(FILE* file)
	{
		int section = 0;
		double* x = new double;
		double* y = new double;
		double* z = new double;
		Vector3f* position = NULL;
		Vector3f* direction = NULL;
		Vector3f* up = NULL;
		char* line = new char[maxlinelenght + 1];
		while (feof(file) == 0)
		{
			//make shure we read something usefull
			if (fgets(line, maxlinelenght, file) != NULL && strlen(line) > 1)
			{
				if (section == 0) {
					//get number of positions
					int numofpos = (int)atof(line);
					if (numofpos < 1)
					{
						printf("Invalid number of path positions, %d\n", maximumNumberPoints);
						exit(-1);
					}
					else
					{
						initialize(numofpos);
						section++;
						printf("Created path of %d points\n", numofpos);
					}
				}
				else if (section == 1) {
					parseLine(line, x, y, z);
					position = new Vector3f(
						(float)(*x),
						(float)(*y),
						(float)(*z));
					section++;
				}
				else if (section == 2) {
					parseLine(line, x, y, z);
					direction = new Vector3f(
						(float)(*x),
						(float)(*y),
						(float)(*z));
					section++;
				}
				else if (section == 3) {
					parseLine(line, x, y, z);
					up = new Vector3f(
						(float)(*x),
						(float)(*y),
						(float)(*z));
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
		printf("added %d path points\n", numberPoints);
	}

	void addPathPoint(Vector3f* position0, Vector3f* direction0, Vector3f* up0) {
		//add data
		pathposition->addPathPoint(position0);
		pathdirection->addPathPoint(direction0);
		pathup->addPathPoint(up0);
		numberPoints++;
	}

	void interpolateMotion(int startingpoint, float n) {
		pathposition->interpolateVector(startingpoint, n);
		pathdirection->interpolateVector(startingpoint, n);
		pathup->interpolateVector(startingpoint, n);
	}

	void interpolateMotion(float n) {
		int pos = (int)floor(n);
		float coef = n - float(pos);
		interpolateMotion(pos, coef);
	}

	void updateCamera()
	{
		cam->lookAt();
	}

	int getNumberOfPathPoints() {
		return numberPoints;
	}
};
#endif
