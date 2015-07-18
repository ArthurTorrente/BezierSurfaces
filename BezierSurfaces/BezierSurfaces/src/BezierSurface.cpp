#include "BezierSurface.h"
#include "Objects\Geometry.h"

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
		petit probl�me est que je suis centr� mais d�cal� d'un demi offset d'o� la deuxi�me partie
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

	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> uvs;

	// On parcourt le pas
	for (uint i = 0; i <= mLOD; ++i)
	{
		mSurfacePoints.push_back(std::vector<Vector3>());
		std::vector<Vector3> tmp;

		for (uint j = 0; j < mNumberControlPoints; ++j)
		{
			tmp.push_back(DeCasteljau((float)i / mLOD, mControlPoints[j]));
		}

		for (uint j = 0; j <= mLOD; ++j)
		{
			Vector3 point = DeCasteljau((float)j / mLOD, tmp);

			mSurfacePoints[i].push_back(point);

			vertices.push_back(point.x);
			vertices.push_back(point.y);
			vertices.push_back(point.z);
		}
	}

	// Cr�ation des indices
	for (uint i = 0; i < mLOD; ++i)
	{
		for (uint j = 0; j < mLOD; ++j)
		{
			// Triangle 1
			indices.push_back(i * mLOD + j + 0);			// 0
			indices.push_back(i * mLOD + j + 1);			// 1
			indices.push_back((i + 1) * mLOD + j);			// 3

			// Triangle 2
			indices.push_back((i + 1) * mLOD + j);			// 3
			indices.push_back(i * mLOD + j + 1);			// 1
			indices.push_back((i + 1) * mLOD + j + 1);		// 2
		}
	}

	// Cr�ation des uvs
	for (uint i = 0; i < mLOD; ++i)
	{
		for (uint j = 0; j < mLOD; ++j)
		{
			uvs.push_back((float)i / (mLOD - 1));			// u
			uvs.push_back((float)j / (mLOD - 1));			// v
		}
	}

	// Cr�ation de la g�om�trie
	Geometry g(vertices, indices, uvs);

}
