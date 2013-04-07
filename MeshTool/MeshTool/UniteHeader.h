/************************************************************************
*                                                                       * 
*        UniteHeader.h   												*
*																		*
*	DESCRIPTION£º														*			
*       	Gather the Constants,class header and shared variables		*
*																		*
*   COMPONENTS£º														*
*			1.Constant Value Define										*
*			2.include class header file									*	
*			3.declare the shared variable of projection					*		
*																		*
************************************************************************/


#ifndef UNITEHEADER_H
#define	UNITEHEADER_H


#define DELTA		0.0001f
#define PI			3.1415926f
#define	ScreenViewAngle				PI/4


#define NO_BUTTON		0
#define MOUSE_LEFT_BUTTON	1
#define MOUSE_RIGHT_BUTTON	2



/************************************************************************
*                                                                       * 
*       Include important head files									*
*																		*
************************************************************************/
#include <stdio.h>
#include <math.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glaux.h>

#include <iostream>
#include <vector>
#include <set>


#include "StructDef.h"
#include "GeoScene.h"
#include "ArcBall.h"



/************************************************************************
*                                                                       * 
*       Define Variables shared in Project								*
*																		*
************************************************************************/

extern CGeoScene   *G_pScene;
extern CView	   *G_pView;
extern CArcBall   *G_pArcBall;

extern Point3D		G_vEye;
extern float		G_fEyeDistScale;
extern int			G_nWinWidth, G_nWinHeight;
extern Point3D		G_vLight;
extern Point3D	    G_vBackgroundColor;


extern float G_fAmbientCoefficent;
extern BOOL	 G_bUseSofeAmbient;		//wheather use the sofe ambient

extern int	G_theMouseButton;

extern CPoint	G_iOldPos;

extern std::string G_sModelPath;


extern void initRenderingWindow(CSize* pDstSize ,CView* pView);
extern void SetLightAttribute(Point3D& vLightPos);


extern void RedirectIOToConsole(const char title[]);


#endif