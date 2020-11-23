#pragma once

#include "../math/Vec3.h"

namespace Astra::Audio
{
	class AudioSource
	{
	private:
		unsigned int m_id;
	public:
		AudioSource(float rollOffFactor = 1, float refDist = 1, float maxDist = 1);
		~AudioSource();

		void Play(unsigned int buffer);
		void Pause();
		void Continue();
		void Stop();

		void SetRollOffFactor(float factor);
		void SetReferenceDistance(float distance);
		void SetMaxDistance(float distance);

		void SetVolume(float volume);
		void SetPitch(float pitch);
		void SetLooping(bool loop);
		void SetPosition(const Math::Vec3& position);
		void SetVelocity(const Math::Vec3& velocity);

		bool GetLooping() const;
	};
}