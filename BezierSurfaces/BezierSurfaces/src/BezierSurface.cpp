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

BezierSurface::BezierSurface(uint number, uint lod)
	: mGeometry(std::vector<float>(), std::vector<uint>()),
	mControlGeometry(std::vector<float>(), std::vector<uint>())
{
	if (number < 3)
		number = 3;
	mNumberControlPoints = number;

	float offset = 400.0f;
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
			mControlPoints[i].push_back(Vector3(-back + i * offset, ((i % 2 == 0) ? 0.0f : 500.0f), -back + j * offset));
		}
	}

	mControlPoints[0][0].y += 500;

	// mControlPoints.insert(mControlPoints.begin() + mNumberControlPoints / 2, mControlPoints[2]);
	// mControlPoints.insert(mControlPoints.begin() + mNumberControlPoints / 2, mControlPoints[2]);


	// Création du mesh des pts de control
	std::vector<float> vertices;
	std::vector<uint> indices;
	for (uint i = 0; i < mNumberControlPoints; ++i)
	{
		for (uint j = 0; j < mNumberControlPoints; ++j)
		{
			vertices.push_back(mControlPoints[i][j].x);
			vertices.push_back(mControlPoints[i][j].y);
			vertices.push_back(mControlPoints[i][j].z);
		}
	}

	for (uint i = 0; i < mNumberControlPoints - 1; ++i)
	{
		for (uint j = 0; j < mNumberControlPoints - 1; ++j)
		{
			indices.push_back(i * mNumberControlPoints + j + 0);			// 0
			indices.push_back(i * mNumberControlPoints + j + 1);			// 1
			indices.push_back((i + 1) * mNumberControlPoints + j);			// 3

			// Triangle 2
			indices.push_back((i + 1) * mNumberControlPoints + j);			// 3
			indices.push_back(i * mNumberControlPoints + j + 1);			// 1
			indices.push_back((i + 1) * mNumberControlPoints + j + 1);		// 2
		}
	}

	mControlGeometry = Geometry(vertices, indices);

	mLOD = lod;

	compute();
}

void BezierSurface::compute()
{
	mSurfacePoints.clear();

	std::vector<float> vertices;
	std::vector<uint> indices;
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



	// Création des indices
	uint decal = mLOD + 1;
	for (uint i = 0; i < mLOD; ++i)
	{
		for (uint j = 0; j < mLOD; ++j)
		{
			// Triangle 1
			indices.push_back(i * decal + j + 0);			// 0
			indices.push_back(i * decal + j + 1);			// 1
			indices.push_back((i + 1) * decal + j);			// 3

			// Triangle 2
			indices.push_back((i + 1) * decal + j);			// 3
			indices.push_back(i * decal + j + 1);			// 1
			indices.push_back((i + 1) * decal + j + 1);		// 2
		}
	}

	// Création des uvs
	for (uint i = 0; i < mLOD; ++i)
	{
		for (uint j = 0; j < mLOD; ++j)
		{
			uvs.push_back((float)i / (mLOD - 1));			// u
			uvs.push_back((float)j / (mLOD - 1));			// v
		}
	}

	// Création de la géométrie
	mGeometry = Geometry(vertices, indices, uvs);

}

Geometry& BezierSurface::getGeometry()
{
	return mGeometry;
}

Geometry& BezierSurface::getControl()
{
	return mControlGeometry;
}

const std::vector<std::vector<Vector3>>& BezierSurface::getControlPoints() const
{
	return mControlPoints;
}