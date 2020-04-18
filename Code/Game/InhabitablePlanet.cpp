#include "Game/InhabitablePlanet.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Renderer/SpriteSheet.hpp"

#include "Game/Game.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

// std::string GetRandomizedName();
// 
// //--------------------------------------------------------------------------
// /**
// * GetRandomizedName
// */
// std::string GetRandomizedName()
// {
// 
// }



//--------------------------------------------------------------------------
/**
* Planet
*/
InhabitablePlanet::InhabitablePlanet()
{

}

//--------------------------------------------------------------------------
/**
* ~Planet
*/
InhabitablePlanet::~InhabitablePlanet()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void InhabitablePlanet::Update(float deltaTime)
{

}


//--------------------------------------------------------------------------
/**
* Render
*/
void InhabitablePlanet::AddVertsForRender( std::vector<Vertex_PCU>& verts ) const
{
	CelesticalObject::AddVertsForRender( verts );
	for( int i = 0; i < NUM_ALLOWED_CHILDREN; ++i )
	{
		if( m_children[i] )
		{
			m_children[i]->AddVertsForRender( verts );
		}
	}
}

//--------------------------------------------------------------------------
/**
* GetTotalPopulation
*/
int InhabitablePlanet::GetTotalPopulation() const
{
	int pop = polulation;
	for( int i = 0; i < NUM_ALLOWED_CHILDREN; ++i )
	{
		if( m_children[i] )
		{
			pop += m_children[i]->GetTotalPopulation();
		}
	}
	return pop;
}

//--------------------------------------------------------------------------
/**
* Update
*/
void CelesticalObject::Update( float deltaTime )
{
	Vec2 mouse_pos = g_theGame->m_controller.GetGameMousePos();

	Vec2 disp_pos_to_mouse = mouse_pos - m_pos;
	float dist_sqrd = disp_pos_to_mouse.GetLengthSquared();
	m_hover = dist_sqrd < m_radius * m_radius;
}

//--------------------------------------------------------------------------
/**
* Render
*/
void CelesticalObject::AddVertsForRender( std::vector<Vertex_PCU>& verts ) const
{
	AABB2 box = AABB2::ONE_BY_ONE;
	box.AddPosition( m_pos );
	box.ScaleUniform( m_radius );

	Vec2 mins;
	Vec2 maxs;

	SpriteSheet& sheet = *(g_theGame->m_planet_sheet);
	sheet.GetSpriteDefinition( sheet_idx ).GetUVs( mins, maxs );

	Rgba tint = m_tint;
	if( m_hover )
	{
		tint.a *= .7f;
	}
	AddVertsForAABB2D( verts, box, m_tint, mins, maxs );
}
