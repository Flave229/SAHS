#include "Force.h"

Force::Force()
{
	_name = "Unknown";
	_force = cocos2d::Vec2(0.0f, 0.0f);
}

Force::Force(std::string name, cocos2d::Vec2 force)
{
	_name = name;
	_force = force;
}

Force::~Force()
{
}
