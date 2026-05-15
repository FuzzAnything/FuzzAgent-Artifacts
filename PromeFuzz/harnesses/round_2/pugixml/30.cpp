// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create XML document with some structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some child nodes using data from fuzzer input
    // We'll use the first few bytes to determine how many children to create
    size_t num_children = (Data[0] % 10) + 1;
    size_t data_index = 1;
    
    for (size_t i = 0; i < num_children && data_index < Size; i++) {
        // Create child name from fuzzer data (ensure null termination)
        size_t name_len = (Data[data_index] % 16) + 1;
        if (name_len > Size - data_index - 1) {
            name_len = Size - data_index - 1;
        }
        if (name_len == 0) break;
        
        char* child_name = new char[name_len + 1];
        for (size_t j = 0; j < name_len && data_index < Size; j++) {
            child_name[j] = static_cast<char>(Data[data_index++]);
        }
        child_name[name_len] = '\0';
        
        root.append_child(child_name);
        delete[] child_name;
        
        if (data_index >= Size) break;
    }
    
    // Create additional child with specific name for testing
    root.append_child("test_child");
    root.append_child("another_child");
    
    // Get some child nodes to use with remove_child(const xml_node&) overload
    pugi::xml_node child1 = root.child("test_child");
    pugi::xml_node child2 = root.child("another_child");
    
    // Now perform the sequence of operations as specified
    // We'll use various string types for the remove_child overloads
    
    // 1. remove_child (const char*)
    root.remove_child("test_child");
    
    // 2. remove_child (string_view)
    std::string_view sv1("another_child");
    root.remove_child(sv1);
    
    // 3. child (const char*)
    // Create null-terminated string from fuzzer data
    if (data_index < Size) {
        size_t name_len = (Data[data_index] % 16) + 1;
        if (name_len > Size - data_index - 1) {
            name_len = Size - data_index - 1;
        }
        if (name_len > 0) {
            char* search_name1 = new char[name_len + 1];
            for (size_t j = 0; j < name_len && data_index < Size; j++) {
                search_name1[j] = static_cast<char>(Data[data_index++]);
            }
            search_name1[name_len] = '\0';
            root.child(search_name1);
            delete[] search_name1;
        }
    }
    
    // 4. child (const char*) - use fixed name
    root.child("nonexistent");
    
    // 5. remove_child (const xml_node&)
    if (!child1.empty()) {
        root.remove_child(child1);
    }
    
    // 6. remove_child (const char*)
    root.remove_child("dummy");
    
    // 7. child (const char*)
    if (data_index < Size) {
        size_t name_len = (Data[data_index] % 16) + 1;
        if (name_len > Size - data_index - 1) {
            name_len = Size - data_index - 1;
        }
        if (name_len > 0) {
            char* search_name2 = new char[name_len + 1];
            for (size_t j = 0; j < name_len && data_index < Size; j++) {
                search_name2[j] = static_cast<char>(Data[data_index++]);
            }
            search_name2[name_len] = '\0';
            root.child(search_name2);
            delete[] search_name2;
        }
    }
    
    // 8. remove_child (string_view)
    std::string_view sv2("random");
    root.remove_child(sv2);
    
    // 9. child (const char*)
    root.child("root");
    
    // 10. remove_child (const xml_node&)
    if (!child2.empty()) {
        root.remove_child(child2);
    }
    
    // 11. remove_child (const char*)
    root.remove_child("child");
    
    // 12. remove_child (string_view)
    std::string str1("test");
    std::string_view sv3(str1);
    root.remove_child(sv3);
    
    // 13. remove_child (const char*)
    root.remove_child("");
    
    // 14. remove_child (const xml_node&)
    // Create a dummy node
    pugi::xml_node dummy_node;
    root.remove_child(dummy_node);
    
    // 15. remove_child (string_view)
    std::string_view sv4("");
    root.remove_child(sv4);
    
    return 0;
}