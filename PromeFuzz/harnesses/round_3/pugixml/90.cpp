// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <fstream>
#include <string>
#include <string_view>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummyPredicate(const pugi::xml_node& node) {
    return !node.empty();
}

extern "C" int LLVMFuzzerTestOneInput_90(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;
    
    // Create dummy file for potential file-based parsing
    writeDummyFile(Data, Size);
    
    // Test with empty node
    {
        pugi::xml_node empty_node;
        empty_node.find_node(dummyPredicate);
        empty_node.last_child();
        empty_node.child("test");
        empty_node.previous_sibling();
        empty_node.first_child();
        empty_node.find_child(dummyPredicate);
    }
    
    // Test with parsed XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result) {
        pugi::xml_node root = doc.root().first_child();
        
        // Test on root node
        root.find_node(dummyPredicate);
        root.last_child();
        
        // Prepare null-terminated string for child() function
        std::string child_name;
        if (Size > 0) {
            size_t name_len = std::min(Size, static_cast<size_t>(100));
            child_name.assign(reinterpret_cast<const char*>(Data), name_len);
            if (!child_name.empty() && child_name.back() != '\0') {
                child_name.push_back('\0');
            }
        }
        
        if (!child_name.empty()) {
            root.child(child_name.c_str());
        }
        
        root.previous_sibling();
        root.first_child();
        root.find_child(dummyPredicate);
        
        // Test on child nodes
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            child.find_node(dummyPredicate);
            child.last_child();
            
            if (!child_name.empty()) {
                child.child(child_name.c_str());
            }
            
            child.previous_sibling();
            child.first_child();
            child.find_child(dummyPredicate);
        }
        
        // Test with string_view
        std::string_view sv(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        root.child(sv);
        root.previous_sibling(sv);
    }
    
    // Test with file loading
    {
        pugi::xml_document doc2;
        doc2.load_file("./dummy_file");
        if (doc2) {
            pugi::xml_node root2 = doc2.root().first_child();
            root2.find_node(dummyPredicate);
            root2.last_child();
            
            std::string child_name2;
            if (Size > 0) {
                size_t name_len = std::min(Size, static_cast<size_t>(100));
                child_name2.assign(reinterpret_cast<const char*>(Data), name_len);
                if (!child_name2.empty() && child_name2.back() != '\0') {
                    child_name2.push_back('\0');
                }
            }
            
            if (!child_name2.empty()) {
                root2.child(child_name2.c_str());
            }
            
            root2.previous_sibling();
            root2.first_child();
            root2.find_child(dummyPredicate);
        }
    }
    
    return 0;
}