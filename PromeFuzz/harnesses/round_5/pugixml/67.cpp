// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
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
#include <string>
#include <vector>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

static std::string createNullTerminatedString(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    size_t len = std::min(size, static_cast<size_t>(100));
    std::string str(reinterpret_cast<const char*>(data), len);
    if (str.back() != '\0') str += '\0';
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;

    // Create dummy file for potential file operations
    writeDummyFile(Data, Size);

    // Create null-terminated strings from fuzzer input
    std::string str1 = createNullTerminatedString(Data, Size);
    std::string str2 = createNullTerminatedString(Data + std::min(Size/2, Size-1), Size/2);
    
    // Try to parse XML from the input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Get root node (might be empty if parsing failed)
    pugi::xml_node root = doc.root();
    
    // Test find_child_by_attribute
    if (!str1.empty() && !str2.empty()) {
        try {
            root.find_child_by_attribute(str1.c_str(), str2.c_str());
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test parent function
    try {
        pugi::xml_node parent = root.parent();
        // Also test on child nodes
        for (pugi::xml_node child : root.children()) {
            child.parent();
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test next_sibling overloads
    try {
        // Overload without parameter
        pugi::xml_node next = root.next_sibling();
        
        // Overload with name parameter
        if (!str1.empty()) {
            root.next_sibling(str1.c_str());
        }
        
        // Test on child nodes
        for (pugi::xml_node child : root.children()) {
            child.next_sibling();
            if (!str1.empty()) {
                child.next_sibling(str1.c_str());
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test child function with string_view
    try {
        if (!str1.empty()) {
            std::string_view sv(str1.c_str(), str1.length() - 1); // Exclude null terminator
            root.child(sv);
        }
        
        // Test on child nodes
        for (pugi::xml_node child : root.children()) {
            if (!str1.empty()) {
                std::string_view sv(str1.c_str(), str1.length() - 1);
                child.child(sv);
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test first_element_by_path
    try {
        if (!str1.empty()) {
            root.first_element_by_path(str1.c_str());
            // Test with different delimiters
            root.first_element_by_path(str1.c_str(), '.');
            root.first_element_by_path(str1.c_str(), '|');
        }
        
        // Test on child nodes
        for (pugi::xml_node child : root.children()) {
            if (!str1.empty()) {
                child.first_element_by_path(str1.c_str());
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test on empty/default constructed nodes
    pugi::xml_node empty_node;
    try {
        empty_node.parent();
        empty_node.next_sibling();
        if (!str1.empty()) {
            empty_node.next_sibling(str1.c_str());
            std::string_view sv(str1.c_str(), str1.length() - 1);
            empty_node.child(sv);
            empty_node.find_child_by_attribute(str1.c_str(), str2.c_str());
            empty_node.first_element_by_path(str1.c_str());
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Try to load from dummy file and test again
    pugi::xml_document doc2;
    pugi::xml_parse_result result2 = doc2.load_file("./dummy_file");
    if (result2) {
        pugi::xml_node root2 = doc2.root();
        
        // Test all functions again with file-loaded document
        if (!str1.empty() && !str2.empty()) {
            try {
                root2.find_child_by_attribute(str1.c_str(), str2.c_str());
            } catch (...) {}
        }
        
        try {
            root2.parent();
            root2.next_sibling();
            if (!str1.empty()) {
                root2.next_sibling(str1.c_str());
                std::string_view sv(str1.c_str(), str1.length() - 1);
                root2.child(sv);
                root2.first_element_by_path(str1.c_str());
            }
        } catch (...) {}
    }
    
    return 0;
}