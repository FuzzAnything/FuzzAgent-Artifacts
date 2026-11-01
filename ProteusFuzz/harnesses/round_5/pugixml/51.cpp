// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
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
#include <cstdio>
#include <string>
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = std::fopen("./dummy_file", "wb");
    if (f) {
        std::fwrite(data, 1, size, f);
        std::fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Ensure the input data is null-terminated for load_string
    // We create a temporary buffer with a null terminator to prevent buffer overflows
    // if the fuzzer provides non-null-terminated data.
    std::vector<char> safe_buffer(Size + 1);
    std::memcpy(safe_buffer.data(), Data, Size);
    safe_buffer[Size] = '\0';

    if (!doc.load_string(safe_buffer.data())) {
        // If parsing fails, try to create a minimal valid document from scratch
        // to ensure we can test the API chain even with invalid input.
        doc.append_child(pugi::node_element).set_name("root");
    }

    // Ensure we have a root element to work with
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child(pugi::node_element);
        root.set_name("root");
    }

    // 1. pugi::xml_node::insert_child_before
    // We need a sibling to insert before. Let's try to add one first if none exists.
    pugi::xml_node sibling = root.append_child(pugi::node_element);
    sibling.set_name("sibling");
    
    // Now insert a new node before the sibling
    pugi::xml_node new_node = root.insert_child_before(pugi::node_element, sibling);
    if (new_node) {
        new_node.set_name("inserted_before");
    }

    // 2. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    (void)first; // Use the result to avoid warnings

    // 3. pugi::xml_node::append_attribute
    pugi::xml_attribute attr = root.append_attribute("fuzz_attr");
    if (attr) {
        // Use fuzz data for attribute value if available, ensuring null termination
        std::string val_str(safe_buffer.data(), std::min(Size, static_cast<size_t>(100)));
        // Ensure the string is null-terminated (std::string does this, but safe_buffer might not be if we sliced it)
        // val_str is already null-terminated by std::string constructor.
        attr.set_value(val_str.c_str());
    }

    // 4. pugi::xml_node::insert_child_before (second invocation)
    // Try inserting before the first child again or the sibling
    pugi::xml_node target_sibling = root.child("sibling");
    if (target_sibling) {
        pugi::xml_node new_node2 = root.insert_child_before(pugi::node_pcdata, target_sibling);
        if (new_node2) {
            new_node2.set_value("pcdata_before");
        }
    }

    // 5. pugi::xml_node::first_child (second invocation)
    pugi::xml_node first2 = root.first_child();
    (void)first2;

    // 6. pugi::xml_node::set_value
    // Try to set value on a node. Elements return false, but we call it anyway.
    // We try on the first child if it exists, otherwise on root (which will fail for elements).
    if (first2) {
        std::string val_sv(safe_buffer.data(), std::min(Size, static_cast<size_t>(50)));
        first2.set_value(val_sv.c_str());
    } else {
        // Fallback if no child
        root.set_value("root_value");
    }

    // 7. pugi::xml_document::save
    // Write to a file
    FILE* f = std::fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        doc.save(writer, "\t", pugi::format_write_bom, pugi::encoding_utf8);
        std::fclose(f);
    }

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}