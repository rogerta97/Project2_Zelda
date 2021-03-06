#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2Point.h"
#include "j1Module.h"
#include <string>
#include <vector>
#include <list>
#include "j1Render.h"

struct TreeNode; 
struct BushNode; 

// ----------------------------------------------------
struct Properties
{
	struct Property
	{
		std::string name;
		int value;
	};

	~Properties()
	{
		Property* item;
		item = list.front();

		for (std::list<Property*>::iterator prop = list.begin(); prop != list.end(); ++prop)
		{
			RELEASE(*prop);
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	std::list<Property*>	list;
};

// ----------------------------------------------------
struct MapLayer
{
	std::string	name;
	int			width;
	int			height;
	uint*		data;
	Properties	properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};

// ----------------------------------------------------
struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	std::string			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	std::list<TileSet*>	tilesets;
	std::list<MapLayer*>layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:
	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	// Return World spawn position for team #team minions
	iPoint GetMinionsSpawn(uint team) const;

	// Retun trunk's position
	iPoint GetTrunkPosition() const;

	// Fills the trees list
	bool GetTreesPosition(vector<TreeNode*>& trees_pos);

	// Fills the bushes list
	bool GetBushesPosition(vector<BushNode*>& bush_pos);

	//Return shop position in world cordinates
	iPoint GetShopPosition(uint team) const;

	// Return World spawn position for team players
	void GetPlayerSpawnPoints(uint team, vector<iPoint> &spawns);

	// Return all unsorted points for minions path
	std::vector<iPoint> GetMinionPath()const;

	//World spawn position for towers
	std::vector<iPoint> GetTowerSpawns(uint team) const;

	//World spawn position for jungle camps
	std::vector<iPoint> GetSnakesSpawns() const;

	std::vector<iPoint> GetSkeletonSpawns() const;

	std::vector<iPoint> GetMageSkeletonSpawns() const;

	std::vector<iPoint> GetGuardsSpawns() const;

	// Return all unsorted Zelda initial path points
	std::vector<iPoint> GetZeldaInitPath()const;

	// Return all unsorted Zelda path points
	std::vector<iPoint> GetZeldaPath()const;

	//Return Zelda position in world cordinates
	iPoint GetZeldaPosition() const;

	//Return base position in world cordinates
	iPoint GetBasePosition(uint team) const;

	// Return eyes coordinates
	std::vector<iPoint> GetEyesPositions()const;

	//Return base position in world cordinates
	iPoint GetWaterfallPosition() const;

private:
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

	void TilesToDraw_x(int& x_ini, int& x_end, MapLayer* layer) const;
	void TilesToDraw_y(int count, int x, int x_end, int& y_ini, int& y_end, MapLayer* layer) const;

	TileSet* GetTilesetFromTileId(int id) const;

public:
	MapData data;
	SDL_Rect fit_square = NULLRECT;

private:
	pugi::xml_document	map_file;
	std::string			folder;
	bool				map_loaded = false;
	int					draw_margin = 0;
	int					offset = 0;
};

#endif // __j1MAP_H__