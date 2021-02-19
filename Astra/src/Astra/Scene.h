#pragma once

#include <vector>

#include "Astra/Application.h"

#include "Astra/layers/Layer3D.h"
#include "Astra/layers/Layer2D.h"

namespace Astra
{
	class Scene
	{
	private:
		Layer3D* m_3dLayer;
		//Layer2D* m_sprites;
		Layer2D* m_canvas;
	public:
		Scene()
		{
			m_3dLayer = new Layer3D();
			m_canvas = new Layer2D();
		}

		~Scene()
		{
			delete m_3dLayer;
			delete m_canvas;
		}

		void Enable() 
		{
			Application::Get().PushLayer(m_3dLayer);
			Application::Get().PushLayer(m_canvas);
		}

		void Disable()
		{
			Application::Get().PopLayer(m_3dLayer);
			Application::Get().PopLayer(m_canvas);
		}

		inline void SetMainCamera(Graphics::Camera* camera) { m_3dLayer->SetCamera(camera); }
		inline void SetDirectionalLight(Graphics::DirectionalLight* light) { m_3dLayer->SetDirectionalLight(light); }
		inline void SetSkyBox(const Graphics::SkyboxMaterial* material) { m_3dLayer->SetSkyBox(material); }
		inline void SetFogColor(const Graphics::Color& color) { m_3dLayer->SetFogColor(color); }

		inline void AddEntity(const Graphics::Entity* entity) { m_3dLayer->AddEntity(entity); }
		inline void AddParticleSystem(const Graphics::ParticleSystem* system) { m_3dLayer->AddParticleSystem(system); }
		inline void AddTerrain(const Graphics::Terrain* terrain) { m_3dLayer->AddTerrain(terrain); }
		inline void AddWaterTile(const Graphics::WaterTile* tile) { m_3dLayer->AddWaterTile(tile); }
		
		inline void AddPointLight(Graphics::PointLight* light) { m_3dLayer->AddPointLight(light); }
		
		inline void AddGui(Graphics::Gui* gui, int layer) { m_canvas->AddGui(gui, layer); }

	#if ASTRA_DEBUG
		void ToggleWireframeMode() 
		{
			static unsigned char wireframe = 1;
			m_3dLayer->ToggleWireframeMode(wireframe); 
			wireframe = (wireframe + 1) % 3;
		}
	#endif

		inline void SetMultisampling(unsigned int sampleSize)
		{
			if (m_3dLayer)
				m_3dLayer->SetMultisampling(sampleSize);
		}

		inline void SetPostProcessing(bool enabled)
		{
			if (m_3dLayer)
				m_3dLayer->SetPostProcessing(enabled);
		}
	};
}