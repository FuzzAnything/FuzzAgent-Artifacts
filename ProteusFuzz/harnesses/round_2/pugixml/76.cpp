// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Parse the input data as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, we can still test with an empty document
        // or try to parse a minimal valid XML if the input is too short
        if (Size < 4) return 0;
        // Try to load a minimal valid XML if the input is garbage
        if (!doc.load_string("<root/>")) {
            return 0;
        }
    }

    // Get a node to work with
    pugi::xml_node root = doc.child("root");
    if (root.empty()) {
        root = doc; // Fallback to document root
    }

    // 1. Test offset_debug
    ptrdiff_t offset = root.offset_debug();
    (void)offset; // Suppress unused variable warning

    // 2. Test set_value overloads
    // Prepare a string from the input data, ensuring null-termination
    size_t str_len = Size;
    if (Size > 0 && Data[Size - 1] == '\0') {
        str_len = Size - 1;
    }
    
    // Create a safe buffer for the string
    std::string safe_str(reinterpret_cast<const char*>(Data), str_len);
    const pugi::char_t* c_str = safe_str.c_str();

    // Test set_value with const char_t*
    root.set_value(c_str);
    
    // Test set_value with const char_t* and size
    root.set_value(c_str, str_len);

    // 3. Test set_name overloads
    // Test set_name with const char_t*
    root.set_name(c_str);
    
    // Test set_name with const char_t* and size
    root.set_name(c_str, str_len);

    // 4. Test xml_text::set
    pugi::xml_text text_node = root.text();
    if (text_node) {
        text_node.set(c_str);
    } else {
        // If no text node exists, try to append a text node or just set on the node itself if it's a text node
        // We can try to set text on the root if it happens to be a text node (e.g. if the document was just text).
        // xml_text constructor takes xml_node_struct*, but we can construct xml_text from xml_node via internal pointer if needed.
        // However, simpler approach: just create a text node if needed or skip if not applicable.
        // Let's try to append a child text node and set its value.
        pugi::xml_node text_child = root.append_child(pugi::node_pcdata);
        if (text_child) {
            pugi::xml_text text_obj = text_child.text();
            text_obj.set(c_str);
        }
    }

    return 0;
}