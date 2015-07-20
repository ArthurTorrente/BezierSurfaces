#ifndef _BEZIER_SURFACE_H_
#define _BEZIER_SURFACE_H_

#include "prerequisites.h"
#include "Math/Vector3.h"
#include "Objects/Geometry.h"

class BezierSurface
{
public:

	BezierSurface(uint number = 10, uint lod = 30);

	void compute();
	Geometry& getGeometry();
	Geometry& getControl();

	const std::vector<std::vector<Vector3>>& getControlPoints() const;

private:

	uint mNumberControlPoints;
	std::vector< std::vector<Vector3> > mControlPoints;

	uint mLOD;
	std::vector< std::vector<Vector3> > mSurfacePoints;

	Geometry mGeometry;
	Geometry mControlGeometry;

};

#endif