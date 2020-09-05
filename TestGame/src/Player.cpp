#include "Player.h"

Player::Player(Window* window)
	: m_camera(new Camera(20, 25, 0)), _window(window), m_movement(Vec3(0))
{
	const VertexArray* bodyModel = ObjLoader::LoadObjectModel("res/pylon.obj");
	m_body = new Entity(bodyModel, Vec3(0));
}

void Player::Update()
{
    m_movement.x = 0;
    m_movement.y = 0;
    m_movement.z = 0;

    if (_window->isKeyPressed(GLFW_KEY_W))
    {
        m_movement.z += 0.01f;
    }
    if (_window->isKeyPressed(GLFW_KEY_S))
    {
        m_movement.z += -0.01f;
    }

    if (_window->isKeyPressed(GLFW_KEY_A))
    {
        m_movement.x += 0.01f;
    }
    if (_window->isKeyPressed(GLFW_KEY_D))
    {
        m_movement.x += -0.01f;
    }

    if (_window->isKeyPressed(GLFW_KEY_UP))
    {
        m_camera->Pitch() += 0.02f;
    }
    if (_window->isKeyPressed(GLFW_KEY_DOWN))
    {
        m_camera->Pitch() += -0.02f;
    }
    if (_window->isKeyPressed(GLFW_KEY_LEFT))
    {
        m_camera->Swivel() += -0.02f;
    }
    if (_window->isKeyPressed(GLFW_KEY_RIGHT))
    {
        m_camera->Swivel() += 0.02f;
    }

    if (_window->isKeyPressed(90))
    {
        m_camera->Distance() += -0.01f;
    }
    if (_window->isKeyPressed(88))
    {
        m_camera->Distance() += 0.01f;
    }

    MovePlayer(m_movement);
}

void Player::MovePlayer(const Vec3& movement)
{
    // TODO: Consider rotation of the camera into the movement direction
    m_camera->GetTranslation().x += movement.x;
    m_body->GetTranslation().x += movement.x;
    m_camera->GetTranslation().z += movement.z;
    m_body->GetTranslation().z += movement.z;
}

Player::~Player()
{
    delete m_body;
    delete m_camera;
}