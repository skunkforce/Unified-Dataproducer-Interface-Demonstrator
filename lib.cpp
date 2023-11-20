#include "lib.hpp"

void pushChunk(unsigned char*& _chunk) {
    int sz = 4096; 
    _chunk = new unsigned char[sz];

    for(int i=0; i<sz; i++)
        _chunk[i] = i % 255;

    chunk = _chunk;
}

unsigned char* popChunk(){ 
     return chunk;
}
