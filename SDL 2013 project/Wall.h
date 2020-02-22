#pragma once
#include "GameObject.h"
class Wall :
	public GameObject
{
public:
	int w, h;

	Wall();
	~Wall();

	//override draw, not update
	void draw();
};

