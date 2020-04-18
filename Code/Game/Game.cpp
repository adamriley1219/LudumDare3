#include "Engine/Renderer/RenderContext.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/SpriteDefinition.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/Shaders/Shader.hpp"
#include "Engine/Renderer/SpriteAnimDefinition.hpp"
#include "Engine/Renderer/Debug/DebugRenderSystem.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/IntVec2.hpp"
#include "Engine/Math/Vec2.hpp"

#include "Engine/Core/Debug/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Time/Clock.hpp"
#include "Engine/Memory/Debug/Profiler.hpp"

#include "Engine/ImGUI/ImGUISystem.hpp"
#include "Engine/ImGUI/ImGUI_Interfaces/ProfilerDisplay.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"
#include <vector>

#include <Math.h>
//--------------------------------------------------------------------------
/**
* Game
*/
Game::Game()
{

}

//--------------------------------------------------------------------------
/**
* ~Game
*/
Game::~Game()
{

}

//--------------------------------------------------------------------------
/**
* GameStartup
*/
void Game::Startup()
{
	m_shader = g_theRenderer->CreateOrGetShaderFromXML( "Data/Shaders/shader.xml" );
	g_theRenderer->m_shader = m_shader;

	m_DevColsoleCamera.SetOrthographicProjection( Vec2( -100.0f, -50.0f ), Vec2( 100.0f,  50.0f ) );
	m_DevColsoleCamera.SetModelMatrix( Matrix44::IDENTITY );
}

//--------------------------------------------------------------------------
/**
* Shutdown
*/
void Game::Shutdown()
{

}

//--------------------------------------------------------------------------
/**
* HandleKeyPressed
*/
bool Game::HandleKeyPressed( unsigned char keyCode )
{
	UNUSED(keyCode);
	return false;
}

//--------------------------------------------------------------------------
/**
* HandleKeyReleased
*/
bool Game::HandleKeyReleased( unsigned char keyCode )
{
	UNUSED(keyCode);
	return false;
}

//--------------------------------------------------------------------------
/**
* GameRender
*/
void Game::GameRender() const
{
	int client_pos_int_x = (int) m_curentCamera.m_screen_offset.x; 
	int client_pos_int_y = (int) m_curentCamera.m_screen_offset.y;

	int clent_amount_into_boarder_x = client_pos_int_x % (int) BOARDER_WIDTH;
	int clent_amount_into_boarder_y = client_pos_int_y % (int) BOARDER_HALF_HEIGHT;

	std::vector<Vertex_PCU> verts;
	Vec2 client_offset_into_cur_background( (float)clent_amount_into_boarder_x, (float)clent_amount_into_boarder_y );

	Vec2 botLeft( -BOARDER_HALF_WIDTH, -BOARDER_HALF_HEIGHT );
	Vec2 topRight( BOARDER_HALF_WIDTH, BOARDER_HALF_HEIGHT );

	AABB2 world_backgroud( botLeft, topRight );
	AddVertsForAABB2D( verts, world_backgroud, Rgba::WHITE );
	world_backgroud.AddPosition( client_offset_into_cur_background );

	static TextureView* background_texture = (TextureView*) g_theRenderer->CreateOrGetTextureViewFromFile( BACKGROUND_TEXTURE_PATH, true );
	
	AddVertsForAABB2D( verts, world_backgroud, Rgba::WHITE );
	g_theRenderer->BindShader( m_shader );
	g_theRenderer->BindTextureViewWithSampler( eTextureSlot::TEXTURE_SLOT_ALBEDO, background_texture, eSampleMode::SAMPLE_MODE_POINT );
	g_theRenderer->DrawVertexArray( verts );

	g_theDebugRenderSystem->RenderToCamera( &m_curentCamera );
}

//--------------------------------------------------------------------------
/**
* UpdateGame
*/
void Game::UpdateGame( float deltaSeconds )
{
	m_controller.Update( deltaSeconds );
	
	UpdateCamera( deltaSeconds );
}

//--------------------------------------------------------------------------
/**
* UpdateCamera
*/
void Game::UpdateCamera( float deltaSeconds )
{
	UNUSED( deltaSeconds );
	
	Vec2 movement		= m_controller.GetFramePan() * deltaSeconds;
	float zoom_amount	= m_controller.GetFrameZoomDir() * m_controller.m_zoomSpeed * deltaSeconds;

	m_curentCamera.AddZoom( zoom_amount );
	m_curentCamera.AddToOffset( movement );
	m_curentCamera.Update( deltaSeconds );
	m_curentCamera.BindCamera( g_theRenderer );
}
