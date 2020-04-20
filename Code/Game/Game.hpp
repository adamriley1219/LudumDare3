#pragma once
#include "Game/GameCommon.hpp"

#include "Engine/Core/Time/StopWatch.hpp"

#include "Game/SpaceCamera.hpp"
#include "Game/GameController.hpp"
#include "Game/UIWidget.hpp"

class Shader;
class Planet;
class Sun;
class SpriteSheet;
class TradeRoute;
struct TradeInfo;

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
	void RenterTradeRoutes() const;
	void RenderSelectedRoute() const;

	void UpdateCycle();
	void UpdateCycleWithSun( Sun& sun );
	void UpdateCycleWithTradeRoute( TradeRoute& trade_route );
	void AttempTrade( TradeInfo& trade_info, Planet& from, Planet& to );
	void UpdateSelectedRoute();

	void CreateTradeRoutesFor( Planet& planet );
	Planet* GetClosestPlanetExcludingDuplicates( Planet& ref_planet );
	Planet* GetClosestPlanetNotConnected( Planet& ref_planet );
	void CreateTradeConnection( Planet* a, Planet* b );

public:
	bool m_isQuitting					= false;

	Shader*	m_shader					= nullptr;
	SpriteSheet* m_planet_sheet			= nullptr;

	mutable Camera m_DevColsoleCamera;

	GameController m_controller;
	TradeRoute*	m_selected_route		= nullptr;

	UICanvas m_UICanvas;

	std::vector<Planet>					m_planets;
	std::vector<TradeRoute>				m_trade_routes;
	std::vector<Sun>					m_suns;

	StopWatch* m_cycle_timer			= nullptr;
	UILabel* m_cycles_label				= nullptr;
	int m_num_cycles					= 0;
	
	UILabel* m_totpop_label				= nullptr;
	int m_total_population				= 0;

};