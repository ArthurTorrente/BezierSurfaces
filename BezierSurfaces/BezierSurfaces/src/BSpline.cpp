#include "BSpline.h"

BSpline::BSpline()
	: mOrder(3),
	mLOD(15),
	mIsApproximedBounds(false),
	mIsClosed(false),
	mUseUniformNodal(true),
	mColor(Vector3(0.0, 1.0, 0.0))
{
}

BSpline::BSpline(uint lod, uint order)
	: mLOD(lod),
	mOrder(order),
	mIsApproximedBounds(false),
	mIsClosed(false),
	mUseUniformNodal(true)
{
}

std::vector< std::vector<Vector3> > BSpline::getApproximedSplines() const
{
	return mApproximedSplines;
}

std::vector<Vector3> BSpline::getControlPoints() const
{
	return mControlPoints;
}

void BSpline::setOrder(uint order)
{
	mOrder = order;
}

uint BSpline::getOrder() const
{
	return mOrder;
}

void BSpline::setLOD(uint lod)
{
	mLOD = (lod > 2) ? lod : 2;
}

uint BSpline::getLOD() const
{
	return mLOD;
}

void BSpline::closeSpline(bool b)
{
	mIsClosed = b;
}

void BSpline::approximeBounds(bool b)
{
	mIsApproximedBounds = b;
}

void BSpline::useUniformNodal(bool b)
{
	mUseUniformNodal = b;
}

bool BSpline::isClosed() const
{
	return mIsClosed;
}

bool BSpline::isApproximed() const
{
	return mIsApproximedBounds;
}

bool BSpline::uniformNodalUsed() const
{
	return mUseUniformNodal;
}

void BSpline::orderUp()
{
	if (mOrder < mControlPoints.size() - 1)
		setOrder(mOrder + 1);
}

void BSpline::orderDown()
{
	if (mOrder > 1)
		setOrder(mOrder - 1);
}

void BSpline::loadParametrisation()
{
	mNodalVector.clear();
	nodalVectorUniform();
}

void BSpline::nodalVectorUniform()
{
	uint size = mControlPoints.size() + mOrder;

	for (uint i = 0; i < size; ++i)
		mNodalVector.push_back(i);
}

void BSpline::nodalVectorCustom(std::vector<float> nodal)
{
	mNodalVector = nodal;
}

void BSpline::approximeSpline()
{
	mApproximedSplines.clear();

	if (mControlPoints.size() >= mOrder)
	{
		// Ici on check si on doit ou non utiliser un vector nodal uniform
		if (mUseUniformNodal || mNodalVector.size() == 0)
		{
			std::cout << "Uniform nodal vector used." << std::endl;
			loadParametrisation();
		}	

		// Pour approcher les extrémités ou fermer la courbe
		if (mIsApproximedBounds)
		{
			for (uint i = 0; i < mOrder; ++i)
			{
				mNodalVector.insert(mNodalVector.begin(), 0);
				mNodalVector.push_back(*(mNodalVector.end() - 1));

				mControlPoints.insert(mControlPoints.begin(), mControlPoints[0]);
				mControlPoints.push_back(*(mControlPoints.end() - 1));
			}
		}
		else if (mIsClosed)
		{
			for (uint i = 0; i < mOrder; ++i)
			{
				mControlPoints.push_back(mControlPoints[i]);
			}

			// Ici on check si on doit ou non utiliser un vector nodal uniform
			if (mUseUniformNodal || mNodalVector.size() == 0)
			{
				std::cout << "Uniform nodal vector used." << std::endl;
				loadParametrisation();
			}
		}

		uint numberControlPoints = mControlPoints.size();

		// On parcourt maintenant chaque sous courbe Bézier de la spline d'ordre mOrder
		uint r;
		float t;
		for (r = mOrder; r < numberControlPoints; ++r)
		{
			mApproximedSplines.push_back(std::vector<Vector3>());
			// Discrétisation de la sous courbe
			for (t = mNodalVector[r]; t <= mNodalVector[r + 1]; t += (float) 1.0f / mLOD)
			{
				mApproximedSplines.back().push_back(Cox_De_Boor(r, t));
			}
		}

		if (mIsApproximedBounds)
		{
			for (uint i = 0; i < mOrder; ++i)
			{
				mNodalVector.erase(mNodalVector.begin());
				mNodalVector.pop_back();

				mControlPoints.erase(mControlPoints.begin());
				mControlPoints.pop_back();
			}
		}
		else if (mIsClosed)
		{
			for (uint i = 0; i < mOrder; ++i)
			{
				mControlPoints.pop_back();
				mNodalVector.pop_back();
			}
		}
	}

}

void BSpline::draw()
{
	// Dessin des pts de contrôle
	Vector3 v;
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (uint i = 0; i < mControlPoints.size(); i++)
	{
		v = mControlPoints[i];
		glVertex2f(v.x, v.y);
	}
	glEnd();

	glBegin(GL_QUADS);
	for (uint i = 0; i < mControlPoints.size(); i++)
	{
		v = mControlPoints[i];
		glVertex2f(v.x - 2, v.y - 2);
		glVertex2f(v.x + 2, v.y - 2);
		glVertex2f(v.x + 2, v.y + 2);
		glVertex2f(v.x - 2, v.y + 2);
	}
	glEnd();

	// Dessin des pts approximés
	glBegin(GL_LINE_STRIP);
	for (uint i = 0; i < mApproximedSplines.size(); i++)
	{
		if (mSubCurveVisible && i % 2 == 0)
		{
			glColor3f(1.0 - mColor.x, 1.0 - mColor.y, 1.0 - mColor.z);
		}
		else
		{
			glColor3f(mColor.x, mColor.y, mColor.z);
		}

		for (uint j = 0; j < mApproximedSplines[i].size(); ++j)
		{
			v = mApproximedSplines[i][j];
			glVertex2f(v.x, v.y);
		}	
	}
	glEnd();


}

Vector3 BSpline::Cox_De_Boor(uint r, float t)
{
	std::vector < std::vector<Vector3> > points;

	// Instantiation
	for (uint i = 0; i <= mOrder; ++i)
	{
		points.push_back(std::vector<Vector3>(r + 1));
	}

	// Initialisation
	for (uint i = r - mOrder; i <= r; ++i)
	{
		points[0][i] = mControlPoints[i];
	}
		
	// Calcul
	for (uint j = 1; j <= mOrder; ++j)
	{
		for (uint i = r - mOrder + j; i <= r; ++i)
		{
			Vector3 tmp_1 = (points[j - 1][i] * (t - mNodalVector[i]));
			Vector3 tmp_2 = (points[j - 1][i - 1] * (mNodalVector[i - j + mOrder + 1] - t));
			float tmp_3 = (mNodalVector[i - j + mOrder + 1] - mNodalVector[i]);

			points[j][i] = (tmp_1 + tmp_2) / tmp_3;
		}
	}

	return points[mOrder][r];
}

void BSpline::addControlPoint(const Vector3& v)
{
	mControlPoints.push_back(v);
}

void BSpline::insert(uint index, const Vector3& v)
{
	assert(index < mControlPoints.size());

	mControlPoints.insert(mControlPoints.begin() + index, v);
}

void BSpline::remove(uint index)
{
	assert(index < mControlPoints.size());

	mControlPoints.erase(mControlPoints.begin() + index);
}

Vector3& BSpline::getVectorAt(uint index)
{
	assert(index < mControlPoints.size());

	return mControlPoints[index];
}

void BSpline::reset()
{
	mNodalVector.clear();
	mControlPoints.clear();
	mApproximedSplines.clear();
}

void BSpline::setColor(const Vector3 color)
{
	mColor = color;
}

Vector3& BSpline::getColor()
{
	return mColor;
}

void BSpline::showSubCurve(bool visible)
{
	mSubCurveVisible = visible;
}
bool BSpline::subCurveVisible()
{
	return mSubCurveVisible;
}

void BSpline::extrudeLinear(float reduction, float height, int inter)
{
	std::vector<float> T;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> uvs;

	// Préparation du pas
	for (uint i = 0; i < inter; ++i)
	{
		T.push_back((float)i / (inter - 1.0f));
	}

	// Création des vertices
	for (uint i = 0; i < inter; ++i)
	{
		for (uint j = 0; j < mApproximedSplines.size(); ++j)
		{
			std::vector<Vector3> spline = mApproximedSplines[j];
			for (uint k = 0; k < spline.size(); ++k)
			{
				vertices.push_back(spline[k].x * (1 + T[i] * (reduction - 1)));
				vertices.push_back(spline[k].y * (1 + T[i] * (reduction - 1)));
				vertices.push_back(T[i] * height);
			}
		}
	}

	// Récupération du nombre totales de vertices pour la courbe
	uint nbVertices = 0;
	for (uint i = 0; i < mApproximedSplines.size(); ++i)
	{
		nbVertices += mApproximedSplines[i].size();
	}

	// Création des indices
	for (uint i = 0; i < inter - 1; ++i)
	{
		for (uint j = 0; j < nbVertices - 1; ++j)
		{
			// Triangle 1
			indices.push_back(i * nbVertices + j + 0);			// 0
			indices.push_back(i * nbVertices + j + 1);			// 1
			indices.push_back((i + 1) * nbVertices + j);		// 3

			// Triangle 2
			indices.push_back((i + 1) * nbVertices + j);		// 3
			indices.push_back(i * nbVertices + j + 1);			// 1
			indices.push_back((i + 1) * nbVertices + j + 1);	// 2
		}
	}

	// Création des uvs
	for (uint i = 0; i < inter; ++i)
	{
		for (uint j = 0; j < nbVertices; ++j)
		{
			uvs.push_back((float)i / (inter-1));				// u
			uvs.push_back((float)j / (nbVertices-1));			// v
		}
	}

	Geometry g(vertices, indices, uvs);
	// BasicMaterial m("Bspline", Vector3(1.0, 0.0, 0.0));
}

void BSpline::extrudeRevolution(int inter)
{
	std::vector<float> T;
	std::vector<float> vertices;
	std::vector<int> indices;
	std::vector<float> uvs;

	// Préparation du pas
	for (uint i = 0; i < inter; ++i)
	{
		T.push_back(2.0f * (float)i / (inter - 1.0f));
	}

	// Extrusion de la surface

	// Création des vertices
	for (uint i = 0; i < inter; ++i)
	{
		for (uint j = 0; j < mApproximedSplines.size(); ++j)
		{
			std::vector<Vector3> spline = mApproximedSplines[j];
			for (uint k = 0; k < spline.size(); ++k)
			{
				vertices.push_back(spline[k].x * sin(T[i]));
				vertices.push_back(spline[k].y);
				vertices.push_back(spline[k].x * cos(T[i]));
			}
		}
	}

	// Récupération du nombre totales de vertices pour la courbe
	uint nbVertices = 0;
	for (uint i = 0; i < mApproximedSplines.size(); ++i)
	{
		nbVertices += mApproximedSplines[i].size();
	}

	// Création des indices
	for (uint i = 0; i < inter - 1; ++i)
	{
		for (uint j = 0; j < nbVertices - 1; ++j)
		{
			// Triangle 1
			indices.push_back(i * nbVertices + j + 0);			// 0
			indices.push_back(i * nbVertices + j + 1);			// 1
			indices.push_back((i + 1) * nbVertices + j);		// 3

			// Triangle 2
			indices.push_back((i + 1) * nbVertices + j);		// 3
			indices.push_back(i * nbVertices + j + 1);			// 1
			indices.push_back((i + 1) * nbVertices + j + 1);	// 2
		}
	}

	// Création des uvs
	for (uint i = 0; i < inter; ++i)
	{
		for (uint j = 0; j < nbVertices; ++j)
		{
			uvs.push_back((float)i / (inter - 1));				// u
			uvs.push_back((float)j / (nbVertices - 1));			// v
		}
	}

	Geometry g(vertices, indices, uvs);
}

void BSpline::extrudeGeneral(const BSpline& ref)
{
	std::vector<std::vector<Vector3>> ame = ref.getApproximedSplines();

	// On ne va faire l'extrude que si la courbe et le ref ont des points
	if (mApproximedSplines.size() * ame.size() != 0)
	{
		std::vector<float> vertices;
		std::vector<int> indices;
		std::vector<float> uvs;

		// On regroupe toutes les sous courbes en une seule => plus facile à gérer
		std::vector<Vector3> A;
		for (uint i = 0; i < ame.size(); ++i)
		{
			// On parcourt chaque point de la sous courbe
			for (uint j = 0; j < ame[i].size(); ++j)
			{
				A.push_back(ame[i][j]);
			}
		}

		// Création des vertices
		// On parcourt chaque point de l'ame
		Vector3 prev, next;
		for (uint i = 1 ; i < A.size() - 1; ++i)
		{
			// Vecteur tangent
			Vector3 T = A[i + 1] - A[i - 1];

			// On crée un repère local à ce point (pour transformer les points de la courbe)
			Vector3 r(T.y, -T.x, 0.0f);
			Vector3 rcp = r.crossProduct(T);	

			// On normalise le tout
			T.normalise();
			r.normalise();
			rcp.normalise();

			// On parcourt les points de la spline et on les met selon le repère crée
			for (uint j = 0; j < mApproximedSplines.size(); ++j)
			{
				std::vector<Vector3> points = mApproximedSplines[j];
				for (uint k = 0; k < points.size(); ++k)
				{
					Vector3 p = points[k];
					Vector3 newPoint(
						A[i].x + p.x * r.x + p.y * rcp.x,
						A[i].y + p.x * r.y + p.y * rcp.y,
						A[i].z + p.x * r.z + p.y * rcp.z
					);

					vertices.push_back(newPoint.x);
					vertices.push_back(newPoint.y);
					vertices.push_back(newPoint.z);
				}
			}
		}

		// Récupération du nombre totales de vertices pour la courbe
		uint nbVertices = 0;
		for (uint i = 0; i < mApproximedSplines.size(); ++i)
		{
			nbVertices += mApproximedSplines[i].size();
		}

		// Création des indices
		for (uint i = 0; i < A.size() - 1; ++i)
		{
			for (uint j = 0; j < nbVertices - 1; ++j)
			{
				// Triangle 1
				indices.push_back(i * nbVertices + j + 0);			// 0
				indices.push_back(i * nbVertices + j + 1);			// 1
				indices.push_back((i + 1) * nbVertices + j);		// 3

				// Triangle 2
				indices.push_back((i + 1) * nbVertices + j);		// 3
				indices.push_back(i * nbVertices + j + 1);			// 1
				indices.push_back((i + 1) * nbVertices + j + 1);	// 2
			}
		}

		// Création des uvs
		for (uint i = 0; i < A.size() - 1; ++i)
		{
			for (uint j = 0; j < nbVertices; ++j)
			{
				uvs.push_back((float)i / (A.size() - 1));			// u
				uvs.push_back((float)j / (nbVertices - 1));			// v
			}
		}

		// Création de la géométrie
		Geometry g(vertices, indices, uvs);
	}

}