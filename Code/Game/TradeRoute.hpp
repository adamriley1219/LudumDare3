#pragma once
#include "Engine/Math/AABB2.hpp"

#include <vector>

struct Vertex_PCU;
class Planet;

struct TradeInfo
{
	int sending_supplies	= 0;
	int sending_bio			= 0;
	int sending_oxygen		= 0;
	int sending_energy		= 0;
};

class TradeRoute
{
public:
	TradeRoute();
	~TradeRoute();

	void Update();
	void UpdateForSelection();
	void AddForRender( std::vector<Vertex_PCU>& verts ) const;

	int GetCombinedTech() const;
	bool IsMeetingRequirements() const;

public:
	Planet* a_route					= nullptr;
	Planet* b_route					= nullptr;
	bool	hovering				= false;

	TradeInfo a_Info;
	TradeInfo b_Info;

};