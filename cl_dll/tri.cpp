//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

// Triangle rendering, if any

#include "winsani_in.h"
#include <windows.h>
#include "winsani_out.h"
#include <gl/gl.h>

#include "hud.h"
#include "cl_util.h"

// Triangle rendering apis are in gEngfuncs.pTriAPI

#include "const.h"
#include "entity_state.h"
#include "cl_entity.h"
#include "triangleapi.h"
#include "Exports.h"

#include "particleman.h"
#include "r_studioint.h"
#include "tri.h"
extern IParticleMan *g_pParticleMan;

extern float g_iFogColor[ 4 ];
extern float g_iStartDist;
extern float g_iEndDist;
extern int g_iWaterLevel;
extern vec3_t FogColor;

extern engine_studio_api_t IEngineStudio;

void BlackFog( void )
{
	static float fColorBlack[ 3 ] = { 0,0,0 };
	bool bFog = g_iStartDist > 0 && g_iEndDist > 0;
	if( bFog )
		gEngfuncs.pTriAPI->Fog( fColorBlack, g_iStartDist, g_iEndDist, bFog );
	else
		gEngfuncs.pTriAPI->Fog( g_iFogColor, g_iStartDist, g_iEndDist, bFog );
}

void RenderFog( void )
{
	float g_iFogColor[ 4 ] = { FogColor.x, FogColor.y, FogColor.z, 1.0 };
	bool bFog = g_iStartDist > 0 && g_iEndDist > 0;
	if( bFog )
	{
		if( IEngineStudio.IsHardware() == 2 )
		{
			gEngfuncs.pTriAPI->Fog( g_iFogColor, g_iStartDist, g_iEndDist, bFog );
		}
		else if( IEngineStudio.IsHardware() == 1 )
		{
			glEnable( GL_FOG );
			glFogi( GL_FOG_MODE, GL_LINEAR );
			glFogfv( GL_FOG_COLOR, g_iFogColor );
			glFogf( GL_FOG_DENSITY, 1.0f );
			glHint( GL_FOG_HINT, GL_DONT_CARE );
			glFogf( GL_FOG_START, g_iStartDist );
			glFogf( GL_FOG_END, g_iEndDist );
		}
	}
}

/*
=================
HUD_DrawNormalTriangles

Non-transparent triangles-- add them here
=================
*/
void CL_DLLEXPORT HUD_DrawNormalTriangles( void )
{
//	RecClDrawNormalTriangles();

	RenderFog();

	gHUD.m_Spectator.DrawOverview();
}

#if defined( _TFC )
void RunEventList( void );
#endif

/*
=================
HUD_DrawTransparentTriangles

Render any triangles with transparent rendermode needs here
=================
*/
void CL_DLLEXPORT HUD_DrawTransparentTriangles( void )
{
//	RecClDrawTransparentTriangles();

#if defined( _TFC )
	RunEventList();
#endif

	BlackFog();

	if ( g_pParticleMan )
		 g_pParticleMan->Update();
}
