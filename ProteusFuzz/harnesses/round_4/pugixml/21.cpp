// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    // Write fuzzing data to a dummy file if needed, though we'll parse from string here
    // This is a placeholder in case file-based parsing is required in future variations
    (void)data;
    (void)size;
}

extern "C" int LLVMFuzzerTestOneInput_21(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Convert input data to a string for XML parsing
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());

    // Proceed even if parsing fails, as we want to test error handling paths
    pugi::xml_node root = doc.root();

    // Prepare string_view for attribute and child names
    std::string_view attr_name1 = "id";
    std::string_view child_name = "child";
    std::string_view attr_name2 = "class";
    std::string_view attr_name3 = "style";
    std::string_view attr_name4 = "data";
    std::string_view attr_name5 = "value";
    std::string_view attr_name6 = "test";

    // 1. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = root.attribute(attr_name1);

    // 2. pugi::xml_node::child
    pugi::xml_node child_node = root.child(child_name);

    // 3. pugi::xml_node::attribute (on child_node)
    pugi::xml_attribute attr2 = child_node.attribute(attr_name2);

    // 4. pugi::xml_node::attribute (on child_node again)
    pugi::xml_attribute attr3 = child_node.attribute(attr_name3);

    // 5. pugi::xml_node::attribute (on child_node again)
    pugi::xml_attribute attr4 = child_node.attribute(attr_name4);

    // 6. pugi::xml_node::last_attribute (on child_node)
    pugi::xml_attribute last_attr = child_node.last_attribute();

    // 7. pugi::xml_node::attribute (on child_node again)
    pugi::xml_attribute attr5 = child_node.attribute(attr_name5);

    // 8. pugi::xml_node::attribute (on child_node again)
    pugi::xml_attribute attr6 = child_node.attribute(attr_name6);

    // No explicit cleanup needed for pugixml as it uses RAII
    // Return 0 to indicate successful execution of the fuzzer test
    return 0;
}