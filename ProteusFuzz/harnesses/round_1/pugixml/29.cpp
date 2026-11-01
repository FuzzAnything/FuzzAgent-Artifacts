// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
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

static std::string generate_xml_from_fuzz(const uint8_t* data, size_t size) {
    std::string result("<root>");
    for (size_t i = 0; i < size; ++i) {
        unsigned char c = data[i];
        if (c >= 32 && c < 127 && c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
            result += static_cast<char>(c);
        } else {
            result += " ";
        }
    }
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    std::string xml_content = generate_xml_from_fuzz(Data, Size);
    
    if (!doc.load_string(xml_content.c_str())) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    // 1. pugi::xml_node::child
    // Use std::string_view as hinted for string_view_t parameters
    std::string_view a_str("root");
    pugi::xml_node child_node = root.child(a_str);

    // If child_node is empty, try to use root directly or create a new structure
    pugi::xml_node target_node = child_node;
    if (!target_node) {
        target_node = root;
    }

    // 2. pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = target_node.append_attribute("attr1");
    if (!attr1) return 0;

    // 3. pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = target_node.append_attribute("attr2");
    if (!attr2) return 0;

    // 4. pugi::xml_attribute::set_value (long overload)
    long val_long = 42;
    if (Size > 0) {
        val_long = static_cast<long>(Data[0]);
    }
    attr1.set_value(val_long);

    // 5. pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = target_node.append_attribute("attr3");
    if (!attr3) return 0;

    // 6. pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = target_node.append_attribute("attr4");
    if (!attr4) return 0;

    // 7. pugi::xml_attribute::set_value (unsigned long overload)
    unsigned long val_ulong = 100;
    if (Size > 1) {
        val_ulong = static_cast<unsigned long>(Data[1]);
    }
    attr2.set_value(val_ulong);

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}