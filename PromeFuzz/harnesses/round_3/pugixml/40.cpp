// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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

static std::string make_null_terminated(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    size_t len = size;
    if (data[size-1] != '\0') {
        len = size + 1;
    }
    std::string result(reinterpret_cast<const char*>(data), std::min(size, len-1));
    if (result.empty() || result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

static std::string_view make_string_view(const uint8_t* data, size_t size) {
    return std::string_view(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_40(const uint8_t *Data, size_t Size) {
    if (Size < 4) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create initial structure with some child nodes
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Split input data for different string parameters
    size_t part_size = Size / 10;
    if (part_size == 0) part_size = 1;
    
    std::vector<std::string> null_terminated_strings;
    std::vector<std::string_view> string_views;
    
    for (size_t i = 0; i < 10; ++i) {
        size_t offset = i * part_size;
        size_t length = (i == 9) ? (Size - offset) : part_size;
        if (offset >= Size) {
            null_terminated_strings.push_back("");
            string_views.push_back(std::string_view());
            continue;
        }
        length = std::min(length, Size - offset);
        null_terminated_strings.push_back(make_null_terminated(Data + offset, length));
        string_views.push_back(make_string_view(Data + offset, length));
    }
    
    try {
        // 1. pugi::xml_node::insert_child_after (const char_t* overload)
        pugi::xml_node new_node1 = root.insert_child_after(null_terminated_strings[0].c_str(), child1);
        
        // 2. pugi::xml_node::child
        pugi::xml_node found1 = root.child(null_terminated_strings[1].c_str());
        
        // 3. pugi::xml_node::child
        pugi::xml_node found2 = root.child(null_terminated_strings[2].c_str());
        
        // 4. pugi::xml_node::child
        pugi::xml_node found3 = root.child(null_terminated_strings[3].c_str());
        
        // 5. pugi::xml_node::insert_child_after (string_view_t overload)
        pugi::xml_node new_node2 = root.insert_child_after(string_views[4], child2);
        
        // 6. pugi::xml_node::insert_child_after (const char_t* overload)
        pugi::xml_node new_node3 = root.insert_child_after(null_terminated_strings[5].c_str(), child3);
        
        // 7. pugi::xml_node::insert_child_after (string_view_t overload)
        pugi::xml_node new_node4 = root.insert_child_after(string_views[6], new_node1);
        
        // 8. pugi::xml_node::insert_child_after (const char_t* overload)
        pugi::xml_node new_node5 = root.insert_child_after(null_terminated_strings[7].c_str(), new_node2);
        
        // 9. pugi::xml_node::insert_child_after (string_view_t overload)
        pugi::xml_node new_node6 = root.insert_child_after(string_views[8], new_node3);
        
        // 10. pugi::xml_node::insert_child_after (const char_t* overload)
        pugi::xml_node new_node7 = root.insert_child_after(null_terminated_strings[9].c_str(), new_node4);
        
        // Try to use the found nodes to explore more states
        if (found1) {
            found1.child(null_terminated_strings[0].c_str());
        }
        if (found2) {
            found2.child(null_terminated_strings[1].c_str());
        }
        if (found3) {
            found3.child(null_terminated_strings[2].c_str());
        }
        
        // Try inserting after empty nodes
        pugi::xml_node empty_node;
        root.insert_child_after("test", empty_node);
        root.insert_child_after(std::string_view("test"), empty_node);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}