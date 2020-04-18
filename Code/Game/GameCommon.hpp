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
constexpr float BOARDER_WIDTH = 200.f;
constexpr float BOARDER_HEIGHT = 100.f;

constexpr float BOARDER_HALF_WIDTH = BOARDER_WIDTH * 0.5f;
constexpr float BOARDER_HALF_HEIGHT = BOARDER_HEIGHT * 0.5f;

extern AABB2 UI_SCREEN;//( Vec2(), Vec2(BOARDER_WIDTH, BOARDER_HEIGHT) );


constexpr const char* BACKGROUND_TEXTURE_PATH = "Data/Images/Terrain_8x8.png";

