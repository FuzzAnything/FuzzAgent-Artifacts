// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7739:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace_own at pugixml.cpp:7763:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
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
#include <fstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void write_dummy_wide_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    pugi::xml_document proto_doc;
    
    // Seed for random options and encoding
    uint32_t seed = 0;
    if (Size >= 4) {
        std::memcpy(&seed, Data, 4);
    }
    
    // Generate random options and encoding
    // Only use valid parsing flags (0x0000 to 0x003F are typical valid combinations in pugixml)
    unsigned int options = seed & 0x3F;
    
    // Clamp encoding to valid range [encoding_auto, encoding_latin1]
    // Valid values are 0 to 9 based on the enum definition
    int enc_val = (seed >> 16) % 10;
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(enc_val);
    
    // Test 1: load_buffer
    {
        doc.reset();
        pugi::xml_parse_result result = doc.load_buffer(Data, Size, options, encoding);
        (void)result;
    }
    
    // Test 2: load_buffer_inplace
    {
        doc.reset();
        std::vector<uint8_t> buffer(Data, Data + Size);
        pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size, options, encoding);
        (void)result;
    }
    
    // Test 3: load_buffer_inplace_own
    {
        doc.reset();
        void* buffer = std::malloc(Size);
        if (buffer) {
            std::memcpy(buffer, Data, Size);
            pugi::xml_parse_result result = doc.load_buffer_inplace_own(buffer, Size, options, encoding);
            (void)result;
            // Note: doc now owns the buffer, we should not free it here
        }
    }
    
    // Test 4: load from stream
    {
        doc.reset();
        std::string str(reinterpret_cast<const char*>(Data), Size);
        std::istringstream stream(str, std::ios::binary);
        pugi::xml_parse_result result = doc.load(stream, options, encoding);
        (void)result;
    }
    
    // Test 5: load_file (using dummy file)
    {
        doc.reset();
        write_dummy_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file("./dummy_file", options, encoding);
        (void)result;
    }
    
    // Test 6: load_file with wide path
    {
        doc.reset();
        write_dummy_wide_file(Data, Size);
        pugi::xml_parse_result result = doc.load_file(L"./dummy_file", options, encoding);
        (void)result;
    }
    
    // Test 7: reset with proto
    {
        proto_doc.reset();
        proto_doc.load_buffer(Data, Size, options, encoding);
        
        doc.reset();
        doc.reset(proto_doc);
    }
    
    // Test 8: Multiple resets
    {
        doc.reset();
        doc.reset();
        doc.reset();
    }
    
    return 0;
}