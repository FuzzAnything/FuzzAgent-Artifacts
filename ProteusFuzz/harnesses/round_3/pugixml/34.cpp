// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5459:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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
#include <string_view>

static bool try_parse_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    
    // Ensure the data is null-terminated for load_string
    // We create a temporary buffer with a null terminator
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    // Attempt to parse the data as UTF-8
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    bool parsed = try_parse_document(Data, Size, doc);

    // If parsing failed, try to create a minimal valid document to test edge cases
    if (!parsed) {
        doc.load_string("<root/>");
    }

    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        // If no children, create one to ensure we have a node to work with
        root = doc.append_child("test");
    }

    // 1. pugi::xml_node::first_child
    pugi::xml_node child = root.first_child();
    if (child.empty()) {
        // Ensure child exists for subsequent attribute operations
        child = root.append_child("child_node");
    }

    // Prepare string_view for attribute names
    // pugixml::string_view_t is typically std::string_view<char> or std::wstring_view depending on config
    // We use the default char version which is most common
    std::string_view a_str = "test_attr";
    
    // Use pugi::char_t directly to avoid type mismatch
    const pugi::char_t* c_str = reinterpret_cast<const pugi::char_t*>(a_str.data());

    // Helper to get attribute safely
    auto get_attr = [&](const pugi::xml_node& n, const pugi::char_t* name) -> pugi::xml_attribute {
        return n.attribute(name);
    };

    // 2-8. pugi::xml_node::attribute (multiple calls with const char_t*)
    pugi::xml_attribute attr1 = child.attribute(c_str);
    pugi::xml_attribute attr2 = child.attribute(c_str);
    pugi::xml_attribute attr3 = child.attribute(c_str);
    pugi::xml_attribute attr4 = child.attribute(c_str);
    pugi::xml_attribute attr5 = child.attribute(c_str);
    pugi::xml_attribute attr6 = child.attribute(c_str);
    pugi::xml_attribute attr7 = child.attribute(c_str);

    // If no attribute exists yet, create one to test set_name
    if (attr1.empty()) {
        attr1 = child.append_attribute(c_str);
        if (attr1.empty()) {
            // Fallback if append fails (unlikely in valid doc)
            return 0;
        }
    }

    // 9. pugi::xml_attribute::set_name
    // Use a simple string for the new name
    const pugi::char_t* new_name = reinterpret_cast<const pugi::char_t*>("new_attr_name");
    attr1.set_name(new_name);

    // 10-13. pugi::xml_node::attribute (mixed overloads)
    // Overload 1: attribute(string_view_t name, xml_attribute& hint)
    pugi::xml_attribute hint = pugi::xml_attribute();
    pugi::xml_attribute attr8 = child.attribute(a_str, hint);
    
    // Overload 2: attribute(const char_t* name)
    pugi::xml_attribute attr9 = child.attribute(c_str);
    pugi::xml_attribute attr10 = child.attribute(c_str);
    pugi::xml_attribute attr11 = child.attribute(c_str);

    // No specific cleanup required for pugixml handles as they are RAII-like wrappers
    // The document will be cleaned up when it goes out of scope
    
    return 0;
}