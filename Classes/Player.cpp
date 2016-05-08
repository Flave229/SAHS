#include "Player.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Player::Player()
{
	SetGravity(-3.81f);

	_timeFalling = 0.0f;
	_speed = 0.0f;
	_frame = 1;
	_running = false;
	_spriteTimer = _spriteTimerDefault;

	_orientationHorizontal = false;
	_orientationVertical = true;
	_fallingVertical = false;
	_fallingHorizontal = false;

	_horizontalVelocity = 0.0f;
	_horizontalVelocityLast = 0.0f;
	_verticalVelocityLast = 0.0f;
	_verticalVelocity = 0.0f;
}

Player* Player::create()
{
	Player* player = new Player();
	if (!player->init()) {
		return nullptr;
	}

	player->autorelease();

	return player;
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

void Player::update(float delta) 
{
	//Debug
	//_falling = true;

	// Never let falling become minus
	if (_timeFalling < 0.0f) {
		_timeFalling = 0.0f;
	}

	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (_fallingHorizontal || _fallingVertical) {
			Fall(delta);
		}
		if (_orientationVertical) {
			if (_targetPos.x != GetSprite()->getPositionX()) {
				MoveToTarget(delta);
			}
		}
		else if (_orientationHorizontal) {
			if (_targetPos.y != GetSprite()->getPositionY()) {
				MoveToTarget(delta);
			}
		}
	}

	if (GameManager::sharedGameManager()->getIsGamePaused()) {
		GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
	}
	else {
		if (_orientationVertical) {
			if (GetSprite()->getPosition().x <= _targetPos.x - 2.0f || GetSprite()->getPosition().x >= _targetPos.x + 2.0f) {
				_spriteTimer -= delta;

				if (!_running) {
					_running = true;
					_frame = 1;
					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}

				if (_spriteTimer <= 0.0f) {
					_spriteTimer = _spriteTimerDefault;
					++_frame;

					if (_frame > 6) {
						_frame = 1;
					}

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}
			}
			else {
				_frame = 1;
				_spriteTimer = _spriteTimerDefault;

				if (_running) {
					_running = false;

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
				}
			}
		}
		else {
			if (GetSprite()->getPosition().y <= _targetPos.y - 2.0f || GetSprite()->getPosition().y >= _targetPos.y + 2.0f) {
				_spriteTimer -= delta;
				_running = true;

				if (_spriteTimer <= 0.0f) {
					_spriteTimer = _spriteTimerDefault;
					++_frame;

					if (_frame > 6) {
						_frame = 1;
					}

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Run_" + StringUtils::format("%d", _frame) + ".png"));
				}
			}
			else {
				_frame = 1;
				_spriteTimer = _spriteTimerDefault;

				if (_running) {
					_running = false;

					GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Husky/Husky_Stand.png"));
				}
			}
		}
	}
}

void Player::SetGravity(float gravity)
{
	_gravity = gravity;
}

void Player::SetVelocity(float y)
{
	_verticalVelocity = y;
}

void Player::CheckPlatformCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

	if (_orientationVertical) {
		scaledPlayerWidth *= 0.77;

		if (_gravity < 0.0f) {
			if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
				&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
				&& GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
				&& GetSprite()->getPositionY() - (scaledPlayerHeight / 8) > collider->getPositionY() - (scaledHeight / 2))
			{
				Land(collider);
			}
			else {
				_fallingVertical = true;
			}
		}
		else {
			if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
				&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
				&& GetSprite()->getPositionY() + (scaledPlayerHeight / 8) < collider->getPositionY() + (scaledHeight / 2)
				&& GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
			{
				Land(collider);
			}
			else {
				_fallingVertical = true;
			}
		}
	}
	else if (_orientationHorizontal) {
		if (GetSprite()->getPositionX() - (scaledPlayerHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (scaledPlayerHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			if (GetSprite()->getPositionY() < collider->getPositionY()) {
				GetSprite()->setPositionY(collider->getPositionY() - (scaledHeight / 2) - (scaledPlayerWidth / 2));
				SetTarget(Vec2(_targetPos.x, GetSprite()->getPositionY()));
			}
			else {
				GetSprite()->setPositionY(collider->getPositionY() + (scaledHeight / 2) + (scaledPlayerWidth / 2));
				SetTarget(Vec2(_targetPos.x, GetSprite()->getPositionY()));
			}

		}
	}
}

void Player::CheckWallCollisions(cocos2d::Sprite* collider)
{
	auto winSize = Director::getInstance()->getVisibleSize();

	float scaledWidth = collider->getContentSize().width * collider->getScaleX();
	float scaledHeight = collider->getContentSize().height * collider->getScaleY();
	float scaledPlayerWidth = GetSprite()->getContentSize().width * GetSprite()->getScaleX();
	float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

	if (_orientationVertical) {
		if (GetSprite()->getPositionX() - (scaledPlayerWidth / 2) < collider->getPositionX() + (scaledWidth / 2)
			&& GetSprite()->getPositionX() + (scaledPlayerWidth / 2) > collider->getPositionX() - (scaledWidth / 2)
			&& GetSprite()->getPositionY() - (scaledPlayerHeight / 2) < collider->getPositionY() + (scaledHeight / 2)
			&& GetSprite()->getPositionY() + (scaledPlayerHeight / 2) > collider->getPositionY() - (scaledHeight / 2))
		{
			if (GetSprite()->getPositionX() < collider->getPositionX()) {
				GetSprite()->setPositionX(collider->getPositionX() - (scaledWidth / 2) - (scaledPlayerWidth / 2));
				SetTarget(Vec2(GetSprite()->getPositionX(), _targetPos.y));
			}
			else {
				GetSprite()->setPositionX(collider->getPositionX() + (scaledWidth / 2) + (scaledPlayerWidth / 2));
				SetTarget(Vec2(GetSprite()->getPositionX(), _targetPos.y));
			}

		}
	}
	else if (_orientationHorizontal) {
		scaledPlayerWidth *= 0.77;

		if (_gravity < 0.0f) {
			if (GetSprite()->getPositionX() - (scaledPlayerHeight / 2) < collider->getPositionX() + (scaledWidth / 2)
				&& GetSprite()->getPositionX() - (scaledPlayerHeight / 8) > collider->getPositionX() - (scaledWidth / 2)
				&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
				&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
			{
				Land(collider);
			}
			else {
				_fallingHorizontal = true;
			}
		}
		else {
			if (GetSprite()->getPositionX() + (scaledPlayerHeight / 8) < collider->getPositionX() + (scaledWidth / 2)
				&& GetSprite()->getPositionX() + (scaledPlayerHeight / 2) > collider->getPositionX() - (scaledWidth / 2)
				&& GetSprite()->getPositionY() - (scaledPlayerWidth / 2) < collider->getPositionY() + (scaledHeight / 2)
				&& GetSprite()->getPositionY() + (scaledPlayerWidth / 2) > collider->getPositionY() - (scaledHeight / 2))
			{
				Land(collider);
			}
			else {
				_fallingHorizontal = true;
			}
		}
	}
}

void Player::Land(cocos2d::Sprite* collider)
{
	if (_fallingVertical) {
		_fallingVertical = false;

		// Cancel velocity
		_verticalVelocity = 0.0f;
		_verticalVelocityLast = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newY;
		float scaledColliderHeight = collider->getContentSize().height * collider->getScaleY();
		float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

		if (_gravity < 0.0f) {
			newY = (collider->getPositionY() + (scaledColliderHeight / 2)) + (scaledPlayerHeight / 2);
		}
		else {
			newY = (collider->getPositionY() - (scaledColliderHeight / 2)) - (scaledPlayerHeight / 2);
		}

		GetSprite()->setPosition(Vec2(GetSprite()->getPositionX(), newY));
	}
	else if (_fallingHorizontal) {
		_fallingHorizontal = false;

		// Cancel velocity
		_horizontalVelocity = 0.0f;
		_horizontalVelocityLast = 0.0f;
		_timeFalling = 0.0f;

		// Make sure the position is set so not inside platform
		float newX;
		float scaledColliderWidth = collider->getContentSize().width * collider->getScaleX();
		float scaledPlayerHeight = GetSprite()->getContentSize().height * GetSprite()->getScaleY();

		if (_gravity > 0.0f) {
			newX = (collider->getPositionX() - (scaledColliderWidth / 2)) - (scaledPlayerHeight / 2);
		}
		else {
			newX = (collider->getPositionX() + (scaledColliderWidth / 2)) + (scaledPlayerHeight / 2);
		}

		GetSprite()->setPosition(Vec2(newX, GetSprite()->getPositionY()));
	}
}

void Player::Fall(float delta)
{
	if (_orientationVertical) {
		if (_fallingVertical) {
			// Update falling time
			_timeFalling += delta;

			// Calculate and set new velocity
			if (_verticalVelocity > -28.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling / 1.5);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}
			else if (_verticalVelocity < 28.0f) {
				_verticalVelocity = _verticalVelocityLast + ((_gravity / 2) * _timeFalling / 1.5);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}
			else {
				_verticalVelocity = -9.0f;
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x, GetSprite()->getPosition().y + _verticalVelocity));
			}

			// Update last velocity
			_verticalVelocityLast = _verticalVelocity;
		}
		else {
			_fallingVertical = true;
			_verticalVelocityLast = 0.0f;
			_verticalVelocity = 0.0f;
			_timeFalling = 0.0f;
		}
	}
	else if (_orientationHorizontal) {
		if (_fallingHorizontal) {
			// Update falling time
			_timeFalling += delta;

			// Calculate and set new velocity
			if (_horizontalVelocity > -28.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling / 1.5);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}
			else if (_horizontalVelocity < 28.0f) {
				_horizontalVelocity = _horizontalVelocityLast + ((_gravity / 2) * _timeFalling / 1.5);
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}
			else {
				_horizontalVelocity = -9.0f;
				GetSprite()->setPosition(Vec2(GetSprite()->getPosition().x + _horizontalVelocity, GetSprite()->getPosition().y));
			}

			// Update last velocity
			_horizontalVelocityLast = _horizontalVelocity;
		}
		else {
			_fallingHorizontal = true;
			_verticalVelocityLast = 0.0f;
			_verticalVelocity = 0.0f;
			_timeFalling = 0.0f;
		}
	}
}

void Player::SetTarget(Vec2 target)
{
	if (!GetOrientationHorizontal()) {
		if (_gravity < 0.0f) {
			if (GetSprite()->getPosition().x > target.x) {
				GetSprite()->setFlippedX(true);
			}
			else if (GetSprite()->getPosition().x == target.x) {
				// Do nothing
			}
			else {
				GetSprite()->setFlippedX(false);
			}
		}
		else {
			if (GetSprite()->getPosition().x < target.x) {
				GetSprite()->setFlippedX(true);
			}
			else if (GetSprite()->getPosition().x == target.x) {
				// Do nothing
			}
			else {
				GetSprite()->setFlippedX(false);
			}
		}
	}
	else if (!GetOrientationVertical()) {
		if (_gravity < 0.0f) {
			if (GetSprite()->getPosition().y < target.y) {
				GetSprite()->setFlippedX(true);
			}
			else if (GetSprite()->getPosition().y == target.y) {
				// Do nothing
			}
			else {
				GetSprite()->setFlippedX(false);
			}
		}
		else {
			if (GetSprite()->getPosition().y > target.y) {
				GetSprite()->setFlippedX(true);
			}
			else if (GetSprite()->getPosition().y == target.y) {
				// Do nothing
			}
			else {
				GetSprite()->setFlippedX(false);
			}
		}
	}

	_targetPos = target;
}

void Player::MoveToTarget(float deltaTime)
{
	if (_orientationVertical) {
		if (GetSprite()->getPositionX() > _targetPos.x) {
			GetSprite()->setPositionX(GetSprite()->getPositionX() - (_speed * deltaTime));

			if (GetSprite()->getPositionX() < _targetPos.x) {
				GetSprite()->setPositionX(_targetPos.x);
			}
		}
		else if (GetSprite()->getPositionX() < _targetPos.x) {
			GetSprite()->setPositionX(GetSprite()->getPositionX() + (_speed * deltaTime));

			if (GetSprite()->getPositionX() > _targetPos.x) {
				GetSprite()->setPositionX(_targetPos.x);
			}
		}
	}
	else if (_orientationHorizontal) {
		if (GetSprite()->getPositionY() > _targetPos.y) {
			GetSprite()->setPositionY(GetSprite()->getPositionY() - (_speed * deltaTime));

			if (GetSprite()->getPositionY() < _targetPos.y) {
				GetSprite()->setPositionY(_targetPos.y);
			}
		}
		else if (GetSprite()->getPositionY() < _targetPos.y) {
			GetSprite()->setPositionY(GetSprite()->getPositionY() + (_speed * deltaTime));

			if (GetSprite()->getPositionY() > _targetPos.y) {
				GetSprite()->setPositionY(_targetPos.y);
			}
		}
	}
}

void Player::SetOrientationVertical(bool orientation)
{
	_orientationVertical = orientation;
}

void Player::SetOrientationHorizontal(bool orientation)
{
	_orientationHorizontal = orientation;
}

void Player::SetFallingVertical(bool falling)
{
	_fallingVertical = falling;
	_orientationVertical = falling;

	_fallingHorizontal = !falling;
	_orientationHorizontal = !falling;

	FlipPlayer();
}

void Player::SetFallingHorizontal(bool falling)
{
	_fallingHorizontal = falling;
	_orientationHorizontal = falling;

	_fallingVertical = !falling;
	_orientationVertical = !falling;

	FlipPlayer();
}

void Player::FlipPlayer()
{
	if (_fallingVertical) {
		if (_gravity < 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 0.0f);
			GetSprite()->runAction(rotateTo);
		}
		else if (_gravity > 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 180.0f);
			GetSprite()->runAction(rotateTo);
		}
	}
	else if (_fallingHorizontal) {
		if (_gravity < 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 90.0f);
			GetSprite()->runAction(rotateTo);
		}
		else if (_gravity > 0)
		{
			auto rotateTo = RotateTo::create(0.5f, 270.0f);
			GetSprite()->runAction(rotateTo);
		}
	}
}

void Player::SetSprite(Sprite* newSprite, Sprite* spawnPoint) 
{ 
	_playerSprite = newSprite;
	if (_playerSprite->getPosition() != newSprite->getPosition())
	{
		_playerSprite->setPosition(spawnPoint->getPosition());
	}

	this->addChild(_playerSprite);
}