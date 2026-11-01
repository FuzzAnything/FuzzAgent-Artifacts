// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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
#include <pugixml.hpp>

static bool ensure_null_terminated(const uint8_t* data, size_t size, size_t& valid_len) {
    if (size == 0) {
        valid_len = 0;
        return false;
    }
    size_t len = 0;
    while (len < size && data[len] != 0) {
        len++;
    }
    valid_len = len;
    // Must have at least one character before the null terminator to be a valid string
    return len > 0 && len < size;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Allocate a buffer to hold the input data, ensuring we can modify it for in-place parsing
    // We add 1 byte to ensure space for a null terminator if needed.
    size_t buffer_size = Size;
    char* buffer = static_cast<char*>(std::malloc(buffer_size + 1));
    if (!buffer) return 0;

    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0'; // Ensure null termination

    pugi::xml_document doc;

    // 1. pugi::xml_document::load_buffer_inplace
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size, pugi::parse_default, pugi::encoding_auto);

    pugi::xml_node root = doc;

    // Prepare a name for append_child. We need a null-terminated string.
    // We try to extract a valid string from the input or use a default.
    const char* child_name = "fuzz_child";
    size_t name_len = 0;
    
    // Check if input starts with a valid null-terminated string
    // We must ensure the string is within bounds and has a null terminator
    if (ensure_null_terminated(Data, Size, name_len)) {
        // Only use input data if it contains a null terminator within bounds
        // We cast to const char* but ensure the string is valid
        child_name = reinterpret_cast<const char*>(Data);
    } else {
        // If no null terminator found, use the safe default string
        child_name = "fuzz_child";
    }

    // 2. pugi::xml_node::append_child (first call)
    pugi::xml_node node1 = root.append_child(child_name);

    // 3. pugi::xml_node::append_copy (first call)
    // Ensure we have an attribute to copy.
    if (node1) {
        node1.append_attribute("fuzz_attr").set_value("fuzz_value");
    }
    
    pugi::xml_attribute attr_to_copy = node1 ? node1.first_attribute() : pugi::xml_attribute();
    pugi::xml_attribute copied_attr = root.append_copy(attr_to_copy);

    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();

    // 5. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();

    // 6. pugi::xml_node::append_child (second call)
    const char* child_name2 = "fuzz_child_2";
    if (Size > 10) {
        size_t len2 = 0;
        if (ensure_null_terminated(Data + 10, Size - 10, len2)) {
            child_name2 = reinterpret_cast<const char*>(Data + 10);
        } else {
            child_name2 = "fuzz_child_2";
        }
    }
    pugi::xml_node node2 = root.append_child(child_name2);

    // 7. pugi::xml_node::append_copy (second call)
    pugi::xml_attribute attr_to_copy2 = node2 ? node2.first_attribute() : attr_to_copy;
    
    if (node2 && !attr_to_copy2) {
        node2.append_attribute("fuzz_attr_2").set_value("fuzz_value_2");
        attr_to_copy2 = node2.first_attribute();
    }
    pugi::xml_attribute copied_attr2 = root.append_copy(attr_to_copy2);

    // 8. pugi::xml_node::first_attribute (second call)
    pugi::xml_attribute first_attr2 = node2.first_attribute();

    // Cleanup: free the buffer manually as required by load_buffer_inplace
    std::free(buffer);

    return 0;
}