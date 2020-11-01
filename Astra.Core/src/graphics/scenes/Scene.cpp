#include "Scene.h"

namespace Astra::Graphics
{
	Scene::Scene(Camera* main, const Math::Vec3& fogColor)
		: m_editing(false), m_mainCamera(main), m_fogColor(fogColor), m_skybox(NULL)
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::Start()
	{
		// Start Scene Creation
		m_editing = true;
	}

	void Scene::End()
	{
		// End Scene Creation and Push to Render Loop
		m_editing = false;
		
	}

	void Scene::AddPointLight(const PointLight* light)
	{
		m_pointlights.push_back(light);
	}

	void Scene::AddSpotLight(const SpotLight* light)
	{
		m_spotLight = light;
	}
}