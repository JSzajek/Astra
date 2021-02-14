#include "astra_pch.h"

#include "LayerStack.h"

namespace Astra
{
	LayerStack::~LayerStack()
	{
		for (auto* layer : m_layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.begin() + m_layerIndex, layer);
		m_layerIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iter = std::find(m_layers.begin(), m_layers.begin() + m_layerIndex, layer);
		if (iter != m_layers.begin() + m_layerIndex)
		{
			layer->OnDetach();
			m_layers.erase(iter);
			m_layerIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		auto it = std::find(m_layers.begin() + m_layerIndex, m_layers.end(), overlay);
		if (it != m_layers.end())
		{
			overlay->OnDetach();
			m_layers.erase(it);
		}
	}
}