// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
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
#include <string_view>

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.load_string("<root/>");
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, load a minimal valid document to ensure the fuzzer continues
        doc.load_string("<root/>");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = create_document_from_input(Data, Size);
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::attribute
    std::string_view attr_name1 = "id";
    pugi::xml_attribute attr1 = root.attribute(attr_name1);

    // 2. pugi::xml_node::child
    std::string_view child_name = "child";
    pugi::xml_node child_node = root.child(child_name);

    // 3. pugi::xml_node::attribute (on child or root if child is empty)
    pugi::xml_node node_for_attr2 = child_node.empty() ? root : child_node;
    std::string_view attr_name2 = "name";
    pugi::xml_attribute attr2 = node_for_attr2.attribute(attr_name2);

    // 4. pugi::xml_node::attribute
    std::string_view attr_name3 = "value";
    pugi::xml_attribute attr3 = node_for_attr2.attribute(attr_name3);

    // 5. pugi::xml_node::attribute
    std::string_view attr_name4 = "type";
    pugi::xml_attribute attr4 = node_for_attr2.attribute(attr_name4);

    // 6. pugi::xml_node::last_attribute
    pugi::xml_attribute last_attr = node_for_attr2.last_attribute();

    // 7. pugi::xml_node::attribute
    std::string_view attr_name5 = "class";
    pugi::xml_attribute attr5 = node_for_attr2.attribute(attr_name5);

    // 8. pugi::xml_node::attribute
    std::string_view attr_name6 = "style";
    pugi::xml_attribute attr6 = node_for_attr2.attribute(attr_name6);

    // No specific cleanup needed for xml_document as it manages its own memory
    // and destructors are called automatically.

    return 0;
}