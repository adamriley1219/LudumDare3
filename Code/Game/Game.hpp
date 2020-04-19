#pragma once
#include "Game/GameCommon.hpp"


#include "Game/SpaceCamera.hpp"
#include "Game/GameController.hpp"
#include "Game/UIWidget.hpp"

class Shader;
class InhabitablePlanet;
class Sun;
class SpriteSheet;

class Game
{
	friend App;
	friend GameController;
public:
	Game();
	~Game();

	void Startup();
	void Shutdown();

	bool HandleKeyPressed( unsigned char keyCode );
	bool HandleKeyReleased( unsigned char keyCode );

	void GameRender() const;
	void UpdateGame( float deltaSeconds );

private:
	void UpdateCamera( float deltaSeconds );
	void UpdateUI();
	void RenderBackground() const;
	void RenderPlanets() const;

public:
	bool m_isQuitting = false;

	Shader* m_shader = nullptr;
	SpriteSheet* m_planet_sheet = nullptr;

	mutable Camera m_DevColsoleCamera;

	GameController m_controller;

	UICanvas m_UICanvas;

	InhabitablePlanet*	m_root_planet	= nullptr;
	Sun*				m_root_sun		= nullptr;

	float m_slider = 50.f;

};