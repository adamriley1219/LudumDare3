#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Math/IntVec2.hpp"

#include "Game/SpaceCamera.hpp"

class GameController
{
public:
	GameController();
	~GameController();

public:
	void Update( float deltaSec ); 

	Vec2 GetFramePan() const; 
	float GetFrameZoomDir() const; 

	Vec2 GetUIMousePos() const;
	Vec2 GetGameMousePos() const;

public:
	float m_panSpeed					= 32.0f; 

	float m_zoomSpeed					= 24.0f; 
	float m_curZoom						= 0.0f;
	float m_zoomDirThisFrame			= 0.0f;

	mutable SpaceCamera m_camera;

	IntVec2 m_mousePos;
}; 