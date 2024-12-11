#include "../include/invertedIndex.h"
#include <iostream>
#include <regex>
#include <mutex>

using namespace std;


vector<string> InvertedIndex::convertTextToUniqWords(const string& _text) {
    vector<string> words;

	if (_text.empty())
		cout << "Text is EMPTY!" << endl;
	else {
		regex regexWordRule{R"((\w+))"};

		string textTemp = _text;

		auto wordBegin = sregex_iterator(textTemp.begin(),
											  textTemp.end(),
											  regexWordRule);

		auto wordEnd = sregex_iterator();

		for (auto &it = wordBegin; it != wordEnd; ++it) {
			words.push_back(it->str());
		}
	}

	return words;
}

size_t getWordCountInString(const string& _text, const string& _regexStr) {
	string regexStr = "(\\b(" + _regexStr + "))([^,. ]*)";
	regex re(regexStr);
	auto wordBegin = std::sregex_iterator(_text.begin(), _text.end(), re);
	auto wordEnd = std::sregex_iterator();
	return distance(wordBegin, wordEnd);
}

void InvertedIndex::UpdateDocumentBase(const vector<string>& _inputDocs) {
	if (!_inputDocs.empty()) {
		vector<vector<string>> docsByWords;
		for (const auto & inputDoc : _inputDocs) {
			bool isAvailable = false;
			if (!docs.empty()) {
				for (const auto &doc: docs) {
					if (inputDoc == doc) {
						isAvailable = true;
						break;
					}
				}
				if (!isAvailable) {
					docs.push_back(inputDoc);
					docsByWords.push_back(convertTextToUniqWords(inputDoc));
				}
			} else {
				docs.push_back(inputDoc);
				docsByWords.push_back(convertTextToUniqWords(inputDoc));
			}
		}
		for (auto &words: docsByWords) {
			for (auto &word: words) {
				this->freqDictionary.emplace(word, GetWordCount(word));
			}
		}
	} else {
		cout << "File InputDocs is EMPTY!!!" << endl;
	}
}

mutex mtxIdx;
vector<Entry> InvertedIndex::GetWordCount(const string& _word) {
	lock_guard<mutex> lg(mtxIdx);
	vector<Entry> tempEntryVector;
	if (!_word.empty()) {
		if (auto search = freqDictionary.find(_word); search != freqDictionary.end()) {
			return search->second;
		} else {
			for (size_t i = 0; i < this->docs.size(); ++i) {
				size_t countTemp = getWordCountInString(docs[i], _word);
				if (countTemp != 0) {
					tempEntryVector.emplace_back(i,countTemp);
				}
			}
		}
	} else {
		cout << "Input _word is EMPTY!" << endl;
	}
	return tempEntryVector;
}