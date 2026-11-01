// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
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

static bool parse_xml_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Create a string from the input data, ensuring null termination
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Ensure null termination for pugixml
    if (xml_str.back() != '\0') {
        xml_str += '\0';
    }
    return doc.load_string(xml_str.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    
    if (!parse_xml_document(Data, Size, doc)) {
        // If parsing fails, try to create a minimal document manually to test the API
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        // If child doesn't exist, create it to ensure we have a node to work with
        child_node = root.append_child("child");
    }

    // 2. pugi::xml_node::append_attribute (First call)
    pugi::xml_attribute attr1 = child_node.append_attribute("attr1");

    // 3. pugi::xml_node::append_attribute (Second call)
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");

    // 4. pugi::xml_attribute::set_value (unsigned long long overload)
    if (attr1) {
        attr1.set_value(static_cast<unsigned long long>(0));
    }

    // 5. pugi::xml_node::append_attribute (Third call)
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");

    // 6. pugi::xml_node::append_attribute (Fourth call)
    pugi::xml_attribute attr4 = child_node.append_attribute("attr4");

    // 7. pugi::xml_attribute::set_value (long long overload)
    if (attr4) {
        attr4.set_value(static_cast<long long>(0));
    }

    return 0;
}