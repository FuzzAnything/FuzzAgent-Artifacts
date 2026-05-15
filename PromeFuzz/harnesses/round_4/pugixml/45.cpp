// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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

static void fuzz_target(const uint8_t* data, size_t size) {
    // Ensure we have at least one byte for null termination if needed
    if (size == 0) return;
    
    // Create null-terminated string for XML content
    std::string xml_content(reinterpret_cast<const char*>(data), size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    // Get root node (document node)
    pugi::xml_node root = doc;
    
    // Call functions in specified order
    pugi::xml_node node1 = root.child("root");
    pugi::xml_node node2 = node1.first_child();
    pugi::xml_node node3 = node2.next_sibling();
    pugi::xml_node node4 = node3.next_sibling();
    pugi::xml_node node5 = node4.last_child();
    pugi::xml_node_type type1 = node5.type();
    const char* value1 = node5.value();
    pugi::xml_node_type type2 = node5.type();
    const char* value2 = node5.value();
    pugi::xml_node node6 = node5.last_child();
    pugi::xml_node node7 = node6.first_child();
    pugi::xml_node node8 = node7.first_child();
    
    // Use the results to avoid optimization
    (void)result;
    (void)type1;
    (void)value1;
    (void)type2;
    (void)value2;
    (void)node8;
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    fuzz_target(Data, Size);
    return 0;
}