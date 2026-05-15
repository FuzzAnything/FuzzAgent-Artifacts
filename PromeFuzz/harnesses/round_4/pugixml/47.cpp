// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <sstream>
#include <string_view>
#include "pugixml.hpp"

static std::string create_null_terminated_string(const uint8_t* data, size_t size) {
    if (size == 0) return "";
    return std::string(reinterpret_cast<const char*>(data), size);
}

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t* Data, size_t Size) {
    if (Size < 2) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Create a reference node to insert before
    pugi::xml_node ref_node = root.append_child(pugi::node_element);
    
    // First insert_child_before
    size_t type1 = Data[0] % (pugi::node_doctype + 1);
    pugi::xml_node new_node1 = root.insert_child_before(static_cast<pugi::xml_node_type>(type1), ref_node);
    
    // First first_child
    pugi::xml_node first_child1 = root.first_child();
    
    // append_attribute
    if (first_child1) {
        std::string attr_name1 = create_null_terminated_string(Data, Size);
        pugi::xml_attribute attr = first_child1.append_attribute(attr_name1.c_str());
    }
    
    // Second insert_child_before
    size_t type2 = Data[1] % (pugi::node_doctype + 1);
    pugi::xml_node new_node2 = root.insert_child_before(static_cast<pugi::xml_node_type>(type2), ref_node);
    
    // Second first_child
    pugi::xml_node first_child2 = root.first_child();
    
    // set_value
    if (first_child2) {
        std::string value_str = create_null_terminated_string(Data, Size);
        first_child2.set_value(value_str.c_str());
    }
    
    // save
    try {
        std::stringstream ss;
        pugi::xml_writer_stream writer(ss);
        doc.save(writer, "  ", pugi::format_default, pugi::encoding_auto);
    } catch (...) {
        // Ignore exceptions during saving
    }
    
    return 0;
}