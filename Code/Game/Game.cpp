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
	ConstructGame();
}

//--------------------------------------------------------------------------
/**
* ~Game
*/
Game::~Game()
{
	DeconstructGame();
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

static int g_index = 0;

//--------------------------------------------------------------------------
/**
* HandleKeyPressed
*/
bool Game::HandleKeyPressed( unsigned char keyCode )
{
	if( keyCode == 'O' )
	{
		g_index = ++g_index % ( 8 * 2 );
	}
	if( keyCode == 'W' )
	{
		EventArgs args;
		g_theEventSystem->FireEvent( "test" );
		g_theEventSystem->FireEvent( "test", args );
		g_theConsole->PrintString( args.GetValue( "test1", "NOT FOUND" ), DevConsole::CONSOLE_INFO );
	}
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


BitmapFont* g_testBitmap = nullptr;
float g_pingPongTimer = 0.0f;
int g_printGlyphCount = 0;
float g_charTimer = 0.0f;

//--------------------------------------------------------------------------
/**
* GameRender
*/
void Game::GameRender() const
{
	g_theRenderer->BindShader( m_shader );

	static TextureView2D* testTextureView = g_theRenderer->CreateOrGetTextureViewFromFile( "Data/Images/Test_StbiFlippedAndOpenGL.png", true );

	std::vector<Vertex_PCU> verts;
	g_theRenderer->BindTextureView( 0, (TextureView*) testTextureView );
	g_theRenderer->BindSampler( SAMPLE_MODE_LINEAR );
	AddVertsForAABB2D( verts, AABB2( 50.f, 50.f, 80.f, 80.f ), Rgba( 1.f, 1.f, 1.f ), Vec2( 0.0f, 0.0f ), Vec2( 1.0f, 1.0f ) );
	g_theRenderer->DrawVertexArray( (int) verts.size(), &verts[0] );
	verts.clear();

	g_theRenderer->BindTextureView( 0, nullptr );
	AddVertsForLine2D( verts, Vec2( 10.0f, 90.0f ), Vec2( 150.0f, 10.0f ), 1.0f, Rgba( 1.0f, 0.1f, 0.1f ) );
	AddVertsForDisc2D( verts, Vec2( 150.0f, 70.0f ), 20.0f, Rgba( 0.1f, 0.1f, 1.0f ) );
	AddVertsForRing2D( verts, Vec2( 120.0f, 30.0f ), 15.0f, 3.0f, Rgba( 1.0f, 1.0f, 1.0f ), 5 );
	AddVertsForRing2D( verts, Vec2( 145.0f, 30.0f ), 10.0f, 1.0f, Rgba( 0.4f, 1.0f, 0.4f ) );

	g_theRenderer->DrawVertexArray( (int) verts.size(), &verts[0] );


	// Test of spriteSheets
	static TextureView2D* testTexture2 = g_theRenderer->CreateOrGetTextureViewFromFile( "Data/Images/Test_SpriteSheet8x2.png", true );

	SpriteSheet spriteSheet( (TextureView*)testTexture2, IntVec2( 8, 2 ), Vec2::ZERO, Vec2::ONE );
	Vec2 uvAtBottomLeft = Vec2(0.0f, 0.0f);
	Vec2 uvAtTopRight = Vec2(1.0f, 1.0f);
	SpriteDefinition sd = spriteSheet.GetSpriteDefinition( g_index );
	sd.GetUVs(uvAtBottomLeft, uvAtTopRight);
	std::vector<Vertex_PCU> ssVerts;
	AddVertsForAABB2D(ssVerts, AABB2( 85.f, 80.f, 90.f, 90 ), Rgba( 1.0f, 1.0f, 1.0f ), uvAtBottomLeft, uvAtTopRight );

	g_theRenderer->BindTextureView( 0, spriteSheet.GetTextureView() );
	g_theRenderer->DrawVertexArray( ssVerts );

	// Bitmap Test
	if( !g_testBitmap )
	{
		g_testBitmap = g_theRenderer->CreateOrGetBitmapFromFile(  "SquirrelFixedFont" );
	}
	g_theRenderer->BindSampler( eSampleMode::SAMPLE_MODE_POINT );
	g_theRenderer->SetBlendMode( BLEND_MODE_ALPHA );

	std::vector<Vertex_PCU> bmVerts;
	g_testBitmap->AddVertsFor2DText( bmVerts, Vec2( 110.0f, 50.0f ), 5.0f, "HELLO, WORLD", .5f);

	g_theRenderer->BindTextureView( 0, (TextureView*)g_testBitmap->GetTextureView() );
	g_theRenderer->DrawVertexArray( bmVerts );


	SpriteAnimDefinition spriteAnimDef( spriteSheet, 0, 15, 30.0f, SPRITE_ANIM_PLAYBACK_PINGPONG );
	uvAtBottomLeft = Vec2(0.0f, 0.0f);
	uvAtTopRight = Vec2(1.0f, 1.0f);
	sd = spriteAnimDef.GetSpriteDefAtTime( (float)g_theApp->GetGameClock()->GetTotalTime() ); 
	sd.GetUVs(uvAtBottomLeft, uvAtTopRight);
	ssVerts.clear();
	AddVertsForAABB2D(ssVerts, AABB2( 95.f, 80.f, 100.f, 90 ), Rgba( 1.0f, 1.0f, 1.0f ), uvAtBottomLeft, uvAtTopRight );

	g_theRenderer->BindTextureView( 0, spriteSheet.GetTextureView() );
	g_theRenderer->DrawVertexArray( ssVerts );

	float x = SinDegrees( (float) g_theApp->GetGameClock()->GetTotalTime() * 360.f / 3.7f + 17.0f ) + .50f;
	x = x > 0.0f ? x : 0.0f;
	x = x < 1.0f ? x : 1.0f;
	float y = CosDegrees( (float) g_theApp->GetGameClock()->GetTotalTime() * 360.0f / 4.0f ) + .50f;
	y = y > 0.0f ? y : 0.0f;
	y = y < 1.0f ? y : 1.0f;
	ssVerts.clear();
	AABB2 box( 25.0f + x * 30.0f, 8.0f + y * 10.0f , Vec2(WORLD_WIDTH/2.0f + 60.0f, WORLD_HEIGHT/2.0f - 20.0f) );
	AABB2 box2( 23.0f + x * 30.0f, 6.0f + y * 10.0f, Vec2(WORLD_WIDTH/2.0f + 60.0f, WORLD_HEIGHT/2.0f - 20.0f) );
	AddVertsForAABB2D( ssVerts, box, Rgba::CYAN, Vec2::ZERO, Vec2::ONE );
	AddVertsForAABB2D( ssVerts, box2, Rgba::BLUE, Vec2::ZERO, Vec2::ONE );
	g_theRenderer->BindTextureView( 0, nullptr );
	g_theRenderer->DrawVertexArray(ssVerts);

	ssVerts.clear();
	Vec2 alignment( x , y );
	g_testBitmap->AddVertsFor2DTextAlignedInBox( ssVerts, g_pingPongTimer * 1.5f, "Hello!\ngood to go", box2, Vec2::ALIGN_CENTERED, BITMAP_MODE_UNCHANGED, 1.0f, Rgba( 1.0f, 0.0f, 0.0f, 0.5f ), g_printGlyphCount );
	g_testBitmap->AddVertsFor2DTextAlignedInBox( ssVerts, g_pingPongTimer * 1.5f, "Hello!\ngood to go", box2, Vec2::ALIGN_CENTERED, BITMAP_MODE_SHRINK_TO_FIT, 1.0f, Rgba::BLACK, g_printGlyphCount );
	g_testBitmap->AddVertsFor2DTextAlignedInBox( ssVerts, 1.0f, "This is the\nTest for\nAlignment", box2, alignment, BITMAP_MODE_SHRINK_TO_FIT, 1.0f, Rgba::MAGENTA );
	g_theRenderer->BindTextureView( 0, g_testBitmap->GetTextureView() );
	g_theRenderer->DrawVertexArray( ssVerts );

	g_theRenderer->SetBlendMode( eBlendMode::BLEND_MODE_ADDITIVE );
	g_theRenderer->BindShader( g_theRenderer->m_shader );

	static TextureView2D* explosionTex = g_theRenderer->CreateOrGetTextureViewFromFile( "Data/Images/Explosion_5x5.png", true );
	SpriteSheet expSheet( (TextureView*)explosionTex, IntVec2( 5, 5 ), Vec2::ZERO, Vec2::ONE );
	SpriteAnimDefinition expAnimDef( expSheet, 0, 24, 5.0f, SPRITE_ANIM_PLAYBACK_PINGPONG );
	uvAtBottomLeft = Vec2(0.0f, 0.0f);
	uvAtTopRight = Vec2(1.0f, 1.0f);
	sd = expAnimDef.GetSpriteDefAtTime( (float) g_theApp->GetGameClock()->GetTotalTime() ); 
	sd.GetUVs(uvAtBottomLeft, uvAtTopRight);
	std::vector<Vertex_PCU> expVerts;
	AABB2 expBox( 20, 20 );
	expBox.AddPosition( 130, 80 );
	AddVertsForAABB2D(expVerts, expBox, Rgba::WHITE, uvAtBottomLeft, uvAtTopRight );
	expBox.AddPosition( 0, 5 );
	AddVertsForAABB2D(expVerts, expBox , Rgba::WHITE, uvAtBottomLeft, uvAtTopRight );
	expBox.AddPosition( 10 + 10 * SinDegrees( (float) g_theApp->GetGameClock()->GetTotalTime() * 80 ), 3);
	AddVertsForAABB2D(expVerts, expBox, Rgba::WHITE, uvAtBottomLeft, uvAtTopRight );

	g_theRenderer->BindTextureView( 0, expSheet.GetTextureView() );
	g_theRenderer->DrawVertexArray( expVerts );

	g_theRenderer->SetBlendMode( eBlendMode::BLEND_MODE_ALPHA );
	g_theRenderer->BindShader( g_theRenderer->m_shader );


	// Text tests
	RenderTextTeirTest();


	ProfilerDisplayRender();

	// Debug
	if( g_isInDebug )
	{
		RenderDebug();
	}
	g_theDebugRenderSystem->RenderToCamera( &m_CurentCamera );
}

//--------------------------------------------------------------------------
/**
* UpdateGame
*/
void Game::UpdateGame( float deltaSeconds )
{
	static bool swapper = true;
	if( g_pingPongTimer < 0.0f || g_pingPongTimer > 7.0f )
	{
		swapper = !swapper;
		if( g_pingPongTimer < 0.0f )
			g_pingPongTimer = 0.0f;
		else
			g_pingPongTimer = 7.0f;
	}
	if( swapper )
		g_pingPongTimer += deltaSeconds;
	else
		g_pingPongTimer -= deltaSeconds;

	g_charTimer += deltaSeconds;
	g_printGlyphCount = (int) g_charTimer;
	if( g_charTimer > 30.0f )
	{
		g_charTimer = 0.0f;
	}
	ImGUIWidget();
	UpdateCamera( deltaSeconds );
}


//--------------------------------------------------------------------------
/**
* GameRenderRenderDebug
*/
void Game::RenderDebug() const
{
	g_theRenderer->BindTextureView( 0, nullptr );
	RenderDebugCosmetics();
	RenderDebugPhysics();
	g_theRenderer->EndCamera();
	RenderDevConsole();
}

//--------------------------------------------------------------------------
/**
* RenderDebugCosmetics
*/
void Game::RenderDebugCosmetics() const
{

}

//--------------------------------------------------------------------------
/**
* RenderDebugPhysics
*/
void Game::RenderDebugPhysics() const
{

}


//--------------------------------------------------------------------------
/**
* RenderDevConsole
*/
void Game::RenderDevConsole() const
{
	g_theConsole->Render( g_theRenderer, m_DevColsoleCamera, 10 );
}

//--------------------------------------------------------------------------
/**
* ImGUIWidget
*/
void Game::ImGUIWidget()
{
	ImGui::Begin("TestWindow");

	ImGui::Text(Stringf("%s", "100% in there.").c_str());

	bool testCheck1 = true;
	bool testCheck2 = false;
	ImGui::Checkbox("test check box 1", &testCheck1);
	ImGui::Checkbox("test check box 2", &testCheck2);

	if (testCheck2)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "Changed test check 2");
	}
	if (!testCheck1)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "Changed test check 1");
	}

	ImGui::SliderFloat("slidy", &m_slider, 0, 100);

	if (m_slider != 50.0f)
	{
		DebugRenderMessage(0.0f, Rgba::GREEN, Rgba::GREEN, "slidy changed");
	}

	float col[3] = { 1.0f, 1.0f, 1.0f };
	ImGui::ColorEdit3("color thing?", col);

	ImGui::End();
}

//--------------------------------------------------------------------------
/**
* UpdateCamera
*/
void Game::UpdateCamera( float deltaSeconds )
{
	UNUSED( deltaSeconds );
	m_CurentCamera.SetModelMatrix( Matrix44::IDENTITY );
	m_CurentCamera.SetOrthographicProjection( Vec2(), Vec2(WORLD_WIDTH, WORLD_HEIGHT) );
	m_CurentCamera.SetColorTargetView( g_theRenderer->GetColorTargetView() );
	m_CurentCamera.SetDepthTargetView( g_theRenderer->GetDepthTargetView() );
	g_theRenderer->BeginCamera( &m_CurentCamera );
}

//--------------------------------------------------------------------------
/**
* ResetGame
*/
void Game::ResetGame()
{
	DeconstructGame();
	ConstructGame();
}

//--------------------------------------------------------------------------
/**
* ConstructGame
*/
void Game::ConstructGame()
{

}

//--------------------------------------------------------------------------
/**
* DeconstructGame
*/
void Game::DeconstructGame()
{

}

//--------------------------------------------------------------------------
/**
* RenderTextTest
*/
void Game::RenderTextTeirTest() const
{
	g_theRenderer->BindSampler(eSampleMode::SAMPLE_MODE_POINT);

	float text_height = 6.0f;
	AABB2 screen(  Vec2::ZERO, Vec2(WORLD_WIDTH, WORLD_HEIGHT)  );
	AABB2 test_bounding_box = screen.CarveBoxOffBottom( 0.0f, text_height );
	std::vector<Vertex_PCU> verts;

	std::string test_text_tier_1 = "Tier 1 Test -- WWWiiiWiWiWi";
	std::string test_text_tier_2 = "Tier 2 Test -- WWWiiiWiWiWi";
	std::string test_text_tier_3 = "Tier 3 Test -- WWWiiiWiWiWi";

	// T1 test
	BitmapFont* tier_1_font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelFixedFont" );
	g_theRenderer->BindTextureView( eTextureSlot::TEXTURE_SLOT_ALBEDO, tier_1_font->GetTextureView() );

	test_bounding_box = screen.CarveBoxOffBottom( 0.0f, text_height );
	tier_1_font->AddVertsFor2DTextAlignedInBox( verts, text_height, test_text_tier_1.c_str(), test_bounding_box, Vec2::ALIGN_CENTER_LEFT );
	g_theRenderer->DrawVertexArray( verts );

	// T2 test
	BitmapFont* tier_2_font = g_theRenderer->CreateOrGetBitmapFromFile( "SquirrelPix_MedBoldProp9_16x16", true );
	g_theRenderer->BindTextureView( eTextureSlot::TEXTURE_SLOT_ALBEDO, tier_2_font->GetTextureView() );

	verts.clear();
	test_bounding_box = screen.CarveBoxOffBottom( 0.0f, text_height );
	tier_2_font->AddVertsFor2DTextAlignedInBox( verts, text_height, test_text_tier_2.c_str(), test_bounding_box, Vec2::ALIGN_CENTER_LEFT );
	g_theRenderer->DrawVertexArray( verts );

	// T3 test
	BitmapFont* tier_3_font = g_theRenderer->CreateOrGetBitmapFromFile( "Ravie" );
	g_theRenderer->BindTextureView( eTextureSlot::TEXTURE_SLOT_ALBEDO, tier_3_font->GetTextureView() );

	verts.clear();
	test_bounding_box = screen.CarveBoxOffBottom(0.0f, text_height);
	tier_3_font->AddVertsFor2DTextAlignedInBox( verts, text_height, test_text_tier_3.c_str(), test_bounding_box, Vec2::ALIGN_CENTER_LEFT );

	// Draw
	g_theRenderer->DrawVertexArray( verts );

}
