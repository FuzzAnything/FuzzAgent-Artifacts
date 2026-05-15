// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::save_file at pugixml.cpp:7822:34 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7496:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load at pugixml.cpp:7697:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::save_file at pugixml.cpp:7814:34 in pugixml.hpp
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
#include <string>

static pugi::xml_encoding get_valid_encoding(const uint8_t* data, size_t size) {
    if (size == 0) return pugi::encoding_auto;
    uint8_t encoding_val = data[0] % 10; // 0-9 valid values
    return static_cast<pugi::xml_encoding>(encoding_val);
}

static unsigned int get_options(const uint8_t* data, size_t size, size_t offset) {
    if (size <= offset) return pugi::parse_default;
    // Use the byte at offset as options value
    return static_cast<unsigned int>(data[offset]);
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    // Create dummy file for file operations
    const char* dummy_file_path = "./dummy_file";
    const wchar_t* dummy_file_wpath = L"./dummy_file";
    
    // Write input data to dummy file
    std::ofstream dummy_file(dummy_file_path, std::ios::binary);
    if (dummy_file.is_open()) {
        dummy_file.write(reinterpret_cast<const char*>(Data), Size);
        dummy_file.close();
    }
    
    // Create XML document objects
    pugi::xml_document doc1;
    pugi::xml_document doc2;
    
    // Test reset() - both overloads
    doc1.reset();  // void reset()
    doc1.reset(doc2);  // void reset(const xml_document& proto)
    
    // Test load_string
    if (Size > 0) {
        // Create null-terminated string from input data
        std::string xml_string(reinterpret_cast<const char*>(Data), Size);
        // Ensure null termination
        if (xml_string.size() > 0 && xml_string.back() != '\0') {
            xml_string.push_back('\0');
        }
        
        unsigned int options = get_options(Data, Size, 0);
        pugi::xml_parse_result result = doc1.load_string(xml_string.c_str(), options);
        (void)result; // Use result to avoid unused variable warning
    }
    
    // Test load from stream
    if (Size > 0) {
        std::istringstream stream(std::string(reinterpret_cast<const char*>(Data), Size));
        unsigned int options = get_options(Data, Size, 1 % Size);
        pugi::xml_encoding encoding = get_valid_encoding(Data, Size);
        
        // Validate encoding before calling load
        if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
            pugi::xml_parse_result result = doc1.load(stream, options, encoding);
            (void)result;
        }
    }
    
    // Test load_file
    unsigned int options = get_options(Data, Size, 2 % (Size > 0 ? Size : 1));
    pugi::xml_encoding encoding = get_valid_encoding(Data + 1, Size > 1 ? Size - 1 : 0);
    
    // Validate encoding before calling load_file
    if (encoding >= pugi::encoding_auto && encoding <= pugi::encoding_latin1) {
        pugi::xml_parse_result result = doc1.load_file(dummy_file_path, options, encoding);
        (void)result;
    }
    
    // Test save_file overloads
    if (doc1) {  // Only save if document has content
        // Get parameters for save_file
        unsigned int flags = get_options(Data, Size, 3 % (Size > 0 ? Size : 1));
        pugi::xml_encoding save_encoding = get_valid_encoding(Data + 2, Size > 2 ? Size - 2 : 0);
        
        // Validate encoding before calling save_file
        if (save_encoding >= pugi::encoding_auto && save_encoding <= pugi::encoding_latin1) {
            // First overload: const char* path
            bool saved1 = doc1.save_file(dummy_file_path, 
                                        PUGIXML_TEXT("\t"), 
                                        flags, 
                                        save_encoding);
            (void)saved1;
            
            // Second overload: const wchar_t* path
            bool saved2 = doc1.save_file(dummy_file_wpath,
                                        PUGIXML_TEXT("\t"),
                                        flags,
                                        save_encoding);
            (void)saved2;
        }
    }
    
    // Additional test: Try loading with different encodings
    for (int i = 0; i < 10; ++i) {
        pugi::xml_encoding test_encoding = static_cast<pugi::xml_encoding>(i);
        if (test_encoding >= pugi::encoding_auto && test_encoding <= pugi::encoding_latin1) {
            pugi::xml_document test_doc;
            std::istringstream test_stream(std::string(reinterpret_cast<const char*>(Data), Size));
            pugi::xml_parse_result test_result = test_doc.load(test_stream, 
                                                              pugi::parse_default, 
                                                              test_encoding);
            (void)test_result;
        }
    }
    
    // Test reset again after operations
    doc1.reset();
    doc2.reset(doc1);
    
    return 0;
}