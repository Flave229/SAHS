#ifndef __Force_H__
#define __Force_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "GameManager.h"

USING_NS_CC;

class Force
{
private:
	std::string _name;
	cocos2d::Vec2 _force;
public:
	Force();
	Force(std::string name, cocos2d::Vec2 force);
	~Force();

	void SetName(std::string name) { _name = name; };
	std::string GetName() { return _name; };

	void SetForce(cocos2d::Vec2 force)	{ _force = force; };
	void SetForce(float x, float y)		{ _force = Vec2(x, y); };
	cocos2d::Vec2 GetForce()			{ return _force; };
};

#endif