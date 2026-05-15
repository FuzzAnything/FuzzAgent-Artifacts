// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
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

static std::string extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return "";
    
    const char* str_start = reinterpret_cast<const char*>(data + offset);
    const char* str_end = str_start;
    size_t remaining = size - offset;
    
    // Find null terminator or end of data
    for (size_t i = 0; i < remaining; ++i) {
        if (str_start[i] == '\0') {
            str_end = str_start + i;
            break;
        }
        str_end = str_start + i + 1;
    }
    
    std::string result(str_start, str_end - str_start);
    offset += (str_end - str_start) + 1; // +1 for null terminator if found
    return result;
}

static std::vector<std::string> extract_strings(const uint8_t* data, size_t size, size_t count) {
    std::vector<std::string> strings;
    size_t offset = 0;
    
    for (size_t i = 0; i < count && offset < size; ++i) {
        strings.push_back(extract_string(data, size, offset));
    }
    
    // Pad with empty strings if we didn't get enough
    while (strings.size() < count) {
        strings.push_back("");
    }
    
    return strings;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* data, size_t size) {
    // Extract strings for child names
    std::vector<std::string> child_names = extract_strings(data, size, 12);
    
    // Parse XML from input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    if (!result) {
        // If parsing fails, create a minimal valid XML document
        const char* dummy_xml = "<root><child1/><child2/><child3/></root>";
        result = doc.load_string(dummy_xml);
        if (!result) {
            return 0;
        }
    }
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        root = doc.first_child();
        if (root.empty()) {
            return 0;
        }
    }
    
    // Start with root node
    pugi::xml_node current_node = root;
    
    // Execute the sequence of API calls
    try {
        // 8 child() calls
        for (int i = 0; i < 8; ++i) {
            if (!child_names[i].empty()) {
                current_node = current_node.child(child_names[i].c_str());
            } else {
                current_node = current_node.child("");
            }
        }
        
        // 4 children() calls - initialize each range with a call to children()
        pugi::xml_object_range<pugi::xml_named_node_iterator> range1 = 
            !child_names[8].empty() ? current_node.children(child_names[8].c_str()) : current_node.children("");
        
        pugi::xml_object_range<pugi::xml_named_node_iterator> range2 = 
            !child_names[9].empty() ? current_node.children(child_names[9].c_str()) : current_node.children("");
        
        pugi::xml_object_range<pugi::xml_named_node_iterator> range3 = 
            !child_names[10].empty() ? current_node.children(child_names[10].c_str()) : current_node.children("");
        
        pugi::xml_object_range<pugi::xml_named_node_iterator> range4 = 
            !child_names[11].empty() ? current_node.children(child_names[11].c_str()) : current_node.children("");
        
        // end() on last range
        range4.end();
        
        // first_child()
        current_node.first_child();
        
        // end() 3 times on last range
        range4.end();
        range4.end();
        range4.end();
        
        // first_child() again
        current_node.first_child();
        
        // last_child()
        current_node.last_child();
        
        // end() 6 times on last range
        for (int i = 0; i < 6; ++i) {
            range4.end();
        }
        
        // first_child() again
        current_node.first_child();
        
        // end() twice on last range
        range4.end();
        range4.end();
        
        // begin() on last range
        range4.begin();
        
        // end() on last range
        range4.end();
        
        // end() again on last range
        range4.end();
        
        // begin() on last range
        range4.begin();
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}