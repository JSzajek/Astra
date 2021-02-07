#pragma once

#include "Mat4.h"
#include "../graphics/guis/Gui.h"
#include "../graphics/entities/Spatial.h"
#include "../graphics/entities/Camera.h"

namespace Astra::Math
{
	class Mat4Utils
	{
	public:
		Mat4Utils (const Mat4Utils&) = delete;
		void operator=(const Mat4Utils&) = delete;

		static Mat4Utils& Get()
		{
			static Mat4Utils instance;
			return instance;
		}

		static Mat4 Transformation(const Graphics::Gui& gui) 
		{
			return Get().TransformationImpl(gui.Position, gui.Rotation, gui.GetSize() * gui.Scale);
		}

		static Mat4 Transformation(const Math::Vec2& position, float rotation, const Math::Vec2& scale)
		{
			return Get().TransformationImpl(position, rotation, scale);
		}

		static Mat4 Transformation(const Graphics::Spatial* entity)
		{
			return Get().TransformationImpl(entity->GetTranslation(), entity->GetRotation(), entity->GetScale());
		}

		static Mat4 ViewMatrix(const Graphics::Camera* camera)
		{
			return Get().ViewMatrixImpl(camera);
		}

	private:
		Mat4Utils() { }

		Mat4 TransformationImpl(const Vec3& translation, const Vec3& rotation, const Vec3& scale)
		{
			Mat4 result(1);
			result = result.Translate(translation);
			result = result.Rotate(rotation.x, Vec3::X_Axis);
			result = result.Rotate(rotation.y, Vec3::Y_Axis);
			result = result.Rotate(rotation.z, Vec3::Z_Axis);
			result = result.Scale(scale);
			return result;
		}
		
		Mat4 TransformationImpl(const Vec2& translation, float rotation, const Vec2& scale)
		{
			Mat4 result(1);
			result = result.Translate(Vec3(translation, 0));
			result = result.Rotate(rotation, Vec3::Z_Axis);
			result = result.Scale(Vec3(scale, 1));
			return result;
		}

		Mat4 ViewMatrixImpl(const Graphics::Camera* camera)
		{
			Mat4 result(1);
			result.Rotate(camera->GetPitch(), Vec3::X_Axis);
			result.Rotate(camera->GetYaw(), Vec3::Y_Axis);
			result.Translate(camera->GetInvertedPosition());
			return result;
		}
	};
}