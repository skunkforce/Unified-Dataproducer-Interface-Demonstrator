# Unified-Dataproducer-Interface-Demonstrator
This project demonstrates the usage of a proposed .dll interface
Purpose of the `.dll`/`.so` is to abstract a measurement-device away from a data-consumer in a precompiled library. 
The proposed interface should be flexible enough to communicate with most devices.
In this Repo, there are to separate programs:
* The caller
* The `DLL`

The caller allocates a variable amount of memory on the heap, each `32kb` large.
Each of this blocks of memory is called a `chunk`. 
Using a function from inside the `DLL` called `pushChunk` a pointer to the allocated memory-location is propagated to the `DLL`.
After the caller loaded the `DLL`, the library itself shall spawn another thread. 
It shall also construct four empty lock-free queues:
* push-Interface-Queue - Stores pointers to incoming chunks
* Command-Queue - Stores commands for the `DLLMain`-thread
* Empty-Chunk-Queue - Stores empty chunks of memory
* pop-Interface-Queue - Stores structs of two pointers, that can be received by the caller

In this example, the Command-Queue is left unused and there is no attempt made to parse incoming commands via the push-Interface-Queue.
The `DLLMain`-thread simply provides an `iota` that walks up to `MAX_UINT` and fills an empty chunk. 
As soon as the chunk is filled, a struct is created that holds two pointers called `meta` and `data`. 
In this example the `meta` pointer shall always be `NULL`. 
Later versions will use this pointer for a chunk of `JSON` meta-data of the dataproducer.

The following diagram might clarify some questions.
[![Architecture-Diagram](https://lucid.app/publicSegments/view/9e17748f-074c-4f78-afd4-eeac6ea42f19/image.png)](https://lucid.app/publicSegments/view/9e17748f-074c-4f78-afd4-eeac6ea42f19/image.png)

From our expectations so far, Linux-`.so` and Windows-`.dll` need to be maintained separately from oneanother.


After creating `lib.hpp` and `lib.cpp`, use the following Terminal commands to create a shared library on Linux:

// Convert the library code to object file
`g++ -std=c++23 -c lib.cpp -fPIC -o lib.o`

// Create a shared `.so` library
`g++ -std=c++23 -shared -o liblib.so lib.o`

// create client source 
`main.cpp`

// Create the executable by linking the shared library
`g++ -std=c++23 -L. -Wall -o main main.cpp -llib`

// Make shared library available at runtime
`export LD_LIBRARY_PATH=.`

// Create `CMakeLists.txt` file and then

`mkdir build`
`cd build`
`cmake ...`
`make`
`./main`