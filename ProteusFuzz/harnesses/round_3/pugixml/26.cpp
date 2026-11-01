// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xpath_node::attribute at pugixml.cpp:12725:41 in pugixml.hpp
// pugi::xpath_node::parent at pugixml.cpp:12730:36 in pugixml.hpp
// pugi::xpath_node::node at pugixml.cpp:12720:36 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_26(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Create a dummy XML string from input data, ensuring it's valid UTF-8/UTF-16
    // We wrap the input in a simple XML structure to ensure parsing succeeds
    char buffer[1024];
    size_t max_size = (Size < sizeof(buffer) - 20) ? Size : sizeof(buffer) - 20;
    
    // Escape potential XML special characters in input to prevent parsing errors
    // Simple approach: just copy a safe prefix or use the data as text content
    std::string safe_input;
    for (size_t i = 0; i < max_size; ++i) {
        unsigned char c = Data[i];
        if (c == '<' || c == '>' || c == '&' || c == '"') {
            safe_input += "\\x";
            char hex[4];
            snprintf(hex, sizeof(hex), "%02x", c);
            safe_input += hex;
        } else if (c >= 32 && c < 127) {
            safe_input += c;
        }
    }
    
    std::string xml_str = "<root><item>" + safe_input + "</item><item attr='test'>data</item></root>";
    
    if (!doc.load_string(xml_str.c_str())) {
        return 0;
    }

    // Initialize xpath_node_set
    pugi::xpath_node_set set;
    
    // 1. pugi::xpath_node_set::type
    pugi::xpath_node_set::type_t type_val = set.type();
    
    // 2. pugi::xpath_node_set::sort
    // Sort in both ascending and descending to explore states
    set.sort(false);
    set.sort(true);
    
    // 3. pugi::xpath_node_set::empty
    bool is_empty = set.empty();
    
    // Prepare an xpath_node for the remaining calls
    // We need a valid node to construct an xpath_node
    pugi::xml_node root_node = doc.root();
    if (!root_node) {
        return 0;
    }
    
    pugi::xpath_node xpath_node_obj(root_node);
    
    // 4. pugi::xpath_node::attribute
    pugi::xml_attribute attr = xpath_node_obj.attribute();
    
    // 5. pugi::xpath_node::parent
    pugi::xml_node parent_node = xpath_node_obj.parent();
    
    // 6. pugi::xpath_node::node
    pugi::xml_node node_from_xpath = xpath_node_obj.node();
    
    // 7. pugi::xml_node::root
    pugi::xml_node root_from_node = node_from_xpath.root();
    
    // Cleanup is automatic via RAII for pugi objects
    
    // Return success
    return 0;
}