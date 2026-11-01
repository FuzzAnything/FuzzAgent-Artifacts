// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7055:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7119:30 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6030:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_69(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<root><child>text</child></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.child("root");
    pugi::xml_node child = root.child("child");
    pugi::xml_attribute attr = child.attribute("attr");
    
    // Create attribute if it doesn't exist
    if (!attr) {
        attr = child.append_attribute("attr");
    }

    pugi::xml_text text = child.text();

    // Prepare string_view from input data
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);

    // 1. pugi::xml_node::set_name
    // Try on root and child nodes
    root.set_name(input_str);
    child.set_name(input_str);
    
    // 2. pugi::xml_text::set
    // Overload 1: string_view_t
    text.set(input_str);
    // Overload 2: bool
    text.set(Size > 0);

    // 3. pugi::xml_attribute::set_value
    // Only if attribute exists
    if (attr) {
        attr.set_value(input_str);
    }

    // 4. pugi::xml_node::set_value
    // Overload 1: string_view_t
    child.set_value(input_str);
    // Overload 2: const char_t*
    child.set_value(reinterpret_cast<const pugi::char_t*>(Data));

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}