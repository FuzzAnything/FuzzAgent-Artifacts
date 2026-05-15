// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
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
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static pugi::xml_encoding get_valid_encoding(uint8_t encoding_byte) {
    pugi::xml_encoding encoding = static_cast<pugi::xml_encoding>(encoding_byte % 10);
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        return encoding;
    }
    return pugi::encoding_auto;
}

extern "C" int LLVMFuzzerTestOneInput_78(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize random parameters from fuzzer input
    uint8_t options_byte = Data[0];
    uint8_t encoding_byte = Size > 1 ? Data[1] : 0;
    pugi::xml_encoding encoding = get_valid_encoding(encoding_byte);
    unsigned int options = static_cast<unsigned int>(options_byte);
    
    // Prepare XML string (null-terminated)
    std::string xml_string;
    if (Size > 2) {
        xml_string.assign(reinterpret_cast<const char*>(Data + 2), Size - 2);
    } else {
        xml_string = "<root/>";
    }
    
    // Test load_string
    {
        pugi::xml_document doc1;
        pugi::xml_parse_result result = doc1.load_string(xml_string.c_str(), options);
        (void)result;
    }
    
    // Test load_file
    {
        write_dummy_file(Data, Size);
        pugi::xml_document doc2;
        pugi::xml_parse_result result = doc2.load_file("./dummy_file", options, encoding);
        (void)result;
    }
    
    // Test load with stream
    {
        std::istringstream stream(xml_string);
        pugi::xml_document doc3;
        pugi::xml_parse_result result = doc3.load(stream, options, encoding);
        (void)result;
    }
    
    // Test save_file overloads
    {
        pugi::xml_document doc4;
        doc4.load_string("<root><child>test</child></root>");
        
        // First overload (const char* path)
        bool saved1 = doc4.save_file("./dummy_save1.xml", PUGIXML_TEXT("\t"), options, encoding);
        (void)saved1;
        
        // Second overload (const wchar_t* path)
        bool saved2 = doc4.save_file(L"./dummy_save2.xml", PUGIXML_TEXT("\t"), options, encoding);
        (void)saved2;
    }
    
    // Test reset functions
    {
        pugi::xml_document doc5;
        doc5.load_string(xml_string.c_str());
        
        // Reset to empty
        doc5.reset();
        
        // Reset with another document
        pugi::xml_document doc6;
        doc6.load_string("<another>content</another>");
        doc5.reset(doc6);
    }
    
    return 0;
}