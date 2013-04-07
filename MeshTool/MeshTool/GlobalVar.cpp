#define  _GLOBAL_VARIABLE_DEFINITION_FILE_

#include "stdafx.h"



//----global variables
CGeoScene  *G_pScene;
CView      *G_pView;
CArcBall   *G_pArcBall;


float		G_fEyeDistScale = 1.0f;
int			G_nWinWidth, G_nWinHeight;
Point3D		G_vLight;
Point3D		G_vEye;
Point3D     G_vBackgroundColor = Point3D(0.15f, 0.20f, 0.267f);

//defined just for ambient
float	G_fAmbientCoefficent = 0.25f;	
BOOL	G_bUseSofeAmbient = TRUE;		//wheather use the sofe ambient


int	G_theMouseButton = NO_BUTTON;

CPoint	G_iOldPos;

std::string G_sModelPath;




//----global functions

void initRenderingWindow(CSize* pDstSize ,CView* pView)
{
	CRect	rect;
	Point3D vMax, vMin, vCent;

	G_pScene->BoundingBox(vMax, vMin);
	G_vEye = Point3D((vMax.x+vMin.x)/2.f, (vMax.y+vMin.y)/2.f, (vMax.z+vMin.z)/2.f+(vMax - vMin).length()*G_fEyeDistScale);
	vCent = Point3D((vMax.x+vMin.x)/2.f, (vMax.y+vMin.y)/2.f, (vMax.z+vMin.z)/2.f);


	if (pDstSize)
	{
		G_nWinWidth =pDstSize->cx;
		G_nWinHeight=pDstSize->cy;
	}
	else
	{
		G_pView->GetClientRect(rect);


		G_nWinWidth=rect.Width();
		G_nWinHeight=rect.Height();
	}


	glViewport ( 0,0, 
		G_nWinWidth, G_nWinHeight );

	glMatrixMode ( GL_PROJECTION );

	glLoadIdentity();
	{		
		double theta=2*atan2(1,tan(PI/2-(ScreenViewAngle/2)));

		gluPerspective ( theta*180/PI, (float)G_nWinWidth/(float)G_nWinHeight,
			G_vEye.length()/1000, G_vEye.length()*2);
	}

	glMatrixMode ( GL_MODELVIEW );
	glLoadIdentity();

	gluLookAt(G_vEye.x, G_vEye.y, G_vEye.z, vCent.x, vCent.y, vCent.z, 0.f, 1.f, 0.f);


	// 1. Set rendering feature
	glShadeModel ( GL_SMOOTH );


	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);


	SetLightAttribute(G_vLight);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_LIGHT_MODEL_LOCAL_VIEWER);


	glClearColor( G_vBackgroundColor.x, G_vBackgroundColor.y, G_vBackgroundColor.z, 1.0f);
}


void SetLightAttribute(Point3D& vLightPos)
{
	// set light attribute
	GLfloat ambient[] = { 0.0f, 0.0f, 0.0f, 1.0 };
	GLfloat diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0 };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0 };
	GLfloat position[] = { vLightPos.x, vLightPos.y, vLightPos.z, 0.0 };

	if (G_bUseSofeAmbient)
	{
		ambient[0]=G_fAmbientCoefficent;
		ambient[1]=G_fAmbientCoefficent;
		ambient[2]=G_fAmbientCoefficent;

	}


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, position);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

}