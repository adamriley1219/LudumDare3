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
#include "Engine/Math/RNG.hpp"

#include "Engine/Core/Debug/DevConsole.hpp"
#include "Engine/Core/EventSystem.hpp"
#include "Engine/Core/Time/Clock.hpp"
#include "Engine/Core/Time/Time.hpp"
#include "Engine/Memory/Debug/Profiler.hpp"

#include "Engine/ImGUI/ImGUISystem.hpp"
#include "Engine/ImGUI/ImGUI_Interfaces/ProfilerDisplay.hpp"
#include "Engine/Input/InputSystem.hpp"

#include "Game/Game.hpp"
#include "Game/GameCommon.hpp"
#include "Game/App.hpp"
#include "Game/Planet.hpp"
#include "Game/TradeRoute.hpp"
#include <vector>

#include <Math.h>
//--------------------------------------------------------------------------
/**
* Game
*/
Game::Game()
{
	m_planets.reserve( NUM_PLANETS );
	m_suns.reserve( NUM_SUNS );
	m_trade_routes.reserve( NUM_TRADEROUTES_PER_PLANET * NUM_PLANETS );
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
	g_theRNG->Seed( (int)GetCurrentTimeSeconds() );

	m_DevColsoleCamera.SetOrthographicProjection( UI_SCREEN.m_bottomLeft, UI_SCREEN.m_topRight );
	m_DevColsoleCamera.SetModelMatrix( Matrix44::IDENTITY );

	AABB2 exitBounds = UI_SCREEN;
	exitBounds.CarveBoxOffBottom( .95f );
	exitBounds.CarveBoxOffLeft( .95f );
	exitBounds.CarveBoxOffTop( .1f );
	exitBounds.CarveBoxOffRight( .1f );

	AABB2 statsBounds = UI_SCREEN;
	statsBounds.CarveBoxOffBottom( .80f );
	statsBounds.CarveBoxOffRight( .90f );
	statsBounds.CarveBoxOffLeft( .1f );
	statsBounds.CarveBoxOffTop( .1f );

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

	Rgba stats_color(1.0f, 0.7f, 0.1f);
	float label_height = 2.0f;

	m_cycles_label = m_UICanvas.CreateChild<UILabel>();
	m_cycles_label->m_text = "Temp text";
	m_cycles_label->m_color = stats_color;
	m_cycles_label->UpdateBounds( statsBounds.CarveBoxOffTop( 0, label_height ) );

	m_totpop_label = m_UICanvas.CreateChild<UILabel>();
	m_totpop_label->m_text = "Temp text";
	m_totpop_label->m_color = stats_color;
	m_totpop_label->UpdateBounds( statsBounds.CarveBoxOffTop( 0, label_height ) );

	UILabel* ctrl_label = m_UICanvas.CreateChild<UILabel>();
	ctrl_label->m_text = "Hold CTRL to hover all";
	ctrl_label->UpdateBounds( statsBounds.CarveBoxOffTop( 0, label_height ) );

	TextureView* planet_texture = (TextureView*) g_theRenderer->CreateOrGetTextureViewFromFile( PLANET_TEXTURE_PATH, true );
	m_planet_sheet = new SpriteSheet( planet_texture, IntVec2( 5,5 ) );
	
	m_cycle_timer = new StopWatch( g_theApp->GetGameClock() );
	m_cycle_timer->SetAndReset( 1.0f );

	Planet root_planet;
	root_planet.m_pos = Vec2( 18.0f, -5.0f );
	root_planet.sheet_idx = 7;
	m_planets.push_back( root_planet );

	for( int planet_count = 1; planet_count < NUM_PLANETS; ++planet_count )
	{
		Planet planet;
		planet.population = 0;
		planet.m_pos = Vec2(g_theRNG->GetRandomFloatInRange( -GAME_HALF_WIDTH, GAME_HALF_WIDTH ), g_theRNG->GetRandomFloatInRange( -GAME_HALF_HEIGHT, GAME_HALF_HEIGHT ));
		planet.sheet_idx = 7;
		m_planets.push_back( planet );
	}

	Sun root_sun;
	root_sun.m_pos = Vec2(-15.0f, 7.0f);
	root_sun.sheet_idx = 23;
	m_suns.push_back(root_sun);

	for( int sun_count = 1; sun_count < NUM_SUNS; ++sun_count )
	{
		Sun sun;
		sun.m_pos = Vec2(g_theRNG->GetRandomFloatInRange(-GAME_HALF_WIDTH, GAME_HALF_WIDTH), g_theRNG->GetRandomFloatInRange(-GAME_HALF_HEIGHT, GAME_HALF_HEIGHT));
		sun.influence_radius = 25.0f;
		sun.sheet_idx  = 23;
		m_suns.push_back( sun );
	}

	for( Planet& planet : m_planets )
	{
		CreateTradeRoutesFor( planet );
	}

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

	RenterTradeRoutes();
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

	m_total_population = 0;
	for( Planet& planet : m_planets )
	{
		planet.Update( deltaSeconds );
		m_total_population += planet.population;
	}

	for( Sun& sun : m_suns )
	{
		sun.Update( deltaSeconds );
	}

	if( m_cycle_timer->HasElapsed() )
	{
		UpdateCycle();
		m_cycle_timer->Reset();
	}

	m_totpop_label->m_text = Stringf( "Population: %u", m_total_population );
	m_cycles_label->m_text = Stringf( "Cycles: %u", m_num_cycles );

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
	std::vector<Vertex_PCU> text_verts;

	for( const Planet& planet : m_planets )
	{
		planet.AddVertsForRender( verts );
		planet.AddVertsForInfoText( text_verts );
	}

	for( const Sun& sun : m_suns )
	{
		sun.AddVertsForRender(verts);
		sun.AddVertsForInfoText(text_verts);
	}

	if( !verts.empty() )
	{
		g_theRenderer->BindTextureView( 0, PLANET_TEXTURE_PATH );
		g_theRenderer->DrawVertexArray( verts );
	}

	if( !text_verts.empty() )
	{
		g_theRenderer->BindTextureView( 0, g_theRenderer->CreateOrGetBitmapFromFile( FONT )->GetTextureView() );
		g_theRenderer->DrawVertexArray( text_verts );
	}
}

//--------------------------------------------------------------------------
/**
* RenterTradeRoutes
*/
void Game::RenterTradeRoutes() const
{
	std::vector<Vertex_PCU> verts;
	for( const TradeRoute& tr : m_trade_routes )
	{
		tr.AddForRender( verts );
	}

	if( !verts.empty() )
	{
		g_theRenderer->BindTextureView( TEXTURE_SLOT_ALBEDO, ROUTE_TEXTURE_PATH );
		g_theRenderer->DrawVertexArray( verts );
	}
}

//--------------------------------------------------------------------------
/**
* UpdateCycle
*/
void Game::UpdateCycle()
{
	++m_num_cycles;

	for( Sun& sun : m_suns )
	{
		UpdateCycleWithSun( sun );
	}

	for( TradeRoute& tr : m_trade_routes )
	{
		UpdateCycleWithTradeRoute( tr );
	}

	for( Planet& planet : m_planets )
	{
		planet.UpdateForCycle();
	}
}

//--------------------------------------------------------------------------
/**
* UpdateCycleWithSun
*/
void Game::UpdateCycleWithSun( Sun& sun )
{
	sun.energy -= sun.release_rate;

	for( Planet& planet : m_planets )
	{
		planet.UpdateFromSun( sun );
	}
}

//--------------------------------------------------------------------------
/**
* UpdateCycleWithTradeRoute
*/
void Game::UpdateCycleWithTradeRoute( TradeRoute& trade_route )
{

}

//--------------------------------------------------------------------------
/**
* CreateTradeRoutesFor
*/
void Game::CreateTradeRoutesFor( Planet& planet )
{
	for( int tr_count = 0; tr_count < NUM_TRADEROUTES_PER_PLANET; ++tr_count )
	{
		//Planet
	}
}

//--------------------------------------------------------------------------
/**
* GetCLosestPlanetExcluding
*/
Planet* Game::GetCLosestPlanetExcluding( std::vector<Planet*> planets )
{
	return nullptr;
}
