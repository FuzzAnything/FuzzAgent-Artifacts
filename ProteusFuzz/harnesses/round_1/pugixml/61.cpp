// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5533:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5561:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5466:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::as_string at pugixml.cpp:5302:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
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
#include <vector>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_61(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;

    // Parse the input as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal document manually to test attribute functions
        pugi::xml_node root = doc.append_child("root");
        pugi::xml_attribute attr = root.append_attribute("test");
        
        // Test set_value with various types
        attr.set_value("initial");
        attr.set_value(42);
        attr.set_value(3.14);
        attr.set_value(true);
        attr.set_value(false);
        
        // Test as_string
        const pugi::char_t* str = attr.as_string();
        (void)str;
        const pugi::char_t* str_def = attr.as_string("default");
        (void)str_def;

        // Test value
        const pugi::char_t* val = attr.value();
        (void)val;

        // Test name
        const pugi::char_t* name = attr.name();
        (void)name;

        // Test set_name
        attr.set_name("renamed");
        attr.set_name("renamed2", 8);

        // Test attribute retrieval
        pugi::xml_attribute found = root.attribute("renamed2");
        if (found) {
            const pugi::char_t* fval = found.value();
            (void)fval;
            const pugi::char_t* fname = found.name();
            (void)fname;
        }

        // Test with null attribute
        pugi::xml_attribute null_attr;
        bool res1 = null_attr.set_value("test");
        (void)res1;
        const pugi::char_t* null_val = null_attr.value();
        (void)null_val;
        const pugi::char_t* null_name = null_attr.name();
        (void)null_name;
        bool res2 = null_attr.set_name("newname");
        (void)res2;
        const pugi::char_t* null_str = null_attr.as_string("default");
        (void)null_str;

        return 0;
    }

    // Iterate over nodes and attributes to fuzz the target functions
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
            // Fuzz set_value with string from input
            std::string input_str(reinterpret_cast<const char*>(Data), Size);
            attr.set_value(input_str.c_str(), input_str.size());
            
            // Fuzz as_string
            const pugi::char_t* str = attr.as_string();
            (void)str;
            const pugi::char_t* str_def = attr.as_string("default");
            (void)str_def;

            // Fuzz value
            const pugi::char_t* val = attr.value();
            (void)val;

            // Fuzz name
            const pugi::char_t* name = attr.name();
            (void)name;

            // Fuzz set_name
            attr.set_name("newname");
            attr.set_name("newname2", 8);
        }

        // Fuzz attribute retrieval
        std::string attr_name("test_attr");
        pugi::xml_attribute found = node.attribute(attr_name.c_str());
        if (found) {
            const pugi::char_t* fval = found.value();
            (void)fval;
            const pugi::char_t* fname = found.name();
            (void)fname;
            const pugi::char_t* fstr = found.as_string("default");
            (void)fstr;
        }
    }

    return 0;
}