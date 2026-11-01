// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
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
#include <cstring>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    (void)data;
    (void)size;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    // Load buffer with PUGIXML_PARSE_NO_EMPTY_ELEMENT_NODES to avoid some edge cases, 
    // but primarily we just want to ensure valid XML structure if possible.
    // If parsing fails, we start fresh.
    if (!doc.load_buffer(Data, Size)) {
        doc = pugi::xml_document();
    }

    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc.append_child("root");
    }

    // Helper to safely extract a null-terminated string from fuzz data
    // Returns a pointer to a static buffer or a safe empty string if invalid
    static char safe_buf[256];
    auto get_safe_str = [&](size_t offset, size_t max_len) -> const char* {
        if (offset >= Size) return "";
        size_t len = 0;
        while (offset + len < Size && len < max_len && Data[offset + len] != 0) {
            len++;
        }
        if (len == 0) return "";
        // Ensure null termination in our safe buffer
        if (len >= sizeof(safe_buf)) len = sizeof(safe_buf) - 1;
        memcpy(safe_buf, &Data[offset], len);
        safe_buf[len] = '\0';
        return safe_buf;
    };

    // Step 1: append_child (overload 1: const char_t* name)
    const char* name1 = get_safe_str(0, 50);
    pugi::xml_node child1 = root.append_child(name1);

    // Step 2: append_child (overload 2: xml_node_type type)
    pugi::xml_node child2 = root.append_child(pugi::node_element);

    // Step 3: append_child (overload 1)
    const char* name3 = get_safe_str(50, 50);
    pugi::xml_node child3 = root.append_child(name3);

    // Step 4: append_child (overload 2)
    pugi::xml_node child4 = root.append_child(pugi::node_element);

    // Step 5: append_attribute
    const char* attr_name1 = get_safe_str(100, 30);
    pugi::xml_attribute attr1 = child1.append_attribute(attr_name1);

    // Step 6: append_attribute
    const char* attr_name2 = get_safe_str(130, 30);
    pugi::xml_attribute attr2 = child2.append_attribute(attr_name2);

    // Step 7: append_attribute
    const char* attr_name3 = get_safe_str(160, 30);
    pugi::xml_attribute attr3 = child3.append_attribute(attr_name3);

    // Set values for attributes to make find_child_by_attribute meaningful
    // We use safe strings for values too
    if (!attr1.empty()) {
        const char* val1 = get_safe_str(190, 30);
        attr1.set_value(val1);
    }
    if (!attr2.empty()) {
        const char* val2 = get_safe_str(220, 30);
        attr2.set_value(val2);
    }
    if (!attr3.empty()) {
        const char* val3 = get_safe_str(250, 30);
        attr3.set_value(val3);
    }

    // Step 8: find_child_by_attribute (overload 1: attr_name, attr_value)
    const char* attr_name_find1 = get_safe_str(280, 30);
    const char* attr_val_find1 = get_safe_str(310, 30);
    pugi::xml_node found1 = root.find_child_by_attribute(attr_name_find1, attr_val_find1);
    (void)found1;

    // Step 9: find_child_by_attribute (overload 2: name, attr_name, attr_value)
    const char* name_find2 = get_safe_str(340, 30);
    const char* attr_name_find2 = get_safe_str(370, 30);
    const char* attr_val_find2 = get_safe_str(400, 30);
    pugi::xml_node found2 = root.find_child_by_attribute(name_find2, attr_name_find2, attr_val_find2);
    (void)found2;

    // Step 10: find_child_by_attribute (overload 1)
    const char* attr_name_find3 = get_safe_str(430, 30);
    const char* attr_val_find3 = get_safe_str(460, 30);
    pugi::xml_node found3 = root.find_child_by_attribute(attr_name_find3, attr_val_find3);
    (void)found3;

    // Step 11: find_child_by_attribute (overload 2)
    const char* name_find4 = get_safe_str(490, 30);
    const char* attr_name_find4 = get_safe_str(520, 30);
    const char* attr_val_find4 = get_safe_str(550, 30);
    pugi::xml_node found4 = root.find_child_by_attribute(name_find4, attr_name_find4, attr_val_find4);
    (void)found4;

    // Step 12: find_child_by_attribute (overload 1)
    const char* attr_name_find5 = get_safe_str(580, 30);
    const char* attr_val_find5 = get_safe_str(610, 30);
    pugi::xml_node found5 = root.find_child_by_attribute(attr_name_find5, attr_val_find5);
    (void)found5;

    // Step 13: find_child_by_attribute (overload 2)
    const char* name_find6 = get_safe_str(640, 30);
    const char* attr_name_find6 = get_safe_str(670, 30);
    const char* attr_val_find6 = get_safe_str(700, 30);
    pugi::xml_node found6 = root.find_child_by_attribute(name_find6, attr_name_find6, attr_val_find6);
    (void)found6;

    // Step 14: find_child_by_attribute (overload 1)
    const char* attr_name_find7 = get_safe_str(730, 30);
    const char* attr_val_find7 = get_safe_str(760, 30);
    pugi::xml_node found7 = root.find_child_by_attribute(attr_name_find7, attr_val_find7);
    (void)found7;

    // Step 15: find_child_by_attribute (overload 2)
    const char* name_find8 = get_safe_str(790, 30);
    const char* attr_name_find8 = get_safe_str(820, 30);
    const char* attr_val_find8 = get_safe_str(850, 30);
    pugi::xml_node found8 = root.find_child_by_attribute(name_find8, attr_name_find8, attr_val_find8);
    (void)found8;

    return 0;
}