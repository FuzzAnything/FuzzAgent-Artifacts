// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5547:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse the input as XML
    pugi::xml_parse_result result = doc->load_buffer(data, size);
    if (result.status == pugi::status_ok) {
        return doc;
    }

    // If parsing fails, create a minimal document with a root element
    // Note: xml_document does not have a clear() method in older versions or specific configurations.
    // We simply assign a new root to the existing empty document.
    pugi::xml_node root = doc->append_child("root");
    if (!root) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->root();
    if (!root) {
        delete doc;
        return 0;
    }

    // Helper to safely append attribute and set value
    auto test_append_set = [&](pugi::xml_node& node, const char* name, const char* value) {
        pugi::xml_attribute attr = node.append_attribute(name);
        if (attr) {
            attr.set_value(value);
        }
    };

    // Helper to safely append attribute with string_view and set value
    auto test_append_set_sv = [&](pugi::xml_node& node, std::string_view name, std::string_view value) {
        pugi::xml_attribute attr = node.append_attribute(name);
        if (attr) {
            attr.set_value(value);
        }
    };

    // 1. append_attribute (const char_t*) -> set_value (const char_t*)
    test_append_set(root, "attr1", "value1");

    // 2. append_attribute (string_view_t) -> set_value (string_view_t)
    std::string_view sv_name = "attr2";
    std::string_view sv_value = "value2";
    test_append_set_sv(root, sv_name, sv_value);

    // 3. append_attribute (const char_t*) -> set_value (bool)
    pugi::xml_attribute attr3 = root.append_attribute("attr3");
    if (attr3) {
        attr3.set_value(true);
    }

    // 4. append_attribute (string_view_t) -> set_value (float)
    std::string_view sv_name4 = "attr4";
    pugi::xml_attribute attr4 = root.append_attribute(sv_name4);
    if (attr4) {
        attr4.set_value(3.14f);
    }

    // 5. append_attribute (const char_t*) -> set_value (double)
    pugi::xml_attribute attr5 = root.append_attribute("attr5");
    if (attr5) {
        attr5.set_value(2.718281828);
    }

    // 6. append_attribute (string_view_t) -> set_value (const char_t*)
    std::string_view sv_name6 = "attr6";
    pugi::xml_attribute attr6 = root.append_attribute(sv_name6);
    if (attr6) {
        attr6.set_value("value6");
    }

    // Cleanup
    delete doc;
    return 0;
}