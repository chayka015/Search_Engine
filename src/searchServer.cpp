#include "../include/searchServer.h"
#include <string>
#include <iostream>
#include <limits>
#include <cmath>
#include <thread>
#include <vector>

using namespace std;

bool isEqualFloat(float x, float y) {
    return (fabs(x - y) <= numeric_limits<float>::epsilon() * fmax(fabs(x), fabs(y)));
}

vector<vector<pair<size_t, float>>> SearchServer::search(const vector<string>& _queriesInput) {

    ConverterJSON converterJson;
    vector<vector<pair<size_t, float>>> relativeIndexVecAllQueries, topRelativeIndexes;

    if (!_queriesInput.empty()) {
        for (const auto& query : _queriesInput) {
            vector<pair<size_t, float>> relativeIndexVec;
            vector<pair<string, vector<Entry>>> freqVec;

            vector<string> queryWords = idx.convertTextToUniqWords(query);  // Прямой доступ к idx
            freqVec = getFrequencyVec(queryWords);  // Получаем частоты для каждого слова

            size_t relevanceAbsMax = 0;
            vector<pair<size_t, size_t>> relevanceAbsVec = calculateRelevanceAbs(freqVec, relevanceAbsMax);  // Вычисляем абсолютную релевантность

            relativeIndexVec = normalizeRelevance(relevanceAbsVec, relevanceAbsMax);  // Нормализуем релевантность
            relativeIndexVecAllQueries.push_back(relativeIndexVec);

            sortByRelevance(relativeIndexVecAllQueries.back());  // Сортируем по релевантности
        }

        converterJson.putAnswers(relativeIndexVecAllQueries);

        for (auto& itVec : relativeIndexVecAllQueries) {
            topRelativeIndexes.push_back(getTopRelativeIndexes(itVec, converterJson.GetResponsesLimit()));  // Постобработка
        }
    } else {
        cout << "_queriesInput file is EMPTY!" << endl;
    }
    return topRelativeIndexes;
}

vector<pair<string, vector<Entry>>> SearchServer::getFrequencyVec(const vector<string>& queryWords) {
    vector<pair<string, vector<Entry>>> freqVec;
    vector<thread> threadVec;

    for (auto& wordIt : queryWords) {
        threadVec.push_back(thread([wordIt, &freqVec, this] {
            vector<Entry> sortedIndexVec = this->idx.GetWordCount(wordIt);  // Прямой доступ к idx
            sort(sortedIndexVec.begin(), sortedIndexVec.end(), [](auto& left, auto& right) { return left.count < right.count; });
            freqVec.emplace_back(wordIt, sortedIndexVec);
        }));
    }

    for (auto& thrd : threadVec) {
        if (thrd.joinable()) thrd.join();
    }

    return freqVec;
}

vector<pair<size_t, size_t>> SearchServer::calculateRelevanceAbs(const vector<pair<string, vector<Entry>>>& freqVec, size_t& relevanceAbsMax) {
    vector<pair<size_t, size_t>> relevanceAbsVec;

    for (int id = 0; id < this->idx.GetDocsSize(); ++id) {  // Прямой доступ к idx
        size_t relevanceAbsolute = 0;
        for (auto& d : freqVec) {
            for (auto& e : d.second) {
                if (e.docId == id) {
                    relevanceAbsolute += e.count;
                    if (relevanceAbsolute > relevanceAbsMax) {
                        relevanceAbsMax = relevanceAbsolute;
                    }
                    break;
                }
            }
        }
        if (relevanceAbsolute > 0)
            relevanceAbsVec.emplace_back(id, relevanceAbsolute);
    }

    return relevanceAbsVec;
}

vector<pair<size_t, float>> SearchServer::normalizeRelevance(const vector<pair<size_t, size_t>>& relevanceAbsVec, size_t relevanceAbsMax) {
    vector<pair<size_t, float>> relativeIndexVec;
    for (auto& it : relevanceAbsVec) {
        relativeIndexVec.emplace_back(it.first, float(it.second) / float(relevanceAbsMax));
    }
    return relativeIndexVec;
}

void SearchServer::sortByRelevance(vector<pair<size_t, float>>& indexVec) {
    for (size_t i = 0; i < indexVec.size(); ++i) {
        for (size_t j = 0; j < indexVec.size() - 1; ++j) {
            if (isless(indexVec[j].second, indexVec[j + 1].second)) {
                swap(indexVec[j], indexVec[j + 1]);
            }
        }
    }
}

vector<pair<size_t, float>> SearchServer::getTopRelativeIndexes(const vector<pair<size_t, float>>& itVec, int respLimit) {
    vector<pair<size_t, float>> tempVec;
    for (int j = 0; (j < itVec.size() && j < respLimit); ++j) {
        if (!isEqualFloat(itVec[j].second, 0))
            tempVec.emplace_back(itVec[j]);
        else
            tempVec.emplace_back();
    }
    return tempVec;
}
