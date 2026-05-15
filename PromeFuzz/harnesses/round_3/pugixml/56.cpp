// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <vector>

static void test_as_utf8(const uint8_t* data, size_t size) {
    // Test wchar_t* overload
    if (size > 0) {
        // Create a wide string from fuzzer input
        size_t wchar_count = size / sizeof(wchar_t);
        if (wchar_count > 0) {
            const wchar_t* wstr = reinterpret_cast<const wchar_t*>(data);
            // Ensure null termination for safety
            std::vector<wchar_t> wchar_buf(wchar_count + 1);
            std::memcpy(wchar_buf.data(), wstr, wchar_count * sizeof(wchar_t));
            wchar_buf[wchar_count] = 0;
            
            pugi::as_utf8(wchar_buf.data());
        }
    }
    
    // Test std::wstring overload
    if (size > 0) {
        size_t wchar_count = size / sizeof(wchar_t);
        if (wchar_count > 0) {
            const wchar_t* wstr = reinterpret_cast<const wchar_t*>(data);
            std::basic_string<wchar_t> ws(wstr, wchar_count);
            pugi::as_utf8(ws);
        }
    }
}

static void test_as_wide(const uint8_t* data, size_t size) {
    // Test char* overload
    if (size > 0) {
        // Ensure null termination
        std::vector<char> char_buf(size + 1);
        std::memcpy(char_buf.data(), data, size);
        char_buf[size] = 0;
        
        pugi::as_wide(char_buf.data());
    }
    
    // Test std::string overload
    if (size > 0) {
        std::string s(reinterpret_cast<const char*>(data), size);
        pugi::as_wide(s);
    }
}

static void test_xml_node_functions(const uint8_t* data, size_t size) {
    // Create a default node (null node)
    pugi::xml_node node;
    
    // Test path() with default delimiter
    node.path();
    
    // Test path() with different delimiter
    if (size > 0) {
        char delimiter = static_cast<char>(data[0] % 128);
        if (delimiter == 0) delimiter = '/';
        node.path(delimiter);
    }
    
    // Test name()
    node.name();
    
    // Try to create a document and get nodes from it
    pugi::xml_document doc;
    if (size > 0) {
        // Try to parse the fuzzer input as XML
        pugi::xml_parse_result result = doc.load_buffer(data, size);
        
        if (result) {
            // Test functions on document node
            pugi::xml_node doc_node = doc;
            doc_node.path();
            doc_node.name();
            
            // Test on first child if exists
            pugi::xml_node child = doc.first_child();
            if (child) {
                child.path();
                child.name();
                
                // Test with different delimiter
                if (size > 1) {
                    char delimiter = static_cast<char>(data[1] % 128);
                    if (delimiter == 0) delimiter = '|';
                    child.path(delimiter);
                }
            }
            
            // Try to traverse all nodes
            for (pugi::xml_node node_iter = doc.first_child(); node_iter; node_iter = node_iter.next_sibling()) {
                node_iter.path();
                node_iter.name();
            }
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t *Data, size_t Size) {
    // Test as_utf8 function
    test_as_utf8(Data, Size);
    
    // Test as_wide function
    test_as_wide(Data, Size);
    
    // Test xml_node functions
    test_xml_node_functions(Data, Size);
    
    return 0;
}