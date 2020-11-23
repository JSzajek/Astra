#include "AudioSource.h"

#include <AL/al.h>

namespace Astra::Audio
{
	AudioSource::AudioSource()
	{
		alGenSources(1, &m_id);
	}

	void AudioSource::Play(unsigned int buffer)
	{
		Stop();
		alSourcei(m_id, AL_BUFFER, buffer);
		Continue();
	}

	void AudioSource::Delete()
	{
		Stop();
		alDeleteSources(1, &m_id);
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