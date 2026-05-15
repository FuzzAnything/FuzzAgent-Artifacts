// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

static bool predicate_true(pugi::xml_node) {
    return true;
}

static bool predicate_false(pugi::xml_node) {
    return false;
}

static bool predicate_by_name(pugi::xml_node node) {
    return node.name() == std::string("target");
}

static bool predicate_by_value(pugi::xml_node node) {
    return node.value() == std::string("test");
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t *Data, size_t Size) {
    // Write data to dummy file
    std::ofstream file("./dummy_file", std::ios::binary);
    if (!file.is_open()) {
        return 0;
    }
    file.write(reinterpret_cast<const char*>(Data), Size);
    file.close();

    // Parse XML from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    
    // Test find_node with various predicates
    if (root) {
        root.find_node(predicate_true);
        root.find_node(predicate_false);
        root.find_node(predicate_by_name);
        root.find_node(predicate_by_value);
        
        // Test find_child with various predicates
        root.find_child(predicate_true);
        root.find_child(predicate_false);
        root.find_child(predicate_by_name);
        root.find_child(predicate_by_value);
        
        // Test parent
        pugi::xml_node child = root.first_child();
        if (child) {
            child.parent();
            
            // Test first_child and last_child
            pugi::xml_node first = child.first_child();
            pugi::xml_node last = child.last_child();
            
            // Test child with name lookup
            if (Size > 0) {
                // Create null-terminated string from fuzz data
                size_t name_len = Size > 100 ? 100 : Size;
                char* name = new char[name_len + 1];
                std::memcpy(name, Data, name_len);
                name[name_len] = '\0';
                
                child.child(name);
                delete[] name;
            }
            
            // Test with std::string_view
            std::string_view sv("test");
            child.child(sv);
        }
    }
    
    // Test on empty nodes
    pugi::xml_node empty;
    empty.find_node(predicate_true);
    empty.find_child(predicate_true);
    empty.parent();
    empty.first_child();
    empty.last_child();
    empty.child("nonexistent");
    
    // Test on various nodes in the tree
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        node.find_node(predicate_true);
        node.find_child(predicate_false);
        node.parent();
        node.first_child();
        node.last_child();
        
        if (Size > 0) {
            size_t name_len = Size > 50 ? 50 : Size;
            char* name = new char[name_len + 1];
            std::memcpy(name, Data, name_len);
            name[name_len] = '\0';
            
            node.child(name);
            delete[] name;
        }
    }
    
    return 0;
}