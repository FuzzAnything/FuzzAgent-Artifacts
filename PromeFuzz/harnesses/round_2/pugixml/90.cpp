// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
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

static std::string makeNullTerminatedString(const uint8_t* data, size_t size, size_t max_len = 100) {
    if (size == 0) return "";
    size_t len = std::min(size, max_len);
    std::string result(reinterpret_cast<const char*>(data), len);
    // Ensure null termination
    if (result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

static std::vector<std::string> extractStrings(const uint8_t* data, size_t size, int count) {
    std::vector<std::string> strings;
    if (size == 0 || count <= 0) {
        // Return vector with empty strings to avoid out-of-bounds access
        strings.resize(count);
        return strings;
    }
    
    size_t segment = size / count;
    for (int i = 0; i < count && i * segment < size; ++i) {
        size_t start = i * segment;
        size_t end = std::min(start + segment, size);
        strings.push_back(makeNullTerminatedString(data + start, end - start));
    }
    
    // Fill remaining with empty strings to ensure we have exactly 'count' elements
    while (strings.size() < static_cast<size_t>(count)) {
        strings.push_back("");
    }
    
    return strings;
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* data, size_t size) {
    // Write data to dummy file for potential file-based parsing
    writeDummyFile(data, size);
    
    // Try to parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // If parsing fails, we can still test with empty nodes
    pugi::xml_node node;
    if (result) {
        node = doc.document_element();
        if (!node) {
            // If no document element, try to get first child
            node = doc.first_child();
        }
    }
    
    // Extract strings for function parameters
    auto strings = extractStrings(data, size, 5);
    
    // Test xml_node::next_sibling - check vector bounds before accessing
    if (strings.size() > 0 && !strings[0].empty()) {
        pugi::xml_node sibling = node.next_sibling(strings[0].c_str());
        (void)sibling;
    }
    
    // Test xml_node::child
    if (strings.size() > 1 && !strings[1].empty()) {
        pugi::xml_node child = node.child(strings[1].c_str());
        (void)child;
    }
    
    // Test xml_node::find_child_by_attribute (first overload with 3 params)
    if (strings.size() > 2 && !strings[0].empty() && !strings[1].empty() && !strings[2].empty()) {
        pugi::xml_node found = node.find_child_by_attribute(
            strings[0].c_str(), 
            strings[1].c_str(), 
            strings[2].c_str()
        );
        (void)found;
    }
    
    // Test xml_node::find_child_by_attribute (second overload with 2 params)
    if (strings.size() > 2 && !strings[1].empty() && !strings[2].empty()) {
        pugi::xml_node found = node.find_child_by_attribute(
            strings[1].c_str(), 
            strings[2].c_str()
        );
        (void)found;
    }
    
    // Test xml_node::first_element_by_path
    if (strings.size() > 3 && !strings[3].empty()) {
        pugi::xml_node path_node = node.first_element_by_path(strings[3].c_str());
        (void)path_node;
        
        // Also test with different delimiter
        path_node = node.first_element_by_path(strings[3].c_str(), '.');
        (void)path_node;
    }
    
    // Test xml_node::parent
    pugi::xml_node parent = node.parent();
    (void)parent;
    
    // Try to explore more states by using different nodes
    if (result) {
        // Try with root node
        pugi::xml_node root = doc;
        (void)root.parent();
        
        // Try with first child if exists
        pugi::xml_node first_child = doc.first_child();
        if (first_child) {
            (void)first_child.parent();
            if (strings.size() > 0 && !strings[0].empty()) {
                (void)first_child.next_sibling(strings[0].c_str());
            }
        }
        
        // Try to traverse the tree
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            (void)child.parent();
            if (strings.size() > 0 && !strings[0].empty()) {
                (void)child.next_sibling(strings[0].c_str());
            }
        }
    }
    
    // Clean up dummy file
    std::remove("./dummy_file");
    
    return 0;
}