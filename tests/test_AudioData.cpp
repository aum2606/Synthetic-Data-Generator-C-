#include "AudioData.h"
#include <iostream>
#include <cassert>
#include <filesystem>

namespace fs = std::filesystem;

void testAudioDataGeneration() {
    std::cout << "Testing AudioData generation..." << std::endl;

    // Test sine wave audio generation
    AudioData audio1(3, 44100, 2);
    audio1.setAudioType(AudioType::SINE_WAVE);
    audio1.generate();
    auto samples1 = audio1.getAudioSamples();

    assert(samples1.size() == 3);
    assert(samples1[0].sampleRate == 44100);
    assert(samples1[0].data.size() == 44100 * 2);  // 2 seconds of audio

    // Test white noise audio generation
    AudioData audio2(2, 22050, 1);
    audio2.setAudioType(AudioType::WHITE_NOISE);
    audio2.setNumChannels(2);
    audio2.generate();
    auto samples2 = audio2.getAudioSamples();

    assert(samples2.size() == 2);
    assert(samples2[0].sampleRate == 22050);
    assert(samples2[0].numChannels == 2);
    assert(samples2[0].data.size() == 22050 * 1 * 2);  // 1 second of stereo audio

    // Test export to directory
    std::string outputDir = "test_audio";
    audio1.exportToDirectory(outputDir);
    assert(fs::exists(outputDir));
    assert(fs::is_directory(outputDir));

    // Count files in the output directory
    int fileCount = 0;
    for (const auto& entry : fs::directory_iterator(outputDir)) {
        if (fs::is_regular_file(entry)) {
            fileCount++;
        }
    }
    assert(fileCount == 3);

    // Clean up test directory
    fs::remove_all(outputDir);

    std::cout << "AudioData tests passed!" << std::endl;
}

int main() {
    testAudioDataGeneration();
    return 0;
}