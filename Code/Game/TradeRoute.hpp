#pragma once

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
	void UpdateCycle();
	void AddForRender( std::vector<Vertex_PCU>& verts ) const;

	int GetCombinedTech() const;
	bool IsMeetingRequirements() const;

	void IncrementStartSup();
	void IncrementStartBio();
	void IncrementStartOxygen();
	void IncrementStartEnergy();

	void DecrementStartSup();
	void DecrementStartBio();
	void DecrementStartOxygen();
	void DecrementStartEnergy();

	void IncrementendSup();
	void IncrementendBio();
	void IncrementendOxygen();
	void IncrementendEnergy();
				  
	void DecrementendSup();
	void DecrementendBio();
	void DecrementendOxygen();
	void DecrementendEnergy();

public:
	Planet* starting_route			= nullptr;
	Planet* ending_route			= nullptr;
	bool	hovering				= false;

	TradeInfo startInfo;
	TradeInfo endInfo;
};