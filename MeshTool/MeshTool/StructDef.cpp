#include "stdafx.h"
#include "StructDef.h"
#include "FileManager.h"


// operator
Point3D& Point3D::operator= (const Point3D &p) {
	if (this == &p)
		return *this;
	this->x = p.x;
	this->y = p.y;
	this->z = p.z;

	return *this;
}

Point3D  Point3D::operator+ (const Point3D &p){
	return Point3D(x + p.x, y + p.y, z + p.z);
}

Point3D  Point3D::operator- (const Point3D &p){
	return Point3D(x - p.x, y - p.y, z - p.z);
}

Point3D  Point3D::operator* (float f){
	return Point3D(x * f, y * f, z * f);
}

Point3D  Point3D::operator/ (float d){
	if (d < DELTA && d > -DELTA)
		return Point3D();
	return Point3D(x / d, y /d, z / d);
}

float Point3D::dot(const Point3D &p){
	return x * p.x + y * p.y + z * p.z;
}

Point3D Point3D::cross(const Point3D &p){
	return Point3D(y*p.z - z*p.y, z*p.x - x*p.z, x*p.y - y*p.x);
}

float Point3D::length(){
	return sqrt(x * x + y * y + z * z);
}

void Point3D::zero() {
	x = y = z = 0.0;
}

Point3D& Point3D::unify() {
	float len = length();
	if( len < 1.0e-20 )
	{
		x =rand()%1000+20;
		y =rand()%1000+20;
		z =rand()%1000+20;
		len=length();
		x/=len;
		y/=len;
		z/=len;
	} else {
		len = 1.0f/len;
		x *= len;
		y *= len;
		z *= len;
	}

	return *this;
}

//////////////////////////////////////

namespace VolumeMesh {

// vertex 
Vertex::Vertex() {
	coordinate = Point3D();
}

Vertex::Vertex(const Point3D& p) {
	coordinate = p;
}

Vertex::Vertex(const Vertex& v) {
	coordinate  = v.coordinate;
	edgeIndices = v.edgeIndices;
	faceIndices = v.faceIndices;
	cellIndices = v.cellIndices;
}

const Vertex& Vertex::operator= (const Vertex& v) {
	if (this == &v)
		return (*this);

	coordinate  = v.coordinate;
	edgeIndices = v.edgeIndices;
	faceIndices = v.faceIndices;
	cellIndices = v.cellIndices;
	return (*this);
}

// edge
Edge::Edge() {
	vertexIndices[0] = vertexIndices[1] = -1;
}

Edge::Edge(const Edge& e) {
	vertexIndices[0] = e.vertexIndices[0];
	vertexIndices[1] = e.vertexIndices[1];
	faceIndices = e.faceIndices;
	cellIndices = e.cellIndices;
}

const Edge& Edge::operator= (const Edge& e) {
	if (this == &e)
		return *this;
	vertexIndices[0] = e.vertexIndices[0];
	vertexIndices[1] = e.vertexIndices[1];
	faceIndices = e.faceIndices;
	cellIndices = e.cellIndices;

	return *this;
}


// face
Face::Face() {
	cellIndices[0] = cellIndices[1] = -1;
}

Face::Face(const Face& f) {
	vertexIndices = f.vertexIndices;
	edgeIndices   = f.edgeIndices;
	cellIndices[0] = f.cellIndices[0];
	cellIndices[1] = f.cellIndices[1];
}

const Face& Face::operator= (const Face& f) {
	if (this == &f)
		return *this;
	vertexIndices = f.vertexIndices;
	edgeIndices   = f.edgeIndices;
	cellIndices[0] = f.cellIndices[0];
	cellIndices[1] = f.cellIndices[1];

	return *this;
}


// cell

Cell::Cell(const Cell& c) {
	vertexIndices = c.vertexIndices;
	edgeIndices   = c.edgeIndices;
	faceIndices   = c.faceIndices;
}

const Cell& Cell::operator= (const Cell& c) {
	if (this == &c)
		return *this;

	vertexIndices = c.vertexIndices;
	edgeIndices   = c.edgeIndices;
	faceIndices   = c.faceIndices;

	return *this;
}
//-------------------------------------
VolumeModel::~VolumeModel() {
	vertices.clear();
	edges.clear();
	faces.clear();
	cells.clear();
}

//-------------------------------------

void VolumeModel::AddVertex(const Point3D& p) {
	Vertex vert;
	vert.coordinate = p;
	vertices.push_back(vert);
}

//-------------------------------------

void VolumeModel::AddEdge(int vs, int ve) {
	Edge e;
	e.vertexIndices[0] = vs;
	e.vertexIndices[1] = ve;

	vertices[vs].edgeIndices.insert(edges.size());
	vertices[ve].edgeIndices.insert(edges.size());

	edges.push_back(e);
}

//-------------------------------------

void VolumeModel::AddFace(const std::vector<int>& edgeLink) {
	Face f;
	std::set<int> vertIndex;
		
	std::vector<int>::const_iterator eIter = edgeLink.begin();
	for (; eIter != edgeLink.end(); ++ eIter) {
		f.edgeIndices.insert(*eIter);

		edges[*eIter].faceIndices.insert(faces.size());

		vertIndex.insert(edges[*eIter].vertexIndices[0]);
		vertIndex.insert(edges[*eIter].vertexIndices[1]);
	}

	std::set<int>::iterator vertexIter = vertIndex.begin();
	for (; vertexIter != vertIndex.end(); ++ vertexIter) {
		f.vertexIndices.insert(*vertexIter);

		vertices[*vertexIter].faceIndices.insert(faces.size());
	}

	faces.push_back(f);
}

//-------------------------------------

void VolumeModel::AddCell(const std::vector<int>& faceLink) {
	Cell c;
	std::set<int> edgeIndex;
	std::set<int> vertIndex;

	std::vector<int>::const_iterator fIter = faceLink.begin();
	for (; fIter != faceLink.end(); ++ fIter) {
		c.faceIndices.insert(*fIter);
			
		if (-1 == faces[*fIter].cellIndices[0]) 
			faces[*fIter].cellIndices[0] = cells.size();
		else if (-1 == faces[*fIter].cellIndices[1])
			faces[*fIter].cellIndices[1] = cells.size();
		else {
			std::cerr << "Error: volume model is not manifold mesh!" << std::endl;
			return;
		}


		std::set<int>::iterator edgeOfFaceIter = faces[*fIter].edgeIndices.begin();
		for (; edgeOfFaceIter != faces[*fIter].edgeIndices.end(); ++ edgeOfFaceIter) {
			edgeIndex.insert(*edgeOfFaceIter);
		}

		std::set<int>::iterator vertOfFaceIter = faces[*fIter].vertexIndices.begin();
		for (; vertOfFaceIter != faces[*fIter].vertexIndices.end(); ++ vertOfFaceIter) {
			vertIndex.insert(*vertOfFaceIter);
		}
	}

	std::set<int>::iterator edgeOfCellIter = edgeIndex.begin();
	for (; edgeOfCellIter != edgeIndex.end(); ++ edgeOfCellIter) {
		c.edgeIndices.insert(*edgeOfCellIter);

		edges[*edgeOfCellIter].cellIndices.insert(cells.size());
	}
		
	std::set<int>::iterator vertOfCellIter = vertIndex.begin();
	for (; vertOfCellIter != vertIndex.end(); ++ vertOfCellIter) {
		c.vertexIndices.insert(*vertOfCellIter);

		vertices[*vertOfCellIter].cellIndices.insert(cells.size());
	}

	cells.push_back(c);
}

//-------------------------------------

Hexahedron::Hexahedron()  : subdivCounter(0)
{
	m_pHexModel = new VolumeModel;
	//InitCube();
}

//-------------------------------------

Hexahedron::~Hexahedron() {
	if (m_pHexModel)
		delete m_pHexModel;
	m_pHexModel = NULL;
	FreeMemory();
}

/************************************************************************/
/* init a cube                                                          */
/************************************************************************/
void Hexahedron::InitCube() {
	float coordinate[8][3] = {
		{ 1.0,  1.0,  1.0}, {-1.0,  1.0,  1.0},
		{-1.0, -1.0,  1.0}, { 1.0, -1.0,  1.0},
		{ 1.0,  1.0, -1.0}, {-1.0,  1.0, -1.0},
		{-1.0, -1.0, -1.0}, { 1.0, -1.0, -1.0}
	};	

	int edges_[12][2] = {
		{0, 1},{1, 2},{2, 3},{3, 0},
		{4, 5},{5, 6},{6, 7},{7, 4},
		{0, 4},{1, 5},{2, 6},{3, 7}
	};

	int faces_[6][4] = {
		{0, 1, 2, 3},{4, 5, 6, 7},
		{0, 8, 4, 9},{1, 9, 5, 10},
		{2, 11, 6, 10},{3, 8, 7, 11},
	};

	int cells_[6] = {0, 1, 2, 3, 4, 5};

	int i;

	// init vertex list
	int numOfVertices = 8;
	for (i = 0; i < numOfVertices; ++ i) {
		/*Vertex v(Point3D(coordinate[i][0], coordinate[i][1], coordinate[i][2]));
		m_pHexModel->vertices.push_back(v);*/
		m_pHexModel->AddVertex(Point3D(coordinate[i][0], coordinate[i][1], coordinate[i][2]));
	}

	// init edge list
	int numOfEdges = 12;
	for (i = 0; i < numOfEdges; ++i) {
		m_pHexModel->AddEdge(edges_[i][0], edges_[i][1]);
	}
	/*Edge e;
	for (i = 0; i < numOfEdges; ++ i) {
		int j, k;
		if (i >= 0 && i < 3) {
			j = i;
			k = i + 1;
		} else if (i >= 4 && i < 7) {
			j = i;
			k = i + 1;
		} else if (i >= 8 && i < numOfEdges) {
			j = i - 8;
			k = j + 4;
		} else if (i == 3 || i == 7) {
			j = i;
			k = i - 3;
		}
		e.vertexIndices[0] = j;
		e.vertexIndices[1] = k;
		m_pHexModel->edges.push_back(e);
		m_pHexModel->vertices[j].edgeIndices.insert(i);
		m_pHexModel->vertices[k].edgeIndices.insert(i);
	}*/

	// init face list
	const int numOfFaces = 6;
	for (i = 0; i < numOfFaces; ++i) {
		std::vector<int> e;
		for (unsigned int j = 0u; j < 4; ++j) {
			e.push_back(faces_[i][j]);
		}
		m_pHexModel->AddFace(e);
	}
	
	/*
	const int numOfVerticesInFace = 4;
	const int numOfEdgesInFace = 4;
	Face face;
	for (i = 0; i < numOfFaces; ++ i) {

		if (i >= 0 && i <= 1) {
			int j, indexTemp;
			for (j = 0; j < numOfVerticesInFace; ++ j) {
				indexTemp = j + i * numOfVerticesInFace;
				face.vertexIndices.insert(indexTemp);
				m_pHexModel->vertices[indexTemp].faceIndices.insert(i);
			}

			m_pHexModel->faces.push_back(face);

			for (j = 0; j < numOfEdgesInFace; ++ j) {
				indexTemp = j + i * numOfEdgesInFace;
				m_pHexModel->faces[i].edgeIndices.insert(indexTemp);
				m_pHexModel->edges[indexTemp].faceIndices.insert(i);
			}

		} else if (i >= 2 && i < numOfFaces) {
				
			int indexOfVertexInFace[numOfVerticesInFace] = {
				i - 2,
				(i - 1) % 4,
				i + 2,
				(i + 3) == 8 ? 4 : i + 3
			};

			int j;
			for (j = 0; j < numOfVerticesInFace; ++ j) {
				face.vertexIndices.insert(indexOfVertexInFace[j]);
				m_pHexModel->vertices[indexOfVertexInFace[j]].faceIndices.insert(i);
			}

			m_pHexModel->faces.push_back(face);	

			int indexOfEdgeInFace[numOfEdgesInFace] = {
				i - 2, 
				i + 6,
				i + 2,
				i == 5 ? 8 : i + 7
			};
			for (j = 0; j < numOfEdgesInFace; ++ j) {
				m_pHexModel->faces[i].edgeIndices.insert(indexOfEdgeInFace[j]);
				m_pHexModel->edges[indexOfEdgeInFace[j]].faceIndices.insert(i);
			}
		}
		face.vertexIndices.clear();
		face.edgeIndices.clear();
	}*/

	// init cell list
	std::vector<int> c(cells_, cells_ + 6);
	m_pHexModel->AddCell(c);
	/*int numOfCells = 1;
	Cell cell;
	for (i = 0; i < numOfVertices; ++ i) {
		cell.vertexIndices.insert(i);
		m_pHexModel->vertices[i].cellIndices.insert(0);
	}
	for (i = 0; i < numOfEdges; ++ i) {
		cell.edgeIndices.insert(i);
		m_pHexModel->edges[i].cellIndices.insert(0);
	}
	for (i = 0; i < numOfFaces; ++ i) {
		cell.faceIndices.insert(i);
		m_pHexModel->faces[i].cellIndices[0] = 0;
		m_pHexModel->faces[i].cellIndices[1] = -1;
	}
	m_pHexModel->cells.push_back(cell);*/

	subdivCounter = 0;
	printf("----------Init Cube--------------\n");
	printf("vertex: %d\n  edge: %d\n  face: %d\n  cell: %d\n",
		m_pHexModel->vertices.size(),
		m_pHexModel->edges.size(),
		m_pHexModel->faces.size(),
		m_pHexModel->cells.size());

}

void Hexahedron::FreeMemory() {
	newVertices.clear();
	edgePoints.clear();
	facePoints.clear();
	cellPoints.clear();
}


/************************************************************************/
/* calculate cell points after volume subdivision                       */
/************************************************************************/
void Hexahedron::CalcCellPoints() {

	Point3D cent;
	std::vector<Cell>::iterator cellIter = m_pHexModel->cells.begin();
	for (; cellIter != m_pHexModel->cells.end(); ++ cellIter) {
		cent.zero();
		std::set<int>::iterator vertexOfCellIter = cellIter->vertexIndices.begin();
		for (; vertexOfCellIter != cellIter->vertexIndices.end(); ++ vertexOfCellIter)
			cent = cent + m_pHexModel->vertices[*vertexOfCellIter].coordinate;
		cent = cent / cellIter->vertexIndices.size();
		cellPoints.push_back(cent);
	}
}
/************************************************************************/
/* calculate face points after approximating subdivision                */
/************************************************************************/
void Hexahedron::CalcFacePoints() {

	Point3D cent;
	std::vector<Face>::iterator faceIter = m_pHexModel->faces.begin();
	for (; faceIter != m_pHexModel->faces.end(); ++faceIter) {
		cent.zero();
		std::set<int>::iterator vertexOfFaceIter = faceIter->vertexIndices.begin();
		for (; vertexOfFaceIter != faceIter->vertexIndices.end(); ++ vertexOfFaceIter) {
			cent = cent + m_pHexModel->vertices[*vertexOfFaceIter].coordinate;
		}
		cent = cent / faceIter->vertexIndices.size();

		if (faceIter->cellIndices[0] > -1 && 
			faceIter->cellIndices[1] > -1) {
				cent = cent * 0.5 + (cellPoints[faceIter->cellIndices[0]]
					+ cellPoints[faceIter->cellIndices[1]]) * 0.25;
		}

		facePoints.push_back(cent);
	}
}


/************************************************************************/
/* calculate face points after interpolating subdivision                */
/************************************************************************/
void Hexahedron::CalcFacePointsInter() {

	Point3D cent;
	std::vector<Face>::iterator faceIter = m_pHexModel->faces.begin();
	for (; faceIter != m_pHexModel->faces.end(); ++faceIter) {
		cent.zero();
		std::set<int>::iterator vertexOfFaceIter = faceIter->vertexIndices.begin();
		for (; vertexOfFaceIter != faceIter->vertexIndices.end(); ++ vertexOfFaceIter) {
			cent = cent + m_pHexModel->vertices[*vertexOfFaceIter].coordinate;
		}
		cent = cent / faceIter->vertexIndices.size();

		/*if (faceIter->cellIndices[0] > -1 && 
			faceIter->cellIndices[1] > -1) {
				cent = cent * 0.5 + (cellPoints[faceIter->cellIndices[0]]
				+ cellPoints[faceIter->cellIndices[1]]) * 0.25;
		}*/

		facePoints.push_back(cent);
	}
}
/************************************************************************/
/* calculate edge points after approximating subdivision                */
/************************************************************************/
void Hexahedron::CalcEdgePoints() {

	// calculate edge point
	Point3D cent;
	std::vector<Edge>::iterator edgeIter = m_pHexModel->edges.begin();
	for (; edgeIter != m_pHexModel->edges.end(); ++ edgeIter) {
		cent.zero();
		cent = (m_pHexModel->vertices[edgeIter->vertexIndices[0]].coordinate + 
			m_pHexModel->vertices[edgeIter->vertexIndices[1]].coordinate) * 0.5;
		int numOfCells = edgeIter->cellIndices.size();
		int numOfFaces = edgeIter->faceIndices.size(); 
		if (numOfCells == numOfFaces) {

			Point3D avgFacePoint;
			avgFacePoint.zero();
			std::set<int>::iterator faceWithEdgeIter = edgeIter->faceIndices.begin();
			for (; faceWithEdgeIter != edgeIter->faceIndices.end(); 
				++ faceWithEdgeIter) {
				avgFacePoint = avgFacePoint + facePoints[*faceWithEdgeIter] ;
			}

			avgFacePoint = avgFacePoint / numOfFaces;

			Point3D avgCellPoint;
			avgCellPoint.zero();
			std::set<int>::iterator cellWithEdgeIter = edgeIter->cellIndices.begin();
			for (; cellWithEdgeIter != edgeIter->cellIndices.end(); 
				++ cellWithEdgeIter) {
				avgCellPoint = avgCellPoint + cellPoints[*cellWithEdgeIter];
			}

			avgCellPoint = avgCellPoint / numOfCells;

			cent = (cent + avgFacePoint + avgCellPoint) / 3;

		} else {

			Point3D avgBorderFacePoint;
			avgBorderFacePoint.zero();
			int coutBorderFaces = 0;
			std::set<int>::iterator faceWithEdgeIter = edgeIter->faceIndices.begin();
			for (; faceWithEdgeIter != edgeIter->faceIndices.end();
				++ faceWithEdgeIter)
			{
				if (m_pHexModel->faces[*faceWithEdgeIter].cellIndices[0] == -1 ||
					m_pHexModel->faces[*faceWithEdgeIter].cellIndices[1] == -1)
				{
					avgBorderFacePoint = avgBorderFacePoint + facePoints[*faceWithEdgeIter];
					++ coutBorderFaces;
				}
			}

			avgBorderFacePoint = avgBorderFacePoint / coutBorderFaces;

			cent = (cent + avgBorderFacePoint) * 0.5;

		}

		edgePoints.push_back(cent);
	}

}



/************************************************************************/
/* calculate edge points after interpolating subdivision                */
/************************************************************************/
void Hexahedron::CalcEdgePointsInter() {

	Point3D cent;
	std::vector<Edge>::iterator edgeIter = m_pHexModel->edges.begin();
	for (; edgeIter != m_pHexModel->edges.end(); ++ edgeIter) {
		cent.zero();
		cent = (m_pHexModel->vertices[edgeIter->vertexIndices[0]].coordinate + 
			m_pHexModel->vertices[edgeIter->vertexIndices[1]].coordinate) * 0.5;
		/*int numOfCells = edgeIter->cellIndices.size();
		int numOfFaces = edgeIter->faceIndices.size(); 
		if (numOfCells == numOfFaces) {

		Point3D avgFacePoint;
		avgFacePoint.zero();
		std::set<int>::iterator faceWithEdgeIter = edgeIter->faceIndices.begin();
		for (; faceWithEdgeIter != edgeIter->faceIndices.end(); 
		++ faceWithEdgeIter) {
		avgFacePoint = avgFacePoint + facePoints[*faceWithEdgeIter] ;
		}

		avgFacePoint = avgFacePoint / numOfFaces;

		Point3D avgCellPoint;
		avgCellPoint.zero();
		std::set<int>::iterator cellWithEdgeIter = edgeIter->cellIndices.begin();
		for (; cellWithEdgeIter != edgeIter->cellIndices.end(); 
		++ cellWithEdgeIter) {
		avgCellPoint = avgCellPoint + cellPoints[*cellWithEdgeIter];
		}

		avgCellPoint = avgCellPoint / numOfCells;

		cent = (cent + avgFacePoint + avgCellPoint) / 3;

		}*/ 
		/*else {

		  Point3D avgBorderFacePoint;
		  avgBorderFacePoint.zero();
		  int coutBorderFaces = 0;
		  std::set<int>::iterator faceWithEdgeIter = edgeIter->faceIndices.begin();
		  for (; faceWithEdgeIter != edgeIter->faceIndices.end();
		  ++ faceWithEdgeIter)
		  {
		  if (m_pHexModel->faces[*faceWithEdgeIter].cellIndices[0] == -1 ||
		  m_pHexModel->faces[*faceWithEdgeIter].cellIndices[1] == -1)
		  {
		  avgBorderFacePoint = avgBorderFacePoint + facePoints[*faceWithEdgeIter];
		  ++ coutBorderFaces;
		  }
		  }

		  avgBorderFacePoint = avgBorderFacePoint / coutBorderFaces;

		  cent = (cent + avgBorderFacePoint) * 0.5;

		  }*/

		edgePoints.push_back(cent);
	}

}
/************************************************************************/
/* calculate new vertices after approximating subdivision               */
/************************************************************************/
void Hexahedron::CalcNewVertices() {

	Point3D cent;
	std::vector<Vertex>::iterator vertexIter = m_pHexModel->vertices.begin();
	for (; vertexIter != m_pHexModel->vertices.end(); ++ vertexIter) 
	{
		cent.zero();
		cent = vertexIter->coordinate;
		int numOfFacesWithVert = vertexIter->faceIndices.size();

		if (numOfFacesWithVert != 12) 
		{
			int numOfBorderFaces = 0;
			Point3D avgBorderFacePoint;
			avgBorderFacePoint.zero();
			std::set<int>::iterator borderFaceWithVertIter = vertexIter->faceIndices.begin();
			for (; borderFaceWithVertIter != vertexIter->faceIndices.end(); 
				++ borderFaceWithVertIter)
			{
					if (m_pHexModel->faces[*borderFaceWithVertIter].cellIndices[0] == -1 ||
						m_pHexModel->faces[*borderFaceWithVertIter].cellIndices[1] == -1)
					{
						++ numOfBorderFaces;
						avgBorderFacePoint = avgBorderFacePoint + facePoints[*borderFaceWithVertIter];
					}
			}

			avgBorderFacePoint = avgBorderFacePoint / numOfBorderFaces;

			int numOfBorderEdges = 0;
			Point3D avgBorderEdgeMid;
			avgBorderEdgeMid.zero();
			std::set<int>::iterator borderEdgeWithVertIter = vertexIter->edgeIndices.begin();
			for (; borderEdgeWithVertIter != vertexIter->edgeIndices.end(); 
				++ borderEdgeWithVertIter)
			{
					if (m_pHexModel->edges[*borderEdgeWithVertIter].faceIndices.size() != 
						m_pHexModel->edges[*borderEdgeWithVertIter].cellIndices.size()) 
					{
						avgBorderEdgeMid = avgBorderEdgeMid + 
							(m_pHexModel->vertices[m_pHexModel->edges[*borderEdgeWithVertIter].vertexIndices[0]].coordinate +
							m_pHexModel->vertices[m_pHexModel->edges[*borderEdgeWithVertIter].vertexIndices[1]].coordinate) * 0.5;
						++ numOfBorderEdges;
					}
			}

			avgBorderEdgeMid = avgBorderEdgeMid / numOfBorderEdges;

			cent = (cent * (numOfBorderFaces - 3) + 
				avgBorderEdgeMid * 2 + avgBorderFacePoint) / numOfBorderFaces;

		} else {

			Point3D avgCellPoint;
			avgCellPoint.zero();
			std::set<int>::iterator cellWithVertIter = vertexIter->cellIndices.begin();
			for (; cellWithVertIter != vertexIter->cellIndices.end(); ++ cellWithVertIter) {
				avgCellPoint = avgCellPoint + cellPoints[*cellWithVertIter];
			}

			avgCellPoint = avgCellPoint / vertexIter->cellIndices.size();

			Point3D avgFacePoint;
			avgFacePoint.zero();
			std::set<int>::iterator faceWithVertIter = vertexIter->faceIndices.begin();
			for (; faceWithVertIter != vertexIter->faceIndices.end(); ++ faceWithVertIter) {
				avgFacePoint = avgFacePoint + facePoints[*faceWithVertIter];
			}

			avgFacePoint = avgFacePoint / vertexIter->faceIndices.size();

			Point3D avgEdgeMid;
			avgEdgeMid.zero();
			std::set<int>::iterator edgeWithVertIter = vertexIter->edgeIndices.begin();
			for (; edgeWithVertIter != vertexIter->edgeIndices.end(); ++ edgeWithVertIter) {
				avgEdgeMid = avgEdgeMid + 
					(m_pHexModel->vertices[m_pHexModel->edges[*edgeWithVertIter].vertexIndices[0]].coordinate +
					m_pHexModel->vertices[m_pHexModel->edges[*edgeWithVertIter].vertexIndices[1]].coordinate) * 0.5;
			}

			avgEdgeMid = avgEdgeMid / vertexIter->edgeIndices.size();

			cent = (cent * (numOfFacesWithVert - 6) + avgEdgeMid * 3 
				+ avgFacePoint * 2 + avgCellPoint) / numOfFacesWithVert;

		}

		newVertices.push_back(cent);
	}

}


/************************************************************************/
/* calculate new vertices after interpolating subdivision               */
/************************************************************************/
void Hexahedron::CalcNewVerticesInter() {

	Point3D cent;
	std::vector<Vertex>::iterator vertexIter = m_pHexModel->vertices.begin();
	for (; vertexIter != m_pHexModel->vertices.end(); ++ vertexIter) 
	{
		cent.zero();
		cent = vertexIter->coordinate;
		/*int numOfFacesWithVert = vertexIter->faceIndices.size();

		if (numOfFacesWithVert == 12) 
		{

		Point3D avgCellPoint;
		avgCellPoint.zero();
		std::set<int>::iterator cellWithVertIter = vertexIter->cellIndices.begin();
		for (; cellWithVertIter != vertexIter->cellIndices.end(); ++ cellWithVertIter) {
		avgCellPoint = avgCellPoint + cellPoints[*cellWithVertIter];
		}

		avgCellPoint = avgCellPoint / vertexIter->cellIndices.size();

		Point3D avgFacePoint;
		avgFacePoint.zero();
		std::set<int>::iterator faceWithVertIter = vertexIter->faceIndices.begin();
		for (; faceWithVertIter != vertexIter->faceIndices.end(); ++ faceWithVertIter) {
		avgFacePoint = avgFacePoint + facePoints[*faceWithVertIter];
		}

		avgFacePoint = avgFacePoint / vertexIter->faceIndices.size();

		Point3D avgEdgeMid;
		avgEdgeMid.zero();
		std::set<int>::iterator edgeWithVertIter = vertexIter->edgeIndices.begin();
		for (; edgeWithVertIter != vertexIter->edgeIndices.end(); ++ edgeWithVertIter) {
		avgEdgeMid = avgEdgeMid + 
		(m_pHexModel->vertices[m_pHexModel->edges[*edgeWithVertIter].vertexIndices[0]].coordinate +
		m_pHexModel->vertices[m_pHexModel->edges[*edgeWithVertIter].vertexIndices[1]].coordinate) * 0.5;
		}

		avgEdgeMid = avgEdgeMid / vertexIter->edgeIndices.size();

		cent = (cent * (numOfFacesWithVert - 6) + avgEdgeMid * 3 
		+ avgFacePoint * 2 + avgCellPoint) / numOfFacesWithVert;

		}*/

		newVertices.push_back(cent);
	}

}
/************************************************************************/
/*             new vertex link                                          */
/************************************************************************/
void Hexahedron::NewVertexList(VolumeModel* pNewModel) {

	Vertex vert;
	std::vector<Point3D>::iterator newVertIter;

	newVertIter = newVertices.begin();
	for (; newVertIter != newVertices.end(); ++ newVertIter) {
		vert.coordinate = *newVertIter;
		pNewModel->vertices.push_back(vert);
	}

	newVertIter = edgePoints.begin();
	for (; newVertIter != edgePoints.end(); ++ newVertIter) {
		vert.coordinate = *newVertIter;
		pNewModel->vertices.push_back(vert);
	}

	newVertIter = facePoints.begin();
	for (; newVertIter != facePoints.end(); ++ newVertIter) {
		vert.coordinate = *newVertIter;
		pNewModel->vertices.push_back(vert);
	}

	newVertIter = cellPoints.begin();
	for (; newVertIter != cellPoints.end(); ++ newVertIter) {
		vert.coordinate = *newVertIter;
		pNewModel->vertices.push_back(vert);
	}

}
/************************************************************************/
/*    new edge link                                                     */
/************************************************************************/
void Hexahedron::NewEdgeList(VolumeModel* pNewModel) {

	Edge edge;
	int vertIndex = 0;
	int edgeIndex = 0;
	int pos1, pos2;
	std::vector<Edge>::iterator edgeIter = m_pHexModel->edges.begin();
	for (; edgeIter != m_pHexModel->edges.end(); ++ edgeIter, ++ vertIndex) {
		pos1 = edgeIter->vertexIndices[0];
		pos2 = vertIndex + newVertices.size();
		edge.vertexIndices[0] = pos1;
		edge.vertexIndices[1] = pos2;
		pNewModel->edges.push_back(edge);

		pNewModel->vertices[pos1].edgeIndices.insert(edgeIndex);
		pNewModel->vertices[pos2].edgeIndices.insert(edgeIndex);
		++ edgeIndex;

		pos1 = edgeIter->vertexIndices[1];
		edge.vertexIndices[0] = pos1;
		pNewModel->edges.push_back(edge);

		pNewModel->vertices[pos1].edgeIndices.insert(edgeIndex);
		pNewModel->vertices[pos2].edgeIndices.insert(edgeIndex);
		++ edgeIndex;
	}

	vertIndex = 0;
	std::vector<Face>::iterator faceIter = m_pHexModel->faces.begin();
	for (; faceIter != m_pHexModel->faces.end(); ++ faceIter, ++ vertIndex) {
		pos2 = vertIndex + newVertices.size() + edgePoints.size();
		edge.vertexIndices[1] = pos2;
		std::set<int>::iterator edgeOfFaceIter = faceIter->edgeIndices.begin();
		for (; edgeOfFaceIter != faceIter->edgeIndices.end(); ++ edgeOfFaceIter) {
			pos1 = *edgeOfFaceIter + newVertices.size();
			edge.vertexIndices[0] = pos1;
			pNewModel->edges.push_back(edge);
			pNewModel->vertices[pos1].edgeIndices.insert(edgeIndex);
			pNewModel->vertices[pos2].edgeIndices.insert(edgeIndex);
			++ edgeIndex;
		}
	}

	vertIndex = 0;
	std::vector<Cell>::iterator cellIter = m_pHexModel->cells.begin();
	for (; cellIter != m_pHexModel->cells.end(); ++ cellIter, ++ vertIndex) {
		pos2 = vertIndex + newVertices.size() + edgePoints.size() + facePoints.size();
		edge.vertexIndices[1] = pos2;
		std::set<int>::iterator faceOfCellIter = cellIter->faceIndices.begin();
		for (; faceOfCellIter != cellIter->faceIndices.end(); ++ faceOfCellIter) {
			pos1 = *faceOfCellIter + newVertices.size() + edgePoints.size();
			edge.vertexIndices[0] = pos1;
			pNewModel->edges.push_back(edge);

			pNewModel->vertices[pos1].edgeIndices.insert(edgeIndex);
			pNewModel->vertices[pos2].edgeIndices.insert(edgeIndex);
			++ edgeIndex;
		}
	}

}
/************************************************************************/
/*    new face link                                                     */
/************************************************************************/
void Hexahedron::NewFaceList(VolumeModel* pNewModel) {
		
	// in every face, it will subdivide one face to four faces
	std::set<int>::iterator SetIntIter;
	int i, j;

	int newFaceIndex;
	int faceIndex = 0;
	const int numOfNewFacesInFace = 4;
	Face faceInFace[numOfNewFacesInFace];
	std::vector<Face>::iterator faceIter = m_pHexModel->faces.begin();
	for (; faceIter != m_pHexModel->faces.end(); ++ faceIter, ++ faceIndex) {

		int facePointIndex = faceIndex + newVertices.size() + edgePoints.size();

		int vertexIndexInFace[4];
		SetIntIter = faceIter->vertexIndices.begin();
		for (i = 0;
			SetIntIter != faceIter->vertexIndices.end() && i < 4;
			++ SetIntIter, ++ i) {
				vertexIndexInFace[i] = *SetIntIter;				
		}

		int edgePointIndexInFace[4];
		SetIntIter = faceIter->edgeIndices.begin();
		for (i = 0;
			SetIntIter != faceIter->edgeIndices.end() && i < 4; 
			++ SetIntIter, ++ i) {
				edgePointIndexInFace[i] = *SetIntIter + newVertices.size();
		}

		for (i = 0; i < numOfNewFacesInFace; ++ i) {

			// vertices in face
			faceInFace[i].vertexIndices.insert(facePointIndex);
			faceInFace[i].vertexIndices.insert(vertexIndexInFace[i]);

			int edgePointIndexInIthFace[2];
			int k = 0;

			SetIntIter = pNewModel->vertices[vertexIndexInFace[i]].edgeIndices.begin();
			for (; SetIntIter != pNewModel->vertices[vertexIndexInFace[i]].edgeIndices.end(); 
				++ SetIntIter) {
					for (j = 0; j < 4; ++ j) {
						if (pNewModel->edges[*SetIntIter].vertexIndices[0] == edgePointIndexInFace[j] ||
							pNewModel->edges[*SetIntIter].vertexIndices[1] == edgePointIndexInFace[j]) {
								if (k < 2)
									edgePointIndexInIthFace[k++] = edgePointIndexInFace[j];
								// put the vertex into face
								faceInFace[i].vertexIndices.insert(edgePointIndexInFace[j]);
								// put the edge into face
								faceInFace[i].edgeIndices.insert(*SetIntIter);
						}
					}
			}

			newFaceIndex = i + numOfNewFacesInFace * faceIndex;

			SetIntIter = faceInFace[i].vertexIndices.begin();
			for (; SetIntIter != faceInFace[i].vertexIndices.end(); ++ SetIntIter) {
				pNewModel->vertices[*SetIntIter].faceIndices.insert(newFaceIndex);
			}

			// edges in face
			SetIntIter = pNewModel->vertices[facePointIndex].edgeIndices.begin();
			for (; SetIntIter != pNewModel->vertices[facePointIndex].edgeIndices.end(); ++ SetIntIter) {
				for (k = 0; k < 2; ++ k) {
					if (pNewModel->edges[*SetIntIter].vertexIndices[0] == edgePointIndexInIthFace[k] ||
						pNewModel->edges[*SetIntIter].vertexIndices[1] == edgePointIndexInIthFace[k])
						faceInFace[i].edgeIndices.insert(*SetIntIter);
				}
			}

			SetIntIter = faceInFace[i].edgeIndices.begin();
			for (; SetIntIter != faceInFace[i].edgeIndices.end(); ++ SetIntIter) {
				pNewModel->edges[*SetIntIter].faceIndices.insert(newFaceIndex);
			}

			pNewModel->faces.push_back(faceInFace[i]);

			faceInFace[i].vertexIndices.clear();
			faceInFace[i].edgeIndices.clear();

		}

	}

	// in every cell, it will create 12 new faces
	const int numOfNewFacesInCell = 12;
	Face faceInCell[numOfNewFacesInCell];
	int cellIndex = 0;
	std::vector<Cell>::iterator cellIter = m_pHexModel->cells.begin();
	for (; cellIter != m_pHexModel->cells.end(); ++ cellIter, ++ cellIndex) {

		int cellPointIndex = cellIndex + newVertices.size() + edgePoints.size() + facePoints.size();

		int facePointIndexInCell[6];
		SetIntIter = cellIter->faceIndices.begin();
		for (i = 0;
			SetIntIter != cellIter->faceIndices.end() && i < 6; 
			++ SetIntIter, ++ i) {
				facePointIndexInCell[i] = *SetIntIter + newVertices.size() + edgePoints.size();
		}

		int edgePointIndexInCell[12];
		SetIntIter = cellIter->edgeIndices.begin();
		for (i = 0;
			SetIntIter != cellIter->edgeIndices.end() && i < 12; 
			++ SetIntIter, ++ i) {
				edgePointIndexInCell[i] = *SetIntIter + newVertices.size();
		}

		for (i = 0; i < numOfNewFacesInCell; ++ i) {

			faceInCell[i].vertexIndices.insert(cellPointIndex);
			faceInCell[i].vertexIndices.insert(edgePointIndexInCell[i]);

			int facePointIndexInIthCell[2];
			int k = 0;

			SetIntIter = pNewModel->vertices[edgePointIndexInCell[i]].edgeIndices.begin();
			for (; SetIntIter != pNewModel->vertices[edgePointIndexInCell[i]].edgeIndices.end();
				++ SetIntIter) {

					for (j = 0; j < 6; ++ j) {

						if (pNewModel->edges[*SetIntIter].vertexIndices[0] == facePointIndexInCell[j] ||
							pNewModel->edges[*SetIntIter].vertexIndices[1] == facePointIndexInCell[j]) {
								if (k < 2) 
									facePointIndexInIthCell[k++] = facePointIndexInCell[j];
								faceInCell[i].vertexIndices.insert(facePointIndexInCell[j]);

								faceInCell[i].edgeIndices.insert(*SetIntIter);
						}
					}

			} 

			newFaceIndex = i + numOfNewFacesInCell * cellIndex + numOfNewFacesInFace * faceIndex;

			SetIntIter = faceInCell[i].vertexIndices.begin();
			for (; SetIntIter != faceInCell[i].vertexIndices.end(); ++ SetIntIter) {
				pNewModel->vertices[*SetIntIter].faceIndices.insert(newFaceIndex);
			}
			// edges in face
			SetIntIter = pNewModel->vertices[cellPointIndex].edgeIndices.begin();
			for (; SetIntIter != pNewModel->vertices[cellPointIndex].edgeIndices.end(); ++ SetIntIter) {
				for (k = 0; k < 2; ++ k) {
					if (pNewModel->edges[*SetIntIter].vertexIndices[0] == facePointIndexInIthCell[k] ||
						pNewModel->edges[*SetIntIter].vertexIndices[1] == facePointIndexInIthCell[k])
						faceInCell[i].edgeIndices.insert(*SetIntIter);
				}
			}

			SetIntIter = faceInCell[i].edgeIndices.begin();
			for (; SetIntIter != faceInCell[i].edgeIndices.end(); ++ SetIntIter) {
				pNewModel->edges[*SetIntIter].faceIndices.insert(newFaceIndex);
			}

			pNewModel->faces.push_back(faceInCell[i]);

			faceInCell[i].vertexIndices.clear();
			faceInCell[i].edgeIndices.clear();

		}
	}
}
/************************************************************************/
/*  new cell link                                                       */
/************************************************************************/
void Hexahedron::NewCellList(VolumeModel* pNewModel) {

	std::set<int>::iterator SetIntIter;
	int i, j;

	int newCellIndex;
	const int numOfNewCellsInCell = 8;
	Cell cellInCell[numOfNewCellsInCell];
	int cellIndex = 0;
	std::vector<Cell>::iterator cellIter = m_pHexModel->cells.begin();
	for (; cellIter != m_pHexModel->cells.end(); ++ cellIter, ++ cellIndex) {

		int cellPointIndex = cellIndex + newVertices.size() + edgePoints.size() + facePoints.size();

		int vertexIndexInCell[8];
		SetIntIter = cellIter->vertexIndices.begin();
		for (i = 0; 
			SetIntIter != cellIter->vertexIndices.end() && i < 8;
			++ SetIntIter, ++ i) {
				vertexIndexInCell[i] = *SetIntIter;
		}

		int edgePointIndexInCell[12];
		SetIntIter = cellIter->edgeIndices.begin();
		for (i = 0; 
			SetIntIter != cellIter->edgeIndices.end() && i < 12;
			++ SetIntIter, ++ i) {
				edgePointIndexInCell[i] = *SetIntIter + newVertices.size();
		}

		int facePointIndexInCell[6];
		SetIntIter = cellIter->faceIndices.begin();
		for (i = 0; 
			SetIntIter != cellIter->faceIndices.end() && i < 6; 
			++ SetIntIter, ++ i) {
				facePointIndexInCell[i] = *SetIntIter + newVertices.size() + edgePoints.size();
		}

		for (i = 0; i < numOfNewCellsInCell; ++ i) {

			// vertex
			cellInCell[i].vertexIndices.insert(cellPointIndex);
			cellInCell[i].vertexIndices.insert(vertexIndexInCell[i]);

			int k = 0;
			int edgePointIndexInIthCell[3];
			SetIntIter = pNewModel->vertices[vertexIndexInCell[i]].edgeIndices.begin();
			for (; SetIntIter != pNewModel->vertices[vertexIndexInCell[i]].edgeIndices.end(); 
				++ SetIntIter)
			{
				for (j = 0; j < 12; ++ j) 
				{
					if (pNewModel->edges[*SetIntIter].vertexIndices[0] == edgePointIndexInCell[j] ||
						pNewModel->edges[*SetIntIter].vertexIndices[1] == edgePointIndexInCell[j])
					{
						if (k < 3)
							edgePointIndexInIthCell[k++] = edgePointIndexInCell[j];

						cellInCell[i].vertexIndices.insert(edgePointIndexInCell[j]);

						cellInCell[i].edgeIndices.insert(*SetIntIter);
					}
				}
			}

			k = 0;
			int facePointIndexInIthCell[3];
			SetIntIter = pNewModel->vertices[vertexIndexInCell[i]].faceIndices.begin();
			for (; SetIntIter != pNewModel->vertices[vertexIndexInCell[i]].faceIndices.end();
				++ SetIntIter)
			{
				for (j = 0; j < 6; ++ j) 
				{
					std::set<int>::iterator vertOfFaceIter = pNewModel->faces[*SetIntIter].vertexIndices.begin();
					for (; vertOfFaceIter != pNewModel->faces[*SetIntIter].vertexIndices.end(); 
						++ vertOfFaceIter)
					{
						if (*vertOfFaceIter == facePointIndexInCell[j]) 
						{
							if (k < 3) 
								facePointIndexInIthCell[k++] = facePointIndexInCell[j];

							cellInCell[i].vertexIndices.insert(facePointIndexInCell[j]);

							cellInCell[i].faceIndices.insert(*SetIntIter);

						}
					}
				}
			}

			newCellIndex = i + numOfNewCellsInCell * cellIndex;

			SetIntIter = cellInCell[i].vertexIndices.begin();
			for (; SetIntIter != cellInCell[i].vertexIndices.end(); ++ SetIntIter) {
				pNewModel->vertices[*SetIntIter].cellIndices.insert(newCellIndex);
			}

			// edge
			for (k = 0; k < 3; ++ k) {
				SetIntIter = pNewModel->vertices[facePointIndexInIthCell[k]].edgeIndices.begin();
				for (; SetIntIter != pNewModel->vertices[facePointIndexInIthCell[k]].edgeIndices.end();
					++ SetIntIter) {
						for (j = 0; j < 3; ++ j) {
							if (pNewModel->edges[*SetIntIter].vertexIndices[0] == edgePointIndexInIthCell[j] ||
								pNewModel->edges[*SetIntIter].vertexIndices[1] == edgePointIndexInIthCell[j]) {
									cellInCell[i].edgeIndices.insert(*SetIntIter);
							}
						}

						if (pNewModel->edges[*SetIntIter].vertexIndices[0] == cellPointIndex ||
							pNewModel->edges[*SetIntIter].vertexIndices[1] == cellPointIndex) {
								cellInCell[i].edgeIndices.insert(*SetIntIter);
						}
				}
			}

			SetIntIter = cellInCell[i].edgeIndices.begin();
			for (; SetIntIter != cellInCell[i].edgeIndices.end(); ++ SetIntIter) {
				pNewModel->edges[*SetIntIter].cellIndices.insert(newCellIndex);
			}

			// face
			SetIntIter = pNewModel->vertices[cellPointIndex].faceIndices.begin();
			for (; SetIntIter != pNewModel->vertices[cellPointIndex].faceIndices.end(); ++ SetIntIter) {
				std::set<int>::iterator vertOfFaceIter = pNewModel->faces[*SetIntIter].vertexIndices.begin();
				for (; vertOfFaceIter != pNewModel->faces[*SetIntIter].vertexIndices.end(); 
					++ vertOfFaceIter) {
						for (j = 0; j < 3; ++ j) {
							if (*vertOfFaceIter == edgePointIndexInIthCell[j]) {
								cellInCell[i].faceIndices.insert(*SetIntIter);
							}
						}
				}
			}

			SetIntIter = cellInCell[i].faceIndices.begin();
			for (; SetIntIter != cellInCell[i].faceIndices.end(); ++ SetIntIter) {
				if (pNewModel->faces[*SetIntIter].cellIndices[0] == -1) 
					pNewModel->faces[*SetIntIter].cellIndices[0] = newCellIndex;
				else if (pNewModel->faces[*SetIntIter].cellIndices[1] == -1) {
					pNewModel->faces[*SetIntIter].cellIndices[1] = newCellIndex;
				}
			}

			pNewModel->cells.push_back(cellInCell[i]);

			cellInCell[i].vertexIndices.clear();
			cellInCell[i].edgeIndices.clear();
			cellInCell[i].faceIndices.clear();

		}

	}
}

/************************************************************************/
/*  Hexahedron Approximating Subdivision                                */
/************************************************************************/
void Hexahedron::ApproximateSubdivision() {

	CalcCellPoints();
	CalcFacePoints();
	CalcEdgePoints();
	CalcNewVertices();
	
	// new model
	VolumeModel* pNewModel = new VolumeModel;

	NewVertexList(pNewModel);
	NewEdgeList(pNewModel);
	NewFaceList(pNewModel);
	NewCellList(pNewModel);

	FreeMemory();

	delete m_pHexModel;
	m_pHexModel = pNewModel;

	std::cerr << std::endl;
	std::cerr << "####### subdivision counter: " << ++subdivCounter << " #######" << std::endl;
	std::cerr << "#vertices: " << m_pHexModel->vertices.size() << std::endl;
	std::cerr << "#edges:    " << m_pHexModel->edges.size() << std::endl;
	std::cerr << "#faces:    " << m_pHexModel->faces.size() << std::endl;
	std::cerr << "#cells:    " << m_pHexModel->cells.size() << std::endl;
}


/************************************************************************/
/*  Hexahedron Interpolating Subdivision                                */
/************************************************************************/
void Hexahedron::InterpolateSubdivision() {

	CalcCellPoints();
	CalcFacePointsInter();
	//CalcFacePoints();
	CalcEdgePointsInter();
	CalcNewVerticesInter();

	// new model
	VolumeModel* pNewModel = new VolumeModel;

	NewVertexList(pNewModel);
	NewEdgeList(pNewModel);
	NewFaceList(pNewModel);
	NewCellList(pNewModel);

	FreeMemory();

	delete m_pHexModel;
	m_pHexModel = pNewModel;

	std::cerr << std::endl;
	std::cerr << "####### subdivision counter: " << ++subdivCounter << " #######" << std::endl;
	std::cerr << "#vertices: " << m_pHexModel->vertices.size() << std::endl;
	std::cerr << "#edges:    " << m_pHexModel->edges.size() << std::endl;
	std::cerr << "#faces:    " << m_pHexModel->faces.size() << std::endl;
	std::cerr << "#cells:    " << m_pHexModel->cells.size() << std::endl;
}

} // namespace VolumeMesh