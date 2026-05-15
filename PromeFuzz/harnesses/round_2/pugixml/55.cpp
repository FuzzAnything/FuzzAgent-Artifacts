// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7724:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>

static void write_to_dummy_file(const uint8_t *data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static void remove_dummy_file() {
    std::remove("./dummy_file");
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Write data to dummy file for file-based operations
    write_to_dummy_file(Data, Size);

    // Initialize parameters from fuzz data
    unsigned int options = 0;
    pugi::xml_encoding encoding = pugi::encoding_auto;
    
    if (Size >= sizeof(unsigned int)) {
        memcpy(&options, Data, sizeof(unsigned int));
    }
    
    if (Size >= sizeof(unsigned int) + 1) {
        uint8_t enc_byte = Data[sizeof(unsigned int)];
        encoding = static_cast<pugi::xml_encoding>(enc_byte % 10);
    }

    // Create XML document
    pugi::xml_document doc;
    
    // Test load_string
    if (Size > 0) {
        std::string xml_string(reinterpret_cast<const char*>(Data), Size);
        pugi::xml_parse_result result1 = doc.load_string(xml_string.c_str(), options);
        (void)result1;
    }

    // Test deprecated load (from string)
    if (Size > 0) {
        std::string xml_string(reinterpret_cast<const char*>(Data), Size);
        pugi::xml_parse_result result2 = doc.load(xml_string.c_str(), options);
        (void)result2;
    }

    // Test load from stream
    if (Size > 0) {
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        pugi::xml_parse_result result3 = doc.load(stream, options, encoding);
        (void)result3;
    }

    // Test load_file (char* version)
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        pugi::xml_parse_result result4 = doc.load_file("./dummy_file", options, encoding);
        (void)result4;
    }

    // Test save_file overloads
    // First overload: const char* path
    bool save_success1 = doc.save_file("./dummy_file", "\t", pugi::format_default, pugi::encoding_auto);
    (void)save_success1;
    
    // Second overload: const wchar_t* path
    // Use PUGIXML_TEXT macro for indent to ensure correct type
    bool save_success2 = doc.save_file(L"./dummy_file", PUGIXML_TEXT("\t"), pugi::format_default, pugi::encoding_auto);
    (void)save_success2;

    // Cleanup
    remove_dummy_file();
    
    return 0;
}