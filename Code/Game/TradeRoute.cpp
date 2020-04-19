#include "Game/TradeRoute.hpp"
#include "Game/Planet.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"

#include "Engine/Math/MathUtils.hpp"

#include "Engine/Renderer/RenderContext.hpp"


//--------------------------------------------------------------------------
/**
* TradeRoute
*/
TradeRoute::TradeRoute()
{

}

//--------------------------------------------------------------------------
/**
* ~TradeRoute
*/
TradeRoute::~TradeRoute()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void TradeRoute::Update()
{
	Vec2 mouse_pos = g_theGame->m_controller.GetGameMousePos();

	bool intersecting = DoesDiscOverlapLineSegment2D( mouse_pos, ROUTE_THICKNESS, starting_route->m_pos, ending_route->m_pos );
	hovering = intersecting; 

	if( intersecting && g_theInputSystem->KeyWasPressed( MOUSE_L ) )
	{
		g_theGame->m_selected_route = this;
	}
}

//--------------------------------------------------------------------------
/**
* UpdateCycle
*/
void TradeRoute::UpdateCycle()
{

}

//--------------------------------------------------------------------------
/**
* Render
*/
void TradeRoute::AddForRender( std::vector<Vertex_PCU>& verts ) const
{
	if( !starting_route || !ending_route )
	{
		return;
	}

	bool isNotMeetingTradeRequirments = IsMeetingRequirements();
	
	Rgba color = Rgba::WHITE;

	if( hovering )
	{
		color = Rgba::BLUE;
	}

	if( g_theGame->m_selected_route == this )
	{
		color = Rgba::YELLOW;
	}

	if( isNotMeetingTradeRequirments )
	{
		color.r *= 0.8f;
		color.g *= 0.8f;
		color.b *= 0.8f;
	}

	AddVertsForLine2D( verts, starting_route->m_pos, ending_route->m_pos, ROUTE_THICKNESS, color );
}

//--------------------------------------------------------------------------
/**
* GetCombinedTech
*/
int TradeRoute::GetCombinedTech() const
{
	int ret = 0;

	ret += starting_route->technology;
	ret += ending_route->technology;

	return ret;
}

//--------------------------------------------------------------------------
/**
* IsMeetingRequirements
*/
bool TradeRoute::IsMeetingRequirements() const
{
	int total_req_start =
		startInfo.sending_supplies
	+	startInfo.sending_bio 
	+	startInfo.sending_oxygen
	+	startInfo.sending_energy;

	if( total_req_start * POPULATION_SEND_ON_TRADE < starting_route->population )
	{
		return false;
	}

	int total_req_end =
			endInfo.sending_supplies
		+	endInfo.sending_bio
		+	endInfo.sending_oxygen
		+	endInfo.sending_energy;

	if( total_req_end * POPULATION_SEND_ON_TRADE < ending_route->population )
	{
		return false;
	}

	return GetCombinedTech() > TECH_NEEDED_TO_TRADE;
}
