#include "include/searchServer.h"
#include <iostream>



using namespace std;

int main(int argc, char* argv[]) {
    InvertedIndex idx;
    ConverterJSON conv;

    idx.UpdateDocumentBase(conv.GetTextDocuments());

    SearchServer searchServer(idx);

    vector<vector<pair<size_t, float>>> result = searchServer.search(conv.GetRequests());
    cout << "------------------------" << endl;
    for (auto& i : result) {
        for (auto& it : i) {
            cout << "{ docId: " << it.first << ", index: " << it.second << " }" << endl;
        }
        cout << "------------------------" << endl;
    }

    return EXIT_SUCCESS;
}