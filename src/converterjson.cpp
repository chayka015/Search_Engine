#include "../include/converterjson.h"

using namespace std;

vector<string> ConverterJSON::GetTextDocuments() {
    nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

    vector<string> docFilePathList, docTextList;

    for (const auto& it : configJsonDic["files"]) {
        ifstream docFile("../" + string(it));
        if (!docFile.is_open()) {
            cout << "Wrong Path " << "../" + string(it) << " or file doesn't exist!" << endl;
            break;
        } else {
            string text;
            while (getline(docFile, text));
            docFile.close();
            docTextList.push_back(text);
        }
    }

    return docTextList;
}

int ConverterJSON::GetResponsesLimit() {
    nlohmann::json configJsonDic = getDicFromJsonFile("config.json");

    return configJsonDic["config"]["max_responses"];
}

vector<string> ConverterJSON::GetRequests() {

    nlohmann::json requestsInJsonDic;

    vector<string> requestsList;

    requestsInJsonDic = getDicFromJsonFile("requests.json");

    for (const auto& it : requestsInJsonDic["requests"]) {
        requestsList.push_back(it);
    }

    return requestsList;
}

void ConverterJSON::putAnswers(const vector<vector<pair<size_t, float>>>& _answers) {

    string answersStr = "{\"answers\":{";
    for (int i = 0; i < _answers.size(); ++i) {
        float count = 0;
        for (auto &it: _answers[i]) {
            if (it.second > count) {
                count += it.second;
                break;
            }
        }
        if (count == 0) {
            answersStr += "\"request00" + to_string(i) + "\":{\"result\":\"false\"}";
        } else {
            answersStr += "\"request00" + to_string(i) + "\":{\"result\":\"true\",";
            answersStr += "\"relevance\":[";
            for (int j = 0; j < _answers[i].size(); ++j) {
                answersStr += "{\"docid\":" + to_string(_answers[i][j].first) + ",\"rank\":" + to_string(_answers[i][j].second) + "}";
                if (j < _answers[i].size() - 1)
                    answersStr += ",";
            }
            answersStr += "]";
        }
        if (i < _answers.size() - 1) {
            answersStr += "},";
        } else
            answersStr += "}";
    }
    answersStr += "}}";

    cout << answersStr << endl;

    putDicToJsonFile("answers.json", nlohmann::json::parse(answersStr));
}
