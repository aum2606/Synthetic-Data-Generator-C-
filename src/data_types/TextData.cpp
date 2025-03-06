#include "TextData.h"
#include "RandomGenerators.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <unordered_map>
#include <cctype>

// Static member initialization
const std::vector<std::string> TextData::loremIpsumWords = {
    "lorem", "ipsum", "dolor", "sit", "amet", "consectetur", "adipiscing", "elit", "sed", "do",
    "eiusmod", "tempor", "incididunt", "ut", "labore", "et", "dolore", "magna", "aliqua", "ut",
    "enim", "ad", "minim", "veniam", "quis", "nostrud", "exercitation", "ullamco", "laboris", "nisi",
    "ut", "aliquip", "ex", "ea", "commodo", "consequat", "duis", "aute", "irure", "dolor", "in",
    "reprehenderit", "in", "voluptate", "velit", "esse", "cillum", "dolore", "eu", "fugiat", "nulla",
    "pariatur", "excepteur", "sint", "occaecat", "cupidatat", "non", "proident", "sunt", "in", "culpa",
    "qui", "officia", "deserunt", "mollit", "anim", "id", "est", "laborum"
};

const std::vector<std::string> TextData::defaultWordList = {
    "the", "be", "to", "of", "and", "a", "in", "that", "have", "I",
    "it", "for", "not", "on", "with", "he", "as", "you", "do", "at",
    "this", "but", "his", "by", "from", "they", "we", "say", "her", "she",
    "or", "an", "will", "my", "one", "all", "would", "there", "their", "what",
    "so", "up", "out", "if", "about", "who", "get", "which", "go", "me",
    "when", "make", "can", "like", "time", "no", "just", "him", "know", "take",
    "people", "into", "year", "your", "good", "some", "could", "them", "see", "other",
    "than", "then", "now", "look", "only", "come", "its", "over", "think", "also",
    "back", "after", "use", "two", "how", "our", "work", "first", "well", "way",
    "even", "new", "want", "because", "any", "these", "give", "day", "most", "us"
};

const std::vector<std::string> TextData::defaultTemplates = {
    "The $ADJECTIVE $NOUN $VERB the $NOUN.",
    "$NOUN $VERB $ADJECTIVE $NOUN.",
    "If $NOUN $VERB, then $NOUN will $VERB.",
    "$ADJECTIVE $NOUN $VERB $ADJECTIVE $NOUN.",
    "The $NOUN is $ADJECTIVE and $ADJECTIVE."
};

TextData::TextData(int numSamples, int wordsPerSample)
    : numSamples(numSamples), wordsPerSample(wordsPerSample), textType(TextType::LOREM_IPSUM) {
    wordList = defaultWordList;
    templates = defaultTemplates;
}

void TextData::setTextType(TextType type) {
	textType = type;
}

void TextData::setTemplates(const std::vector<std::string>& templates) {
	this->templates = templates;
}

void TextData::setWordList(const std::vector<std::string>& words) {
	wordList = words;
}

void TextData::generate() {
    textSamples.clear();
    for (int i = 0;i < numSamples;++i) {
        std::string sample;
        switch (textType) {
        case TextType::LOREM_IPSUM:
            sample = generateLoremIpsum();
            break;
        case TextType::RANDOM_WORDS:
            sample = generateRandomWords();
            break;
        case TextType::MARKOV_CHAIN:
            sample = generateMarkovChain();
            break;
        case TextType::TEMPLATE_BASED:
            sample = generateTemplateBasedText();
            break;
        }
        textSamples.push_back(sample);
    }
}

std::string TextData::generateLoremIpsum() {
    std::ostringstream oss;

    for (int i = 0;i < wordsPerSample;++i) {
        int wordIndex = RandomGenerators::getRandomInt(0, loremIpsumWords.size() - 1);
        oss << loremIpsumWords[wordIndex];

        //add punctuation occasionally
        if (i > 0 && i % 10 == 0) {
            oss << ".";
        }
        //add space if not last word
        if (i < wordsPerSample - 1) {
            oss << " ";
        }
    }

    //Ensure the text ends with a period
    if (oss.str().back() != '.') {
        oss << ".";
    }

    //capitalize the first letter
    std::string result = oss.str();
    if (!result.empty()) {
        result[0] = std::toupper(result[0]);
    }
    return result;
}

std::string TextData::generateRandomWords() {
    std::ostringstream oss;
    for (int i = 0;i < wordsPerSample;++i) {
        int wordIndex = RandomGenerators::getRandomInt(0, wordList.size() - 1);
        oss << wordList[wordIndex];

        //add punctutaion occasionally
		if (i > 0 && i % 8 == 0) {
			oss << ".";
		}
        else if (i > 0 && i % 15 == 0) {
			oss << "?";
		}
        else if (i > 0 && i % 20 == 0) {
			oss << "!";
        }

        //add space if not last word
        if (i < wordsPerSample - 1) {
			oss << " ";
        }

    }

    //ensure the text ends with a period
    if (oss.str().back() != '.' && oss.str().back() != '?' && oss.str().back() != '!') {
        oss << ".";
    }

	//capitalize the first letter
    std::string result = oss.str();
    if (!result.empty()) {
		result[0] = std::toupper(result[0]);
    }
    return result;
}

std::string TextData::generateMarkovChain() {
    // Build a simple markov chain from the word list
	std::unordered_map<std::string, std::vector<std::string>> transitions;

    //build transition from the word list
    for (size_t i = 0;i < wordList.size() - 1;++i) {
		transitions[wordList[i]].push_back(wordList[i + 1]);
    }

    //generate text using the markov chain
    std::ostringstream oss;

    //start with a random word
	std::string currentWord = wordList[RandomGenerators::getRandomInt(0, wordList.size() - 1)];
    oss << currentWord;
    for(int i=1;i<wordsPerSample;++i){
        // If the current word has no transitions, pick a random word
        if (transitions.find(currentWord) == transitions.end() || transitions[currentWord].empty()) {
            currentWord = wordList[RandomGenerators::getRandomInt(0, wordList.size() - 1)];
        }
        else {
            // Otherwise, pick a random transition
            const auto& nextWords = transitions[currentWord];
            currentWord = nextWords[RandomGenerators::getRandomInt(0, nextWords.size() - 1)];
        }

        //add punctuation occasionally
        if (i > 0 && i % 8 == 0) {
            oss << ".";
			//capitalize the next letter
            if (!currentWord.empty()) {
				currentWord[0] = std::toupper(currentWord[0]);
            }
        }
        else if (i > 0 && i % 15 == 0) {
			oss << "?";
			//capitalize the next word
            if (!currentWord.empty()) {
				currentWord[0] = std::toupper(currentWord[0]);
			}
        }
		oss << " " << currentWord;
    }
    //ensure the text ends with a period
	if (oss.str().back() != '.' && oss.str().back() != '?' && oss.str().back() != '!') {
		oss << ".";
	}

    //capitalize the first letter
    std::string result = oss.str();
    if (!result.empty()) {
		result[0] = std::toupper(result[0]);
    }
    return result;
}

std::string TextData::generateTemplateBasedText() {
    //select a random templates
    std::string templateStr = templates[RandomGenerators::getRandomInt(0, templates.size() - 1)];

    //define word categories
    std::vector<std::string> nouns = { "time", "person", "year", "way", "day", "thing", "man", "world", "life", "hand", "part", "child", "eye", "woman", "place", "work", "week", "case", "point", "government" };
    std::vector<std::string> verbs = { "is", "are", "was", "were", "has", "have", "had", "can", "could", "will", "would", "should", "may", "might", "must", "shall", "do", "does", "did", "make", "makes", "made" };
    std::vector<std::string> adjectives = { "good", "new", "first", "last", "long", "great", "little", "own", "other", "old", "right", "big", "high", "different", "small", "large", "next", "early", "young", "important" };

    //replace placeholders in the template
    size_t pos;
    std::string result = templateStr;

    while ((pos = result.find("$NOUN")) != std::string::npos) {
        result.replace(pos, 5, nouns[RandomGenerators::getRandomInt(0, nouns.size() - 1)]);
    }

    while ((pos = result.find("$VERB")) != std::string::npos) {
        result.replace(pos, 5, verbs[RandomGenerators::getRandomInt(0, verbs.size() - 1)]);
    }

    while ((pos = result.find("$ADJECTIVE")) != std::string::npos) {
        result.replace(pos, 10, adjectives[RandomGenerators::getRandomInt(0, adjectives.size() - 1)]);
    }

    return result;
}

void TextData::exportToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file for writing: " + filename);
    }

    for (const auto& sample : textSamples) {
        file << sample << "\n\n";
    }

    file.close();
}

std::vector<std::string> TextData::getTextSamples() const {
    return textSamples;
}