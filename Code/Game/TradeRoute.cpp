#include "Game/TradeRoute.hpp"
#include "Game/Planet.hpp"
#include "Game/GameCommon.hpp"

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
* Render
*/
void TradeRoute::AddForRender( std::vector<Vertex_PCU>& verts ) const
{
	if( !starting_route || !ending_route )
	{
		return;
	}

	Vec2 start_to_end = ending_route->m_pos - starting_route->m_pos; 
	AddVertsForLine2D( verts, starting_route->m_pos, ending_route->m_pos, ROUTE_THICKNESS, Rgba::WHITE );
}
