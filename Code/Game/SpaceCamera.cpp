#include "Game/SpaceCamera.hpp"
#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Math/Vec3.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"

//--------------------------------------------------------------------------
/**
* RTSCamera
*/
SpaceCamera::SpaceCamera()
{
}

//--------------------------------------------------------------------------
/**
* ~RTSCamera
*/
SpaceCamera::~SpaceCamera()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void SpaceCamera::Update( float deltaSeconds )
{
	UNUSED( deltaSeconds );
	Vec2 min = m_screen_boarder.m_bottomLeft * m_zoom; 
	Vec2 max = m_screen_boarder.m_topRight * m_zoom;
		
	max += m_screen_offset;
	min += m_screen_offset;

	SetOrthographicProjection( min, max );
}

//--------------------------------------------------------------------------
/**
* SetFocalPoint
*/
void SpaceCamera::SetOffset( Vec2 const &pos )
{
	m_screen_offset = pos;
	Vec2 world_half_dims( GAME_HALF_WIDTH, GAME_HALF_HEIGHT );
	m_screen_offset = Clamp( m_screen_offset, world_half_dims * -1.0f, world_half_dims );
}

//--------------------------------------------------------------------------
/**
* AddToFocalPoint
*/
void SpaceCamera::AddToOffset( Vec2 const& offset )
{
	float dependent_zoom_var = m_zoom * 1.0f;
	m_screen_offset += offset * dependent_zoom_var;
	Vec2 world_half_dims( GAME_HALF_WIDTH, GAME_HALF_HEIGHT );
	m_screen_offset = Clamp( m_screen_offset, world_half_dims * -1.0f, world_half_dims );
}

//--------------------------------------------------------------------------
/**
* SetZoom
*/
void SpaceCamera::SetZoom( float zoom )
{
	m_zoom = zoom;
	m_zoom = Clamp( m_zoom, m_minZoomIn, m_maxZoomOut );
}


//--------------------------------------------------------------------------
/**
* AddZoom
*/
void SpaceCamera::AddZoom( float amount_zoom )
{
	if( amount_zoom != 0.0f )
	{
		m_zoom += amount_zoom;
		m_zoom = Clamp( m_zoom, m_minZoomIn, m_maxZoomOut );
	}
}

//--------------------------------------------------------------------------
/**
* BindCamera
*/
void SpaceCamera::BindCamera( RenderContext* context )
{
	SetColorTargetView( context->GetColorTargetView() );
	SetDepthTargetView( context->GetDepthTargetView() );
	context->BeginCamera( this );
}

