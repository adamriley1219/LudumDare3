#include "Game/Planet.hpp"
#include "Engine/Core/EngineCommon.hpp"

#include "Engine/Math/RNG.hpp"

#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/BitmapFont.hpp"
#include "Engine/Renderer/RenderContext.hpp"

#include "Game/Game.hpp"
#include "Engine/Renderer/SpriteSheet.hpp"

constexpr const int num_names = 120;
constexpr const char* names[num_names] = 
{ 
	"Ginroria  ",
	"Venavis   ",
	"Lignov	   ",
	"Icrade	   ",
	"Raihines  ",
	"Thuturn   ",
	"Drusenus  ",
	"Brabozuno ",
	"Gnomia 84K",
	"Thyria P1K", 
	"Enalea	   ",
	"Rangauthea",
	"Yoderon   ",
	"Ralolla   ",
	"Cegantu   ",
	"Huihines  ",
	"Thaatov   ",
	"Niganov   ",
	"Dreron Q0H",
	"Lone ON   ",
	"Guveuyama ",
	"Yugnuwei  ",
	"Busarvis  ",
	"Hebrilles ",
	"Cheruta   ",
	"Authea	   ",
	"Mootania  ",
	"Chixuter  ",
	"Niri K8D5 ",
	"Veron 801W",
	"Zinuter   ",
	"Osuahiri  ",
	"Xilrilles ",
	"Onvyke	   ",
	"Hinov	   ",
	"Thagantu  ",
	"Thizeter  ",
	"Muxupra   ",
	"Trone 83MC",
	"Struna EVP",
	"Zunualiv  ",
	"Gitratera ",
	"Minvonoe  ",
	"Yongorix  ",
	"Penope	   ",
	"Muetis	   ",
	"Lomebos   ",
	"Birenides ",
	"Crolla 64 ",
	"Siea 3819 ",
	"Kozanope  ",
	"Vulmaohiri",
	"Dudriuq	",
	"Abbeshan   ",
	"Muinus		",
	"Rigawa		",
	"Bedicarro  ",
	"Drieclite  ",
	"Cora 4L18  ",
	"Neon 9SMY  ",
	"Uselea		",
	"Olruevis   ",
	"Munadus	",
	"Hophore	",
	"Reobos	    ",
	"Ozuno	    ",
	"Dokigawa   ",
	"Golehines  ",
	"Geron A23  ",
	"Seshan X97 ",
	"Salualara  ",
	"Enzithea   ",
	"Anzade	    ",
	"Elvippe	",
	"Eotune	    ",
	"Zuitis	    ",
	"Bromitis   ",
	"Phaponides ",
	"Treron C6Q ",
	"Llion VTQD ",
	"Rivanus	",
	"Thinkeitune",
	"Thozolla   ",
	"Kepholla   ",
	"Xionerth   ",
	"Yilea		",
	"Thirorilia ",
	"Chuzenus   ",
	"Pholla ZQDP",
	"Zoth WR2   ",
	"Rivanus	",
	"Thinkeitune",
	"Thozolla   ",
	"Kepholla   ",
	"Xionerth   ",
	"Yilea		",
	"Thirorilia ",
	"Chuzenus   ",
	"Pholla ZQDP",
	"Zoth WR2   ",
	"Rivanus	",
	"Thinkeitune",
	"Thozolla   ",
	"Kepholla   ",
	"Xionerth   ",
	"Yilea		",
	"Thirorilia ",
	"Chuzenus   ",
	"Pholla ZQDP",
	"Zoth WR2   ",
	"Lesoruta   ",
	"Maphounia  ",
	"Ocrolla	",
	"Gacconoe   ",
	"Homia		",
	"Nayama		",
	"Gnayinus   ",
	"Cineria	",
	"Ceshan 34H ",
	"Thippe PK5 ",
};
// constexpr int num_vowels = 5;
// constexpr int num_consonants = 21;
// constexpr const char vowels[num_vowels] = { 'A', 'E', 'I', 'O', 'U' };
// constexpr const char consonants[num_consonants] 
// = { 
// 	'B', 'C', 'D', 'F', 'G', 'H', 'J',
// 	'K', 'L', 'M', 'N', 'P', 'Q', 'R',
// 	'S', 'T', 'V', 'W', 'X', 'Y', 'Z'
// };
// 
// 
// 
// char GetRandomConsonant()
// {
// 	int c_idx = g_theRNG->GetRandomIntInRange( 0, num_consonants - 1 );
// 	return consonants[c_idx];
// }
// 
// char GetRandomVowel()
// {
// 	int v_idx = g_theRNG->GetRandomIntInRange( 0, num_vowels - 1 );
// 	return vowels[v_idx];
// }

//--------------------------------------------------------------------------
/**
* GetRandomizedName
*/
std::string GetRandomizedName()
{
	std::string to_ret = "";
	int pattern_idx = g_theRNG->GetRandomIntInRange( 0, num_names - 1 );
	return names[pattern_idx];
}



//--------------------------------------------------------------------------
/**
* Planet
*/
Planet::Planet()
	: CelesticalObject()
{

}

//--------------------------------------------------------------------------
/**
* ~Planet
*/
Planet::~Planet()
{

}

//--------------------------------------------------------------------------
/**
* Update
*/
void Planet::Update( float deltaTime )
{
	CelesticalObject::Update( deltaTime );	
}


//--------------------------------------------------------------------------
/**
* Render
*/
void Planet::AddVertsForRender( std::vector<Vertex_PCU>& verts ) const
{
	CelesticalObject::AddVertsForRender( verts );
}

//--------------------------------------------------------------------------
/**
* AddVertsForInfoText
*/
void Planet::AddVertsForInfoText(std::vector<Vertex_PCU>& verts) const
{
	float zoom = g_theGame->m_controller.m_camera.m_zoom;
	float cell_height = 1.0f * zoom;

	AABB2 box = AABB2::ONE_BY_ONE;
	box.ScaleUniform(m_radius * 2.0f);
	box.m_bottomLeft.x *= zoom;
	box.m_topRight.x *= zoom;
	
	box.AddPosition(m_pos);

	box.AddPosition(0.0f, -box.GetHeight());
	box.m_bottomLeft.y -= 500.0f;
	

	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile(FONT);
	font->AddVertsFor2DTextAlignedInBox( verts, cell_height, m_name.c_str(), box.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_CENTERED );

	if (m_hover)
	{
		AABB2 left_side = box.CarveBoxOffLeft( 0.5f );
		AABB2& right_side = box;
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Population:" ).c_str()		, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", population ).c_str()	, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Energy:" ).c_str()			, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", energy ).c_str()		, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Technology:" ).c_str()		, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", technology ).c_str()	, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );

		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Supplies:" ).c_str()			, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", supplies ).c_str()		, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Biomatter:" ).c_str()		, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", biomatter ).c_str()		, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "Oxygen:" ).c_str()			, left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT );
		font->AddVertsFor2DTextAlignedInBox( verts, cell_height, Stringf( "%u", oxygen ).c_str()		, right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT );
	}
}

//--------------------------------------------------------------------------
/**
* UpdateFromSun
*/
void Planet::UpdateFromSun( Sun& sun )
{
	Vec2 sun_to_planet = m_pos - sun.m_pos;
	float dist_squr = sun_to_planet.GetLengthSquared();
	if( dist_squr > ( sun.influence_radius * sun.influence_radius + m_radius ) )
	{
		energy += sun.release_rate;
	}
}

//--------------------------------------------------------------------------
/**
* UpdateForCycle
*/
void Planet::UpdateForCycle()
{
	if( supplies > SUPPLIES_NEEDED_FOR_POPULATION_INC && population > 1 )
	{
		supplies -= SUPPLIES_NEEDED_FOR_POPULATION_INC;
		population += POPULATION_INC;
	}

	if( biomatter > BIOMAT_NEEDED_FOR_SUPPLIES_INC )
	{
		biomatter -= BIOMAT_NEEDED_FOR_SUPPLIES_INC;
		supplies += SUPPLIES_INC;
	}

	if( supplies > SUP_NEEDED_FOR_BIO && energy > ENERGY_NEEDED_FOR_BIO )
	{
		supplies	-= SUP_NEEDED_FOR_BIO;
		energy		-= ENERGY_NEEDED_FOR_BIO;
		biomatter	+= BIO_INC;
	}

	if( energy > ENERGY_NEEDED_FOR_TECH )
	{
		energy -= ENERGY_NEEDED_FOR_TECH;
		technology += TECH_INC;
	}

	oxygen += BIOMAT_INFLUENCE_ON_OXYGEN * biomatter;
	oxygen -= TECH_INFLUENCE_ON_OXYGEN * technology;

	if( oxygen < population * 0.5f )
	{
		population /= 2;
	}

	population -= 1;
	
	if( population < 0 )
		population = 0;
	if( energy < 0 )
		energy = 0;
	if( supplies < 0 )
		supplies = 0;
	if( biomatter < 0 )
		biomatter = 0;
	if( oxygen < 0 )
		oxygen = 0;
}

//--------------------------------------------------------------------------
/**
* GetTotalPopulation
*/
int Planet::GetPopulation() const
{
	int pop = population;
	return pop;
}

//--------------------------------------------------------------------------
/**
* CelesticalObject
*/
CelesticalObject::CelesticalObject()
{
	m_name = GetRandomizedName();
}

//--------------------------------------------------------------------------
/**
* Update
*/
void CelesticalObject::Update( float deltaTime )
{
	UNUSED(deltaTime);
	
	const Vec2& screen_offset = g_theGame->m_controller.m_camera.m_screen_offset;
	AABB2 screen = g_theGame->m_controller.m_camera.m_screen_boarder;
	screen.ScaleUniform( g_theGame->m_controller.m_camera.m_zoom );
	Vec2 position_in_player_space = m_pos - screen_offset;
	bool ctrl_override = g_theInputSystem->IsCtrlPressed() && IsPointInAABB2( position_in_player_space, screen );

	Vec2 mouse_pos = g_theGame->m_controller.GetGameMousePos();
	Vec2 disp_pos_to_mouse = mouse_pos - m_pos;
	float dist_sqrd = disp_pos_to_mouse.GetLengthSquared();
	m_hover = dist_sqrd < ( m_radius * m_radius ) || ctrl_override;
}

//--------------------------------------------------------------------------
/**
* Render
*/
void CelesticalObject::AddVertsForRender( std::vector<Vertex_PCU>& verts ) const
{
	AABB2 box = AABB2::ONE_BY_ONE;
	box.ScaleUniform( m_radius * 2.0f );
	box.AddPosition( m_pos );

	Vec2 mins;
	Vec2 maxs;

	SpriteSheet& sheet = *(g_theGame->m_planet_sheet);
	sheet.GetSpriteDefinition( sheet_idx ).GetUVs( mins, maxs );

	Rgba tint = m_tint;
	if( m_hover )
	{
		tint.r *= .7f;
		tint.g *= .7f;
		tint.b *= .7f;
		tint.a *= .7f;
	}
	AddVertsForAABB2D( verts, box, tint, mins, maxs );
}

//--------------------------------------------------------------------------
/**
* AddVertsForInfoText
*/
void Sun::AddVertsForInfoText( std::vector<Vertex_PCU>& verts ) const
{
	float zoom = g_theGame->m_controller.m_camera.m_zoom;
	float cell_height = 1.0f * zoom;

	AABB2 box = AABB2::ONE_BY_ONE;
	box.ScaleUniform(m_radius * 2.0f);
	box.m_bottomLeft.x *= zoom;
	box.m_topRight.x *= zoom;
	
	box.AddPosition(m_pos);

	box.AddPosition(0.0f, -box.GetHeight());
	box.m_bottomLeft.y -= 500.0f;


	BitmapFont* font = g_theRenderer->CreateOrGetBitmapFromFile(FONT);
	font->AddVertsFor2DTextAlignedInBox(verts, cell_height, m_name.c_str(), box.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_CENTERED);

	if( m_hover )
	{
		AABB2 left_side = box.CarveBoxOffLeft(0.5f);
		AABB2& right_side = box;
		font->AddVertsFor2DTextAlignedInBox(verts, cell_height, Stringf("Energy:").c_str(), left_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_LEFT);
		font->AddVertsFor2DTextAlignedInBox(verts, cell_height, Stringf("%u", energy).c_str(), right_side.CarveBoxOffTop(0.0f, cell_height), Vec2::ALIGN_TOP_RIGHT);
	}
}
