#pragma once
#include "Game/GameCommon.hpp"

#include "Game/SpaceCamera.hpp"
#include "Game/GameController.hpp"

class Shader;

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


private:
	bool m_isQuitting = false;

	Shader* m_shader;

	mutable SpaceCamera m_curentCamera;
	mutable Camera m_DevColsoleCamera;

	GameController m_controller;

	float m_slider = 50.f;

};