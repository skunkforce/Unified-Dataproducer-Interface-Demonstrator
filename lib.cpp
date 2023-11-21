#include "lib.hpp"

#include <thread>
#include <vector>

static std::vector<unsigned char*> freeChunks;
static std::vector<unsigned char*> fullChunks;
static std::jthread                dataGeneratorThread;
static std::mutex                  mutex;

static void fillData(unsigned char*) {
    //TODO fill data
}

static void dataGenerator(std::stop_token stoken) {
    while(!stoken.stop_requested()) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            if(!freeChunks.empty()) {
                auto currentChunk = freeChunks.back();
                freeChunks.pop_back();
                fillData(currentChunk);
                fullChunks.push_back(currentChunk);
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds{100});
    }
}

void start() { dataGeneratorThread = std::jthread{dataGenerator}; }

void stop() {
    if(dataGeneratorThread.joinable()) {
        dataGeneratorThread.request_stop();
        dataGeneratorThread.join();
    }
}

void pushChunk(unsigned char* chunk) {
    std::lock_guard<std::mutex> lock(mutex);
    freeChunks.push_back(chunk);
}

unsigned char* popChunk() {
    std::lock_guard<std::mutex> lock(mutex);
    if(!fullChunks.empty()) {
        auto ret = fullChunks.front();
        fullChunks.erase(fullChunks.begin());
        return ret;
    }
    return nullptr;
}
