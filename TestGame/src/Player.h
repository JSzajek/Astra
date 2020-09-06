#pragma once

#include "Astra.h"
#include "Synchronous.h"

#define		MAX_DISTANCE	 80
#define		MIN_DISTANCE	 80
#define		MAX_PITCH		 90
#define		MIN_PITCH		-90

#define		GRAVITY			-50

#define		TERRAIN_HEIGHT	 0

using namespace Astra::Graphics;
using namespace Astra::Math;

class Player : public Synchronous
{
private:
	float RunSpeed = 20;
	float TurnSpeed = 160;  // In Degrees
	float JumpPower = 30;
	
	Window* _window;
	Camera* m_camera;
	Entity* m_body;

	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardsSpeed = 0;

	bool isGrounded = true;

	Vec3 m_movement;
public:
	Player(Window* window);
	~Player();
	void Update() override;
	void CheckInput();
	void MovePlayer(const Vec3& movement);
	inline Camera* GetCamera() { return m_camera; }
	inline Entity* GetRendering() { return m_body; }
};