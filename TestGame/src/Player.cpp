#include "Player.h"

Player::Player(Window* window)
	: m_camera(new Camera()), _window(window), m_movement(Vec3(0))
{
	const VertexArray* bodyModel = ObjLoader::LoadObjectModel("res/pylon.obj");
	const Texture* texture = Loader::LoadTexture("res/red.jpg");
	m_body = new Entity(bodyModel, texture, Vec3(0));
}

void Player::Update()
{
    m_movement.x = 0;
    m_movement.y = 0;
    m_movement.z = 0;

    if (_window->isKeyPressed(GLFW_KEY_W))
    {
        m_movement.z += -0.01f;
    }
    if (_window->isKeyPressed(GLFW_KEY_S))
    {
        m_movement.z += 0.01f;
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
    float rotAngle = ToRadians(m_camera->GetYaw());
    float c = cos(rotAngle);
    float s = sin(rotAngle);

    float x = c * movement.x + s * movement.z;
    float z = -s * movement.x + c * movement.z;
    m_camera->GetTranslation().x += x;
    m_body->GetTranslation().x += x;
    m_camera->GetTranslation().z += z;
    m_body->GetTranslation().z += z;
}

Player::~Player()
{
    delete m_body;
    delete m_camera;
}