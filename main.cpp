#include "lib.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

int main() {
    std::vector<unsigned char*> freeCunks;
    freeCunks.resize(128);
    std::generate(freeCunks.begin(), freeCunks.end(), []() {
        return new unsigned char[CHUNKSIZE];
    });

    auto pushFreeCunks = [&]() {
        while(!freeCunks.empty()) {
            pushChunk(freeCunks.back());
            freeCunks.pop_back();
        }
    };

    start();

    while(true) {
        pushFreeCunks();
        auto currentChunk = popChunk();
        if(currentChunk) {
            std::cout << "got new chunk\n";
            //TODO do somthing with the data
            freeCunks.push_back(currentChunk);
        }
    }

    stop();

    return 0;
}
