// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_parse_result::description at pugixml.cpp:7430:45 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <fstream>
#include <sstream>
#include <random>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_105(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Initialize random generator with input data to ensure deterministic behavior for same input
    std::mt19937 gen(Data[0]);
    std::uniform_int_distribution<unsigned int> options_dist(0, 0xFFFFFFFF);
    std::uniform_int_distribution<int> encoding_dist(0, 9); // 0 to 9 covers all xml_encoding values

    pugi::xml_document doc;
    unsigned int options = options_dist(gen);
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_dist(gen));

    // 1. Fuzz load_buffer_inplace
    // Note: load_buffer_inplace modifies the buffer. We need a copy of the data.
    // We allocate a buffer on the heap to simulate user-controlled memory that persists.
    std::vector<uint8_t> inplace_buffer(Size);
    std::memcpy(inplace_buffer.data(), Data, Size);
    {
        pugi::xml_parse_result result = doc.load_buffer_inplace(
            inplace_buffer.data(), 
            Size, 
            options, 
            encoding
        );
        // Access description to exercise that API
        const char* desc = result.description();
        (void)desc; // Suppress unused variable warning
    }
    doc.reset(); // Clear document before next operation

    // 2. Fuzz load_buffer
    // This copies the data, so we can use the original input directly
    pugi::xml_parse_result result_buffer = doc.load_buffer(
        Data, 
        Size, 
        options, 
        encoding
    );
    const char* desc_buffer = result_buffer.description();
    (void)desc_buffer;
    doc.reset();

    // 3. Fuzz load_file
    // Write data to dummy file first
    write_dummy_file(Data, Size);
    pugi::xml_parse_result result_file = doc.load_file(
        "./dummy_file", 
        options, 
        encoding
    );
    const char* desc_file = result_file.description();
    (void)desc_file;
    doc.reset();

    // 4. Fuzz load_string
    // load_string requires a null-terminated string.
    // We cannot pass raw binary directly. We check if the data contains a null terminator.
    // If not, we try to find a safe substring or skip.
    // To maximize coverage, we try to construct a valid C-string from the input if possible.
    // If the input has a null byte, we can use the prefix.
    const char* c_str = nullptr;
    size_t str_len = 0;
    
    // Find first null byte
    const char* null_pos = reinterpret_cast<const char*>(Data);
    size_t i = 0;
    while (i < Size && null_pos[i] != '\0') {
        i++;
    }
    
    if (i < Size) {
        // Found a null terminator within the buffer
        c_str = reinterpret_cast<const char*>(Data);
        str_len = i;
    } else {
        // No null terminator found. 
        // We can try to create a temporary buffer with a null terminator at the end
        // However, load_string expects a const char_t*. 
        // To be safe and avoid heap allocation issues in fuzzing loops, 
        // we only test load_string if the input naturally contains a null terminator
        // or if we can safely cast a prefix.
        // If no null is found, we skip load_string for this input to avoid UB.
        // Alternatively, we could append a null, but that requires a mutable buffer.
        // Let's skip load_string if no null is found to keep it safe and simple.
        c_str = nullptr;
    }

    if (c_str != nullptr) {
        pugi::xml_parse_result result_str = doc.load_string(c_str, options);
        const char* desc_str = result_str.description();
        (void)desc_str;
        doc.reset();
    }

    // 5. Fuzz load (deprecated)
    // Similar to load_string, it requires a null-terminated string.
    if (c_str != nullptr) {
        pugi::xml_parse_result result_load = doc.load(c_str, options);
        const char* desc_load = result_load.description();
        (void)desc_load;
        doc.reset();
    }

    return 0;
}