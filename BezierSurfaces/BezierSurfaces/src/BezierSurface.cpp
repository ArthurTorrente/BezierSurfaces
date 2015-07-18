#include "BezierSurface.h"

static Vector3 DeCasteljau(float step, std::vector<Vector3> points)
{
	int n = points.size() - 1;

	// Initialisation
	std::vector< std::vector<Vector3> > P;
	P.push_back(std::vector<Vector3>());
	for (uint i = 0; i <= n; ++i)
	{
		P[0].push_back(points[i]);
	}

	// Calcul
	Vector3 point;
	for (uint i = 1; i <= n; ++i)
	{
		P.push_back(std::vector<Vector3>());
		for (uint j = 0; j <= n - i; ++j)
		{
			point = P[i - 1][j] * (1 - step) + P[i - 1][j + 1] * step;
			P[i].push_back(point);
		}
	}

	return P.back().front();

}

BezierSurface::BezierSurface()
	: mNumberControlPoints(3),
	mLOD(3)
{
	compute();
}

BezierSurface::BezierSurface(uint number, uint lod)
{
	if (number < 3)
		number = 3;
	mNumberControlPoints = number;

	float offset = 200.0f;
	/*	Explication de ce calcul
		(number / 2.0f * offset) => permet de diviser par deux et donc de ce centrer en 0
		petit problème est que je suis centré mais décalé d'un demi offset d'où la deuxième partie
	*/
	float back = (float) (number / 2.0f * offset) - (offset / 2.0f);
	for (uint i = 0; i < mNumberControlPoints; ++i)
	{
		mControlPoints.push_back(std::vector<Vector3>());
		for (uint j = 0; j < mNumberControlPoints; ++j)
		{
			mControlPoints[i].push_back(Vector3(-back + i * offset, 0, -back + j * offset));
		}
	}

	mLOD = lod;

	compute();
}

void BezierSurface::compute()
{
	mSurfacePoints.clear();

	// On parcourt le pas
	for (uint i = 0; i < mLOD; ++i)
	{
		mSurfacePoints.push_back(std::vector<Vector3>());
		std::vector<Vector3> tmp;

		for (uint j = 0; j < mNumberControlPoints; ++j)
		{
			tmp.push_back(DeCasteljau((float)j / mLOD, mControlPoints[j]));
		}

		for (uint j = 0; j < mLOD; ++j)
		{
			mSurfacePoints[i].push_back(DeCasteljau((float)j / mLOD, tmp));
		}
	}

}

