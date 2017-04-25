#ifndef _MAPPING_H_
#define _MAPPING_H_

enum map_keys
{
	m_k_ability1,
	m_k_ability2,
	m_k_ability3,
	m_k_ability4,
	m_k_confirm,
	m_k_back,
	m_k_shop,
	m_k_minimap,
	m_k_null,
};

struct key_mapping
{
	int key_id = -1;
	bool is_button = true;
};

class Mapping
{
public:
	Mapping();
	~Mapping();

	void MapKey(map_keys function, int key_id);
	void GetKey(map_keys function);

private:

public:

private:
	key_mapping* mapping = nullptr;


};


#endif