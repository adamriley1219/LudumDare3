#pragma once
#include "Engine/Core/EngineCommon.hpp"
#include "Engine/Core/Vertex/Vertex_PCU.hpp"
#include "Game/GameUtils.hpp"

#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vec2.hpp"

class RenderContext; // All may know about the renderer and the globe. You must include to use it.
extern RenderContext* g_theRenderer;

class App;
extern App* g_theApp;				// Created and owned by Main_Windows.cpp

class InputSystem;
extern InputSystem* g_theInputSystem;

class PhysicsSystem;
extern PhysicsSystem* g_thePhysicsSystem;

class AudioSystem;
extern AudioSystem* g_theAudioSystem;

class RNG;
extern RNG* g_theRNG;

class Game;
extern Game* g_theGame;

class WindowContext;
extern WindowContext* g_theWindowContext;

class ImGUISystem;
extern ImGUISystem* g_theImGUISystem;

extern bool g_isInDebug;

//--------------------------------------------------------------------------
// Constant global variables.
//--------------------------------------------------------------------------
constexpr float GAME_HALF_WIDTH		= 500.0f;
constexpr float GAME_HALF_HEIGHT	= 500.0f;

constexpr float BOARDER_WIDTH = 200.f;
constexpr float BOARDER_HEIGHT = 100.f;

constexpr float BOARDER_HALF_WIDTH = BOARDER_WIDTH * 0.5f;
constexpr float BOARDER_HALF_HEIGHT = BOARDER_HEIGHT * 0.5f;

extern AABB2 UI_SCREEN;//( Vec2(), Vec2(BOARDER_WIDTH, BOARDER_HEIGHT) );

constexpr const char* BACKGROUND_TEXTURE_PATH	= "Data/Images/stars.jpg";
constexpr const char* PLANET_TEXTURE_PATH		= "Data/Images/Explosion_5x5.png";
constexpr const char* ROUTE_TEXTURE_PATH		= nullptr;

constexpr const char* FONT						= "SquirrelFixedFont";

constexpr const float ROUTE_THICKNESS				= 3.0f;

//--------------------------------------------------------------------------
constexpr int NUM_TRADEROUTES_PER_PLANET					= 2;
constexpr int NUM_PLANETS							= 50;
constexpr int NUM_SUNS								= 10;


constexpr int SUPPLIES_NEEDED_FOR_POPULATION_INC	= 50;
constexpr int POPULATION_INC						= 15;
constexpr int BIOMAT_NEEDED_FOR_SUPPLIES_INC		= 10;
constexpr int SUPPLIES_INC							= 5;
constexpr int ENERGY_NEEDED_FOR_BIO					= 20;
constexpr int SUP_NEEDED_FOR_BIO					= 2;
constexpr int BIO_INC								= 8;
constexpr int BIOMAT_INFLUENCE_ON_OXYGEN			= 2;
constexpr int TECH_INFLUENCE_ON_OXYGEN				= 2;
constexpr int ENERGY_NEEDED_FOR_TECH				= 50;
constexpr int TECH_INC								= 45;


constexpr int POPULATION_SEND_ON_TRADE				= 5;
constexpr int AMOUNT_SENT_ON_TRADE					= 8;