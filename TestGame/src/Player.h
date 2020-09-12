#pragma once

#include "Astra.h"
#include "Synchronous.h"

#define		MAX_DISTANCE	 120
#define		MIN_DISTANCE	 5
#define		MAX_PITCH		 90
#define		MIN_PITCH		 0

#define		GRAVITY			-50
#define		TERRAIN_HEIGHT	 0

#define		LOCKED_CAMERA	 0

using namespace Astra::Graphics;
using namespace Astra::Math;

class Player : public Synchronous
{
private:
	float RunSpeed = 20;
	float TurnSpeed = 160;  // In Degrees
	float JumpPower = 30;
	float ZoomPower = 20;
	float PanSpeed = 1;
	
	Window* _window;
	Camera* m_camera;
	Entity* m_body;
	Terrain* m_terrain;

	bool m_rotating;
	Vec2 m_oldPosition;

	float currentSpeed = 0;
	float currentTurnSpeed = 0;
	float upwardsSpeed = 0;

	bool isGrounded = true;

	Vec3 m_movement;
public:
	Player(const Vec3& position, Window* window, Terrain* terrain);
	~Player();
	void Update() override;
	void CheckInput();
	inline Camera* GetCamera() { return m_camera; }
	inline Entity* GetRendering() { return m_body; }
};