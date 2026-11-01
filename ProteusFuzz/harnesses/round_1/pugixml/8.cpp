// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_8(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    
    // Ensure the input data is null-terminated before passing to load_string
    // to prevent heap-buffer-overflow when pugixml calculates string length.
    std::string input_str(reinterpret_cast<const char*>(Data), Size);
    // Append a null terminator to ensure safety
    input_str += '\0';

    if (!doc.load_string(input_str.c_str())) {
        // If parsing fails, create a minimal valid document to ensure we can test the APIs
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have some attributes to work with for copy operations
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    if (attr1) attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    if (attr2) attr2.set_value("value2");
    
    pugi::xml_attribute attr3 = root.append_attribute("attr3");
    if (attr3) attr3.set_value("value3");

    // 1. pugi::xml_node::insert_attribute_after
    // Insert a new attribute after attr1
    if (attr1) {
        pugi::xml_attribute new_attr_after = root.insert_attribute_after("new_attr_after", attr1);
        if (new_attr_after) {
            new_attr_after.set_value("after_val");
        }
    }

    // 2. pugi::xml_node::insert_attribute_before
    // Insert a new attribute before attr3
    if (attr3) {
        pugi::xml_attribute new_attr_before = root.insert_attribute_before("new_attr_before", attr3);
        if (new_attr_before) {
            new_attr_before.set_value("before_val");
        }
    }

    // 3. pugi::xml_node::first_child
    pugi::xml_node child1 = root.first_child();

    // 4. pugi::xml_node::first_child
    pugi::xml_node child2 = root.first_child();

    // 5. pugi::xml_node::first_child
    pugi::xml_node child3 = root.first_child();

    // 6. pugi::xml_node::first_child
    pugi::xml_node child4 = root.first_child();

    // 7. pugi::xml_node::append_copy
    // Copy attr2 to the end
    if (attr2) {
        pugi::xml_attribute copied_attr_append = root.append_copy(attr2);
        if (copied_attr_append) {
            copied_attr_append.set_name("copied_append");
        }
    }

    // 8. pugi::xml_node::prepend_copy
    // Copy attr3 to the beginning
    if (attr3) {
        pugi::xml_attribute copied_attr_prepend = root.prepend_copy(attr3);
        if (copied_attr_prepend) {
            copied_attr_prepend.set_name("copied_prepend");
        }
    }

    // 9. pugi::xml_node::insert_copy_after
    // Copy attr1 after attr2
    if (attr1 && attr2) {
        pugi::xml_attribute copied_attr_after = root.insert_copy_after(attr1, attr2);
        if (copied_attr_after) {
            copied_attr_after.set_name("copied_after");
        }
    }

    // 10. pugi::xml_node::insert_copy_before
    // Copy attr3 before attr1
    if (attr3 && attr1) {
        pugi::xml_attribute copied_attr_before = root.insert_copy_before(attr3, attr1);
        if (copied_attr_before) {
            copied_attr_before.set_name("copied_before");
        }
    }

    return 0;
}