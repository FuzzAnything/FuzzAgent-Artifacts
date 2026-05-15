// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
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
#include <string_view>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static bool dummyPredicate(const pugi::xml_node&) {
    return false;
}

static bool dummyChildPredicate(const pugi::xml_node&) {
    return false;
}

extern "C" int LLVMFuzzerTestOneInput_77(const uint8_t* data, size_t size) {
    if (size == 0) {
        return 0;
    }

    writeDummyFile(data, size);
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    if (!result) {
        doc.reset();
    }
    
    pugi::xml_node root = doc.root();
    
    // Test find_node with dummy predicate
    root.find_node(dummyPredicate);
    
    // Test previous_sibling
    if (root) {
        pugi::xml_node prev = root.previous_sibling();
        if (prev) {
            prev.previous_sibling();
        }
    }
    
    // Test first_child
    pugi::xml_node first = root.first_child();
    if (first) {
        first.first_child();
    }
    
    // Test child with name
    if (size > 0) {
        size_t name_len = size % 256;
        if (name_len > 0) {
            char* name = new char[name_len + 1];
            memcpy(name, data, name_len > size ? size : name_len);
            name[name_len] = '\0';
            
            std::string_view name_sv(name, name_len);
            root.child(name_sv.data());
            
            delete[] name;
        }
    }
    
    // Test last_child
    pugi::xml_node last = root.last_child();
    if (last) {
        last.last_child();
    }
    
    // Test find_child with dummy predicate
    root.find_child(dummyChildPredicate);
    
    // Additional traversal to explore more states
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        child.find_node(dummyPredicate);
        child.previous_sibling();
        child.first_child();
        child.last_child();
        child.find_child(dummyChildPredicate);
        
        if (size > 1) {
            size_t child_name_len = (size % 128) + 1;
            if (child_name_len > 0) {
                char* child_name = new char[child_name_len + 1];
                memcpy(child_name, data + (size % (size - 1)), 
                       child_name_len > (size - 1) ? (size - 1) : child_name_len);
                child_name[child_name_len] = '\0';
                
                std::string_view child_name_sv(child_name, child_name_len);
                child.child(child_name_sv.data());
                
                delete[] child_name;
            }
        }
    }
    
    return 0;
}