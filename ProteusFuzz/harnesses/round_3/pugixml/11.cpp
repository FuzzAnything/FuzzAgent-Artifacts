// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Try to parse input as XML
    pugi::xml_parse_result result = doc->load_buffer(data, size);
    if (!result.status) {
        // If parsing fails, create a minimal valid document
        doc->reset();
        pugi::xml_node root = doc->append_child("root");
        if (!root) {
            delete doc;
            return nullptr;
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_11(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->root();
    if (!root) {
        root = doc->append_child("root");
        if (!root) {
            delete doc;
            return 0;
        }
    }

    // Helper to safely append attribute and set value
    auto fuzz_attribute = [&](const char* name, pugi::xml_attribute& attr) {
        attr = root.append_attribute(name);
        if (!attr) return;

        // Invoke all set_value overloads
        attr.set_value(42.0);
        attr.set_value(42);
        attr.set_value(true);
        attr.set_value("test_value");
        std::string_view sv("string_view_test");
        attr.set_value(sv);
    };

    // First sequence: append_attribute (char_t*) -> set_value
    pugi::xml_attribute attr1;
    fuzz_attribute("attr1", attr1);

    // Second sequence: append_attribute (string_view_t) -> set_value
    pugi::xml_attribute attr2;
    std::string_view sv_name("attr2");
    attr2 = root.append_attribute(sv_name);
    if (attr2) {
        attr2.set_value(3.14);
        attr2.set_value(3);
        attr2.set_value(false);
        attr2.set_value("float_test");
        std::string_view sv_val("sv_val");
        attr2.set_value(sv_val);
    }

    // Third sequence
    pugi::xml_attribute attr3;
    fuzz_attribute("attr3", attr3);

    // Fourth sequence
    pugi::xml_attribute attr4;
    std::string_view sv_name4("attr4");
    attr4 = root.append_attribute(sv_name4);
    if (attr4) {
        attr4.set_value(100.5);
        attr4.set_value(100);
        attr4.set_value(true);
        attr4.set_value("int_test");
        std::string_view sv_val4("sv_val4");
        attr4.set_value(sv_val4);
    }

    // Fifth sequence
    pugi::xml_attribute attr5;
    fuzz_attribute("attr5", attr5);

    // Sixth sequence
    pugi::xml_attribute attr6;
    std::string_view sv_name6("attr6");
    attr6 = root.append_attribute(sv_name6);
    if (attr6) {
        attr6.set_value(0.0);
        attr6.set_value(0);
        attr6.set_value(false);
        attr6.set_value("zero_test");
        std::string_view sv_val6("sv_val6");
        attr6.set_value(sv_val6);
    }

    delete doc;
    return 0;
}