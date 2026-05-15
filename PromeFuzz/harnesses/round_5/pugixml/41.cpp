// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Create XML document and initial structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial child nodes to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Prepare node names from fuzzer input
    static std::vector<std::string> node_names;
    node_names.clear();
    
    // Split input into null-terminated strings for node names
    const uint8_t* current = Data;
    const uint8_t* end = Data + Size;
    
    while (current < end) {
        // Find next null byte or end of input
        const uint8_t* null_pos = current;
        while (null_pos < end && *null_pos != 0) {
            ++null_pos;
        }
        
        size_t len = null_pos - current;
        if (len > 0) {
            node_names.emplace_back(reinterpret_cast<const char*>(current), len);
        } else {
            node_names.emplace_back("");
        }
        
        // Move past null byte if found
        current = (null_pos < end) ? null_pos + 1 : end;
    }
    
    // Ensure we have at least some names
    if (node_names.empty()) {
        node_names.emplace_back("default");
    }
    
    // Helper to get a name from node_names with bounds checking
    static auto get_name = [&](size_t index) -> const char* {
        if (index < node_names.size()) {
            return node_names[index].c_str();
        }
        return node_names[0].c_str();
    };
    
    // Helper to get a string_view from node_names with bounds checking
    static auto get_name_sv = [&](size_t index) -> std::string_view {
        if (index < node_names.size()) {
            return std::string_view(node_names[index]);
        }
        return std::string_view(node_names[0]);
    };
    
    // Execute API calls in specified order
    try {
        // 1. pugi::xml_node::insert_child_before (const char* version)
        root.insert_child_before(get_name(0), child2);
        
        // 2. pugi::xml_node::child
        root.child(get_name(1));
        
        // 3. pugi::xml_node::child
        root.child(get_name(2));
        
        // 4. pugi::xml_node::child
        root.child(get_name(3));
        
        // 5. pugi::xml_node::insert_child_before (string_view_t version)
        root.insert_child_before(get_name_sv(4), child3);
        
        // 6. pugi::xml_node::insert_child_before (const char* version)
        root.insert_child_before(get_name(5), child1);
        
        // 7. pugi::xml_node::insert_child_before (string_view_t version)
        root.insert_child_before(get_name_sv(6), child2);
        
        // 8. pugi::xml_node::insert_child_before (const char* version)
        root.insert_child_before(get_name(7), child3);
        
        // 9. pugi::xml_node::insert_child_before (string_view_t version)
        root.insert_child_before(get_name_sv(8), child1);
        
        // 10. pugi::xml_node::insert_child_before (const char* version)
        root.insert_child_before(get_name(9 % node_names.size()), child2);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}