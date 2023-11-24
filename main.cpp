#include <iostream>
#include "lib.hpp"

int main() {
    unsigned char *chnk;
    pushChunk(chnk);

    unsigned char *p = popChunk();

    for(int i=97; i<123; i++)
        std::cout << p[i] << ' ';

   return 0;     
}