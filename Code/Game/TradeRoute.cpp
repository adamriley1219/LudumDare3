#include "Game/TradeRoute.hpp"
#include "Game/Planet.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Game.hpp"

#include "Engine/Math/MathUtils.hpp"

#include "Engine/Renderer/RenderContext.hpp"

#include "Engine/ImGUI/ImGUISystem.hpp"

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
	if (!ImGui::IsAnyWindowHovered())
	{
		Vec2 mouse_pos = g_theGame->m_controller.GetGameMousePos();

		bool intersecting = DoesDiscOverlapLineSegment2D(mouse_pos, ROUTE_THICKNESS, a_route->m_pos, b_route->m_pos);
		hovering = intersecting;

		if (intersecting && g_theInputSystem->KeyWasPressed(MOUSE_L))
		{
			g_theGame->m_selected_route = this;
		}
	}
}


//--------------------------------------------------------------------------
/**
* UpdateForSelection
*/
void TradeRoute::UpdateForSelection()
{
	ImGui::Begin("Trade Route");

	ImGui::Text(Stringf("Amount %s(a) is Sending:", a_route->m_name.c_str()).c_str());

	ImGui::SliderInt( "a supplies", &a_Info.sending_supplies, 0, MAX_TRADE_AMOUNT );
	ImGui::SliderInt( "a energy", &a_Info.sending_energy, 0, MAX_TRADE_AMOUNT );
	ImGui::SliderInt( "a biomatter", &a_Info.sending_bio, 0, MAX_TRADE_AMOUNT );
	ImGui::SliderInt( "a oxygen", &a_Info.sending_oxygen, 0, MAX_TRADE_AMOUNT );
	
	ImGui::Text(Stringf("Amount %s(b) is Sending:", b_route->m_name.c_str()).c_str());
	ImGui::SliderInt("b supplies", &b_Info.sending_supplies, 0, MAX_TRADE_AMOUNT);
	ImGui::SliderInt("b energy", &b_Info.sending_energy, 0, MAX_TRADE_AMOUNT);
	ImGui::SliderInt("b biomatter", &b_Info.sending_bio, 0, MAX_TRADE_AMOUNT);
	ImGui::SliderInt("b oxygen", &b_Info.sending_oxygen, 0, MAX_TRADE_AMOUNT);

	ImGui::End();
}

//--------------------------------------------------------------------------
/**
* Render
*/
void TradeRoute::AddForRender( std::vector<Vertex_PCU>& verts ) const
{
	if( !a_route || !b_route )
	{
		return;
	}

	bool isNotMeetingTradeRequirments = !IsMeetingRequirements();
	
	Rgba color = Rgba::WHITE;

	if( hovering )
	{
		color = Rgba::BLUE;
	}

	if( isNotMeetingTradeRequirments )
	{
		color = Rgba::RED;
	}
	else if( a_route->population < BASE_POP_NEEDED_TO_TRADE || b_route->population < BASE_POP_NEEDED_TO_TRADE )
	{
		color = Rgba( 1.0f, 0.8f, 0.5f );
	}

	if( g_theGame->m_selected_route == this )
	{
		color = Rgba::YELLOW;
	}

	AddVertsForLine2D( verts, a_route->m_pos, b_route->m_pos, ROUTE_THICKNESS, color );
}


//--------------------------------------------------------------------------
/**
* GetCombinedTech
*/
int TradeRoute::GetCombinedTech() const
{
	int ret = 0;

	ret += a_route->technology;
	ret += b_route->technology;

	return ret;
}

//--------------------------------------------------------------------------
/**
* IsMeetingRequirements
*/
bool TradeRoute::IsMeetingRequirements() const
{
	int total_req_start =
		a_Info.sending_supplies
	+	a_Info.sending_bio 
	+	a_Info.sending_oxygen
	+	a_Info.sending_energy;

	if( total_req_start * POPULATION_SEND_ON_TRADE > a_route->population )
	{
		return false;
	}

	int total_req_end =
			b_Info.sending_supplies
		+	b_Info.sending_bio
		+	b_Info.sending_oxygen
		+	b_Info.sending_energy;

	if( total_req_end * POPULATION_SEND_ON_TRADE > b_route->population )
	{
		return false;
	}

	return GetCombinedTech() > TECH_NEEDED_TO_TRADE;
}
