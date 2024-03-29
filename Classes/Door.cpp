#include "Door.h"

Door::Door()
{
}

Door* Door::create()
{
	Door* door = new Door();
	if (!door->init())
	{
		return nullptr;
	}

	door->autorelease();

	return door;
}

bool Door::init()
{
	if (!Node::init()) {
		return false;
	}

	this->scheduleUpdate();

	return true;
}

Door::~Door()
{
}

void Door::update(float delta)
{
	if (_timer > 0.0f) {
		_timer -= delta;
	}

	if (_linkedButton->GetActive()) {
		_timer = _timerDefault;

		if (!_open) {
			Open(delta);
		}
	}
	else {
		if (_timer <= 0.0f) {
			Close(delta);
		}
	}
}

bool Door::Open(float delta)
{
	_open = true;

	GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Scene_Backgrounds/Transparent.png"));

	return true;
}

bool Door::Close(float delta)
{
	_open = false;

	if (getName().find("Hatch")) {
		GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Doors/Hatch_Hardlight.png"));
	}
	else {
		GetSprite()->setTexture(Director::getInstance()->getTextureCache()->addImage("Art/Level_Main_Assets/Doors/Door_Hardlight.png"));
	}

	return true;
}

void Door::SetSprite(std::vector<FloorButton*> buttons, Sprite* newSprite)
{ 
	_sprite = newSprite; 

	cocostudio::ComExtensionData* data = dynamic_cast<cocostudio::ComExtensionData*>(_sprite->getComponent("ComExtensionData"));
	std::string userdata = data->getCustomProperty();

	for (int i = 0; i < (int)buttons.size(); i++) {
		if (buttons.at(i)->getName() == userdata) {
			_linkedButton = buttons.at(i);
			_timerDefault = _linkedButton->GetTimer();
		}
	}

	this->addChild(_sprite); 
	_open = false;
};
