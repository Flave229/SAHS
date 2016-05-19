#include "GameObject.h"


GameObject::GameObject()
{
	_sprite = nullptr;
	_particle = new ParticleModel();
}

GameObject::GameObject(ParticleModel* particle)
{
	_sprite = nullptr;
	_particle = particle;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float delta)
{
	_particle->Update(delta, _sprite);

	_sprite->setPosition(_particle->GetDisplacement());
}

void GameObject::SetSprite(cocos2d::Sprite* sprite)
{
	_sprite = sprite;

	_particle->SetDisplacement(_sprite->getPosition());
	_particle->SetFutureDisplacement(_sprite->getPosition());

	_particle->GetBoundingBox()->Update(0.0f, (sprite->getContentSize().width * sprite->getScaleX()), (sprite->getContentSize().height * sprite->getScaleY()),
		_particle->GetDisplacement().x, _particle->GetDisplacement().y,
		_particle->GetFutureDisplacement().x, _particle->GetFutureDisplacement().y);

	addChild(_sprite);
}

void GameObject::FlipSprite()
{
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 0.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 180.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 90.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 270.0f);
		GetSprite()->runAction(rotateTo);
	}
}

void GameObject::CheckCollision(GameObject* collider, float cR, bool moveThis, bool moveCollider)
{
	// Need to get the midpoint of the current and previous position, as boundingbox is calculated from this
	cocos2d::Vec2 midpointThis = Vec2((GetParticleModel()->GetDisplacement().x + GetParticleModel()->GetFutureDisplacement().x) / 2,
		(GetParticleModel()->GetDisplacement().y + GetParticleModel()->GetFutureDisplacement().y) / 2);
	cocos2d::Vec2 midpointCollider = Vec2((collider->GetParticleModel()->GetDisplacement().x + collider->GetParticleModel()->GetFutureDisplacement().x) / 2,
		(collider->GetParticleModel()->GetDisplacement().y + collider->GetParticleModel()->GetFutureDisplacement().y) / 2);

	// Check Collisions
	float debugThisMinX = midpointThis.x + GetParticleModel()->GetBoundingBox()->GetMinX();
	float debugColliderMaxX = midpointCollider.x + collider->GetParticleModel()->GetBoundingBox()->GetMaxX();

	float debugThisMaxX = midpointThis.x + GetParticleModel()->GetBoundingBox()->GetMaxX();
	float debugColliderMinX = midpointCollider.x + collider->GetParticleModel()->GetBoundingBox()->GetMinX();

	float debugThisMinY = midpointThis.y + GetParticleModel()->GetBoundingBox()->GetMinY();
	float debugColliderMaxY = midpointCollider.y + collider->GetParticleModel()->GetBoundingBox()->GetMaxY();

	float debugThisMaxY = midpointThis.y + GetParticleModel()->GetBoundingBox()->GetMaxY();
	float debugColliderMinY = midpointCollider.y + collider->GetParticleModel()->GetBoundingBox()->GetMinY();


	if (midpointThis.x + GetParticleModel()->GetBoundingBox()->GetMinX() < midpointCollider.x + collider->GetParticleModel()->GetBoundingBox()->GetMaxX()
		&& midpointThis.x + GetParticleModel()->GetBoundingBox()->GetMaxX() > midpointCollider.x + collider->GetParticleModel()->GetBoundingBox()->GetMinX()
		&& midpointThis.y + GetParticleModel()->GetBoundingBox()->GetMinY() < midpointCollider.y + collider->GetParticleModel()->GetBoundingBox()->GetMaxY()
		&& midpointThis.y + GetParticleModel()->GetBoundingBox()->GetMaxX() > midpointCollider.y + collider->GetParticleModel()->GetBoundingBox()->GetMinY())
	{
		FixCollision(collider);
		DealWithCollision(collider, cR, moveThis, moveCollider);
	}
}

void GameObject::FixCollision(GameObject* collider)
{
	// Check which way velocity is going
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
	{
		// Object is traveling right
		GetParticleModel()->SetDisplacementX(collider->GetParticleModel()->GetDisplacementX() + collider->GetParticleModel()->GetBoundingBox()->GetMinX() - GetParticleModel()->GetBoundingBox()->GetMaxX());
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
	{
		// Object is traveling left
		GetParticleModel()->SetDisplacementX(collider->GetParticleModel()->GetDisplacementX() + collider->GetParticleModel()->GetBoundingBox()->GetMaxX() - GetParticleModel()->GetBoundingBox()->GetMinX());
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
	{
		// Object is traveling up
		GetParticleModel()->SetDisplacementY(collider->GetParticleModel()->GetDisplacementY() + collider->GetParticleModel()->GetBoundingBox()->GetMinY() + GetParticleModel()->GetBoundingBox()->GetMinY());
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
	{
		// Object is traveling down
		GetParticleModel()->SetDisplacementY(collider->GetParticleModel()->GetDisplacementY() + collider->GetParticleModel()->GetBoundingBox()->GetMaxY() - GetParticleModel()->GetBoundingBox()->GetMinY());
	}
}

void GameObject::DealWithCollision(GameObject* collider, float cR, bool moveThis, bool moveCollider)
{
	// Using elastic collision formula: (m1 * u1) + (m2 * u2) = (m1 * v1) + (m2 * v2)
	cocos2d::Vec2 newVelocityThis = GetParticleModel()->GetVelocity();
	cocos2d::Vec2 newVelocityCollider = collider->GetParticleModel()->GetVelocity();

	if (moveThis)
	{
		newVelocityThis.x = ((newVelocityThis.x * (GetParticleModel()->GetMass() - collider->GetParticleModel()->GetMass()) + (2 * (collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().x))) * cR) / (GetParticleModel()->GetMass() + GetParticleModel()->GetMass());
		newVelocityThis.y = ((newVelocityThis.y * (GetParticleModel()->GetMass() - collider->GetParticleModel()->GetMass()) + (2 * (collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().y))) * cR) / (GetParticleModel()->GetMass() + GetParticleModel()->GetMass());
		//newVelocityThis.x = (((GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().x) + (collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().x) + (collider->GetParticleModel()->GetMass() * cR * (collider->GetParticleModel()->GetVelocity().x - GetParticleModel()->GetVelocity().x))) / (GetParticleModel()->GetMass() + collider->GetParticleModel()->GetMass()));
		//newVelocityThis.y = (((GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().y) + (collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().y) + (collider->GetParticleModel()->GetMass() * cR * (collider->GetParticleModel()->GetVelocity().y - GetParticleModel()->GetVelocity().y))) / (GetParticleModel()->GetMass() + collider->GetParticleModel()->GetMass()));
	}

	if (moveCollider)
	{
		newVelocityCollider.x = (newVelocityCollider.x * (collider->GetParticleModel()->GetMass() - GetParticleModel()->GetMass()) + (2 * (GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().x))) / (GetParticleModel()->GetMass() + GetParticleModel()->GetMass());
		newVelocityCollider.y = (newVelocityCollider.y * (collider->GetParticleModel()->GetMass() - GetParticleModel()->GetMass()) + (2 * (GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().y))) / (GetParticleModel()->GetMass() + GetParticleModel()->GetMass());
		//newVelocityCollider.x = (((collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().x) + (GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().x) + (GetParticleModel()->GetMass() * cR * (GetParticleModel()->GetVelocity().x - collider->GetParticleModel()->GetVelocity().x))) / (GetParticleModel()->GetMass() + collider->GetParticleModel()->GetMass()));
		//newVelocityCollider.y = (((collider->GetParticleModel()->GetMass() * collider->GetParticleModel()->GetVelocity().y) + (GetParticleModel()->GetMass() * GetParticleModel()->GetVelocity().y) + (GetParticleModel()->GetMass() * cR * (GetParticleModel()->GetVelocity().y - collider->GetParticleModel()->GetVelocity().y))) / (GetParticleModel()->GetMass() + collider->GetParticleModel()->GetMass()));
	}

	GetParticleModel()->SetVelocity(newVelocityThis);
	collider->GetParticleModel()->SetVelocity(newVelocityCollider);
}