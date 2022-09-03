#pragma once

class Component
{
public:
	class GameObject* gameObject;
public:
	virtual void start() = 0;
	virtual void update() = 0;
};