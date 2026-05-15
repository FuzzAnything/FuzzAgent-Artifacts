// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
#include <string>
#include <fstream>

static void create_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t* Data, size_t Size) {
    // Create dummy file for any potential file operations
    create_dummy_file(Data, Size);
    
    // Test pugi::as_utf8 overloads
    if (Size > 0) {
        // Create wide string from fuzzer data
        size_t wchar_count = Size / sizeof(wchar_t);
        if (wchar_count > 0) {
            const wchar_t* wchar_data = reinterpret_cast<const wchar_t*>(Data);
            std::wstring wstr(wchar_data, wchar_count);
            
            // Test both overloads
            try {
                pugi::as_utf8(wstr);
                pugi::as_utf8(wchar_data);
            } catch (...) {
                // Discard exceptions
            }
        }
    }
    
    // Test pugi::as_wide overloads
    if (Size > 0) {
        const char* char_data = reinterpret_cast<const char*>(Data);
        std::string str(char_data, Size);
        
        // Test both overloads
        try {
            pugi::as_wide(str);
            pugi::as_wide(char_data);
        } catch (...) {
            // Discard exceptions
        }
    }
    
    // Test xml_node::path and xml_node::name
    try {
        // Create an XML document from fuzzer data
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_buffer(Data, Size);
        
        if (result) {
            // Get root node
            pugi::xml_node root = doc.root();
            
            // Test path() with default delimiter
            root.path();
            
            // Test path() with custom delimiter (use first byte if available)
            if (Size > 0) {
                char delimiter = static_cast<char>(Data[0]);
                root.path(delimiter);
            }
            
            // Test name()
            root.name();
            
            // Test on child nodes if they exist
            for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
                child.path();
                child.name();
            }
        }
        
        // Also test on null node
        pugi::xml_node null_node;
        null_node.path();
        null_node.name();
        
    } catch (...) {
        // Discard exceptions
    }
    
    return 0;
}