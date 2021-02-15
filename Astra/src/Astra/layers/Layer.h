#pragma once

#include "Astra/Core.h"
#include "Astra/events/Event.h"

namespace Astra
{
	class ASTRA_API Layer
	{
	protected:
		std::string m_debugName;
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() { }
		virtual void OnDetach() { }
		virtual void OnUpdate(float delta) { }
		virtual void OnEvent(Event& _event) { }
		virtual void UpdateScreen(unsigned int width, unsigned int height) { }

		inline const std::string& GetName() const { return m_debugName; }
	};
}