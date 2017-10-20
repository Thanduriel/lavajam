#pragma once
#include "component.hpp"
class physicsComponent :
	public Component
{
public:
	physicsComponent(Actor* actor, bool isActive = true);
	~physicsComponent();
};

