#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H

#pragma once

#include <string>
#include <vector>
#include <map>
#include "converterjson.h"

using namespace std;

struct Entry {
	size_t docId, count;
	bool operator ==(const Entry& other) const {
		return (docId == other.docId &&
				count == other.count);
	}
};

class InvertedIndex {
public:
	InvertedIndex() = default;
	InvertedIndex(const InvertedIndex& other) {
		this->docs.clear();
		this->docs.shrink_to_fit();
		this->docs = other.docs;

		this->freqDictionary.clear();
		this->freqDictionary = other.freqDictionary;
	}
	InvertedIndex& operator=(const InvertedIndex& other) {
		if (this == &other)
			return *this;
		this->docs.clear();
		this->docs.shrink_to_fit();
		this->docs = other.docs;

		this->freqDictionary.clear();
		this->freqDictionary = other.freqDictionary;
		return  *this;
	}
	~InvertedIndex() = default;

	size_t GetDocsSize() {
		return this->docs.size();
	}

	string replaceString(const string& subject, const string &search);

	vector<string> convertTextToUniqWords(const string& _text);

	void UpdateDocumentBase(const vector<string>& _inputDocs);

	vector<Entry> GetWordCount(const string& _word);

private:

	vector<string> docs;
	map<string, vector<Entry>> freqDictionary;
};

#endif //SEARCH_ENGINE_INVERTEDINDEX_H
