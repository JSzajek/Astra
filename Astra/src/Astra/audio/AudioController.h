#pragma once

#include <vector>

#include <AL/al.h>
#include <AL/alext.h>

#include <dr_lib/dr_mp3.h>
#include <dr_lib/dr_wav.h>

#include "../math/Vec3.h"

namespace Astra::Audio
{
	enum class DistanceModel : ALenum
	{
		Exponent		= AL_EXPONENT_DISTANCE,
		ExponentClamped = AL_EXPONENT_DISTANCE_CLAMPED,
		Inverse			= AL_INVERSE_DISTANCE,
		InverseClamped	= AL_INVERSE_DISTANCE_CLAMPED,
		Linear			= AL_LINEAR_DISTANCE,
		LinearClamped	= AL_LINEAR_DISTANCE_CLAMPED
	};

	struct WavData
	{
		unsigned int channels;
		unsigned int sampleRate;
		drwav_uint64 totalPCMFrameCount;
		uint16_t* pcmData;
		
		WavData()
			: channels(0), sampleRate(0), totalPCMFrameCount(0), pcmData(NULL)
		{
		}

		WavData(const WavData& other)
			: channels (other.channels), sampleRate(other.sampleRate), totalPCMFrameCount(other.totalPCMFrameCount)
		{
			pcmData = new uint16_t[getTotalSamples()];
			std::memcpy(pcmData, other.pcmData, getTotalSamples() * sizeof(uint16_t));
		}

		~WavData()
		{
			delete[] pcmData;
		}

		inline size_t getTotalSamples() const { return static_cast<size_t>(totalPCMFrameCount * channels); }
	};

	class AudioController
	{
	public:
		AudioController(const AudioController&) = delete;
		void operator=(const AudioController&) = delete;

		static AudioController& Get()
		{
			static AudioController instance;
			return instance;
		}

		static void SetListenerPosition(const Math::Vec3& position)
		{
			Get().SetListenerPositionImpl(position);
		}

		static void SetDistanceModel(DistanceModel model)
		{
			Get().SetDistanceModelImpl(model);
		}

		static ALuint LoadSound(const char* filepath)
		{
			return Get().LoadSoundImpl(filepath);
		}

	private:
		std::vector<ALuint> m_buffers;
		ALCdevice* m_device;
		ALCcontext* m_context;
	private:
		AudioController();
		~AudioController();
		void SetListenerPositionImpl(const Math::Vec3& position);
		void SetDistanceModelImpl(DistanceModel model);
		ALuint LoadSoundImpl(const char* filepath);
	private:
		void InitializeListener();
		WavData* LoadWavFile(const char* filepath);
	};
}