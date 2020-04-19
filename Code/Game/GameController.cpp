#include "Game/GameController.hpp"
#include "Game/GameCommon.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/WindowContext.hpp"
#include "Game/Game.hpp"
#include "Game/SpaceCamera.hpp"

//--------------------------------------------------------------------------
/**
* GameController
*/
GameController::GameController()
{
	g_theWindowContext->SetMouseMode(MOUSE_MODE_ABSOLUTE);
	g_theWindowContext->ShowMouse();
	g_theWindowContext->LockMouse();
}

//--------------------------------------------------------------------------
/**
* ~GameController
*/
GameController::~GameController()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void GameController::Update( float deltaSec )
{
	UNUSED( deltaSec );
	m_mousePos = g_theWindowContext->GetClientMousePosition();
 	
	float wheelOffset		= g_theInputSystem->GetMouseScroll();
	m_zoomDirThisFrame		= wheelOffset;

	g_theInputSystem->WheelMovement( 0.0f );
}


//--------------------------------------------------------------------------
/**
* GetFramePan
*/
Vec2 GameController::GetFramePan() const
{
	Vec2 ret = Vec2::ZERO;
	ret += Vec2( g_theInputSystem->KeyIsDown( KEY_LEFTARROW )		| g_theInputSystem->KeyIsDown( KEY_A )	? -1.0f : 0.0f, 0.0f );
	ret += Vec2( g_theInputSystem->KeyIsDown( KEY_RIGHTARROW )		| g_theInputSystem->KeyIsDown( KEY_D )	? 1.0f : 0.0f, 0.0f );
	ret += Vec2( 0.0f, g_theInputSystem->KeyIsDown( KEY_DOWNARROW )	| g_theInputSystem->KeyIsDown( KEY_S )	? -1.0f : 0.0f );
	ret += Vec2( 0.0f, g_theInputSystem->KeyIsDown( KEY_UPARROW )	| g_theInputSystem->KeyIsDown( KEY_W )	? 1.0f : 0.0f );

	IntVec2 pos = g_theWindowContext->GetClientMousePosition();
	AABB2 screen = g_theWindowContext->GetClientScreen();

	ret += Vec2( pos.x >= screen.GetTopRight().x - 1 ? 1.0f : 0.0f, 0.0f );
	ret += Vec2( pos.x <= screen.GetBottomLeft().x ? -1.0f : 0.0f, 0.0f );
	ret += Vec2( 0.0f, pos.y >= screen.GetBottomLeft().y - 1 ? -1.0f : 0.0f );
	ret += Vec2( 0.0f, pos.y <= screen.GetTopRight().y ? 1.0f : 0.0f );

	ret.Normalize();
	ret *= m_panSpeed;
	return ret;
}

//--------------------------------------------------------------------------
/**
* GetFrameZoom
*/
float GameController::GetFrameZoomDir() const
{
	return m_zoomDirThisFrame;
}

//--------------------------------------------------------------------------
/**
* GetUIMousePos
*/
Vec2 GameController::GetUIMousePos() const
{
	AABB2 clientScreen = g_theWindowContext->GetClientScreen();

	float ui_x = RangeMapFloat( (float)m_mousePos.x, 0, clientScreen.GetWidth(), 0, UI_SCREEN.GetWidth() );
	float ui_y = RangeMapFloat( (float)m_mousePos.y, fabsf(clientScreen.GetHeight()), 0, 0, UI_SCREEN.GetHeight() );

	return Vec2( ui_x, ui_y );
}

//--------------------------------------------------------------------------
/**
* GetGameMousePos
*/
Vec2 GameController::GetGameMousePos() const
{
	AABB2 clientScreen = g_theWindowContext->GetClientScreen();

	Vec2& offset = m_camera.m_screen_offset;
	AABB2 gameScreen = m_camera.m_screen_boarder;
	gameScreen.ScaleUniform( m_camera.m_zoom );
	gameScreen.AddPosition( offset );

	float ui_x = RangeMapFloat( (float)m_mousePos.x, 0, clientScreen.GetWidth(), gameScreen.m_bottomLeft.x, gameScreen.m_topRight.x );
	float ui_y = RangeMapFloat( (float)m_mousePos.y, fabsf(clientScreen.GetHeight()), 0, gameScreen.m_bottomLeft.y, gameScreen.m_topRight.y );

	return Vec2( ui_x, ui_y );
}
