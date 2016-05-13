#ifndef __ParticleModel_H__
#define __ParticleModel_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "GameManager.h"
#include "Force.h"
#include "BoundingBox.h"

USING_NS_CC;

class ParticleModel
{
private:
	//// FORCES
	std::vector<Force*>		_forces;
	cocos2d::Vec2			_netForce;
	float					_mass;

	//// MOVEMENT AND POSITION DATA
	cocos2d::Vec2			_acceleration;
	cocos2d::Vec2			_velocity;
	cocos2d::Vec2			_displacement;
	cocos2d::Vec2			_displacementFuture;

	//// COLLISIONS
	BoundingBox* _boundingBox;
public:
	ParticleModel();
	~ParticleModel();

	// UPDATES
	void Update(float delta, cocos2d::Sprite* sprite);
	void UpdateNetForce(float delta);
	void UpdateAcceleration(float delta);
	void Move(float delta);

	// MOVEMENT AND POSITION DATA
	void SetAcceleration(cocos2d::Vec2 accel)	{ _acceleration = accel; };
	cocos2d::Vec2 GetAcceleration()				{ return _acceleration; };

	void SetVelocity(cocos2d::Vec2 vel)	{ _velocity = vel; };
	cocos2d::Vec2 GetVelocity() { return _velocity; };

	void SetDisplacement(cocos2d::Vec2 dis)		{ _displacement = dis; };
	void SetDisplacementX(float disX)			{ _displacement.x = disX; };
	void SetDisplacementY(float disY)			{ _displacement.y = disY; };
	cocos2d::Vec2 GetDisplacement()				{ return _displacement; };
	float GetDisplacementX()					{ return _displacement.x; };
	float GetDisplacementY()					{ return _displacement.y; };

	void SetFutureDisplacement(cocos2d::Vec2 dis)	{ _displacementFuture = dis; };
	cocos2d::Vec2 GetFutureDisplacement()			{ return _displacementFuture; };

	cocos2d::Vec2 GetNetForce() { return _netForce; };

	// FORCES
	void AddForce(std::string name, cocos2d::Vec2 force);
	void AddForce(Force* force);
	std::vector<Force*> GetForces() { return _forces; };
	Force* GetForce(std::string name);

	void SetMass(float mass)	{ _mass = mass; };
	float GetMass()				{ return _mass; };

	// COLLISIONS
	BoundingBox* GetBoundingBox() { return _boundingBox; };
};

#endif