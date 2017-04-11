#ifndef _COLLISIONFILTERS__H__
#define _COLLISIONFILTERS__H__

// -----------------------------------------
// -----------------------------------------

		  // COLLISION CONTROL //

// -----------------------------------------
// -----------------------------------------

struct collision_filters
{
	collision_filters() {};

	// Category (1, 2, 4, 8, 16, 32, 64 ... 2^n)
	short int CATEGORY_PLAYER = 0x0001;
	short int CATEGORY_SCENERY = 0x0002;
	short int CATEGORY_ABILITIES = 0x0004;
	short int CATEGORY_NONCOLLISIONABLE = 0x0008;
	short int CATEGORY_PAYLOAD_AREA = 0x0016;
	short int CATEGORY_LINK_ULTIMATE = 0x0032;

	// Mask
	short int MASK_PLAYER = CATEGORY_SCENERY | CATEGORY_ABILITIES | CATEGORY_PLAYER | CATEGORY_PAYLOAD_AREA;
	short int MASK_ABILITIES = CATEGORY_PLAYER;
	short int MASK_SCENERY = -1;
	short int MASK_NONCOLLISIONABLE = 0;
	short int MASK_PAYLOAD_AREA = CATEGORY_SCENERY | CATEGORY_PLAYER;
	short int MASK_LINK_ULTIMATE = CATEGORY_PLAYER;
};

// PhysBody enum
enum class pbody_type
{
	p_t_null = 0,
	p_t_player,
	p_t_boomerang,
	p_t_world,
	p_t_npc,
	p_t_snake,
	p_t_tower,
	p_t_tower_attack,
	p_t_snake_poison,
	p_t_trunk,
	p_t_tree,
	p_t_bush
};

// Fixture enum
enum class fixture_type
{
	f_t_null = 0,
	f_t_attack,
	f_t_hit_box,
	f_t_collision_box,
	f_t_minion_hit_sensor,
	f_t_zelda_area,
	f_t_aesthetics
};

#endif // !_COLLISIONFILTERS__H__