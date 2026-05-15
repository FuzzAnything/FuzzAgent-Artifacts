// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static std::string make_null_terminated(const uint8_t* data, size_t size, size_t max_len = 100) {
    std::string result;
    if (size == 0) return "node";
    
    size_t len = size;
    if (len > max_len) len = max_len;
    
    result.assign(reinterpret_cast<const char*>(data), len);
    
    // Ensure null termination
    if (result.empty() || result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    // Create document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial child nodes to have something to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Prepare string data from fuzzer input
    std::vector<std::string> strings;
    size_t offset = 0;
    size_t chunk_size = Size / 10;
    if (chunk_size == 0) chunk_size = 1;
    
    for (int i = 0; i < 10 && offset < Size; i++) {
        size_t this_chunk = chunk_size;
        if (offset + this_chunk > Size) this_chunk = Size - offset;
        strings.push_back(make_null_terminated(Data + offset, this_chunk));
        offset += this_chunk;
    }
    
    // Ensure we have at least 10 strings
    while (strings.size() < 10) {
        strings.push_back("node");
    }
    
    try {
        // 1. pugi::xml_node::insert_child_before
        pugi::xml_node new_node1 = root.insert_child_before(strings[0].c_str(), child2);
        
        // 2. pugi::xml_node::child
        pugi::xml_node found1 = root.child(strings[1].c_str());
        
        // 3. pugi::xml_node::child
        pugi::xml_node found2 = root.child(strings[2].c_str());
        
        // 4. pugi::xml_node::child
        pugi::xml_node found3 = root.child(strings[3].c_str());
        
        // 5. pugi::xml_node::insert_child_before (first overload - string_view)
        std::string_view sv1(strings[4].c_str());
        pugi::xml_node new_node2 = root.insert_child_before(sv1, child3);
        
        // 6. pugi::xml_node::insert_child_before (second overload - const char*)
        pugi::xml_node new_node3 = root.insert_child_before(strings[5].c_str(), child1);
        
        // 7. pugi::xml_node::insert_child_before (string_view)
        std::string_view sv2(strings[6].c_str());
        pugi::xml_node new_node4 = root.insert_child_before(sv2, new_node1);
        
        // 8. pugi::xml_node::insert_child_before (const char*)
        pugi::xml_node new_node5 = root.insert_child_before(strings[7].c_str(), new_node2);
        
        // 9. pugi::xml_node::insert_child_before (string_view)
        std::string_view sv3(strings[8].c_str());
        pugi::xml_node new_node6 = root.insert_child_before(sv3, new_node3);
        
        // 10. pugi::xml_node::insert_child_before (const char*)
        pugi::xml_node new_node7 = root.insert_child_before(strings[9].c_str(), child2);
        
        // Try to access some of the inserted nodes
        if (new_node1) {
            pugi::xml_node test = new_node1.child("test");
        }
        if (new_node2) {
            pugi::xml_node test = new_node2.child("test");
        }
        
        // Also test child() on inserted nodes
        if (found1) {
            pugi::xml_node nested = found1.child("nested");
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}