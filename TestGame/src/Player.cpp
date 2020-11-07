#include "Player.h"

#include <iostream>

Player::Player(const Vec3& position, Window* window, Terrain* terrain)
	: m_camera(new Camera(20, 45, 0)), _window(window), m_movement(Vec3(0)),
           m_rotating(false), m_oldPosition(Vec2(0,0)), m_terrain(terrain)
{
    //ImageMaterial* grassMat = new ImageMaterial("res/textures/container.png", (char*)NULL, 1, 32, false, false);
    ImageMaterial* containerMat = new ImageMaterial("res/textures/container.png", "res/textures/container_specular.png", "res/textures/container_emission.jpg", 1, 32, false);
	m_body = new Entity("res/cube.obj", containerMat, 0, position, Vec3(0), Vec3(2));
}

void Player::Update()
{
    float delta = _window->delta;
    CheckInput();

    (*m_body)(ROTATION, SUM_EQ, Y, currentTurnSpeed * delta);
#if LOCKED_CAMERA
    //m_camera->Swivel() = m_body->GetRotation().y;
#else
    if (!_window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
    {
        m_rotating = false;
    }

    if (!m_rotating && _window->isMouseButtonPressed(GLFW_MOUSE_BUTTON_2))
    {
        m_rotating = true;
        m_oldPosition = _window->getMousePosition();
    }

    else if (m_rotating)
    {
        Vec2 change(_window->getMousePosition());
        change -= m_oldPosition;

        change.Normalize();
        change *= PanSpeed;

        m_camera->operator()(SWIVEL, SUM_EQ, NULL, -change.x);
        m_camera->operator()(PITCH, EQ, NULL, Clamp(m_camera->GetPitch() + change.y, (float)MIN_PITCH, (float)MAX_PITCH));
    }
#endif

    float distance = currentSpeed * delta;
    float y_rot = ToRadians(m_body->GetRotation().y);
    (*m_body)(TRANSLATION, SUM_EQ, X, distance * sin(y_rot));
    (*m_body)(TRANSLATION, SUM_EQ, Z, distance * cos(y_rot));

    upwardsSpeed += GRAVITY * delta;
    (*m_body)(TRANSLATION, SUM_EQ, Y, upwardsSpeed * delta);

    m_camera->LookAt(m_body->GetTranslation());
    
    float terrainHeight = m_terrain->GetHeightOfTerrain(m_body->GetTranslation().x, m_body->GetTranslation().z);
    if (m_body->GetTranslation().y < terrainHeight)
    {
        upwardsSpeed = 0;
        isGrounded = true;
        (*m_body)(TRANSLATION, EQ, Y, terrainHeight);
    }

    float mouseWheel = _window->getMouseWheel();
    if (mouseWheel != 0)
    {
        m_camera->operator()(DISTANCE, EQ, NULL, Clamp(m_camera->GetDistance() + (mouseWheel * -1 * ZoomPower * 0.1f), (float)MIN_DISTANCE, (float)MAX_DISTANCE));
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