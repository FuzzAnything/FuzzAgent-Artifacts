// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // If loading fails, try to parse from string buffer as fallback
    if (!result) {
        // Ensure null termination for string parsing
        std::string buffer(reinterpret_cast<const char*>(Data), Size);
        buffer += '\0';
        result = doc.load_string(buffer.c_str());
        if (!result) {
            return 0;
        }
    }

    // Use doc itself as root if no child element exists or to ensure a valid node
    pugi::xml_node root = doc;
    // Try to get the first child element if available, otherwise stick with document root
    // xml_document inherits from xml_node, so we can use first_child() and check type
    pugi::xml_node child_elem = root.first_child();
    if (child_elem && child_elem.type() == pugi::node_element) {
        root = child_elem;
    }

    // 1. pugi::xml_node::child
    const char* child_name = "node";
    pugi::xml_node child_node = root.child(child_name);

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
    pugi::xml_node_struct* root_internal_2 = root.internal_object();

    // 9. pugi::xml_attribute::internal_object (on first_attr again)
    pugi::xml_attribute_struct* attr_internal_2 = first_attr.internal_object();

    // 10. pugi::xml_node::internal_object (on child_node again)
    pugi::xml_node_struct* child_internal_2 = child_node.internal_object();

    // 11. pugi::xml_attribute::internal_object (on first_attr again)
    pugi::xml_attribute_struct* attr_internal_3 = first_attr.internal_object();

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}