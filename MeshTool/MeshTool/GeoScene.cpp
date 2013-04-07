// GeoScene.cpp: implementation of the CGeoScene class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GeoScene.h"
#include "FileManager.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif





//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeoScene::CGeoScene()
{
	m_pHexahedron = new VolumeMesh::Hexahedron;

	m_nRenderingStyles = DRAW_POINT_SCENE;
}

CGeoScene::~CGeoScene()
{
	Destory();
}

//////////////////////////////////////////////////////////////////////
// free all memory and reset all variables
// called before load a new scene or before this class is destructed
//////////////////////////////////////////////////////////////////////
void CGeoScene::Destory()
{
	if (m_pHexahedron)
		delete m_pHexahedron;
	m_pHexahedron = NULL;
}

void CGeoScene::SetMaterial(float fR,float fG,float fB) {

	float fAmbient[4]={0.5f,0.5f,0.5f,1.0};
	float fDiffuse[4]={1.0f,1.0f,1.0f,1.0};
	float fSpecular[4]={0.0f,0.0f,0.0f,1.0};
	float fEmissive[4]={0.0,0.0,0.0,1.0};


	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fAmbient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,fSpecular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, fEmissive);

	fDiffuse[0]=fR;
	fDiffuse[1]=fG;
	fDiffuse[2]=fB;


	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDiffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fDiffuse);
}

//////////////////////////////////////////////////////////////////////
// Draw the scene using points only
//////////////////////////////////////////////////////////////////////
void CGeoScene::DrawSceneByPoints()
{
	if (NULL == m_pHexahedron)
		return;

	glDisable(GL_TEXTURE_2D);
	glColor3f (1.0f, 1.0f, 1.0f);
	
	SetMaterial(1.0f, 1.0f, 1.0f);

	Point3D normal;

	glBegin(GL_POINTS);
	std::vector<VolumeMesh::Vertex>::iterator vertexIter = m_pHexahedron->m_pHexModel->vertices.begin();
	for (; vertexIter != m_pHexahedron->m_pHexModel->vertices.end(); ++ vertexIter)
	{
		normal = vertexIter->coordinate;
		normal = normal.unify();
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(vertexIter->coordinate.x, vertexIter->coordinate.y, vertexIter->coordinate.z);
	}
	glEnd();
}

//////////////////////////////////////////////////////////////////////
// Draw the scene using line wire
//////////////////////////////////////////////////////////////////////
void CGeoScene::DrawSceneByLine()
{

	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);

	Point3D v[2], n[2];

	std::vector<VolumeMesh::Edge>::iterator it_edge = m_pHexahedron->m_pHexModel->edges.begin();
	for (; it_edge != m_pHexahedron->m_pHexModel->edges.end(); ++ it_edge)
	{
		/*if (m_pEdges[i].m_nFlag==BOUNDARY_EDGE)
			glColor3f (1.0f, 0.0f, 0.0f);
		else*/
			glColor3f (1.0f, 1.0f, 1.0f);
		SetMaterial(1.0f,1.0f,1.0f);
		n[0] = v[0] = m_pHexahedron->m_pHexModel->vertices[it_edge->vertexIndices[0]].coordinate;
		n[1] = v[1] = m_pHexahedron->m_pHexModel->vertices[it_edge->vertexIndices[1]].coordinate;
		n[0] = n[0].unify();
		n[1] = n[1].unify();
		glNormal3f(n[0].x, n[0].y, n[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glNormal3f(n[1].x, n[1].y, n[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);

	}

	glEnd();

}

//////////////////////////////////////////////////////////////////////
// Draw the scene using shading triangles
// highlight the curvature triangles
//////////////////////////////////////////////////////////////////////
void CGeoScene::DrawSceneByFaces()
{	
	Point3D v[4];
	float fDiffuse[4][4] = {
		{1.f, 1.f, 1.f, 1.f},
		{0.f, 0.f, 1.f, 1.f},		//blue color
		{0.f, 1.f, 0.f, 1.f},		//green color
		{1.f, 0.f, 0.f, 0.f}		//red color
	};	
	int edgeIndex[4][2];
	int vertexIndexSorted[4];

	float fColorDif[4]={0,0,0,0};

	glDisable(GL_TEXTURE_2D);

	std::vector<VolumeMesh::Face>::iterator faceIter = m_pHexahedron->m_pHexModel->faces.begin();
	for (int f = 0; faceIter != m_pHexahedron->m_pHexModel->faces.end(); ++faceIter, ++f)
	{
		if (faceIter->cellIndices[0] > -1 && faceIter->cellIndices[1] > -1)
			continue;

		Point3D cellPoint;
		std::set<int>::iterator vIter;
		vIter = m_pHexahedron->m_pHexModel->cells[faceIter->cellIndices[0]].vertexIndices.begin();
		for (; vIter != m_pHexahedron->m_pHexModel->cells[faceIter->cellIndices[0]].vertexIndices.end();
			++vIter) {
				cellPoint = cellPoint + m_pHexahedron->m_pHexModel->vertices[*vIter].coordinate;
		}

		cellPoint = cellPoint / m_pHexahedron->m_pHexModel->cells[faceIter->cellIndices[0]].vertexIndices.size();
		
		Point3D vMidOfFace;
		std::set<int>::iterator vertOfFaceIter;
		vertOfFaceIter = m_pHexahedron->m_pHexModel->faces[f].vertexIndices.begin();
		for (; vertOfFaceIter != m_pHexahedron->m_pHexModel->faces[f].vertexIndices.end(); 
			++vertOfFaceIter) {
				vMidOfFace = vMidOfFace + m_pHexahedron->m_pHexModel->vertices[*vertOfFaceIter].coordinate;
		}

		vMidOfFace = vMidOfFace / m_pHexahedron->m_pHexModel->faces[f].vertexIndices.size();

		Point3D normal = vMidOfFace - cellPoint;
		normal = normal.unify();

		int i = 0; 

		float fDiffTemp[4] = {.5, 0., 0., .5};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, fDiffTemp);
		//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fDiffuse[0]);
		glColor3f (fDiffuse[0][0], fDiffuse[0][0], fDiffuse[0][0]);
		//glColor3f(0.5, 0.0, 0.0);
		//-------------------------
		
		std::set<int>::iterator edgeIter = faceIter->edgeIndices.begin();
		for (i = 0; edgeIter != faceIter->edgeIndices.end() && i < 4; ++ edgeIter, ++ i) {
			edgeIndex[i][0] = m_pHexahedron->m_pHexModel->edges[*edgeIter].vertexIndices[0];
			edgeIndex[i][1] = m_pHexahedron->m_pHexModel->edges[*edgeIter].vertexIndices[1];
		}
		
		vertexIndexSorted[0] = edgeIndex[0][0];
		vertexIndexSorted[1] = edgeIndex[0][1];
		for (i = 1; i < 4; ++ i) {
			if (vertexIndexSorted[1] == edgeIndex[i][0]) {
				vertexIndexSorted[2] = edgeIndex[i][1];
			} else if (vertexIndexSorted[1] == edgeIndex[i][1]) {
				vertexIndexSorted[2] = edgeIndex[i][0];
			}
		}
		for (i = 1; i < 4; ++ i) {
			if (vertexIndexSorted[2] == edgeIndex[i][0] && 
				vertexIndexSorted[1] != edgeIndex[i][1]) {
				vertexIndexSorted[3] = edgeIndex[i][1];
			} else if (vertexIndexSorted[2] == edgeIndex[i][1] &&
				vertexIndexSorted[1] != edgeIndex[i][0]) {
				vertexIndexSorted[3] = edgeIndex[i][0];
			}
		}

		for (i = 0; i < 4; ++ i) {
			v[i] = m_pHexahedron->m_pHexModel->vertices[vertexIndexSorted[i]].coordinate;
		}
		//-------------------------
		/*Point3D n1 = v[0] - v[1];
		Point3D n2 = v[1] - v[2];
		Point3D normal = n1.cross(n2);
		if (normal.dot(v[0]) < 0)
			normal = normal * (-1);
		normal = normal.unify() * 0.5;*/

		glBegin(GL_POLYGON);
		glNormal3f(normal.x, normal.y, normal.z);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);
		glVertex3f(v[2].x, v[2].y, v[2].z);
		glVertex3f(v[3].x, v[3].y, v[3].z);

		glEnd();

	}

}

//////////////////////////////////////////////////////////////////////
// Called to show the scene
//////////////////////////////////////////////////////////////////////
void CGeoScene::DrawScene()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Lighting switch
	if (m_nRenderingStyles & LIGHT_ON_WHEN_RENDERING)
		glEnable(GL_LIGHTING);
	else	glDisable(GL_LIGHTING);


	int iRenderCount=0;

	// Drawing element
	if (m_nRenderingStyles & DRAW_POINT_SCENE)
	{
		DrawSceneByPoints();
		iRenderCount++;
	}
	else if (m_nRenderingStyles & DRAW_LINE_SCENE)
	{
		DrawSceneByLine();
		iRenderCount++;
	}
	else if (m_nRenderingStyles & DRAW_FACE_SCENE)
	{
		DrawSceneByFaces();
		iRenderCount++;
	}
	

	ASSERT(iRenderCount==1);

	if (m_nRenderingStyles & SHOW_COORDINATE)
		DrawCoordinate();

	if (m_nRenderingStyles & SHOW_BOUNDING_BOX)
		DrawBoundingBox();

	if (m_nRenderingStyles & SHOW_WIREFRAME)
		DrawWireframe();
}

void CGeoScene::DrawCoordinate() {
	float fCoordLineLen = (m_vMax-m_vMin).length() * 0.5f;
	float fCoordArrowLen = fCoordLineLen/15;
	float fCoordArrowWid = fCoordArrowLen/10;

	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);


#define DetailNum	12


	float angleStep=2 * PI / DetailNum;

	//»æÖÆxÖá
	glColor3f(1.0,0,0);
	glBegin(GL_LINES);
	{
		glVertex3f(0,0,0);
		glVertex3f(fCoordLineLen,0,0);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	{
		for (int i = 0;i < DetailNum; ++ i)
		{
			glVertex3f(fCoordLineLen,cos(i*angleStep)*fCoordArrowWid,sin(i*angleStep)*fCoordArrowWid);
			glVertex3f(fCoordLineLen+fCoordArrowLen,0,0);
			glVertex3f(fCoordLineLen,cos((i+1)*angleStep)*fCoordArrowWid,sin((i+1)*angleStep)*fCoordArrowWid);
		}


	}
	glEnd();

	//»æÖÆyÖá
	glColor3f(0,1.0,0);
	glBegin(GL_LINES);
	{
		glVertex3f(0,0,0);
		glVertex3f(0,fCoordLineLen,0);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	{
		for (int i=0;i<DetailNum;i++)
		{
			glVertex3f(cos(i*angleStep)*fCoordArrowWid,fCoordLineLen,sin(i*angleStep)*fCoordArrowWid);
			glVertex3f(0,fCoordLineLen+fCoordArrowLen,0);
			glVertex3f(cos((i+1)*angleStep)*fCoordArrowWid,fCoordLineLen,sin((i+1)*angleStep)*fCoordArrowWid);
		}
	}
	glEnd();

	//»æÖÆzÖá
	glColor3f(0,0,1.0);
	glBegin(GL_LINES);
	{
		glVertex3f(0,0,0);
		glVertex3f(0,0,fCoordLineLen);
	}
	glEnd();
	glBegin(GL_TRIANGLES);
	{
		for (int i=0;i<DetailNum;i++)
		{
			glVertex3f(cos(i*angleStep)*fCoordArrowWid,sin(i*angleStep)*fCoordArrowWid,fCoordLineLen);
			glVertex3f(0,0,fCoordLineLen+fCoordArrowLen);
			glVertex3f(cos((i+1)*angleStep)*fCoordArrowWid,sin((i+1)*angleStep)*fCoordArrowWid,fCoordLineLen);
		}
	}
	glEnd();

}

void CGeoScene::SetRenderingStyles(int nStyles)
{
	m_nRenderingStyles = nStyles;
}

//////////////////////////////////////////////////////////////////////
void CGeoScene::BoundingBox(Point3D &vMax, Point3D &vMin)
{	
	vMax = m_vMax;
	vMin = m_vMin;
}



void CGeoScene::ConvertData(void)
{
	m_vMax.zero();
	m_vMin.zero();
	std::vector<VolumeMesh::Vertex>::iterator vertIter; 
	vertIter = m_pHexahedron->m_pHexModel->vertices.begin();
	for (; vertIter != m_pHexahedron->m_pHexModel->vertices.end(); ++vertIter) {
		m_vMax.x = max(m_vMax.x, vertIter->coordinate.x);
		m_vMax.y = max(m_vMax.y, vertIter->coordinate.y);
		m_vMax.z = max(m_vMax.z, vertIter->coordinate.z);

		m_vMin.x = min(m_vMin.x, vertIter->coordinate.x);
		m_vMin.y = min(m_vMin.y, vertIter->coordinate.y);
		m_vMin.z = min(m_vMin.z, vertIter->coordinate.z);
	}

	Point3D vMid = (m_vMax + m_vMin) * 0.5;

	vertIter = m_pHexahedron->m_pHexModel->vertices.begin();
	for (; vertIter != m_pHexahedron->m_pHexModel->vertices.end(); ++vertIter) {
		vertIter->coordinate = vertIter->coordinate - vMid;
	}
	m_vMax = m_vMax - vMid;
	m_vMin = m_vMin - vMid;
}


void CGeoScene::LoadVMModel(std::string& pathName)
{
	if (G_pScene->m_pHexahedron)
		delete G_pScene->m_pHexahedron;
	G_pScene->m_pHexahedron = new VolumeMesh::Hexahedron;

	VolumeMesh::IO::FileManager fileManager;
	fileManager.readFile(pathName, G_pScene->m_pHexahedron);
	G_pScene->ConvertData();
}


void CGeoScene::DrawBoundingBox(void)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	// Drawing wireframe

	glColor3f (1.f, 0.f, 0.f);
	glBegin(GL_LINES);

	float v[8][3] = {
		{m_vMax.x, m_vMax.y, m_vMax.z},{m_vMax.x, m_vMin.y, m_vMax.z},
		{m_vMin.x, m_vMin.y, m_vMax.z},{m_vMin.x, m_vMax.y, m_vMax.z},
		{m_vMax.x, m_vMax.y, m_vMin.z},{m_vMax.x, m_vMin.y, m_vMin.z},
		{m_vMin.x, m_vMin.y, m_vMin.z},{m_vMin.x, m_vMax.y, m_vMin.z}
	};


	for (int i = 0; i < 12; ++i)
	{
		int p1, p2;
		if ((i >= 0 && i < 3) || (i >= 4 && i < 7)) {
			p1 = i; p2 = i + 1;
		} else if (i == 3 || i == 7) {
			p1 = i; p2 = i - 3;
		} else {
			p1 = i - 8; p2 = i - 4;
		}
		glVertex3f(v[p1][0],v[p1][1],v[p1][2]);
		glVertex3f(v[p2][0],v[p2][1],v[p2][2]);

	}
	glEnd();
}

void CGeoScene::DrawWireframe() {
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);

	Point3D v[2], n[2];

	std::vector<VolumeMesh::Edge>::iterator it_edge = m_pHexahedron->m_pHexModel->edges.begin();
	for (; it_edge != m_pHexahedron->m_pHexModel->edges.end(); ++ it_edge)
	{
		if (it_edge->faceIndices.size() == it_edge->cellIndices.size()) 
			continue;
		glColor3f (0.0f, 0.5f, 0.5f);
		SetMaterial(1.0f,1.0f,1.0f);
		n[0] = v[0] = m_pHexahedron->m_pHexModel->vertices[it_edge->vertexIndices[0]].coordinate;
		n[1] = v[1] = m_pHexahedron->m_pHexModel->vertices[it_edge->vertexIndices[1]].coordinate;
		n[0] = n[0].unify();
		n[1] = n[1].unify();
		glNormal3f(n[0].x, n[0].y, n[0].z);
		glVertex3f(v[0].x, v[0].y, v[0].z);
		glNormal3f(n[1].x, n[1].y, n[1].z);
		glVertex3f(v[1].x, v[1].y, v[1].z);

	}

	glEnd();
}