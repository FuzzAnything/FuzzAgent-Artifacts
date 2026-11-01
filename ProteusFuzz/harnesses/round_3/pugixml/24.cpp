// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
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

static std::string create_xml_string(const uint8_t* data, size_t size) {
    // Ensure the data is valid UTF-8 or ASCII for XML
    // We will create a simple XML structure with the fuzzed data as content
    std::string result = "<root><child>";
    // Limit the size to prevent huge allocations
    size_t max_len = 10000;
    size_t len = (size < max_len) ? size : max_len;
    
    // Only include printable characters or escape them to ensure valid XML
    for (size_t i = 0; i < len; ++i) {
        unsigned char c = data[i];
        if (c == '<' || c == '>') {
            result += "&lt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c >= 32 && c < 127) {
            result += static_cast<char>(c);
        } else {
            // Skip non-printable characters to maintain valid XML
            continue;
        }
    }
    result += "</child></root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Create a valid XML string from the input data
    std::string xml_str = create_xml_string(Data, Size);
    
    // Load the XML string
    if (!doc.load_string(xml_str.c_str())) {
        return 0;
    }

    // Step 1: pugi::xml_node::child
    // Find the "child" node
    pugi::xml_node child_node = doc.child("child");
    if (!child_node) {
        return 0;
    }

    // Step 2: pugi::xml_node::append_attribute
    pugi::xml_attribute attr1 = child_node.append_attribute("attr1");
    if (!attr1) return 0;

    // Step 3: pugi::xml_attribute::set_value (3 overloads)
    // Overload 1: const char_t*
    attr1.set_value("value1");
    
    // Overload 2: unsigned int
    attr1.set_value(static_cast<unsigned int>(42));
    
    // Overload 3: int
    attr1.set_value(static_cast<int>(-10));

    // Step 4: pugi::xml_node::append_attribute
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");
    if (!attr2) return 0;

    // Step 5: pugi::xml_node::append_attribute
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");
    if (!attr3) return 0;

    // Step 6: pugi::xml_attribute::set_value (3 overloads)
    attr2.set_value("value2");
    attr2.set_value(static_cast<unsigned int>(100));
    attr2.set_value(static_cast<int>(-20));

    // Step 7: pugi::xml_node::append_attribute
    pugi::xml_attribute attr4 = child_node.append_attribute("attr4");
    if (!attr4) return 0;

    // Step 8: pugi::xml_node::append_attribute
    pugi::xml_attribute attr5 = child_node.append_attribute("attr5");
    if (!attr5) return 0;

    // Step 9: pugi::xml_attribute::set_value (3 overloads)
    attr4.set_value("value4");
    attr4.set_value(static_cast<unsigned int>(200));
    attr4.set_value(static_cast<int>(-30));

    // Cleanup is handled automatically by RAII (xml_document destructor)
    return 0;
}