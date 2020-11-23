#include "AudioController.h"

#include "../logger/Logger.h"

namespace Astra::Audio
{
	AudioController::AudioController()
		: m_device(NULL), m_context(NULL)
	{
		m_device = alcOpenDevice(NULL);
		if (!m_device)
		{
			Logger::LogError("OpenAL: could not open device");
			return;
		}

		m_context = alcCreateContext(m_device, NULL);
		if (!m_context)
		{
			alcCloseDevice(m_device);
			Logger::LogError("OpenAL: could not create context");
			return;
		}
		if (!alcMakeContextCurrent(m_context))
		{
			alcCloseDevice(m_device);
			Logger::LogError("OpenAL: could not set context");
			return;
		}

		const char* name = NULL;
		if (alcIsExtensionPresent(m_device, "ALC_ENUMERATE_ALL_EXT"))
		{
			name = alcGetString(m_device, ALC_ALL_DEVICES_SPECIFIER);
		}
		if (!name || alcGetError(m_device) != AL_NO_ERROR)
		{
			name = alcGetString(m_device, ALC_DEVICE_SPECIFIER);
		}
		Logger::Log("OpenAL: Opened " + std::string(name));

		InitializeListener();
	}

	AudioController::~AudioController()
	{
		for (auto buffer : m_buffers)
		{
			alDeleteBuffers(1, &buffer);
		}

		if (!alcMakeContextCurrent(NULL))
		{
			Logger::LogError("OpenAL: failed to set context to nullptr");
		}

		alcDestroyContext(m_context);
		if (m_context)
		{
			Logger::LogError("OpenAL: failed to unset context");
		}

		alcCloseDevice(m_device);
		if (m_device)
		{
			Logger::LogError("OpenAL: failed to close device");
		}
	}

	void AudioController::SetListenerPositionImpl(const Math::Vec3& position)
	{
		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void AudioController::InitializeListener()
	{
		SetDistanceModelImpl(DistanceModel::LinearClamped);
		alListener3f(AL_POSITION, 0, 0, 0);
		alListener3f(AL_VELOCITY, 0, 0, 0);
		ALfloat forwardAndUpVectors[] = 
		{
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f
		};
		alListenerfv(AL_ORIENTATION, forwardAndUpVectors);
	}

	void AudioController::SetDistanceModelImpl(DistanceModel model)
	{
		alDistanceModel(static_cast<ALenum>(model));
	}

	ALuint AudioController::LoadSoundImpl(const char* filepath)
	{
		const auto* data = LoadWavFile(filepath);

		ALuint buffer;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, data->channels > 1 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data->pcmData, data->getTotalSamples() * sizeof(uint16_t), data->sampleRate);
		m_buffers.push_back(buffer);
		
		delete data;
		
		return buffer;
	}
	
	WavData* AudioController::LoadWavFile(const char* filepath)
	{
		WavData* data = new WavData();
		drwav_int16* sampleData = drwav_open_file_and_read_pcm_frames_s16(filepath, &data->channels, &data->sampleRate, &data->totalPCMFrameCount, NULL);
		if (!sampleData)
		{
			Logger::LogError("Failed to load audio wav file " + std::string(filepath));
		}
		else
		{
			if (data->getTotalSamples() > drwav_uint64(std::numeric_limits<size_t>::max()))
			{
				Logger::LogError("Loaded audio file is too large for 32-bit address");
			}

			data->pcmData = new uint16_t[data->getTotalSamples()];
			std::memcpy(data->pcmData, sampleData, data->getTotalSamples() * sizeof(uint16_t));
			drwav_free(sampleData, NULL);
		}
		return data;
	}
}