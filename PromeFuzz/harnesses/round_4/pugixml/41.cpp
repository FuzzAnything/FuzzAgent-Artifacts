// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6387:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6350:34 in pugixml.hpp
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
#include <string_view>

static void initialize_document(pugi::xml_document& doc) {
    // Create a basic XML structure to work with
    pugi::xml_node root = doc.append_child("root");
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    
    // Add some attributes
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
}

static void cleanup_document(pugi::xml_document& doc) {
    doc.reset();
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    // Create a null-terminated string from the input data
    char* null_terminated_data = new char[Size + 1];
    if (!null_terminated_data) return 0;
    
    memcpy(null_terminated_data, Data, Size);
    null_terminated_data[Size] = '\0';
    
    // Create string_view from the data
    std::string_view data_view(reinterpret_cast<const char*>(Data), Size);
    
    // Initialize the XML document
    pugi::xml_document doc;
    initialize_document(doc);
    
    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        delete[] null_terminated_data;
        return 0;
    }
    
    try {
        // Create some child nodes to use as reference nodes
        pugi::xml_node ref_node1 = root.child("child1");
        pugi::xml_node ref_node2 = root.child("child2");
        pugi::xml_node ref_node3 = root.child("child3");
        
        // 1. First insert_child_before (string_view_t overload)
        if (ref_node2) {
            root.insert_child_before(data_view, ref_node2);
        }
        
        // 2. First child lookup
        pugi::xml_node child1 = root.child(null_terminated_data);
        
        // 3. Second child lookup
        pugi::xml_node child2 = root.child(null_terminated_data);
        
        // 4. Third child lookup
        pugi::xml_node child3 = root.child(null_terminated_data);
        
        // 5. Second insert_child_before (const char_t* overload)
        if (ref_node3) {
            root.insert_child_before(null_terminated_data, ref_node3);
        }
        
        // 6. Third insert_child_before (string_view_t overload)
        if (ref_node1) {
            root.insert_child_before(data_view, ref_node1);
        }
        
        // 7. Fourth insert_child_before (const char_t* overload)
        if (child1) {
            root.insert_child_before(null_terminated_data, child1);
        }
        
        // 8. Fifth insert_child_before (string_view_t overload)
        if (child2) {
            root.insert_child_before(data_view, child2);
        }
        
        // 9. Sixth insert_child_before (const char_t* overload)
        if (child3) {
            root.insert_child_before(null_terminated_data, child3);
        }
        
        // 10. Seventh insert_child_before (string_view_t overload)
        // Use the root node itself as reference (edge case)
        root.insert_child_before(data_view, root);
        
    } catch (...) {
        // Handle any exceptions
    }
    
    // Cleanup
    cleanup_document(doc);
    delete[] null_terminated_data;
    
    return 0;
}