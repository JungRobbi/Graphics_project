#include "Player.h"
#include "Transform3D.h"
#include "Camera.h"
#include "PlayerMovement.h"

Player::Player() 
{
	AddComponent<Transform3D>();
	AddComponent<Camera>();
	AddComponent<PlayerMovement>();
}