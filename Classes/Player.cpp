#include "Player.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Player::Player() : GameObject(new ParticleModel())
{
	_speed = 250.0f;
	_frame = 1;
	_running = false;
	_spriteTimer = _spriteTimerDefault;
}

Player::Player(ParticleModel* particle) : GameObject(particle)
{
	_speed = 250.0f;
	_frame = 1;
	_running = false;
	_spriteTimer = _spriteTimerDefault;
}

Player::~Player()
{
}

bool Player::init()
{
	if (!Node::init())
	{
		return false;
	}

	this->scheduleUpdate();

	// Init member level variables
	_speed = 250;

	return true;
}

void Player::Update(float delta) 
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_targetPos.x != GetParticleModel()->GetDisplacementX() || _targetPos.y != GetParticleModel()->GetDisplacementY())
		{
			MoveToTarget(delta);
		}
	}

	if (GameManager::sharedGameManager()->getIsGamePaused()) 
	{
		GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
	}
	else 
	{
		// Vertical Gravity
		if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y != 0.0f)
		{
			if (GetParticleModel()->GetDisplacementX() <= _targetPos.x - 2.0f || GetParticleModel()->GetDisplacementX() >= _targetPos.x + 2.0f)
			{
				_spriteTimer -= delta;

				if (!_running)
				{
					_running = true;
					_frame = 1;
					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}

				if (_spriteTimer <= 0.0f)
				{
					_spriteTimer = _spriteTimerDefault;
					++_frame;

					if (_frame > 6)
					{
						_frame = 1;
					}

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}
			}
			else
			{
				_frame = 1;
				_spriteTimer = _spriteTimerDefault;

				if (_running)
				{
					_running = false;

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
				}
			}
		}
		// Horizontal Gravity
		else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x != 0.0f)
		{
			if (GetParticleModel()->GetDisplacementY() <= _targetPos.y - 2.0f || GetParticleModel()->GetDisplacementY() >= _targetPos.y + 2.0f)
			{
				_spriteTimer -= delta;
				_running = true;

				if (_spriteTimer <= 0.0f) 
				{
					_spriteTimer = _spriteTimerDefault;
					++_frame;

					if (_frame > 6) 
					{
						_frame = 1;
					}

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}
			}
			else 
			{
				_frame = 1;
				_spriteTimer = _spriteTimerDefault;

				if (_running) 
				{
					_running = false;

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
				}
			}
		}
	}

	GameObject::Update(delta);
}

//void Player::CheckPlatformCollisions(cocos2d::Sprite* collider)
//{
//	auto winSize = Director::getInstance()->getVisibleSize();
//
//	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
//	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
//	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//	float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
//
//	if (_orientationVertical) {
//		scaledPlayerWidth *= 0.77;
//
//		if (_gravity < 0.0f) {
//			if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
//				&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
//				&& GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
//				&& GetSprite()->getPositionY() - (scaledPlayerHeight / 8) > collider->getPositionY() - (scaledHeight / 2))
//			{
//				Land(collider);
//			}
//			else {
//				_fallingVertical = true;
//			}
//		}
//		else {
//			if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
//				&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
//				&& GetSprite()->getPositionY() + (scaledPlayerHeight / 8) < collider->getPositionY() + (scaledHeight / 2)
//				&& GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
//			{
//				Land(collider);
//			}
//			else {
//				_fallingVertical = true;
//			}
//		}
//	}
//	else if (_orientationHorizontal) {
//		if (GetSprite()->getPositionX() - (scaledPlayerHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (scaledPlayerHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			if (GetSprite()->getPositionY() < collider->getPositionY()) {
//				GetSprite()->setPositionY(collider->getPositionY() - (scaledHeight / 2) - (scaledPlayerWidth / 2));
//				SetTarget(Vec2(_targetPos.x, GetSprite()->getPositionY()));
//			}
//			else {
//				GetSprite()->setPositionY(collider->getPositionY() + (scaledHeight / 2) + (scaledPlayerWidth / 2));
//				SetTarget(Vec2(_targetPos.x, GetSprite()->getPositionY()));
//			}
//
//		}
//	}
//}
//
//void Player::CheckWallCollisions(cocos2d::Sprite* collider)
//{
//	auto winSize = Director::getInstance()->getVisibleSize();
//
//	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
//	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
//	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
//	float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();
//
//	if (_orientationVertical) {
//		if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
//			&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
//			&& GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
//			&& GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
//		{
//			if (GetSprite()->getPositionX() < collider->getPositionX()) {
//				GetSprite()->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
//				SetTarget(Vec2(GetSprite()->getPositionX(), _targetPos.y));
//			}
//			else {
//				GetSprite()->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
//				SetTarget(Vec2(GetSprite()->getPositionX(), _targetPos.y));
//			}
//
//		}
//	}
//	else if (_orientationHorizontal) {
//		scaledPlayerWidth *= 0.77;
//
//		if (_gravity < 0.0f) {
//			if (GetSprite()->getPositionX() - (scaledPlayerHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
//				&& GetSprite()->getPositionX() - (scaledPlayerHeight / 8) > collider->getPositionX() - (scaledWidth / 2)
//				&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
//				&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
//			{
//				Land(collider);
//			}
//			else {
//				_fallingHorizontal = true;
//			}
//		}
//		else {
//			if (GetSprite()->getPositionX() + (scaledPlayerHeight / 8) < collider->getPositionX() + (scaledWidth / 2)
//				&& GetSprite()->getPositionX() + (scaledPlayerHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
//				&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
//				&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
//			{
//				Land(collider);
//			}
//			else {
//				_fallingHorizontal = true;
//			}
//		}
//	}
//}

void Player::SetTarget(Vec2 target)
{
	// Vertical Gravity
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y < 0.0f)
	{
		if (GetParticleModel()->GetDisplacementX() > target.x)
		{
			GetSprite()->setFlippedX(true);
		}
		else if (GetParticleModel()->GetDisplacementX() == target.x)
		{
			// Do nothing
		}
		else
		{
			GetSprite()->setFlippedX(false);
		}
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y > 0.0f)
	{
		if (GetParticleModel()->GetDisplacementX() < target.x)
		{
			GetSprite()->setFlippedX(true);
		}
		else if (GetParticleModel()->GetDisplacementX() == target.x)
		{
			// Do nothing
		}
		else
		{
			GetSprite()->setFlippedX(false);
		}
	}
	// Horizontal Gravity
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x < 0.0f)
	{
		if (GetParticleModel()->GetDisplacementY() < target.y)
		{
			GetSprite()->setFlippedX(true);
		}
		else if (GetParticleModel()->GetDisplacementY() == target.y)
		{
			// Do nothing
		}
		else
		{
			GetSprite()->setFlippedX(false);
		}
	}
	else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x > 0.0f)
	{
		if (GetParticleModel()->GetDisplacementY() > target.y) 
		{
			GetSprite()->setFlippedX(true);
		}
		else if (GetParticleModel()->GetDisplacementY() == target.y) 
		{
			// Do nothing
		}
		else 
		{
			GetSprite()->setFlippedX(false);
		}
	}

	_targetPos = target;
}

void Player::MoveToTarget(float delta)
{
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y != 0.0f)
	{
		if (GetParticleModel()->GetDisplacementX() > _targetPos.x)
		{
			GetParticleModel()->SetDisplacementX(GetParticleModel()->GetDisplacementX() - (_speed * delta));

			if (GetParticleModel()->GetDisplacementX() < _targetPos.x)
			{
				GetParticleModel()->SetDisplacementX(_targetPos.x);
			}
		}
		else if (GetParticleModel()->GetDisplacementX() < _targetPos.x)
		{
			GetParticleModel()->SetDisplacementX(GetParticleModel()->GetDisplacementX() + (_speed * delta));

			if (GetParticleModel()->GetDisplacementX() > _targetPos.x)
			{
				GetParticleModel()->SetDisplacementX(_targetPos.x);
			}
		}
	}
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x != 0.0f)
	{
		if (GetParticleModel()->GetDisplacementY() > _targetPos.y) 
		{
			GetParticleModel()->SetDisplacementY(GetParticleModel()->GetDisplacementY() - (_speed * delta));

			if (GetParticleModel()->GetDisplacementY() < _targetPos.y) 
			{
				GetParticleModel()->SetDisplacementY(_targetPos.y);
			}
		}
		else if (GetParticleModel()->GetDisplacementY() < _targetPos.y) 
		{
			GetParticleModel()->SetDisplacementY(GetParticleModel()->GetDisplacementY() + (_speed * delta));

			if (GetParticleModel()->GetDisplacementY() > _targetPos.y) 
			{
				GetParticleModel()->SetDisplacementY(_targetPos.y);
			}
		}
	}
}