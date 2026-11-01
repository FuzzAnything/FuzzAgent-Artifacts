// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
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
#include <string>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    // Try to parse as XML string
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_document* doc = new pugi::xml_document();
    
    // Parse the input as XML
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    
    if (!result) {
        // If parsing fails, create an empty document
        delete doc;
        doc = new pugi::xml_document();
    }
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_15(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) {
        return 0;
    }
    
    try {
        pugi::xml_node root = *doc;
        
        // 1. append_child
        pugi::xml_node node1 = root.append_child(pugi::node_element);
        
        // 2. first_child
        pugi::xml_node child1 = root.first_child();
        
        // 3. first_child (again)
        pugi::xml_node child2 = root.first_child();
        
        // 4. insert_child_after
        // Only attempt if we have a valid child to insert after
        if (child1 && child1 != child2) {
            pugi::xml_node node2 = root.insert_child_after(pugi::node_element, child1);
        } else if (child1) {
            pugi::xml_node node2 = root.insert_child_after(pugi::node_element, child1);
        }
        
        // 5. first_child
        pugi::xml_node child3 = root.first_child();
        
        // 6. insert_child_before
        // Only attempt if we have a valid child to insert before
        if (child3) {
            pugi::xml_node node3 = root.insert_child_before(pugi::node_element, child3);
        }
        
        // 7. first_child
        pugi::xml_node child4 = root.first_child();
        
        // 8. prepend_child
        pugi::xml_node node4 = root.prepend_child(pugi::node_element);
        
    } catch (...) {
        // Handle any exceptions
    }
    
    delete doc;
    return 0;
}