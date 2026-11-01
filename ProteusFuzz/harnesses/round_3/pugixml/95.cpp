// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6112:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5497:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5783:39 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t *Data, size_t Size) {
    FILE *f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(Data, 1, Size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_95(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for load_string to avoid buffer overflow
    // We create a buffer with a null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If parsing fails, we still proceed to test attribute functions on the document
    // which might be empty or partially parsed, but safe to manipulate.
    
    // Get root or create a test node
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have an element node to test attribute functions
    if (root.type() != pugi::node_element) {
        root = doc.append_child("test_element");
    }

    // Extract potential attribute names from input data safely
    // We ensure we don't read past the buffer bounds and create valid string_views
    std::string_view input_view(buffer.data(), Size);
    
    // Helper lambda to get a safe substring
    auto get_safe_substring = [&](size_t start, size_t len) -> std::string_view {
        if (start >= Size) return std::string_view();
        size_t actual_len = std::min(len, Size - start);
        return std::string_view(buffer.data() + start, actual_len);
    };

    std::string_view name1 = get_safe_substring(0, 10);
    std::string_view name2 = get_safe_substring(10, 10);
    std::string_view name3 = get_safe_substring(20, 10);
    
    // 1. Test prepend_attribute
    pugi::xml_attribute attr_prepend = root.prepend_attribute(name1);
    if (attr_prepend) {
        attr_prepend.set_value(name2);
    }

    // 2. Test append_attribute
    pugi::xml_attribute attr_append = root.append_attribute(name3);
    if (attr_append) {
        attr_append.set_value(name1);
    }

    // 3. Test set_name on existing attributes
    if (attr_prepend) {
        attr_prepend.set_name(name3);
    }
    if (attr_append) {
        attr_append.set_name(name1);
    }

    // 4. Test remove_attribute by name
    root.remove_attribute(name1);
    root.remove_attribute(name2);
    root.remove_attribute(name3);

    // 5. Test attribute retrieval
    pugi::xml_attribute attr_found = root.attribute(name1);
    if (attr_found) {
        attr_found.set_value(name2);
    }

    // 6. Test set_value on retrieved attribute
    pugi::xml_attribute attr_val = root.attribute(name3);
    if (attr_val) {
        attr_val.set_value(name1);
    }

    // Additional edge case: operations on empty attributes
    pugi::xml_attribute empty_attr;
    empty_attr.set_name(name1); // Should return false
    empty_attr.set_value(name2); // Should return false

    // Operations on non-element node (if root was somehow not element, though we ensured it is)
    // We try to get a child that might not be an element
    pugi::xml_node non_element = doc.first_child(); 
    if (non_element && non_element.type() != pugi::node_element) {
        non_element.prepend_attribute(name1); // Should return empty
        non_element.append_attribute(name1); // Should return empty
        non_element.remove_attribute(name1); // Should return false
    }

    return 0;
}