#pragma once

#include "../math/Vec3.h"

namespace Astra::Audio
{
	class AudioSource
	{
	private:
		unsigned int m_id;
	public:
		AudioSource();
		void Play(unsigned int buffer);
		void Delete();

		void Pause();
		void Continue();
		void Stop();

		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetLooping(bool loop);
		void SetPosition(const Math::Vec3& position);
		void SetVelocity(const Math::Vec3& velocity);

		bool GetLooping() const;
	};
}