#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H

#pragma once

#include "converterjson.h"
#include "invertedIndex.h"

using namespace std;

class SearchServer {
public:
    explicit SearchServer(InvertedIndex& _idx) : idx(_idx) {};  // Конструктор с передачей индекса
    SearchServer(const SearchServer& other) { idx = other.idx; };  // Конструктор копирования
    SearchServer& operator=(const SearchServer& other) {  // Оператор присваивания
        if (this == &other)
            return *this;
        idx = other.idx;
        return *this;
    };
    ~SearchServer() = default;  // Деструктор

    vector<vector<pair<size_t, float>>> search(const vector<string>& queriesInput);  // Основной метод поиска

private:
    InvertedIndex idx;  // Индекс

    // Объявления вспомогательных методов
    vector<pair<string, vector<Entry>>> getFrequencyVec(const vector<string>& queryWords);
    vector<pair<size_t, size_t>> calculateRelevanceAbs(const vector<pair<string, vector<Entry>>>& freqVec, size_t& relevanceAbsMax);
    vector<pair<size_t, float>> normalizeRelevance(const vector<pair<size_t, size_t>>& relevanceAbsVec, size_t relevanceAbsMax);
    void sortByRelevance(vector<pair<size_t, float>>& indexVec);
    vector<pair<size_t, float>> getTopRelativeIndexes(const vector<pair<size_t, float>>& itVec, int respLimit);
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
