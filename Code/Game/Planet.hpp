#pragma once
#include "Game/GameCommon.hpp"
#include "Game/UIWidget.hpp"

#include <vector>

class SpriteSheet;
class Sun;

class CelesticalObject
{
public:
	CelesticalObject();
	virtual ~CelesticalObject() {};

	virtual void Update( float deltaTime );
	virtual void AddVertsForRender( std::vector<Vertex_PCU>& verts ) const;
	virtual void AddVertsForInfoText( std::vector<Vertex_PCU>& verts ) const = 0;

public:
	Vec2 m_pos					= Vec2::ZERO;
	float m_radius				= 10.0f;
	Rgba m_tint					= Rgba::WHITE;
	bool m_hover				= false;
	std::string	m_name			= "UNNAMED";

public:
	int sheet_idx				= 0;
};

class Planet : public CelesticalObject
{
public:
	Planet();
	virtual ~Planet();

	virtual void Update( float deltaTime );
	virtual void AddVertsForRender( std::vector<Vertex_PCU>& verts ) const;
	virtual void AddVertsForInfoText( std::vector<Vertex_PCU>& verts ) const;

	void UpdateFromSun( Sun& sun );
	void UpdateForCycle();

public:
	int GetPopulation() const; 
	
public:
	int energy			= 0;
	int technology		= 0;

	int population		= 50;

	int supplies		= 50'000;
	int biomatter		= 20'000;
	int oxygen			= 100'000'000;

};

class Sun : public CelesticalObject
{
public:
	Sun() : CelesticalObject() {};
	virtual ~Sun() {};

	virtual void AddVertsForInfoText( std::vector<Vertex_PCU>& verts ) const;

public:
	int		energy				= 50'000'000;
	float	influence_radius	= 10.0f;
	int		release_rate		= 100;

};