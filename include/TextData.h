#ifndef TEXT_DATA_H
#define TEXT_DATA_H

#include <vector>
#include <string>
#include<unordered_map>	


enum class TextType {
	LOREM_IPSUM,
	RANDOM_WORDS,
	MARKOV_CHAIN,
	TEMPLATE_BASED
};

class TextData {
public:
	TextData(int numSamples, int wordsPerSample);

	void setTextType(TextType type);
	void setTemplates(const std::vector<std::string>& templates);
	void setWordList(const std::vector<std::string>& wordList);
	void generate();
	void exportToFile(const std::string& fileName) const;
	std::vector<std::string> getTextSamples() const;

private:
    int numSamples;
    int wordsPerSample;
    TextType textType;
    std::vector<std::string> templates;
    std::vector<std::string> wordList;
    std::vector<std::string> textSamples;

    std::string generateLoremIpsum();
    std::string generateRandomWords();
    std::string generateMarkovChain() ;
    std::string generateTemplateBasedText();

    static const std::vector<std::string> loremIpsumWords;
    static const std::vector<std::string> defaultWordList;
    static const std::vector<std::string> defaultTemplates;

};

#endif // !TEXT_DATA_H
