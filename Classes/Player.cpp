#include "Player.h"

USING_NS_CC;

using namespace cocostudio::timeline;

Player::Player() : GameObject(new ParticleModel())
{
	// Sprite
	_frame = 1;
	_spriteTimerDefault = 1.0f / 12.0f;
	_spriteTimer = _spriteTimerDefault;
	_running = false;

	// Move To
	_targetPos = Vec2(0.0f, 0.0f);
	_speed = 250.0f;
}

Player::Player(ParticleModel* particle) : GameObject(particle)
{
	// Sprite
	_frame = 1;
	_spriteTimerDefault = 1.0f / 12.0f;
	_spriteTimer = _spriteTimerDefault;
	_running = false;

	// Move To
	_targetPos = Vec2(0.0f, 0.0f);
	_speed = 250.0f;
}

Player::~Player()
{
}

void Player::Update(float delta) 
{
	if (!GameManager::sharedGameManager()->getIsGamePaused())
	{
		if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().y != 0.0f)
		{
			if (GetParticleModel()->GetDisplacementX() <= _targetPos.x - 2.0f || GetParticleModel()->GetDisplacementX() >= _targetPos.x + 2.0f)
			{
				MoveToTarget(delta);
			}
			else
			{
				GetParticleModel()->SetVelocityX(0.0f);
			}
		}
		else if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x != 0.0f)
		{
			if (GetParticleModel()->GetDisplacementY() <= _targetPos.y - 2.0f || GetParticleModel()->GetDisplacementY() >= _targetPos.y + 2.0f)
			{
				MoveToTarget(delta);
			}
			else
			{
				GetParticleModel()->SetVelocityY(0.0f);
			}
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
			GetParticleModel()->SetVelocityX(-1 * _speed);

			if (GetParticleModel()->GetDisplacementX() <= _targetPos.x)
			{
				GetParticleModel()->SetVelocityX(0.0f);
			}
		}
		else if (GetParticleModel()->GetDisplacementX() < _targetPos.x)
		{
			GetParticleModel()->SetVelocityX(_speed);

			if (GetParticleModel()->GetDisplacementX() >= _targetPos.x)
			{
				GetParticleModel()->SetVelocityX(0.0f);
			}
		}
		else
		{
			GetParticleModel()->SetVelocityX(0.0f);
		}
	}
	if (GetParticleModel()->GetForce("Gravity") != nullptr && GetParticleModel()->GetForce("Gravity")->GetForce().x != 0.0f)
	{
		if (GetParticleModel()->GetDisplacementY() > _targetPos.y) 
		{
			GetParticleModel()->SetVelocityY(-1 * _speed);

			if (GetParticleModel()->GetDisplacementY() <= _targetPos.y) 
			{
				GetParticleModel()->SetVelocityY(0.0f);
			}
		}
		else if (GetParticleModel()->GetDisplacementY() < _targetPos.y) 
		{
			GetParticleModel()->SetVelocityY(_speed);

			if (GetParticleModel()->GetDisplacementY() >= _targetPos.y) 
			{
				GetParticleModel()->SetVelocityY(0.0f);
			}
		}
		else
		{
			GetParticleModel()->SetVelocityY(0.0f);
		}
	}
}