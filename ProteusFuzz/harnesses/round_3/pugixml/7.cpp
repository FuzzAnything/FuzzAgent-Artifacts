// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
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
#include <cstring>

// Use pugi::char_t directly to match the library's character type
using char_t = pugi::char_t;

static const char_t* get_fuzz_name(const uint8_t* data, size_t size, size_t offset) {
    if (size == 0) return nullptr;
    if (offset >= size) return nullptr;
    
    // Find a null terminator or use a safe substring
    size_t len = 0;
    while (offset + len < size && data[offset + len] != 0 && len < 100) {
        len++;
    }
    
    if (len == 0) return nullptr;
    
    // Use a static buffer for the name (fuzzing context allows this)
    static char_t name_buf[101];
    for (size_t i = 0; i < len; ++i) {
        name_buf[i] = static_cast<char_t>(data[offset + i]);
    }
    name_buf[len] = 0;
    
    return name_buf;
}

extern "C" int LLVMFuzzerTestOneInput_7(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Load fuzzed data as XML
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal valid document
        doc.append_child("root");
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Step 1: insert_attribute_after
    // We need an existing attribute to insert after
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    if (attr1) {
        const char_t* name1 = get_fuzz_name(Data, Size, 0);
        if (name1) {
            root.insert_attribute_after(name1, attr1);
        }
    }

    // Step 2: insert_attribute_before
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    if (attr2) {
        const char_t* name2 = get_fuzz_name(Data, Size, 1);
        if (name2) {
            root.insert_attribute_before(name2, attr2);
        }
    }

    // Step 3: first_child (1st call)
    pugi::xml_node child1 = root.first_child();
    (void)child1; // Suppress unused variable warning

    // Step 4: first_child (2nd call)
    pugi::xml_node child2 = root.first_child();
    (void)child2;

    // Step 5: first_child (3rd call)
    pugi::xml_node child3 = root.first_child();
    (void)child3;

    // Step 6: first_child (4th call)
    pugi::xml_node child4 = root.first_child();
    (void)child4;

    // Step 7: append_copy
    // Need an attribute to copy
    pugi::xml_attribute proto_attr = root.attribute("attr1");
    if (!proto_attr) {
        proto_attr = root.append_attribute("proto_attr");
    }
    if (proto_attr) {
        root.append_copy(proto_attr);
    }

    // Step 8: prepend_copy
    if (proto_attr) {
        root.prepend_copy(proto_attr);
    }

    // Step 9: insert_copy_after
    pugi::xml_attribute ref_attr = root.attribute("attr2");
    if (!ref_attr) {
        ref_attr = root.append_attribute("ref_attr");
    }
    if (proto_attr && ref_attr) {
        root.insert_copy_after(proto_attr, ref_attr);
    }

    // Step 10: insert_copy_before
    if (proto_attr && ref_attr) {
        root.insert_copy_before(proto_attr, ref_attr);
    }

    return 0;
}