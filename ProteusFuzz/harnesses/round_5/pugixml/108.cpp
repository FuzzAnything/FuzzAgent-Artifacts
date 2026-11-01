// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <string>
#include <vector>
#include <functional>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_108(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for loading
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    
    // Try loading from file
    if (!doc.load_file("./dummy_file")) {
        // If load fails, try parsing as string with error handling
        std::string buffer(reinterpret_cast<const char*>(Data), Size);
        pugi::xml_parse_result result = doc.load_string(buffer.c_str());
        if (!result) {
            // Still proceed to test with potentially empty or partially parsed doc
        }
    }

    // Get root node or create one if empty
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Add some initial attributes to have something to work with
    root.append_attribute("attr1").set_value("value1");
    root.append_attribute("attr2").set_value("value2");
    root.append_attribute("attr3").set_value("value3");

    // Test 1: insert_copy_before
    {
        pugi::xml_attribute proto = root.attribute("attr1");
        pugi::xml_attribute attr = root.attribute("attr2");
        if (proto && attr) {
            root.insert_copy_before(proto, attr);
        }
        
        // Edge cases
        pugi::xml_attribute empty_attr;
        root.insert_copy_before(proto, empty_attr);
        root.insert_copy_before(empty_attr, attr);
        root.insert_copy_before(empty_attr, empty_attr);
    }

    // Test 2: insert_attribute_after
    {
        pugi::xml_attribute attr = root.attribute("attr2");
        if (attr) {
            root.insert_attribute_after("new_attr", attr);
        }
        
        // Edge cases
        pugi::xml_attribute empty_attr;
        root.insert_attribute_after("new_attr2", empty_attr);
        // Fixed: Do not pass nullptr as the name argument to avoid assertion failure in pugixml
        // root.insert_attribute_after(nullptr, attr); 
    }

    // Test 3: attribute
    {
        pugi::xml_attribute attr = root.attribute("attr1");
        (void)attr; // Suppress unused warning
        
        // Test with string_view_t
        std::string_view a_str = "attr2";
        pugi::xml_attribute attr2 = root.attribute(a_str);
        (void)attr2;
        
        // Test with non-existent attribute
        pugi::xml_attribute attr3 = root.attribute("nonexistent");
        (void)attr3;
    }

    // Test 4: last_attribute
    {
        pugi::xml_attribute last = root.last_attribute();
        (void)last;
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_attribute last_empty = empty_node.last_attribute();
        (void)last_empty;
    }

    // Test 5: first_attribute
    {
        pugi::xml_attribute first = root.first_attribute();
        (void)first;
        
        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_attribute first_empty = empty_node.first_attribute();
        (void)first_empty;
    }

    // Test 6: find_attribute with various predicates
    {
        // Predicate 1: Find attribute with specific name
        auto pred1 = [](const pugi::xml_attribute& attr) {
            return attr.name() && std::strcmp(attr.name(), "attr1") == 0;
        };
        pugi::xml_attribute found1 = root.find_attribute(pred1);
        (void)found1;

        // Predicate 2: Find attribute with specific value
        auto pred2 = [](const pugi::xml_attribute& attr) {
            return attr.value() && std::strcmp(attr.value(), "value2") == 0;
        };
        pugi::xml_attribute found2 = root.find_attribute(pred2);
        (void)found2;

        // Predicate 3: Find attribute with name length > 0
        auto pred3 = [](const pugi::xml_attribute& attr) {
            return attr.name() && strlen(attr.name()) > 0;
        };
        pugi::xml_attribute found3 = root.find_attribute(pred3);
        (void)found3;

        // Predicate 4: Always return false
        auto pred4 = [](const pugi::xml_attribute& attr) {
            (void)attr;
            return false;
        };
        pugi::xml_attribute found4 = root.find_attribute(pred4);
        (void)found4;

        // Test on empty node
        pugi::xml_node empty_node;
        pugi::xml_attribute found_empty = empty_node.find_attribute(pred1);
        (void)found_empty;
    }

    return 0;
}