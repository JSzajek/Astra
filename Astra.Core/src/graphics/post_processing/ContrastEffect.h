#pragma once

#include "ImageEffect.h"
#include "../shaders/ContrastShader.h"

namespace Astra::Graphics
{
	class ContrastEffect : public ImageEffect
	{
	public:
		ContrastEffect()
			: ImageEffect(new ContrastShader())
		{
		}
	};
}