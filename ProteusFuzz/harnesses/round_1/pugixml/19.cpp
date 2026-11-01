// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static pugi::xml_document* create_document_from_fuzz_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure the input is null-terminated for parsing
    if (size == 0) {
        delete doc;
        return nullptr;
    }

    // Create a buffer with null termination
    char* buffer = new char[size + 1];
    if (!buffer) {
        delete doc;
        return nullptr;
    }

    std::memcpy(buffer, data, size);
    buffer[size] = '\0';

    pugi::xml_parse_result result = doc->load_string(buffer);
    delete[] buffer;

    if (!result) {
        delete doc;
        return nullptr;
    }

    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_19(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_fuzz_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->root();
    if (!root) {
        delete doc;
        return 0;
    }

    // Ensure we have some attributes to remove
    if (!root.attribute("id")) {
        root.append_attribute("id").set_value("test");
    }
    if (!root.attribute("name")) {
        root.append_attribute("name").set_value("test");
    }
    if (!root.attribute("value")) {
        root.append_attribute("value").set_value("test");
    }

    // 1. remove_attribute (const char_t* name)
    root.remove_attribute("id");

    // 2. remove_attribute (string_view_t name)
    std::string_view sv_name = "name";
    root.remove_attribute(sv_name);

    // 3. remove_attribute (const xml_attribute& a)
    pugi::xml_attribute attr = root.attribute("value");
    if (attr) {
        root.remove_attribute(attr);
    }

    // 4. child (const char_t* name)
    pugi::xml_node child1 = root.child("child1");
    if (!child1) {
        child1 = root.append_child("child1");
    }

    // 5. child (const char_t* name)
    pugi::xml_node child2 = root.child("child2");
    if (!child2) {
        child2 = root.append_child("child2");
    }

    // Ensure attributes exist on child1 for removal
    if (!child1.attribute("attr1")) {
        child1.append_attribute("attr1").set_value("val1");
    }
    if (!child1.attribute("attr2")) {
        child1.append_attribute("attr2").set_value("val2");
    }

    // 6. remove_attribute (const char_t* name)
    child1.remove_attribute("attr1");

    // 7. remove_attribute (string_view_t name)
    std::string_view sv_attr2 = "attr2";
    child1.remove_attribute(sv_attr2);

    // 8. attribute (const char_t* name)
    pugi::xml_attribute attr8 = child1.attribute("nonexistent");
    (void)attr8; // Suppress unused warning

    // 9. remove_attribute (const char_t* name)
    child1.remove_attribute("nonexistent");

    // 10. attribute (const char_t* name)
    pugi::xml_attribute attr10 = child2.attribute("attr3");
    if (!attr10) {
        attr10 = child2.append_attribute("attr3");
        attr10.set_value("val3");
    }

    // 11. remove_attribute (const char_t* name)
    child2.remove_attribute("attr3");

    // 12. remove_attribute (string_view_t name)
    std::string_view sv_attr3 = "attr3";
    child2.remove_attribute(sv_attr3);

    // 13. remove_attribute (const xml_attribute& a)
    pugi::xml_attribute attr13 = child2.attribute("dummy");
    if (!attr13) {
        attr13 = child2.append_attribute("dummy");
        attr13.set_value("dummy_val");
    }
    child2.remove_attribute(attr13);

    // 14. remove_attribute (const char_t* name)
    child2.remove_attribute("dummy");

    delete doc;
    return 0;
}