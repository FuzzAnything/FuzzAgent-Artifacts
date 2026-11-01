// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    // Step 1: Prepare environment
    if (!doc.load_string("<root/>")) {
        return 0;
    }
    
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // 1. pugi::xml_node::append_child
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    
    // 2. pugi::xml_node::first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. pugi::xml_node::first_child (again)
    pugi::xml_node first2 = root.first_child();
    
    // 4. pugi::xml_node::insert_child_after
    pugi::xml_node node2 = root.insert_child_after(pugi::node_element, first1);
    
    // 5. pugi::xml_node::first_child
    pugi::xml_node first3 = root.first_child();
    
    // 6. pugi::xml_node::insert_child_before
    pugi::xml_node node3 = root.insert_child_before(pugi::node_element, first2);
    
    // 7. pugi::xml_node::first_child
    pugi::xml_node first4 = root.first_child();
    
    // 8. pugi::xml_node::prepend_child
    pugi::xml_node node4 = root.prepend_child(pugi::node_element);
    
    // Cleanup is handled automatically by RAII when doc goes out of scope
    (void)node1;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)first1;
    (void)first2;
    (void)first3;
    (void)first4;
    
    return 0;
}