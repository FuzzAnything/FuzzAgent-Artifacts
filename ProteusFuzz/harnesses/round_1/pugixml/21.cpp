// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::internal_object at pugixml.cpp:5384:52 in pugixml.hpp
// pugi::xml_node::internal_object at pugixml.cpp:6841:42 in pugixml.hpp
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
#include <cstring>
#include <fstream>

static std::string generate_xml_from_data(const uint8_t* data, size_t size) {
    std::string result;
    result.reserve(size * 2 + 100);
    result += "<root>";
    for (size_t i = 0; i < size; ++i) {
        // Escape basic characters to ensure valid-ish XML structure
        uint8_t c = data[i];
        if (c < 32 || c > 126) {
            result += " ";
        } else if (c == '<' || c == '>') {
            result += " ";
        } else {
            result += static_cast<char>(c);
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // 1. Prepare the environment
    std::string xml_str = generate_xml_from_data(Data, Size);
    
    pugi::xml_document doc;
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try to parse a minimal valid document to test API on valid nodes
        // This ensures we can still test the API logic even if input is garbage
        doc.load_string("<root attr='val'><child/></root>");
        if (!doc.load_string("<root attr='val'><child/></root>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        return 0;
    }

    // 2. Invoke target functions in the specified order

    // 1. pugi::xml_node::child
    // Use a string_view_t parameter hint: std::string_view a_str; 
    // However, the signature takes const char_t*. We will construct a safe string.
    std::string child_name = "child";
    pugi::xml_node child_node = root.child(child_name.c_str());

    // 2. pugi::xml_node::first_attribute
    pugi::xml_attribute attr = child_node.empty() ? root.first_attribute() : child_node.first_attribute();

    // 3. pugi::xml_node::first_child
    pugi::xml_node first_child_node = child_node.empty() ? root.first_child() : child_node.first_child();

    // 4. pugi::xml_node::internal_object (on child_node or root if empty)
    pugi::xml_node_struct* node_struct_1 = child_node.empty() ? root.internal_object() : child_node.internal_object();
    (void)node_struct_1; // Suppress unused warning

    // 5. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct_1 = attr.empty() ? nullptr : attr.internal_object();
    (void)attr_struct_1; // Suppress unused warning

    // 6. pugi::xml_node::internal_object (on first_child_node or fallback)
    pugi::xml_node_struct* node_struct_2 = first_child_node.empty() ? root.internal_object() : first_child_node.internal_object();
    (void)node_struct_2;

    // 7. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct_3 = root.internal_object();
    (void)node_struct_3;

    // 8. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct_4 = root.internal_object();
    (void)node_struct_4;

    // 9. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct_2 = attr.empty() ? nullptr : attr.internal_object();
    (void)attr_struct_2;

    // 10. pugi::xml_node::internal_object
    pugi::xml_node_struct* node_struct_5 = root.internal_object();
    (void)node_struct_5;

    // 11. pugi::xml_attribute::internal_object
    pugi::xml_attribute_struct* attr_struct_3 = attr.empty() ? nullptr : attr.internal_object();
    (void)attr_struct_3;

    // 3. Cleanup is handled by RAII (pugi::xml_document destructor)
    // No explicit cleanup needed for the objects created above.

    return 0;
}