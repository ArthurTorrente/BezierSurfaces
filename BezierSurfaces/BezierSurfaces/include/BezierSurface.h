#ifndef _BEZIER_SURFACE_H_
#define _BEZIER_SURFACE_H_

#include "prerequisites.h"
#include "Math\Vector3.h"

class BezierSurface
{
public:

	BezierSurface();
	BezierSurface(uint number, uint lod = 3);

	void compute();

private:

	uint mNumberControlPoints;
	std::vector< std::vector<Vector3> > mControlPoints;

	uint mLOD;
	std::vector< std::vector<Vector3> > mSurfacePoints;

};

#endif