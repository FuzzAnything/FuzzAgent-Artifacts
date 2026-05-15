// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_44(const uint8_t *Data, size_t Size) {
    // Initialize document and nodes
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create initial child nodes for insertion targets
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Use fuzzer data for node names
    // Ensure null-terminated strings for const char* parameters
    std::string name1, name2, name3, name4, name5, name6, name7, name8;
    
    if (Size > 0) {
        size_t len1 = Size % 256;
        if (len1 > 0) {
            name1.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len1, Size));
            name1 += '\0';
        }
    }
    
    if (Size > 1) {
        size_t len2 = (Size / 2) % 256;
        if (len2 > 0) {
            name2.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len2, Size));
            name2 += '\0';
        }
    }
    
    if (Size > 2) {
        size_t len3 = (Size / 3) % 256;
        if (len3 > 0) {
            name3.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len3, Size));
            name3 += '\0';
        }
    }
    
    if (Size > 3) {
        size_t len4 = (Size / 4) % 256;
        if (len4 > 0) {
            name4.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len4, Size));
            name4 += '\0';
        }
    }
    
    if (Size > 4) {
        size_t len5 = (Size / 5) % 256;
        if (len5 > 0) {
            name5.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len5, Size));
            name5 += '\0';
        }
    }
    
    if (Size > 5) {
        size_t len6 = (Size / 6) % 256;
        if (len6 > 0) {
            name6.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len6, Size));
            name6 += '\0';
        }
    }
    
    if (Size > 6) {
        size_t len7 = (Size / 7) % 256;
        if (len7 > 0) {
            name7.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len7, Size));
            name7 += '\0';
        }
    }
    
    if (Size > 7) {
        size_t len8 = (Size / 8) % 256;
        if (len8 > 0) {
            name8.assign(reinterpret_cast<const char*>(Data), 
                        std::min(len8, Size));
            name8 += '\0';
        }
    }
    
    // Default names if no data
    if (name1.empty()) name1 = "node1\0";
    if (name2.empty()) name2 = "node2\0";
    if (name3.empty()) name3 = "node3\0";
    if (name4.empty()) name4 = "node4\0";
    if (name5.empty()) name5 = "node5\0";
    if (name6.empty()) name6 = "node6\0";
    if (name7.empty()) name7 = "node7\0";
    if (name8.empty()) name8 = "node8\0";
    
    try {
        // 1. pugi::xml_node::insert_child_before (string_view_t overload)
        std::string_view sv1(name1.c_str(), name1.size() - 1);
        pugi::xml_node inserted1 = root.insert_child_before(sv1, child2);
        
        // 2. pugi::xml_node::child (const char_t*)
        pugi::xml_node found1 = root.child(name2.c_str());
        
        // 3. pugi::xml_node::child (const char_t*)
        pugi::xml_node found2 = root.child(name3.c_str());
        
        // 4. pugi::xml_node::child (const char_t*)
        pugi::xml_node found3 = root.child(name4.c_str());
        
        // 5. pugi::xml_node::insert_child_before (const char_t* overload)
        pugi::xml_node inserted2 = root.insert_child_before(name5.c_str(), child3);
        
        // 6. pugi::xml_node::insert_child_before (string_view_t overload)
        std::string_view sv2(name6.c_str(), name6.size() - 1);
        pugi::xml_node inserted3 = root.insert_child_before(sv2, child1);
        
        // 7. pugi::xml_node::insert_child_before (const char_t* overload)
        pugi::xml_node inserted4 = root.insert_child_before(name7.c_str(), inserted1);
        
        // 8. pugi::xml_node::insert_child_before (string_view_t overload)
        std::string_view sv3(name8.c_str(), name8.size() - 1);
        pugi::xml_node inserted5 = root.insert_child_before(sv3, inserted2);
        
        // 9. pugi::xml_node::insert_child_before (const char_t* overload)
        pugi::xml_node inserted6 = root.insert_child_before(name1.c_str(), inserted3);
        
        // 10. pugi::xml_node::insert_child_before (string_view_t overload)
        std::string_view sv4(name2.c_str(), name2.size() - 1);
        pugi::xml_node inserted7 = root.insert_child_before(sv4, inserted4);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}