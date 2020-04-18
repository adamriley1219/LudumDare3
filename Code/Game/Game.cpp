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
#include "Game/InhabitablePlanet.hpp"
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

	m_DevColsoleCamera.SetOrthographicProjection( UI_SCREEN.m_bottomLeft, UI_SCREEN.m_topRight );
	m_DevColsoleCamera.SetModelMatrix( Matrix44::IDENTITY );

	AABB2 exitBounds = UI_SCREEN;
	exitBounds.CarveBoxOffBottom( .95f );
	exitBounds.CarveBoxOffLeft( .95f );
	exitBounds.CarveBoxOffTop( .1f );
	exitBounds.CarveBoxOffRight( .1f );

	m_UICanvas.UpdateBounds( UI_SCREEN );

	UIButton* exit = m_UICanvas.CreateChild<UIButton>();
	exit->m_text				= "EXIT";
	exit->m_eventOnClick		= "quit";
	exit->m_nutralColor			= Rgba(.3f, .8f, .1f);
	exit->m_nutralBoarderColor	= Rgba::BLACK;
	exit->m_hoveredBoarderColor = Rgba::RED;
	exit->m_hoveredColor		= Rgba(.8f, .3f, .1f);
	exit->m_boarderThickness	= 5.0f;
	exit->m_boarderScaleWithParent = false;
	exit->UpdateBounds( exitBounds );

	TextureView* planet_texture = (TextureView*) g_theRenderer->CreateOrGetTextureViewFromFile( PLANET_TEXTURE_PATH, true );
	m_planet_sheet = new SpriteSheet( planet_texture, IntVec2( 5,5 ) );
	
	m_root_planet = new InhabitablePlanet();
	m_root_planet->m_pos = Vec2::ZERO;
	m_root_planet->sheet_idx = 7;
	m_root_planet->m_planet_sheet = m_planet_sheet;
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

	RenderPlanets();

	g_theDebugRenderSystem->RenderToCamera( &m_controller.m_camera );

	m_DevColsoleCamera.SetColorTargetView( g_theRenderer->GetColorTargetView() );
	m_DevColsoleCamera.SetDepthTargetView( g_theRenderer->GetDepthTargetView() );
	g_theRenderer->BeginCamera( &m_DevColsoleCamera );
	g_theRenderer->SetBlendMode( BLEND_MODE_ALPHA );
	m_UICanvas.Render();
}

//--------------------------------------------------------------------------
/**
* UpdateGame
*/
void Game::UpdateGame( float deltaSeconds )
{
	m_controller.Update( deltaSeconds );
	
	UpdateUI();

	m_root_planet->Update( deltaSeconds );

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

	m_controller.m_camera.AddZoom( zoom_amount );
	m_controller.m_camera.AddToOffset( movement );
	m_controller.m_camera.Update( deltaSeconds );
	m_controller.m_camera.BindCamera( g_theRenderer );
}

//--------------------------------------------------------------------------
/**
* UpdateUI
*/
void Game::UpdateUI()
{
	Vec2 mousePos = m_controller.GetUIMousePos();
	if( g_theInputSystem->KeyWasPressed( MOUSE_L ) )
	{
		Event event(Stringf("click x=%f y=%f", mousePos.x, mousePos.y));
		m_UICanvas.ProcessInput(event);
	}
	else
	{
		Event event(Stringf("hover x=%f y=%f", mousePos.x, mousePos.y));
		m_UICanvas.ProcessInput(event);
	}
}

//--------------------------------------------------------------------------
/**
* RenderBackground
*/
void Game::RenderBackground() const
{
	float client_pos_int_x = (float)m_controller.m_camera.m_screen_offset.x - BOARDER_HALF_WIDTH;
	float client_pos_int_y = (float)m_controller.m_camera.m_screen_offset.y - BOARDER_HALF_HEIGHT;

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

//--------------------------------------------------------------------------
/**
* RenderPlanets
*/
void Game::RenderPlanets() const
{
	std::vector<Vertex_PCU> verts;

	m_root_planet->AddVertsForRender( verts );

	if( !verts.empty() )
	{
		g_theRenderer->BindTextureView( 0, PLANET_TEXTURE_PATH );
		g_theRenderer->DrawVertexArray( verts );
	}
}
