// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
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
#include <cstring>
#include <string>

static bool parse_xml_from_fuzz_input(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    
    // Ensure the input is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (xml_str.empty()) {
        return false;
    }
    
    // Try to parse the XML string
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the fuzz input as XML
    if (!parse_xml_from_fuzz_input(Data, Size, doc)) {
        // If parsing fails, try with a minimal valid XML structure
        std::string fallback_xml = "<?xml version=\"1.0\"?><root attr=\"value\"><child/></root>";
        if (!doc.load_string(fallback_xml.c_str())) {
            return 0;
        }
    }

    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.first_child();
        if (!root) {
            return 0;
        }
    }

    // 1. pugi::xml_node::child
    std::string_view child_name = "child";
    pugi::xml_node child_node = root.child(child_name.data());

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child_node = root.first_child();

    // 4. pugi::xml_node::internal_object (on root)
    pugi::xml_node_struct* root_internal = root.internal_object();

    // 5. pugi::xml_attribute::internal_object (on first_attr)
    pugi::xml_attribute_struct* attr_internal = first_attr.internal_object();

    // 6. pugi::xml_node::internal_object (on child_node)
    pugi::xml_node_struct* child_internal = child_node.internal_object();

    // 7. pugi::xml_node::internal_object (on first_child_node)
    pugi::xml_node_struct* first_child_internal = first_child_node.internal_object();

    // 8. pugi::xml_node::internal_object (on root again)
    pugi::xml_node_struct* root_internal_again = root.internal_object();

    // 9. pugi::xml_attribute::internal_object (on first_attr again)
    pugi::xml_attribute_struct* attr_internal_again = first_attr.internal_object();

    // 10. pugi::xml_node::internal_object (on child_node again)
    pugi::xml_node_struct* child_internal_again = child_node.internal_object();

    // 11. pugi::xml_attribute::internal_object (on first_attr again)
    pugi::xml_attribute_struct* attr_internal_final = first_attr.internal_object();

    // Use the pointers to prevent compiler optimization
    volatile void* unused = reinterpret_cast<void*>(root_internal);
    unused = reinterpret_cast<void*>(attr_internal);
    unused = reinterpret_cast<void*>(child_internal);
    unused = reinterpret_cast<void*>(first_child_internal);
    unused = reinterpret_cast<void*>(root_internal_again);
    unused = reinterpret_cast<void*>(attr_internal_again);
    unused = reinterpret_cast<void*>(child_internal_again);
    unused = reinterpret_cast<void*>(attr_internal_final);

    return 0;
}