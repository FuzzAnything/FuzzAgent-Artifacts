// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>

static uint32_t get_random_uint32() {
    static std::mt19937 gen(0);
    static std::uniform_int_distribution<uint32_t> dist(0, UINT32_MAX);
    return dist(gen);
}

static pugi::xml_encoding get_random_encoding() {
    static std::mt19937 gen(0);
    static std::uniform_int_distribution<int> dist(0, 9); // 0 to 9 covers all enum values
    int val = dist(gen);
    return static_cast<pugi::xml_encoding>(val);
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure we have enough data for random generation if needed
    if (Size < 2) {
        return 0;
    }

    // Create a copy of the input data to manipulate safely
    std::vector<uint8_t> buffer(Data, Data + Size);
    
    // Ensure null-termination for string-based functions
    buffer.push_back(0);

    pugi::xml_document doc;
    
    // Seed random generator based on input data to ensure reproducibility
    std::mt19937 gen(Data[0] ^ (Data[1] << 8));
    std::uniform_int_distribution<uint32_t> options_dist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> encoding_dist(0, 9);

    // 1. Fuzz load_buffer_inplace
    // Note: We must keep the buffer alive. We use the vector's data.
    // We create a separate vector to ensure it persists if we were to store the doc elsewhere, 
    // but here we just test the call.
    {
        pugi::xml_document doc_inplace;
        uint32_t opts = options_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(encoding_dist(gen));
        doc_inplace.load_buffer_inplace(buffer.data(), buffer.size(), opts, enc);
    }

    // 2. Fuzz load (overload 1: const char_t*)
    // This is deprecated but requested.
    {
        pugi::xml_document doc_load_str;
        uint32_t opts = options_dist(gen);
        // Ensure the string is valid (it is null-terminated by our push_back)
        doc_load_str.load(reinterpret_cast<const pugi::char_t*>(buffer.data()), opts);
    }

    // 2. Fuzz load (overload 2: std::basic_istream)
    {
        pugi::xml_document doc_load_stream;
        std::istringstream stream(reinterpret_cast<const char*>(buffer.data()));
        uint32_t opts = options_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(encoding_dist(gen));
        doc_load_stream.load(stream, opts, enc);
    }

    // 3. Fuzz load_file
    // We write the data to a dummy file first
    {
        const char* filename = "./dummy_file";
        std::ofstream file(filename, std::ios::binary);
        if (file.is_open()) {
            file.write(reinterpret_cast<const char*>(buffer.data()), buffer.size());
            file.close();
        }
        
        pugi::xml_document doc_file;
        uint32_t opts = options_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(encoding_dist(gen));
        doc_file.load_file(filename, opts, enc);
        
        // Cleanup
        std::remove(filename);
    }

    // 4. Fuzz load_buffer
    {
        pugi::xml_document doc_buf;
        uint32_t opts = options_dist(gen);
        pugi::xml_encoding enc = static_cast<pugi::xml_encoding>(encoding_dist(gen));
        doc_buf.load_buffer(buffer.data(), buffer.size(), opts, enc);
    }

    // 5. Fuzz load_string
    {
        pugi::xml_document doc_str;
        uint32_t opts = options_dist(gen);
        doc_str.load_string(reinterpret_cast<const pugi::char_t*>(buffer.data()), opts);
    }

    return 0;
}