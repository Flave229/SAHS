#ifndef __GameObject_H__
#define __GameObject_H__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "GameManager.h"
#include "ParticleModel.h"

USING_NS_CC;

class GameObject : public Node
{
private:
	ParticleModel* _particle;

	cocos2d::Sprite* _sprite;
public:
	GameObject();
	GameObject(ParticleModel* particle);
	~GameObject();

	void Update(float delta);

	void SetParticleModel(ParticleModel* particle) { _particle = particle; };
	ParticleModel* GetParticleModel() { return _particle; };

	void SetSprite(cocos2d::Sprite* sprite);
	cocos2d::Sprite* GetSprite() { return _sprite; };

	void FlipSprite();

	void CheckCollision(GameObject* collider, float cR, bool moveThis = true, bool moveCollider = true);
	void FixCollision(GameObject* collider);
	void DealWithCollision(GameObject* collider, float cR, bool moveThis = true, bool moveCollider = true);
};

#endif