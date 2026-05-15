// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_children at pugixml.cpp:6607:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_buffer at pugixml.cpp:6628:42 in pugixml.hpp
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

static void initializeNodeName(std::string& name, const uint8_t* data, size_t size, size_t max_len = 100) {
    if (size == 0) {
        name = "node";
        return;
    }
    
    size_t len = std::min(size, max_len);
    name.assign(reinterpret_cast<const char*>(data), len);
    
    // Ensure null termination for C-string usage
    if (!name.empty() && name.back() != '\0') {
        name.push_back('\0');
    }
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* Data, size_t Size) {
    // Initialize document and root node
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    if (root.empty()) {
        return 0;
    }
    
    // Prepare node names from fuzzer input
    std::string node_name1, node_name2, child_name;
    
    // Use different portions of input for different names
    size_t segment = Size / 3;
    initializeNodeName(node_name1, Data, std::min(segment, Size));
    initializeNodeName(node_name2, Data + segment, std::min(segment, Size - segment));
    initializeNodeName(child_name, Data + 2*segment, Size - 2*segment);
    
    // 1. First append_child (with node_type)
    pugi::xml_node_type node_type = static_cast<pugi::xml_node_type>(
        (Size > 0 ? Data[0] : 0) % (pugi::node_doctype + 1)
    );
    pugi::xml_node child1 = root.append_child(node_type);
    
    // 2. Second append_child (with name)
    pugi::xml_node child2 = root.append_child(node_name1.c_str());
    
    // 3. First child call
    pugi::xml_node found1 = root.child(child_name.c_str());
    
    // 4. First append_buffer
    if (Size > 0) {
        pugi::xml_parse_result result1 = root.append_buffer(Data, Size);
        (void)result1; // Result intentionally unused
    }
    
    // 5. Second child call
    pugi::xml_node found2 = root.child(node_name2.c_str());
    
    // 6. Third child call
    pugi::xml_node found3 = root.child(node_name1.c_str());
    
    // 7. remove_children
    bool removed = root.remove_children();
    (void)removed; // Result intentionally unused
    
    // 8. Fourth child call
    pugi::xml_node found4 = root.child(child_name.c_str());
    
    // 9. Second append_buffer
    if (Size > 0) {
        // Use a different portion or the same data
        size_t buffer_size = std::min(Size, static_cast<size_t>(100));
        pugi::xml_parse_result result2 = root.append_buffer(Data, buffer_size);
        (void)result2; // Result intentionally unused
    }
    
    // 10. Fifth child call
    pugi::xml_node found5 = root.child(node_name1.c_str());
    
    // All nodes are automatically cleaned up when doc goes out of scope
    return 0;
}