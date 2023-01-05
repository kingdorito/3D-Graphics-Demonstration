#include "Model.h"

Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

}


//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

}

Model::~Model()
{

}

//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}


size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
}

void Model::ApplyTransformToLocalVertices(const Matrix &transform)
{
	_transform.clear();
	int verticesSize = int(_vertices.size());
	for (int i = 0; i < verticesSize; i++)
	{
		_transform.push_back(transform * _vertices[i]);
	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &transform)
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i] = transform * _transform[i];
	}
}

void Model::Dehomogenize()
{
	int transformSize = int(_transform.size());
	for (int i = 0; i < transformSize; i++)
	{
		_transform[i].Dehomogenize();
	}
}

void Model::CalculateBackfaces(Camera camera)
{
	//TODO : Similar to the philosofy of drawwireframe, create a method that for each triangle will calculate if this triangle is culled or not
	int polygonSize = int(_polygons.size());
	for (int i = 0; i < polygonSize; i++)
	{
		//Get the indices

		//Get the vertices

		//Contructing vectors by subtracting vertices

		//Calculating the normal (cross product) between the two vectors

		//Creating an eye-vector by subtracting the camera position from vertex0

		// Calculate the dot product and check it
		// If <0 the mark the polygon as culled. See the code bellow



	/*	bool cull;
			if (float dotProduct = normal.DotProduct(eyeVector) < 0)
			{
				//Flag this polygon for culling
				cull = true;
				_polygons[i].SetCull(cull);
			}
			else
			{
				//Flag the polygon to not be culled any more, so it can be displayed when it rotates round
				cull = false;
				_polygons[i].SetCull(cull);
			}
	*/

	}
}


void Model::Sort(void)
{
	//TODO : Create a method for sorting the polygons	
}





