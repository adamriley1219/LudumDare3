#pragma once
#include "Engine/Math/Vec2.hpp"
#include "Engine/Renderer/Camera.hpp"

//--------------------------------------------------------------------------

class RenderContext;

//--------------------------------------------------------------------------

class SpaceCamera 
	: public Camera
{
public:
	SpaceCamera();
	~SpaceCamera();

public:
	// Called each frame to update the underlying camera with the RTSCamera's options; 
	void Update( float deltaSeconds ); 

	void SetOffset( Vec2 const &pos ); 
	void AddToOffset( Vec2 const &offset ); 
	void SetZoom( float zoom ); 
	void AddZoom( float zoom ); 

	void BindCamera( RenderContext* context );

public:
	AABB2 m_screen_boarder			= AABB2( -200.0f, -100.0f, 200.0f, 100.0f );
	Vec2 m_screen_offset			= Vec2( 0.0f, 0.0f ); 
	float m_zoom					= 1.0f;

	// configuration - mess with these numbers to get a view you like; 
	float m_minZoomIn					= .1f;
	float m_maxZoomOut					= 4.0f;


}; 