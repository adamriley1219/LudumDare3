#pragma once

#include <vector>

struct Vertex_PCU;
class Planet;

class TradeRoute
{
public:
	TradeRoute();
	~TradeRoute();

	void AddForRender( std::vector<Vertex_PCU>& verts ) const;

public:
	Planet* starting_route	= nullptr;
	Planet* ending_route		= nullptr;


};