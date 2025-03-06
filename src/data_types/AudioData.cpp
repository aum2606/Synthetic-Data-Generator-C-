#include "AudioData.h"
#include "RandomGenerators.h"
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <algorithm>
#include <cmath>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
namespace fs = std::filesystem;

AudioData::AudioData(int numSamples, int sampleRate, int durationSeconds)
    : numSamples(numSamples), sampleRate(sampleRate), durationSeconds(durationSeconds),
    numChannels(1), audioType(AudioType::SINE_WAVE) {
}

void AudioData::setAudioType(AudioType type) {
    audioType = type;
}

void AudioData::setNumChannels(int channels) {
    numChannels = channels;
}

void AudioData::generate() {
    audioSamples.clear();

    for (int i = 0; i < numSamples; ++i) {
        AudioSample sample;
        sample.sampleRate = sampleRate;
        sample.numChannels = numChannels;

        std::vector<float> channelData;

        switch (audioType) {
        case AudioType::SINE_WAVE:
            channelData = generateSineWave();
            break;
        case AudioType::WHITE_NOISE:
            channelData = generateWhiteNoise();
            break;
        case AudioType::PINK_NOISE:
            channelData = generatePinkNoise();
            break;
        case AudioType::CHIRP:
            channelData = generateChirp();
            break;
        case AudioType::COMBINED:
            channelData = generateCombined();
            break;
        }

        // Duplicate the channel data for multi-channel audio
        sample.data.resize(channelData.size() * numChannels);

        for (size_t j = 0; j < channelData.size(); ++j) {
            for (int c = 0; c < numChannels; ++c) {
                sample.data[j * numChannels + c] = channelData[j];
            }
        }

        audioSamples.push_back(sample);
    }
}

std::vector<float> AudioData::generateSineWave() {
    int totalSamples = sampleRate * durationSeconds;
    std::vector<float> data(totalSamples);

    // Generate sine wave parameters
    float frequency = RandomGenerators::getRandomFloat(220.0f, 880.0f);  // Random frequency between A3 and A5
    float amplitude = RandomGenerators::getRandomFloat(0.5f, 1.0f);

    // Generate sine wave
    for (int i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        data[i] = amplitude * std::sin(2.0f * M_PI * frequency * t);
    }

    return data;
}

std::vector<float> AudioData::generateWhiteNoise() {
    int totalSamples = sampleRate * durationSeconds;
    std::vector<float> data(totalSamples);

    // Generate white noise parameters
    float amplitude = RandomGenerators::getRandomFloat(0.1f, 0.5f);

    // Generate white noise
    for (int i = 0; i < totalSamples; ++i) {
        data[i] = amplitude * (RandomGenerators::getRandomFloat(-1.0f, 1.0f));
    }

    return data;
}

std::vector<float> AudioData::generatePinkNoise() {
    int totalSamples = sampleRate * durationSeconds;
    std::vector<float> data(totalSamples);

    // Generate pink noise parameters
    float amplitude = RandomGenerators::getRandomFloat(0.1f, 0.5f);

    // Simple approximation of pink noise using filtered white noise
    float b0 = 0.0f, b1 = 0.0f, b2 = 0.0f, b3 = 0.0f, b4 = 0.0f, b5 = 0.0f, b6 = 0.0f;

    for (int i = 0; i < totalSamples; ++i) {
        float white = RandomGenerators::getRandomFloat(-1.0f, 1.0f);

        // Filter white noise to approximate pink noise
        b0 = 0.99886f * b0 + white * 0.0555179f;
        b1 = 0.99332f * b1 + white * 0.0750759f;
        b2 = 0.96900f * b2 + white * 0.1538520f;
        b3 = 0.86650f * b3 + white * 0.3104856f;
        b4 = 0.55000f * b4 + white * 0.5329522f;
        b5 = -0.7616f * b5 - white * 0.0168980f;

        data[i] = amplitude * (b0 + b1 + b2 + b3 + b4 + b5 + b6 + white * 0.5362f);
        data[i] = std::clamp(data[i], -1.0f, 1.0f);
    }

    return data;
}

std::vector<float> AudioData::generateChirp() {
    int totalSamples = sampleRate * durationSeconds;
    std::vector<float> data(totalSamples);

    // Generate chirp parameters
    float startFrequency = RandomGenerators::getRandomFloat(100.0f, 500.0f);
    float endFrequency = RandomGenerators::getRandomFloat(1000.0f, 5000.0f);
    float amplitude = RandomGenerators::getRandomFloat(0.5f, 1.0f);

    // Generate chirp signal (frequency sweep)
    for (int i = 0; i < totalSamples; ++i) {
        float t = static_cast<float>(i) / sampleRate;
        float normalizedTime = t / durationSeconds;
        float instantFrequency = startFrequency + (endFrequency - startFrequency) * normalizedTime;

        // Phase is the integral of frequency
        float phase = 2.0f * M_PI * (startFrequency * t + 0.5f * (endFrequency - startFrequency) * normalizedTime * t);

        data[i] = amplitude * std::sin(phase);
    }

    return data;
}

std::vector<float> AudioData::generateCombined() {
    int totalSamples = sampleRate * durationSeconds;

    // Generate individual components
    std::vector<float> sineWave = generateSineWave();
    std::vector<float> noise = generateWhiteNoise();

    // Mix components
    std::vector<float> combined(totalSamples);
    float sineWeight = RandomGenerators::getRandomFloat(0.3f, 0.7f);
    float noiseWeight = 1.0f - sineWeight;

    for (int i = 0; i < totalSamples; ++i) {
        combined[i] = sineWeight * sineWave[i] + noiseWeight * noise[i];
        combined[i] = std::clamp(combined[i], -1.0f, 1.0f);
    }

    return combined;
}

void AudioData::exportToDirectory(const std::string& directory) const {
    // Create directory if it doesn't exist
    if (!fs::exists(directory)) {
        fs::create_directories(directory);
    }

    // Export audio samples
    for (size_t i = 0; i < audioSamples.size(); ++i) {
        std::string filename = directory + "/audio_" + std::to_string(i + 1) + ".wav";
        writeWAVFile(filename, audioSamples[i]);
    }
}

void AudioData::writeWAVFile(const std::string& filename, const AudioSample& sample) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    // WAV file header
    const int dataSize = sample.data.size() * sizeof(short);
    const int fileSize = 36 + dataSize;

    // RIFF header
    file.write("RIFF", 4);
    file.write(reinterpret_cast<const char*>(&fileSize), 4);
    file.write("WAVE", 4);

    // Format chunk
    file.write("fmt ", 4);
    int fmtSize = 16;
    file.write(reinterpret_cast<const char*>(&fmtSize), 4);
    short audioFormat = 1;  // PCM
    file.write(reinterpret_cast<const char*>(&audioFormat), 2);
    short numChannels = sample.numChannels;
    file.write(reinterpret_cast<const char*>(&numChannels), 2);
    int sampleRate = sample.sampleRate;
    file.write(reinterpret_cast<const char*>(&sampleRate), 4);
    int byteRate = sampleRate * numChannels * sizeof(short);
    file.write(reinterpret_cast<const char*>(&byteRate), 4);
    short blockAlign = numChannels * sizeof(short);
    file.write(reinterpret_cast<const char*>(&blockAlign), 2);
    short bitsPerSample = 16;
    file.write(reinterpret_cast<const char*>(&bitsPerSample), 2);

    // Data chunk
    file.write("data", 4);
    file.write(reinterpret_cast<const char*>(&dataSize), 4);

    // Write audio data
    for (float sample : sample.data) {
        // Convert float [-1.0, 1.0] to short [-32768, 32767]
        short pcmSample = static_cast<short>(sample * 32767.0f);
        file.write(reinterpret_cast<const char*>(&pcmSample), sizeof(short));
    }

    file.close();
}

std::vector<AudioSample> AudioData::getAudioSamples() const {
    return audioSamples;
}