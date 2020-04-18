#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/IntVec2.hpp"

class GameController
{
public:
	GameController();
	~GameController();

public:
	void Update( float deltaSec ); 

	Vec2 GetFramePan() const; 
	float GetFrameZoomDir() const; 

public:
	float m_panSpeed					= 32.0f; 

	float m_zoomSpeed					= 24.0f; 
	float m_curZoom						= 0.0f;
	float m_zoomDirThisFrame			= 0.0f;

	IntVec2 m_mousePos;
}; 