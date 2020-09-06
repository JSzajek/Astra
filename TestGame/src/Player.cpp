#include "Player.h"

#include <iostream>

Player::Player(Window* window)
	: m_camera(new Camera(20, 25, 0)), _window(window), m_movement(Vec3(0))
{
	const VertexArray* bodyModel = ObjLoader::LoadObjectModel("res/bunny.obj");
	m_body = new Entity(bodyModel, Vec3(0), Vec3(0), Vec3(0.4f));
}

void Player::Update()
{
    CheckInput();

    m_body->Rotation().y += currentTurnSpeed * _window->GetDelta();
    
    //m_camera->Swivel() = m_body->GetRotation().y;

    float distance = currentSpeed * _window->GetDelta();
    float y_rot = ToRadians(m_body->GetRotation().y);
    m_body->Translation().x += distance * sin(y_rot);
    m_body->Translation().z += distance * cos(y_rot);

    upwardsSpeed += GRAVITY * _window->GetDelta();
    m_body->Translation().y += upwardsSpeed * _window->GetDelta();
    
    //m_camera->Translation() = m_body->GetTranslation();

    if (m_body->GetTranslation().y < TERRAIN_HEIGHT)
    {
        upwardsSpeed = 0;
        isGrounded = true;
        m_body->Translation().y = 0;
    }

    if (_window->isKeyPressed(90))
    {
        m_camera->Distance() += -0.01f;
    }
    if (_window->isKeyPressed(88))
    {
        m_camera->Distance() += 0.01f;
    }
}

void Player::CheckInput()
{
    if (_window->isKeyPressed(GLFW_KEY_W))
    {
        currentSpeed = RunSpeed;
    }
    else if (_window->isKeyPressed(GLFW_KEY_S))
    {
        currentSpeed = -RunSpeed;
    }
    else
    {
        currentSpeed = 0;
    }

    if (_window->isKeyPressed(GLFW_KEY_D))
    {
        currentTurnSpeed = -TurnSpeed;
    }
    else if (_window->isKeyPressed(GLFW_KEY_A))
    {
        currentTurnSpeed = TurnSpeed;
    }
    else
    {
        currentTurnSpeed = 0;
    }

    if (_window->isKeyPressed(GLFW_KEY_SPACE))
    {
        if (isGrounded)
        {
            upwardsSpeed = 30;
            isGrounded = false;
        }
    }
}

Player::~Player()
{
    delete m_body;
    delete m_camera;
}