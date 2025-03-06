#include "TextData.h"
#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <string>

namespace fs = std::filesystem;

void testTextDataGeneration() {
    std::cout << "Testing TextData generation..." << std::endl;

    // Test Lorem Ipsum text generation
    TextData text1(5, 50);
    text1.setTextType(TextType::LOREM_IPSUM);
    text1.generate();
    auto samples1 = text1.getTextSamples();

    assert(samples1.size() == 5);

    // Test random words text generation
    TextData text2(3, 20);
    text2.setTextType(TextType::RANDOM_WORDS);
    text2.generate();
    auto samples2 = text2.getTextSamples();

    assert(samples2.size() == 3);

    // Test Markov chain text generation
    TextData text3(2, 30);
    text3.setTextType(TextType::MARKOV_CHAIN);
    text3.generate();
    auto samples3 = text3.getTextSamples();

    assert(samples3.size() == 2);

    // Test template-based text generation
    TextData text4(4, 10);
    text4.setTextType(TextType::TEMPLATE_BASED);

    std::vector<std::string> templates = {
    "The $ADJECTIVE $NOUN $VERB quickly.",
    "$NOUN $VERB $ADJECTIVE today."
    };

    text4.setTemplates(templates);
    text4.generate();
    auto samples4 = text4.getTextSamples();

    assert(samples4.size() == 4);

    // Test export to file
    std::string outputFile = "test_text.txt";
    text1.exportToFile(outputFile);
    assert(fs::exists(outputFile));

    // Check file content
    std::ifstream file(outputFile);
    assert(file.is_open());

    std::string line;
    int lineCount = 0;
    while (std::getline(file, line)) {
        lineCount++;
    }

    // Each sample followed by an empty line, so 5 samples = 10 lines
    assert(lineCount >= 5);

    // Clean up test file
    file.close();
    fs::remove(outputFile);

    std::cout << "TextData tests passed!" << std::endl;
}

int main() {
    testTextDataGeneration();
    return 0;
}