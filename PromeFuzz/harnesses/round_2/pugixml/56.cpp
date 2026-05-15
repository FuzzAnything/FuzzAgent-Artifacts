// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7861:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_wide at pugixml.cpp:7854:59 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7842:44 in pugixml.hpp
// pugi::as_utf8 at pugixml.cpp:7849:44 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::path at pugixml.cpp:6711:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_56(const uint8_t* Data, size_t Size) {
    // Test pugi::as_wide overloads
    if (Size > 0) {
        // Create UTF-8 string from input data
        std::string utf8_str(reinterpret_cast<const char*>(Data), Size);
        
        // Test first overload: const std::basic_string<char>&
        std::basic_string<wchar_t> wide_result1 = pugi::as_wide(utf8_str);
        
        // Test second overload: const char*
        std::basic_string<wchar_t> wide_result2 = pugi::as_wide(utf8_str.c_str());
        
        // Test with empty string (safe)
        std::basic_string<wchar_t> wide_empty = pugi::as_wide("");
    }
    
    // Test pugi::as_utf8 overloads
    if (Size > 0) {
        // Create wide string from input data (treat bytes as wchar_t)
        size_t wide_size = Size / sizeof(wchar_t);
        if (wide_size > 0) {
            std::basic_string<wchar_t> wide_str(
                reinterpret_cast<const wchar_t*>(Data), 
                wide_size
            );
            
            // Test first overload: const std::basic_string<wchar_t>&
            std::basic_string<char> utf8_result1 = pugi::as_utf8(wide_str);
            
            // Test second overload: const wchar_t*
            std::basic_string<char> utf8_result2 = pugi::as_utf8(wide_str.c_str());
            
            // Test with empty wide string (safe)
            std::basic_string<wchar_t> empty_wide;
            std::basic_string<char> utf8_empty = pugi::as_utf8(empty_wide);
        }
    }
    
    // Test pugi::xml_node functions
    writeDummyFile(Data, Size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result parse_result = doc.load_file("./dummy_file");
    
    if (parse_result) {
        // Test xml_node::path
        pugi::xml_node root = doc.root();
        if (root) {
            // Test with default delimiter
            pugi::string_t path1 = root.path();
            
            // Test with custom delimiter
            pugi::string_t path2 = root.path('.');
            pugi::string_t path3 = root.path('|');
            
            // Test on child nodes
            for (pugi::xml_node child : root.children()) {
                pugi::string_t child_path = child.path('/');
                pugi::string_t child_path2 = child.path('-');
                
                // Test xml_node::name
                const pugi::char_t* child_name = child.name();
            }
            
            // Test xml_node::name on root
            const pugi::char_t* root_name = root.name();
        }
        
        // Test on document element
        pugi::xml_node doc_element = doc.document_element();
        if (doc_element) {
            pugi::string_t elem_path = doc_element.path('/');
            const pugi::char_t* elem_name = doc_element.name();
        }
        
        // Test on null nodes
        pugi::xml_node null_node;
        pugi::string_t null_path = null_node.path();
        const pugi::char_t* null_name = null_node.name();
    } else {
        // Test on null document nodes
        pugi::xml_node null_node;
        pugi::string_t null_path = null_node.path('/');
        const pugi::char_t* null_name = null_node.name();
    }
    
    // Test xml_node::name on various node types
    pugi::xml_document test_doc;
    const char* test_xml = "<root><child attr='value'>text</child></root>";
    if (test_doc.load_string(test_xml)) {
        pugi::xml_node root = test_doc.root();
        
        // Test on element node
        pugi::xml_node child = root.child("child");
        if (child) {
            const pugi::char_t* child_name = child.name();
            pugi::string_t child_path = child.path('/');
        }
        
        // Test on text node (nameless)
        pugi::xml_node text_node = child.first_child();
        if (text_node) {
            const pugi::char_t* text_name = text_node.name();
            pugi::string_t text_path = text_node.path('/');
        }
    }
    
    return 0;
}