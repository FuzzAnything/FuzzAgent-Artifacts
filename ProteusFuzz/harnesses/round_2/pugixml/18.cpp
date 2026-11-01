// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
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

static bool parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return false;
    }
    std::string str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(str.c_str());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_18(const uint8_t *Data, size_t Size) {
    if (Size < 4) {
        return 0;
    }

    pugi::xml_document doc;
    if (!parse_document(doc, Data, Size)) {
        return 0;
    }

    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Prepare attributes for testing overloads
    // Use pugi::char_t to match the library's character type
    const pugi::char_t* attr_name_cstr = "test_attr";
    pugi::string_view_t attr_name_sv = "test_attr";
    pugi::xml_attribute attr_handle;

    // 1. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute(attr_name_cstr);

    // 2. pugi::xml_node::remove_attribute (const xml_attribute& a)
    // First try to get an attribute to remove by reference
    attr_handle = root.attribute(attr_name_cstr);
    root.remove_attribute(attr_handle);

    // 3. pugi::xml_node::child (const char_t* name)
    pugi::xml_node child1 = root.child("child_node");

    // 4. pugi::xml_node::child (const char_t* name)
    pugi::xml_node child2 = root.child("another_node");

    // 5. pugi::xml_node::remove_attribute (string_view_t name)
    root.remove_attribute(attr_name_sv);

    // 6. pugi::xml_node::remove_attribute (const char_t* name) - again
    root.remove_attribute("id");

    // 7. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr1 = root.attribute("type");

    // 8. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute("value");

    // 9. pugi::xml_node::attribute (const char_t* name)
    pugi::xml_attribute attr2 = root.attribute("name");

    // 10. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute("class");

    // 11. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute("style");

    // 12. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute("data");

    // 13. pugi::xml_node::remove_attribute (const char_t* name)
    root.remove_attribute("meta");

    return 0;
}