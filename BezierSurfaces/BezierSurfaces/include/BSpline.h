#ifndef _BSPLINE_H_
#define _BSPLINE_H_

#include "prerequisites.h"
#include "Math/Vector3.h"
#include "Objects/Mesh.h"
#include "Materials/BasicMaterial.h"

class BSpline
{
public:

	BSpline();
	BSpline(uint lod, uint order = 0);

	std::vector< std::vector<Vector3> > BSpline::getApproximedSplines() const;
	std::vector<Vector3> getControlPoints() const;

	void setOrder(uint order);
	uint getOrder() const;
	void setLOD(uint lod);
	uint getLOD() const;

	void closeSpline(bool b);
	void approximeBounds(bool b);
	void useUniformNodal(bool b);
	bool isClosed() const;
	bool isApproximed() const;
	bool uniformNodalUsed() const;

	void orderUp();
	void orderDown();

	void addControlPoint(const Vector3& v);
	void insert(uint index, const Vector3& v);
	void remove(uint index);
	Vector3& getVectorAt(uint index);
	void reset();

	void setColor(const Vector3 color);
	Vector3& getColor();
	void showSubCurve(bool visible);
	bool subCurveVisible();

	void approximeSpline();
	void draw();

	void loadParametrisation();
	void nodalVectorUniform();
	void BSpline::nodalVectorCustom(std::vector<float> nodal);

	Vector3 Cox_De_Boor(uint r, float t);

	void extrudeLinear(float reduction, float height, int inter = 20);
	void extrudeRevolution(int inter = 20);
	void extrudeGeneral(const BSpline& ref);

private:

	std::vector<float> mNodalVector;
	std::vector<Vector3> mControlPoints;
	std::vector< std::vector<Vector3> > mApproximedSplines;

	Vector3 mColor;

	uint mOrder;
	uint mLOD;

	bool mIsClosed;
	bool mIsApproximedBounds;
	bool mUseUniformNodal;
	bool mSubCurveVisible;

	// Mesh mSurface;

};

#endif