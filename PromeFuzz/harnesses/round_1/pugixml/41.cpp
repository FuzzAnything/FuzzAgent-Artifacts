// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Create a document with a root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial child nodes for testing
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Use fuzzer input to create node names
    // Ensure we have at least some data for names
    if (Size < 1) return 0;
    
    // Create null-terminated strings from fuzzer input
    size_t name_len = Size > 100 ? 100 : Size;
    char* name1 = new char[name_len + 1];
    memcpy(name1, Data, name_len);
    name1[name_len] = '\0';
    
    // Create another name with different offset
    size_t offset = name_len / 2;
    size_t name2_len = Size - offset > 50 ? 50 : Size - offset;
    char* name2 = new char[name2_len + 1];
    if (name2_len > 0) {
        memcpy(name2, Data + offset, name2_len);
    }
    name2[name2_len] = '\0';
    
    // Create string_view objects
    std::string_view sv1(name1, name_len);
    std::string_view sv2(name2, name2_len);
    
    try {
        // 1. First insert_child_after (string_view overload)
        pugi::xml_node new_node1 = root.insert_child_after(sv1, child1);
        
        // 2. Second insert_child_after (const char* overload)
        pugi::xml_node new_node2 = root.insert_child_after(name2, child2);
        
        // 3. First child() call
        pugi::xml_node found1 = root.child(name1);
        
        // 4. Second child() call
        pugi::xml_node found2 = root.child(name2);
        
        // 5. Third child() call - using a different name
        pugi::xml_node found3 = root.child("child3");
        
        // Create more nodes for subsequent insert operations
        pugi::xml_node child4 = root.append_child("child4");
        pugi::xml_node child5 = root.append_child("child5");
        pugi::xml_node child6 = root.append_child("child6");
        pugi::xml_node child7 = root.append_child("child7");
        pugi::xml_node child8 = root.append_child("child8");
        
        // 6. Third insert_child_after (string_view)
        pugi::xml_node new_node3 = root.insert_child_after(sv1, child4);
        
        // 7. Fourth insert_child_after (const char*)
        pugi::xml_node new_node4 = root.insert_child_after(name2, child5);
        
        // 8. Fifth insert_child_after (string_view)
        pugi::xml_node new_node5 = root.insert_child_after(sv2, child6);
        
        // 9. Sixth insert_child_after (const char*)
        pugi::xml_node new_node6 = root.insert_child_after(name1, child7);
        
        // 10. Seventh insert_child_after (string_view)
        pugi::xml_node new_node7 = root.insert_child_after(sv1, child8);
        
        // 11. Eighth insert_child_after (const char*)
        // Use existing child for insertion
        if (found1) {
            pugi::xml_node new_node8 = root.insert_child_after(name2, found1);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup
    delete[] name1;
    delete[] name2;
    
    return 0;
}