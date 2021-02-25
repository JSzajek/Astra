#pragma once

#include <vector>

#include "Astra/Application.h"
#include "Astra/graphics/Resource.h"

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

		template <class T>
		inline T* Get(std::string name) 
		{
			if (auto* ptr = m_3dLayer->Get(name))
				return reinterpret_cast<T*>(ptr);
			if (auto* ptr = m_canvas->Get(name))
				return reinterpret_cast<T*>(ptr);
			return NULL; 
		}

		inline void SetMainCamera(Graphics::Camera* camera) { m_3dLayer->SetCamera(camera); }
		inline void SetDirectionalLight(const Graphics::DirectionalLight& light) { m_3dLayer->SetDirectionalLight(light); }
		inline void SetSkyBox(const Graphics::SkyboxMaterial& material) { m_3dLayer->SetSkyBox(material); }
		inline void SetFogColor(const Graphics::Color& color) { m_3dLayer->SetFogColor(color); }

		inline Graphics::Model* AddModel(const Graphics::Model& model) { return m_3dLayer->AddModel(model); }
		inline void AddParticleSystem(const Graphics::ParticleSystem& system) { m_3dLayer->AddParticleSystem(system); }
		inline Graphics::Terrain* AddTerrain(const Graphics::Terrain& terrain) { return m_3dLayer->AddTerrain(terrain); }
		inline void AddWaterTile(const Graphics::WaterTile& tile) { m_3dLayer->AddWaterTile(tile); }
		
		inline void AddPointLight(const Graphics::PointLight& light) { m_3dLayer->AddPointLight(light); }
		
		template<class T>
		inline T* Add(const T& gui, int layer) { return m_canvas->Add(gui, layer); }

	#if ASTRA_DEBUG
		void ToggleWireframeMode() 
		{
			static unsigned char wireframe = 1;
			m_3dLayer->ToggleWireframeMode(wireframe); 
			wireframe = (wireframe + 1) % 3;
		}
	#endif

		inline void CheckResources()
		{
			Graphics::Resource::Check();
		}

		inline void SetMultisampling(unsigned int sampleSize)
		{
			if (m_3dLayer)
			{
				m_3dLayer->SetMultisampling(sampleSize);
			}
		}

		inline void SetBloom(bool enabled)
		{
			if (m_3dLayer)
			{
				m_3dLayer->SetBloom(enabled);
			}
		}

		inline void SetHDR(bool enabled)
		{
			if (m_3dLayer)
			{
				m_3dLayer->SetHDR(enabled);
			}
		}

		inline void SetReflections(bool enabled)
		{
			if (m_3dLayer)
			{
				m_3dLayer->SetReflections(enabled);
			}
		}
	};
}