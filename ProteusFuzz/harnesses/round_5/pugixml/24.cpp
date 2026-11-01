// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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

static bool parse_xml_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    
    // Ensure null termination for safety
    std::string content(reinterpret_cast<const char*>(data), size);
    
    // Try parsing as UTF-8
    pugi::xml_parse_result result = doc.load_string(content.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Parse the input as XML
    if (!parse_xml_document(doc, Data, Size)) {
        // If parsing fails, try to create a minimal valid XML structure
        // to still exercise the API functions
        doc.append_child("root");
    }

    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root exists, create one
        root = doc.append_child("root");
    }

    // Add some initial children to ensure we have nodes to work with
    if (!root.child("child1")) {
        root.append_child("child1");
    }
    if (!root.child("child2")) {
        root.append_child("child2");
    }

    // Execute the sequence of API calls as specified
    pugi::xml_node n1 = root.child("child1");
    pugi::xml_node n2 = n1.child("subchild");
    pugi::xml_node n3 = n1.insert_copy_before(n2, n1);
    pugi::xml_node n4 = n1.child("subchild");
    pugi::xml_node n5 = n1.insert_copy_before(n4, n1);
    pugi::xml_node n6 = n1.child("subchild");
    pugi::xml_node n7 = n1.child("subchild");
    pugi::xml_node n8 = n1.child("subchild");
    pugi::xml_node n9 = n1.child("subchild");
    pugi::xml_node n10 = n1.insert_copy_before(n9, n1);
    pugi::xml_node n11 = n1.child("subchild");
    pugi::xml_node n12 = n1.first_child();
    const pugi::char_t* name1 = n12.name();
    pugi::xml_node n13 = n1.child("subchild");
    pugi::xml_node n14 = n1.insert_copy_before(n13, n1);
    pugi::xml_node n15 = n1.child("subchild");
    pugi::xml_node n16 = n1.first_child();
    pugi::xml_node n17 = n1.child("subchild");
    pugi::xml_node n18 = n1.last_child();
    const pugi::char_t* name2 = n18.name();
    pugi::xml_node n19 = n1.child("subchild");
    pugi::xml_node n20 = n1.insert_copy_before(n19, n1);
    pugi::xml_node n21 = n1.child("subchild");
    pugi::xml_node n22 = n1.first_child();
    pugi::xml_node n23 = n22.next_sibling();
    pugi::xml_node n24 = n22.child("subchild");
    pugi::xml_node n25 = n22.first_child();
    const pugi::char_t* value1 = n25.value();

    // Suppress unused variable warnings
    (void)n1; (void)n2; (void)n3; (void)n4; (void)n5; (void)n6; (void)n7;
    (void)n8; (void)n9; (void)n10; (void)n11; (void)n12; (void)n13;
    (void)n14; (void)n15; (void)n16; (void)n17; (void)n18; (void)n19;
    (void)n20; (void)n21; (void)n22; (void)n23; (void)n24; (void)n25;
    (void)name1; (void)name2; (void)value1;

    return 0;
}