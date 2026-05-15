// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5754:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
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

static void test_xml_node_functions(pugi::xml_node node, const uint8_t* data, size_t size) {
    if (!node) return;
    
    // Extract strings from fuzzer input for function parameters
    // We'll split the input into multiple null-terminated strings
    std::vector<std::string> strings;
    const char* ptr = reinterpret_cast<const char*>(data);
    const char* end = ptr + size;
    
    while (ptr < end) {
        const char* str_start = ptr;
        while (ptr < end && *ptr != '\0') {
            ++ptr;
        }
        if (ptr > str_start) {
            strings.emplace_back(str_start, ptr - str_start);
        }
        if (ptr < end && *ptr == '\0') {
            ++ptr; // Skip the null terminator
        }
    }
    
    // If we have no strings, use some defaults
    if (strings.empty()) {
        strings.push_back("");
        strings.push_back("node");
        strings.push_back("attr");
        strings.push_back("value");
        strings.push_back("path");
    }
    
    // Ensure we have at least 5 strings (pad with empty strings if needed)
    while (strings.size() < 5) {
        strings.push_back("");
    }
    
    // Test parent()
    (void)node.parent();
    
    // Test next_sibling()
    if (!strings[0].empty()) {
        (void)node.next_sibling(strings[0].c_str());
    }
    
    // Test find_child_by_attribute()
    if (!strings[0].empty() && !strings[1].empty() && !strings[2].empty()) {
        (void)node.find_child_by_attribute(strings[0].c_str(), 
                                          strings[1].c_str(), 
                                          strings[2].c_str());
    }
    
    // Test child()
    if (!strings[0].empty()) {
        (void)node.child(strings[0].c_str());
    }
    
    // Test previous_sibling()
    if (!strings[0].empty()) {
        (void)node.previous_sibling(strings[0].c_str());
    }
    
    // Test first_element_by_path()
    if (!strings[3].empty()) {
        (void)node.first_element_by_path(strings[3].c_str(), '/');
        // Test with different delimiter
        if (strings.size() > 4 && !strings[3].empty() && !strings[4].empty()) {
            char delimiter = strings[4].empty() ? '.' : strings[4][0];
            (void)node.first_element_by_path(strings[3].c_str(), delimiter);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    static bool initialized = false;
    if (!initialized) {
        // One-time initialization if needed
        initialized = true;
    }
    
    // Try to parse the input as XML
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Test functions on document root
    test_xml_node_functions(doc.root(), Data, Size);
    
    // Also test on all nodes in the document
    if (result) {
        // Traverse all nodes
        pugi::xml_node node = doc.first_child();
        while (node) {
            test_xml_node_functions(node, Data, Size);
            
            // Test on child nodes recursively
            for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
                test_xml_node_functions(child, Data, Size);
            }
            
            // Move to next sibling at current level
            node = node.next_sibling();
        }
    }
    
    // Also test with an empty node
    pugi::xml_node empty_node;
    test_xml_node_functions(empty_node, Data, Size);
    
    // Test with a constructed node from null pointer
    pugi::xml_node null_node(nullptr);
    test_xml_node_functions(null_node, Data, Size);
    
    // Try to create a document from a file if parsing from buffer fails
    if (!result && Size > 0) {
        FILE* f = fopen("./dummy_file", "wb");
        if (f) {
            fwrite(Data, 1, Size, f);
            fclose(f);
            
            pugi::xml_document doc2;
            pugi::xml_parse_result result2 = doc2.load_file("./dummy_file");
            if (result2) {
                test_xml_node_functions(doc2.root(), Data, Size);
            }
            remove("./dummy_file");
        }
    }
    
    return 0;
}