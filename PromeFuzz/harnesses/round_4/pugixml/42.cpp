// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
#include <vector>

static std::string make_null_terminated(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    std::string result(reinterpret_cast<const char*>(data), size);
    if (result.back() != '\0') result.push_back('\0');
    return result;
}

static std::string_view make_string_view(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    return std::string_view(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_42(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create initial child nodes to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Prepare some string data from fuzzer input
    size_t offset = 0;
    std::vector<std::string> strings;
    std::vector<std::string_view> string_views;
    
    // Extract 10 strings from the fuzzer input
    for (int i = 0; i < 10 && offset < Size; i++) {
        size_t chunk_size = (Size - offset) / (10 - i);
        if (chunk_size == 0) chunk_size = 1;
        if (offset + chunk_size > Size) chunk_size = Size - offset;
        
        strings.push_back(make_null_terminated(Data + offset, chunk_size));
        string_views.push_back(make_string_view(Data + offset, chunk_size));
        offset += chunk_size;
    }
    
    // Ensure we have at least some strings
    if (strings.empty()) {
        strings.push_back("");
        string_views.push_back("");
    }
    
    // 1. First insert_child_after
    pugi::xml_node new_node1 = root.insert_child_after(strings[0 % strings.size()].c_str(), child1);
    
    // 2. First child call
    pugi::xml_node found1 = root.child(strings[1 % strings.size()].c_str());
    
    // 3. Second child call
    pugi::xml_node found2 = root.child(strings[2 % strings.size()].c_str());
    
    // 4. Third child call
    pugi::xml_node found3 = root.child(strings[3 % strings.size()].c_str());
    
    // 5. Second insert_child_after (string_view overload)
    pugi::xml_node new_node2 = root.insert_child_after(string_views[4 % string_views.size()], child2);
    
    // 6. Third insert_child_after (char* overload)
    pugi::xml_node new_node3 = root.insert_child_after(strings[5 % strings.size()].c_str(), child3);
    
    // 7. Fourth insert_child_after (string_view overload)
    if (new_node1) {
        pugi::xml_node new_node4 = root.insert_child_after(string_views[6 % string_views.size()], new_node1);
    }
    
    // 8. Fifth insert_child_after (char* overload)
    if (new_node2) {
        pugi::xml_node new_node5 = root.insert_child_after(strings[7 % strings.size()].c_str(), new_node2);
    }
    
    // 9. Sixth insert_child_after (string_view overload)
    if (new_node3) {
        pugi::xml_node new_node6 = root.insert_child_after(string_views[8 % string_views.size()], new_node3);
    }
    
    // 10. Seventh insert_child_after (char* overload)
    if (found1) {
        pugi::xml_node new_node7 = root.insert_child_after(strings[9 % strings.size()].c_str(), found1);
    }
    
    return 0;
}