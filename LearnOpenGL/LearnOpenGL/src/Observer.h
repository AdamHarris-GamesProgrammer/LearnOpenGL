#pragma once
#include "Entity.h"
#include "Event.h"

class Observer
{
public:
	virtual ~Observer() {}
	virtual void OnNotify(const Entity& entity, ObserverEvent event) = 0;
	virtual void OnNotify(const Entity* entity, ObserverEvent event) = 0;
	virtual void OnNotify(ObserverEvent event) = 0;
};