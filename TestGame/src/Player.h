#pragma once

#include "Astra.h"
#include "Synchronous.h"

#define		MAX_DISTANCE	 80
#define		MIN_DISTANCE	 80
#define		MAX_PITCH		 90
#define		MIN_PITCH		-90

using namespace Astra::Graphics;
using namespace Astra::Math;

class Player : public Synchronous
{
private:
	Window* _window;
	Camera* m_camera;
	Entity* m_body;

	Vec3 m_movement;
public:
	Player(Window* window);
	~Player();
	void Update() override;
	void MovePlayer(const Vec3& movement);
	inline Camera* GetCamera() { return m_camera; }
	inline Entity* GetRendering() { return m_body; }
};