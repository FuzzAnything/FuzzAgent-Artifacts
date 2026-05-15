// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t max_len = 100) {
    if (size == 0) return "";
    size_t len = size < max_len ? size : max_len;
    std::string str(reinterpret_cast<const char*>(data), len);
    // Ensure null termination for C-string APIs
    if (str.find('\0') == std::string::npos) {
        str.push_back('\0');
    }
    return str;
}

extern "C" int LLVMFuzzerTestOneInput_49(const uint8_t* Data, size_t Size) {
    // Write data to dummy file for potential file-based parsing (not used directly here)
    write_dummy_file(Data, Size);
    
    // Create XML document and parse from buffer
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_ws_pcdata);
    
    // Get root node (or empty node if parsing failed)
    pugi::xml_node root = doc.root();
    
    // Prepare names for child/children calls
    std::vector<std::string> names;
    for (int i = 0; i < 12; ++i) { // Need names for 8 child calls + 4 children calls
        size_t offset = i * 10 % (Size > 0 ? Size : 1);
        size_t name_len = 10;
        if (offset + name_len > Size) name_len = Size > offset ? Size - offset : 0;
        names.push_back(create_null_terminated_string(Data + offset, name_len));
    }
    
    // Call sequence of functions
    // 8 calls to pugi::xml_node::child
    std::vector<pugi::xml_node> child_nodes;
    for (int i = 0; i < 8; ++i) {
        if (!names[i].empty()) {
            child_nodes.push_back(root.child(names[i].c_str()));
        } else {
            child_nodes.push_back(root.child(""));
        }
    }
    
    // 4 calls to pugi::xml_node::children
    std::vector<pugi::xml_object_range<pugi::xml_named_node_iterator>> children_ranges;
    for (int i = 0; i < 4; ++i) {
        if (!names[i+8].empty()) {
            children_ranges.push_back(root.children(names[i+8].c_str()));
        } else {
            children_ranges.push_back(root.children(""));
        }
    }
    
    // Remaining calls in sequence
    if (!children_ranges.empty()) {
        auto it1 = children_ranges[0].end();
        (void)it1;
    }
    
    pugi::xml_node first = root.first_child();
    (void)first;
    
    if (children_ranges.size() > 1) {
        auto it2 = children_ranges[1].end();
        (void)it2;
    }
    
    if (children_ranges.size() > 2) {
        auto it3 = children_ranges[2].end();
        (void)it3;
    }
    
    if (children_ranges.size() > 3) {
        auto it4 = children_ranges[3].end();
        (void)it4;
    }
    
    pugi::xml_node first2 = root.first_child();
    (void)first2;
    
    pugi::xml_node last = root.last_child();
    (void)last;
    
    for (int i = 0; i < 6; ++i) {
        if (!children_ranges.empty()) {
            auto it = children_ranges[i % children_ranges.size()].end();
            (void)it;
        }
    }
    
    pugi::xml_node first3 = root.first_child();
    (void)first3;
    
    for (int i = 0; i < 2; ++i) {
        if (!children_ranges.empty()) {
            auto it = children_ranges[i % children_ranges.size()].end();
            (void)it;
        }
    }
    
    if (!children_ranges.empty()) {
        auto begin1 = children_ranges[0].begin();
        (void)begin1;
    }
    
    if (children_ranges.size() > 1) {
        auto it5 = children_ranges[1].end();
        (void)it5;
    }
    
    if (!children_ranges.empty()) {
        auto begin2 = children_ranges[0].begin();
        (void)begin2;
    }
    
    return 0;
}