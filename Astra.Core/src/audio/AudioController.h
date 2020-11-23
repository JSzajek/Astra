#pragma once

#include <vector>

#include <AL/al.h>
#include <AL/alext.h>

#include "../utils/vendor/dr_lib/dr_mp3.h"
#include "../utils/vendor/dr_lib/dr_wav.h"

#include "../math/Vec3.h"

namespace Astra::Audio
{
	struct WavData
	{
		unsigned int channels;
		unsigned int sampleRate;
		drwav_uint64 totalPCMFrameCount;
		std::vector<uint16_t> pcmData;
		
		WavData()
			: channels(0), sampleRate(0), totalPCMFrameCount(0)
		{
		}

		WavData(const WavData& other)
			: channels (other.channels), sampleRate(other.sampleRate), totalPCMFrameCount(other.totalPCMFrameCount)
		{
			pcmData.resize(getTotalSamples());
			std::memcpy(pcmData.data(), other.pcmData.data(), getTotalSamples() * sizeof(uint16_t));
		}

		inline drwav_uint64 getTotalSamples() const { return totalPCMFrameCount * channels; }
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
		ALuint LoadSoundImpl(const char* filepath);
	private:
		void InitializeListener();
		WavData* LoadWavFile(const char* filepath);
	};
}