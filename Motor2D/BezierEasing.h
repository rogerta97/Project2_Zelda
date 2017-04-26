#ifndef _BEZIEREASING_H_
#define _BEZIEREASING_H_

#include "p2Point.h"
#include <vector>

class BezierEasing
{
public:
	//Create a cubic Bezier easing for any p1 and p2 with components between 0 and 1
	BezierEasing(fPoint p1, fPoint p2);

	~BezierEasing();

	//Return progress for desired time if valid curve. -1 if invalid.
	float GetEasingProgress(float t);

private:

	// Calculate support vectors
	float VecACoord(float p1_coord, float p2_coord);
	float VecBCoord(float p1_coord, float p2_coord);
	float VecCCoord(float p1_coord);

	//Calculate the Bezier point
	float CalcBezier(float time, float p1_coord, float p2_coord);

	//Calculate Bezier Slope
	float GetSlope(float time, float p1_coord, float p2_coord);

	float BinarySubdivide(float time, float interval_start, float next_interval_step, float p1_coord, float p2_coord);

	float NewtonRaphsonIterate(float time, float guessed_t, float p1_coord, float p2_coord);

	float GetXForTime(float time);

	//Check if the points are in the valid range
	bool CheckPoints(fPoint p1, fPoint p2);

public:

private:

	fPoint				p0 = { 0,0 };
	fPoint				p1 = { 0,0 };
	fPoint				p2 = { 0,0 };
	fPoint				p3 = { 1,1 };

	std::vector<float>	sample_values;

	float				last_value = 0.0f;

	bool valid = true;

};

#endif // !_BEZIEREASING_H_