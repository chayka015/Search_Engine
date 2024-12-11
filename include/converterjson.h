#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H

#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;

class ConverterJSON {
public:
	ConverterJSON() = default;
	~ConverterJSON() = default;

	nlohmann::json getDicFromJsonFile(const string& _jsonFilePath) {
		nlohmann::json jsonDic;

		ifstream inJsonFile("../" + _jsonFilePath);

		if (!inJsonFile.is_open()) {
			cout << "File with path \"" << "../" + _jsonFilePath << "\" doesn't exist!" << endl;
			return jsonDic;
		}

		jsonDic = nlohmann::json::parse(inJsonFile);

		inJsonFile.close();

		return jsonDic;
	}

	void putDicToJsonFile(const string& _jsonFilePath, const nlohmann::json& _jsonDic) {

		ofstream outJsonFile("../" + _jsonFilePath, ios::trunc);

		if (!outJsonFile.is_open()) {
			cout << "File with path \"" << "../" + _jsonFilePath << "\" doesn't exist!" << endl;
		} else {

			outJsonFile << _jsonDic;

			outJsonFile.close();
		}
	}

	void UpdateConfigJsonFile(const vector<string>& _docs);

	vector<string> GetTextDocuments();

	int GetResponsesLimit();

	void putRequests(const vector<string>& _requests);

	vector<string> GetRequests();

	void putAnswers(const vector<vector<pair<size_t, float>>>& _answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
