
#include "Box.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Box::Box(float startingWeight) : GameObject(new ParticleModel())
{
	scaler1 = 0;
	scaler2 = 0;
	scaler3 = 0;
	scaler = 0;
	unselect = 0;
	isSelected = false;
	_touchTimer = 0.0f;
	SetStartingScale();
}

Box::Box(ParticleModel* particle, float startingWeight) : GameObject(particle)
{
	scaler1 = 0;
	scaler2 = 0;
	scaler3 = 0;
	scaler = 0;
	unselect = 0;
	isSelected = false;
	_touchTimer = 0.0f;
	SetStartingScale();
}

Box::~Box()
{
}

void Box::Update(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_touchTimer > 0.0f) {
			_touchTimer -= delta;
		}

		Scaling();
		//UpdateBoxScale();
	}

	GameObject::Update(delta);
}

void Box::SetStartingScale()
{
	scaler1 = 1.0f;
	scaler2 = 1.5f;
	scaler3 = 2.0f;

	scaler = scaler1;
}

void Box::Scaling()
{
	if (isSelected)
	{
		if (totalDiff > 10 && totalDiff < 200)
		{
			//_box->setScale(scaler1);
			scaler = scaler1;
			GetParticleModel()->SetMass(1.0f);
		}
		else if (totalDiff > 200 && totalDiff < 500)
		{
			//_box->setScale(scaler2);
			scaler = scaler2;
			GetParticleModel()->SetMass(2.0f);
		}
		else if (totalDiff > 500)
		{
			//_box->setScale(scaler3);
			scaler = scaler3;
			GetParticleModel()->SetMass(3.0f);
		}
		GetSprite()->setScale(scaler);
		Selected();
	}
	else
	{
		//_box->setScale(scaler);
		Deselected();
	}
}

void Box::SetTotalDiff(float totalDiffNew)
{
	totalDiff = totalDiffNew;
}

void Box::Selected()
{
	GetSprite()->setOpacity(75);
}

void Box::Deselected()
{
	GetSprite()->setOpacity(255);
}

void Box::Collision(cocos2d::Touch* touch)
{
	//cocos2d::Vec2 p = touch->getLocation();
	//cocos2d::Rect rect = GetParticleModel()->GetBoundingBox();

	//if (rect.containsPoint(p))
	//{
	//	if (_touchTimer <= 0.0f) {
	//		_touchTimer = _defaultTouchTimer;
	//		isSelected = !isSelected;
	//		//isSelected = true;
	//	}

	//}
}

void Box::SetSprite(Sprite* newSprite) {
	// Call parent method to set sprite
	GameObject::SetSprite(newSprite);

	// Grab user data for the box sizes
	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(GetSprite()->getComponent("ComExtensionData"));
	std::string userdata = data->getCustomProperty();

	if (userdata == "Large" || userdata == "large") 
	{
		scaler = scaler3;
	}
	else if (userdata == "Medium" || userdata == "medium") 
	{
		scaler = scaler2;
	}
	else if (userdata == "Small" || userdata == "small") 
	{
		scaler = scaler1;
	}

	GetSprite()->setScale(scaler);
}