#ifndef AUDIO_DATA_H
#define AUDIO_DATA_H

#include <vector>
#include<string>

enum class AudioType {
	SINE_WAVE,
	WHITE_NOISE,
	PINK_NOISE,
	CHIRP,
	COMBINED
};

struct AudioSample {
	std::vector<float> data;
	int sampleRate;
	int numChannels;
};

class AudioData {
public:
	AudioData(int numSamples, int sampleRate, int durationSeconds);

	void setAudioType(AudioType type);
	void setNumChannels(int channels);
	void generate();
	void exportToDirectory(const std::string& directory) const;

	std::vector<AudioSample> getAudioSamples() const;

private:
	int numSamples;
	int sampleRate;
	int durationSeconds;
	AudioType audioType;
	int numChannels;
	std::vector<AudioSample> audioSamples;
	std::vector<float> generateSineWave();
	std::vector<float> generateWhiteNoise();
	std::vector<float> generatePinkNoise();
	std::vector<float> generateChirp();
	std::vector<float> generateCombined();

	void writeWAVFile(const std::string& filename, const AudioSample& sample) const;
};

#endif // AUDIO_DATA_H