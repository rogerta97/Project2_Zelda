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
	short int CATEGORY_NONCOLLISIONABLE = 0x0004;

	// Mask
	short int MASK_PLAYER = CATEGORY_SCENERY;
	short int MASK_SCENERY = -1;
	short int MASK_NONCOLLISIONABLE = 0;
};

// PhysBody enum
enum class pbody_type
{
	p_t_null = 0,
	p_t_link,
	p_t_boomerang,
	p_t_world,
	p_t_npc,
	p_t_tower,
	p_t_tower_attack,
};

// Fixture enum
enum class fixture_type
{
	f_t_null = 0,
	f_t_attack,
	f_t_hit_box,
	f_t_minion_hit_sensor,
};

#endif // !_COLLISIONFILTERS__H__