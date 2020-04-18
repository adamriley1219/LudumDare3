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
	RenderBackground();

	std::vector<Vertex_PCU> verts;
	AddVertsForDisc2D(verts, m_curentCamera.m_screen_offset, 2, Rgba::RED);
	g_theRenderer->BindTextureView(TEXTURE_SLOT_ALBEDO, nullptr);
	g_theRenderer->DrawVertexArray(verts);

	verts.clear();
	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelPix_MedBoldProp9_16x16", true );
	font->AddVertsFor2DText( verts, m_curentCamera.m_screen_offset, 4, Stringf( "Pos: %.02f, %.02f", m_curentCamera.m_screen_offset.x, m_curentCamera.m_screen_offset.y ).c_str() );

	g_theRenderer->BindTextureView( 0, font->GetTextureView() );
	g_theRenderer->SetBlendMode( eBlendMode::BLEND_MODE_ALPHA );
	g_theRenderer->DrawVertexArray(verts);
	
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

//--------------------------------------------------------------------------
/**
* RenderBackground
*/
void Game::RenderBackground() const
{
	float client_pos_int_x = (float)m_curentCamera.m_screen_offset.x - BOARDER_HALF_WIDTH;
	float client_pos_int_y = (float)m_curentCamera.m_screen_offset.y - BOARDER_HALF_HEIGHT;

	bool neg_x = client_pos_int_x < 0.0f;
	bool neg_y = client_pos_int_y < 0.0f;

	float client_amount_into_boarder_x = fabsf(client_pos_int_x);
	int offset_x = 0;
	while (client_amount_into_boarder_x > BOARDER_WIDTH)
	{
		client_amount_into_boarder_x -= BOARDER_WIDTH;
		++offset_x;
	}

	float client_amount_into_boarder_y = fabsf(client_pos_int_y);
	int offset_y = 0;
	while (client_amount_into_boarder_y > BOARDER_HEIGHT)
	{
		client_amount_into_boarder_y -= BOARDER_HEIGHT;
		++offset_y;
	}


	if( neg_x )
	{
		offset_x *= -1;
	}
	else
	{
		++offset_x;
	}

	if( neg_y )
	{
		offset_y *= -1;
	}
	else
	{
		++offset_y;
	}

	float boarder_stride_width		= BOARDER_WIDTH * offset_x;
	float boarder_stride_height		= BOARDER_HEIGHT * offset_y;

	Vec2 botLeft( -BOARDER_HALF_WIDTH, -BOARDER_HALF_HEIGHT );
	Vec2 topRight( BOARDER_HALF_WIDTH, BOARDER_HALF_HEIGHT );

	AABB2 world_background( botLeft, topRight );

	Vec2 offset( boarder_stride_width, boarder_stride_height );

	world_background.AddPosition( offset );
	AABB2 topLeftBrd = world_background.GetOffset(	Vec2( -BOARDER_WIDTH, BOARDER_HEIGHT )	);
	AABB2 leftBrd = world_background.GetOffset(		Vec2( -BOARDER_WIDTH, 0 )				);
	AABB2 botLeftBrd = world_background.GetOffset(	Vec2( -BOARDER_WIDTH, -BOARDER_HEIGHT )	);

	AABB2 topBrd = world_background.GetOffset(		Vec2( 0, BOARDER_HEIGHT )	);
	AABB2 botBrd = world_background.GetOffset(		Vec2( 0, -BOARDER_HEIGHT )	);

	AABB2 topRightBrd = world_background.GetOffset(	Vec2( BOARDER_WIDTH, BOARDER_HEIGHT )	);
	AABB2 rightBrd = world_background.GetOffset(	Vec2( BOARDER_WIDTH, 0 )				);
	AABB2 botRightBrd = world_background.GetOffset(	Vec2( BOARDER_WIDTH, -BOARDER_HEIGHT )	);

	std::vector<Vertex_PCU> verts;
	AddVertsForAABB2D( verts, world_background, Rgba::WHITE );

	AddVertsForAABB2D( verts, topLeftBrd, Rgba::WHITE );
	AddVertsForAABB2D( verts, leftBrd, Rgba::WHITE );
	AddVertsForAABB2D( verts, botLeftBrd, Rgba::WHITE );

	AddVertsForAABB2D( verts, topBrd, Rgba::WHITE );
	AddVertsForAABB2D( verts, botBrd, Rgba::WHITE );

	AddVertsForAABB2D( verts, topRightBrd, Rgba::WHITE );
	AddVertsForAABB2D( verts, rightBrd, Rgba::WHITE );
	AddVertsForAABB2D( verts, botRightBrd, Rgba::WHITE );

	static TextureView* background_texture = (TextureView*)g_theRenderer->CreateOrGetTextureViewFromFile(BACKGROUND_TEXTURE_PATH, true);

	g_theRenderer->BindShader(m_shader);
	g_theRenderer->BindTextureViewWithSampler(eTextureSlot::TEXTURE_SLOT_ALBEDO, background_texture, eSampleMode::SAMPLE_MODE_POINT);
	g_theRenderer->DrawVertexArray(verts);
}
