#include "Player.h"
#include "Transform3D.h"
#include "Camera.h"
#include "PlayerMovement.h"

Player::Player() : GameObject()
{
}

void Player::start()
{
	AddComponent<Transform3D>();
	AddComponent<Camera>();
	AddComponent<PlayerMovement>();
}