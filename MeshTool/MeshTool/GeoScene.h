// GeoScene.h: interface for the CGeoScene class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


#ifndef __MY_GEOMERTIC_SCENE_
#define __MY_GEOMERTIC_SCENE_

///////////////////////////////////////////////////////////////////////////////
// Drawing typies
#define DRAW_POINT_SCENE			0x01
#define DRAW_LINE_SCENE				0x02
#define DRAW_FACE_SCENE				0x04

// Rendering styles
#define SHOW_BOUNDING_BOX           0x10
#define SHOW_COORDINATE				0x20
#define SHOW_WIREFRAME				0x40
#define LIGHT_ON_WHEN_RENDERING		0x80



///////////////////////////////////////////////////////////////////////////////
// The geometric scene
class CGeoScene  
{
public:
	
#define MaxViewWidth	1600
#define MaxViewHeight	1400

public:

	CGeoScene();
	virtual ~CGeoScene();
	void SetRenderingStyles(int nStyles);
	void DrawScene();
	void BoundingBox(Point3D &vMax, Point3D &vMin);

protected:

	void Destory();		
	void DrawSceneByPoints();
	void DrawSceneByLine();
	void DrawSceneByFaces();
	void DrawCoordinate();
	void DrawWireframe();

	void SetMaterial(float fR,float fG,float fB);

private:
	int m_nRenderingStyles;
	Point3D m_vMax, m_vMin;

public:
	VolumeMesh::Hexahedron* m_pHexahedron;
	void ConvertData(void);
	void LoadVMModel(std::string& pathName);
	void DrawBoundingBox(void);
};

#endif // __MY_GEOMERTIC_SCENE_
