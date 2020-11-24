#include "Player.h"

#include <iostream>

Player::Player(const Vec3& position, Terrain* terrain)
	: m_camera(new Camera(20, 45, 0)), m_movement(Vec3(0)),
           m_rotating(false), m_oldPosition(Vec2(0,0)), m_terrain(terrain)
{
    ImageMaterial* containerMat = new ImageMaterial("res/textures/container.png", "res/textures/container_specular.png", 1, 32, false);
	m_body = new Entity("res/cube.obj", containerMat, 0, position, Vec3(0), Vec3(1));
}

void Player::Update(float delta)
{
    CheckInput();

    (*m_body)(ROTATION, SUM_EQ, Y_POS, currentTurnSpeed * delta);
#if LOCKED_CAMERA
    //m_camera->Swivel() = m_body->GetRotation().y;
#else
    if (!Input::IsMouseButtonPressed(Mouse::BUTTON_2))
    {
        m_rotating = false;
    }

    if (!m_rotating && Input::IsMouseButtonPressed(Mouse::BUTTON_2))
    {
        m_rotating = true;
        m_oldPosition = Input::GetMousePosition();
    }

    else if (m_rotating)
    {
        Vec2 change(Input::GetMousePosition());
        change -= m_oldPosition;

        change.Normalize();
        change *= PanSpeed;

        m_camera->operator()(SWIVEL, SUM_EQ, NULL, -change.x);
        m_camera->operator()(PITCH, EQ, NULL, Clamp(m_camera->GetPitch() + change.y, (float)MIN_PITCH, (float)MAX_PITCH));
    }
#endif

    float distance = currentSpeed * delta;
    float y_rot = ToRadians(m_body->GetRotation().y);
    (*m_body)(TRANSLATION, SUM_EQ, X_POS, distance * sin(y_rot));
    (*m_body)(TRANSLATION, SUM_EQ, Z_POS, distance * cos(y_rot));

    upwardsSpeed += GRAVITY * delta;
    (*m_body)(TRANSLATION, SUM_EQ, Y_POS, upwardsSpeed * delta);

    m_camera->LookAt(m_body->GetTranslation());
    
    float terrainHeight = m_terrain->GetHeightOfTerrain(static_cast<int>(m_body->GetTranslation().x), static_cast<int>(m_body->GetTranslation().z));
    if (m_body->GetTranslation().y < terrainHeight + GROUND_OFFSET)
    {
        upwardsSpeed = 0;
        isGrounded = true;
        (*m_body)(TRANSLATION, EQ, Y_POS, terrainHeight + GROUND_OFFSET);
    }

    float mouseWheel = Input::GetMouseScroll();
    if (mouseWheel != 0)
    {
        m_camera->operator()(DISTANCE, EQ, NULL, Clamp(m_camera->GetDistance() + (mouseWheel * -1 * ZoomPower * 0.1f), (float)MIN_DISTANCE, (float)MAX_DISTANCE));
    }

    Astra::Audio::AudioController::SetListenerPosition(m_body->GetTranslation());
}

void Player::CheckInput()
{
    if (Input::IsKeyJustReleased(Key::W))
    {
        currentSpeed = RunSpeed;
    }
    else if (Input::IsKeyPressed(Key::S))
    {
        currentSpeed = -RunSpeed;
    }
    else
    {
        currentSpeed = 0;
    }

    if (Input::IsKeyPressed(Key::D))
    {
        currentTurnSpeed = -TurnSpeed;
    }
    else if (Input::IsKeyPressed(Key::A))
    {
        currentTurnSpeed = TurnSpeed;
    }
    else
    {
        currentTurnSpeed = 0;
    }

    if (Input::IsKeyPressed(Key::SPACE))
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