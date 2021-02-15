#include "astra_pch.h"

#include "AudioSource.h"
#include <AL/al.h>

namespace Astra::Audio
{
	AudioSource::AudioSource(float rollOffFactor, float refDist, float maxDist)
	{
		alGenSources(1, &m_id);
		SetRollOffFactor(rollOffFactor);
		SetReferenceDistance(refDist);
		SetMaxDistance(maxDist);
	}

	AudioSource::~AudioSource()
	{
		Stop();
		alDeleteSources(1, &m_id);
	}

	void AudioSource::SetRollOffFactor(float factor)
	{
		alSourcef(m_id, AL_ROLLOFF_FACTOR, factor);
	}

	void AudioSource::SetReferenceDistance(float distance)
	{
		alSourcef(m_id, AL_REFERENCE_DISTANCE, distance);
	}
	
	void AudioSource::SetMaxDistance(float distance)
	{
		alSourcef(m_id, AL_MAX_DISTANCE, distance);
	}

	void AudioSource::Play(unsigned int buffer)
	{
		Stop();
		alSourcei(m_id, AL_BUFFER, buffer);
		Continue();
	}

	void AudioSource::Pause()
	{
		alSourcePause(m_id);
	}

	void AudioSource::Continue()
	{
		alSourcePlay(m_id);
	}

	void AudioSource::Stop()
	{
		alSourceStop(m_id);
	}

	void AudioSource::SetVolume(float volume)
	{
		alSourcef(m_id, AL_GAIN, volume);
	}

	void AudioSource::SetPitch(float pitch)
	{
		alSourcef(m_id, AL_PITCH, pitch);
	}

	void AudioSource::SetLooping(bool loop)
	{
		alSourcei(m_id, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
	}

	bool AudioSource::GetLooping() const
	{
		ALint state;
		alGetSourcei(m_id, AL_SOURCE_STATE, &state);
		return state == AL_PLAYING;
	}

	void AudioSource::SetPosition(const Math::Vec3& position)
	{
		alSource3f(m_id, AL_POSITION, position.x, position.y, position.z);
	}

	void AudioSource::SetVelocity(const Math::Vec3& velocity)
	{
		alSource3f(m_id, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
	}
}