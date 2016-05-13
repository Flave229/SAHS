
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

bool Box::init()
{
	if (!Node::init()) {
		return false;
	}

	this->scheduleUpdate();

	return true;
}

void Box::Update(float delta)
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_touchTimer > 0.0f) {
			_touchTimer -= delta;
		}

		//_box->setScale(scaler2);
		//_weight = 2.0f;
		Scaling();
		//UpdateBoxScale();
	}

	//float TotalDiff = TouchManager().totalDiff;

	//ScalingObject().update(delta, TotalDiff);

	// Add box movement when pushed by player
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

//void Box::CheckPlatformCollisions(cocos2d::Sprite* collider)
//{
//	auto winSize = Director::getInstance()->getVisibleSize();
//
//	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
//	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
//	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//
//	float boxScaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//	float boxScaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
//
//	if (_orientationVertical) {
//		if (GetSprite()->getPositionX() - (boxScaledWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (boxScaledWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (boxScaledHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (boxScaledHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			Land(collider);
//		}
//		else {
//			_fallingVertical = true;
//		}
//	}
//	else if (_orientationHorizontal) {
//		if (GetSprite()->getPositionX() - (boxScaledHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (boxScaledHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (boxScaledWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (boxScaledWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			if (GetSprite()->getPositionY() < collider->getPositionY()) {
//				GetSprite()->setPositionY(collider->getPositionY() - (scaledHeight / 2) - (scaledPlayerWidth / 2));
//			}
//			else {
//				GetSprite()->setPositionY(collider->getPositionY() + (scaledHeight / 2) + (scaledPlayerWidth / 2));
//			}
//		}
//	}
//}
//
//void Box::CheckWallCollisions(cocos2d::Sprite* collider)
//{
//	auto winSize = Director::getInstance()->getVisibleSize();
//
//	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
//	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
//	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//
//	float boxScaledWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//	float boxScaledHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
//
//
//	if (_orientationVertical) {
//		if (GetSprite()->getPositionX() - (boxScaledWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (boxScaledWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (boxScaledHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (boxScaledHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			if (GetSprite()->getPositionX() < collider->getPositionX()) {
//				GetSprite()->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
//			}
//			else {
//				GetSprite()->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
//			}
//		}
//	}
//	else if (_orientationHorizontal) {
//		if (GetSprite()->getPositionX() - (boxScaledHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (boxScaledHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (boxScaledWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (boxScaledWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			Land(collider);
//		}
//		else {
//			_fallingHorizontal = true;
//		}
//	}
//}

void Box::Flip()
{
	if (GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 270.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 90.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 180.0f);
		GetSprite()->runAction(rotateTo);
	}
	else if (GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
	{
		auto rotateTo = RotateTo::create(0.5f, 0.0f);
		GetSprite()->runAction(rotateTo);
	}
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