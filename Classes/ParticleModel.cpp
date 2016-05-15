#include "ParticleModel.h"


ParticleModel::ParticleModel()
{
	_acceleration = Vec2(0.0f, 0.0f);
	_velocity = Vec2(0.0f, 0.0f);
	_displacement = Vec2(0.0f, 0.0f);
	_boundingBox = new BoundingBox();
}


ParticleModel::~ParticleModel()
{
}

void ParticleModel::Update(float delta, cocos2d::Sprite* sprite)
{
	// Update Bounding Box
	GetBoundingBox()->Update(delta, (sprite->getContentSize().width * sprite->getScaleX()), (sprite->getContentSize().height * sprite->getScaleY()),
		GetDisplacement().x, GetDisplacement().y,
		GetFutureDisplacement().x, GetFutureDisplacement().y);

	UpdateNetForce(delta);
	UpdateAcceleration(delta);
	Move(delta);

	SetFutureDisplacement(GetDisplacement() + (GetVelocity() * delta));
}

void ParticleModel::UpdateNetForce(float delta)
{
	_netForce = Vec2(0.0f, 0.0f);

	for (int i = 0; i < _forces.size(); i++)
	{
		// Force class does not have access to weight, so force must be applied per kilogram
		if (_forces.at(i)->GetName().find("Gravity") != -1)
		{
			_netForce.x += _forces.at(i)->GetForce().x * GetMass() * GetMass();
			_netForce.y += _forces.at(i)->GetForce().y * GetMass() * GetMass();
		}
		else
		{
			_netForce.x += _forces.at(i)->GetForce().x;
			_netForce.y += _forces.at(i)->GetForce().y;
		}
	}
}

void ParticleModel::UpdateAcceleration(float delta)
{
	// Using force equation: F = ma
	_acceleration = Vec2(GetNetForce().x / GetMass(),
		GetNetForce().y / GetMass());
}

void ParticleModel::Move(float delta)
{
	// Using SUVAT equation: v = u + at 
	Vec2 newVel = GetVelocity();
	newVel.x = GetVelocity().x + GetAcceleration().x * delta;
	newVel.y = GetVelocity().y + GetAcceleration().y * delta;

	// Using SUVAT equation: s = ut + 0.5(a * sq(t))
	Vec2 newPos = GetDisplacement();
	newPos.x = newPos.x + GetVelocity().x * delta + 0.5 * GetAcceleration().x * delta * delta;
	newPos.y = newPos.y + GetVelocity().y * delta + 0.5 * GetAcceleration().y * delta * delta;

	SetVelocity(newVel);
	SetDisplacement(newPos);
}

void ParticleModel::AddForce(std::string name, cocos2d::Vec2 force)
{
	_forces.push_back(new Force(name, force));
}

void ParticleModel::AddForce(Force* force)
{
	_forces.push_back(force);
}

void ParticleModel::RemoveForce(std::string name)
{
	for (int i = 0; i < _forces.size(); i++)
	{
		// Force class does not have access to weight, so force must be applied per kilogram
		if (_forces.at(i)->GetName().find(name) != -1)
		{
			_forces.erase(_forces.begin() + i);
		}
	}
}

Force* ParticleModel::GetForce(std::string name)
{
	for (int i = 0; i < _forces.size(); i++)
	{
		if (_forces.at(i)->GetName().find(name) != -1)
		{
			return _forces.at(i);
		}
	}

	return nullptr;
}
