#include "Player.h"

#include <iostream>

Player::Player(const Vec3& position, Window* window, Terrain* terrain)
	: m_camera(new Camera(20, 45, 0)), _window(window), m_movement(Vec3(0)),
           m_rotating(false), m_oldPosition(Vec2(0,0)), m_terrain(terrain)
{
    ImageMaterial* grassMat = new ImageMaterial("res/textures/fernTexture.png", 1, 1, 0, true, true);
	const VertexArray* bodyModel = ObjLoader::LoadObjectModel("res/fern.obj");
	m_body = new Entity(bodyModel, grassMat, position, Vec3(0), Vec3(2));
}

void Player::Update()
{
    float delta = _window->delta;
    CheckInput();

    m_body->Rotation().y += currentTurnSpeed * delta;
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

        m_camera->Swivel() += -change.x;
        m_camera->Pitch() += change.y;
        Clamp(m_camera->Pitch(), (float)MIN_PITCH, (float)MAX_PITCH);
    }
#endif

    float distance = currentSpeed * delta;
    float y_rot = ToRadians(m_body->GetRotation().y);
    m_body->Translation().x += distance * sin(y_rot);
    m_body->Translation().z += distance * cos(y_rot);

    upwardsSpeed += GRAVITY * delta;
    m_body->Translation().y += upwardsSpeed * delta;

    m_camera->LookAt(m_body->GetTranslation());
    
    float terrainHeight = m_terrain->GetHeightOfTerrain(m_body->GetTranslation().x, m_body->GetTranslation().z);
    if (m_body->GetTranslation().y < terrainHeight)
    {
        upwardsSpeed = 0;
        isGrounded = true;
        m_body->Translation().y = terrainHeight;
    }

    float mouseWheel = _window->getMouseWheel();
    if (mouseWheel != 0)
    {
        m_camera->Distance() += mouseWheel * -1 * ZoomPower * 0.1f;
        Clamp(m_camera->Distance(), (float)MIN_DISTANCE, (float)MAX_DISTANCE);
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