#ifndef _STRUCTDEF_H_
#define _STRUCTDEF_H_

// general geometric class 
class Point3D {
public:
	// constructor
	Point3D() : x(0.0f), y(0.0f), z(0.0f) {}
	Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
	Point3D(const Point3D & p) : x(p.x), y(p.y), z(p.z) {}
	// destructor
	virtual ~Point3D(){}

	// operator
	Point3D& operator= (const Point3D &);
	Point3D  operator+ (const Point3D &);
	Point3D  operator- (const Point3D &);
	Point3D  operator* (float f);
	Point3D  operator/ (float d);

	float	dot(const Point3D &);
	Point3D cross(const Point3D &);
	Point3D& unify();
	float	length();
	void	zero();

public:
	float x, y, z;
};

///////////////////////////////////////////////

namespace VolumeMesh {

typedef std::set<int> IntSet;

// class vertex
class Vertex {
public:
	// constructor
	Vertex();
	Vertex(const Point3D&);
	Vertex(const Vertex& );

	const Vertex& operator= (const Vertex&);
	// destructor
	virtual ~Vertex() {}

public:
	Point3D	  coordinate;             // the coordinate of vertex 

	IntSet edgeIndices;			// link of edges containing the vertex;
	IntSet faceIndices;			// link of faces containing the vertex;
	IntSet cellIndices;			// link of cells containing the vertex;
};

// class edge
class Edge {
public:
	// constructor
	Edge();
	Edge(const Edge&);

	const Edge& operator= (const Edge&);
	// destructor
	virtual ~Edge(){}

public:
	int    vertexIndices[2];
	IntSet faceIndices;
	IntSet cellIndices;

};

// class face
class Face {
public:
	Face();
	Face(const Face&);
	const Face& operator= (const Face&);

	virtual ~Face() {}

public:
	IntSet vertexIndices;
	IntSet edgeIndices;
	int	   cellIndices[2];
};

// class cell
class Cell {
public:
	Cell() {}
	Cell(const Cell&);
	const Cell& operator= (const Cell&);

	virtual ~Cell() {}

public:
	IntSet vertexIndices;
	IntSet edgeIndices;
	IntSet faceIndices;
};

// volume mesh model
class VolumeModel {
public:
	VolumeModel() {}
	virtual ~VolumeModel();

	void AddVertex(const Point3D& p);
	void AddEdge(int vs, int ve);
	void AddFace(const std::vector<int>& edgeLink);
	void AddCell(const std::vector<int>& faceLink);

public:
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Face> faces;
	std::vector<Cell> cells;	
};

class Hexahedron {
public:
	Hexahedron();
	virtual ~Hexahedron();
	void InitCube();
	void ApproximateSubdivision();
	void InterpolateSubdivision();

protected:
	void CalcCellPoints();
	void CalcFacePoints();
	void CalcFacePointsInter();
	void CalcEdgePoints();
	void CalcEdgePointsInter();
	void CalcNewVertices();
	void CalcNewVerticesInter();
	void NewVertexList(VolumeModel* pNewModel);
	void NewEdgeList(VolumeModel* pNewModel);
	void NewFaceList(VolumeModel* pNewModel);
	void NewCellList(VolumeModel* pNewModel);

	void FreeMemory();
	
public:
	VolumeModel* m_pHexModel;

protected:
	std::vector<Point3D> newVertices;
	std::vector<Point3D> edgePoints;
	std::vector<Point3D> facePoints;
	std::vector<Point3D> cellPoints;

private:
	// count the number of  subdivision 
	int subdivCounter;
};

} // namespace VolumeMesh




#endif

