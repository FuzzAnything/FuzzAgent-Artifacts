// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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

static std::string generate_xml_from_fuzz_data(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "<root></root>";
    }
    
    std::string result = "<root>";
    size_t idx = 0;
    
    // Generate a safe XML string from fuzz data, escaping special characters
    while (idx < size) {
        char c = static_cast<char>(data[idx]);
        if (c == '<') {
            result += "&lt;";
        } else if (c == '>') {
            result += "&gt;";
        } else if (c == '&') {
            result += "&amp;";
        } else if (c == '"') {
            result += "&quot;";
        } else if (c == '\'') {
            result += "&apos;";
        } else if (c >= 32 && c < 127) {
            result += c;
        } else {
            // Skip non-printable characters to avoid malformed XML
        }
        idx++;
    }
    
    result += "</root>";
    return result;
}

static std::string sanitize_name(const uint8_t* data, size_t size, size_t offset) {
    std::string name = "test";
    if (offset + 1 <= size) {
        size_t len = (size - offset) % 10;
        if (len == 0) len = 1;
        if (len > 10) len = 10;
        
        name.clear();
        for (size_t i = 0; i < len && (offset + i) < size; ++i) {
            char c = static_cast<char>(data[offset + i]);
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
                name += c;
            } else {
                name += 'x';
            }
        }
        if (name.empty()) name = "test";
    }
    return name;
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: Create a document and parse XML from fuzz data
    pugi::xml_document doc;
    std::string xml_str = generate_xml_from_fuzz_data(Data, Size);
    
    if (!doc.load_string(xml_str.c_str())) {
        // If parsing fails, try a minimal valid XML
        xml_str = "<root></root>";
        if (!doc.load_string(xml_str.c_str())) {
            return 0;
        }
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Ensure we have an element node to work with
    // In pugixml, type() returns pugi::node_element for elements
    if (!root || root.type() != pugi::node_element) {
        return 0;
    }

    // Test 1: prepend_attribute
    std::string attr_name = sanitize_name(Data, Size, 0);
    pugi::xml_attribute prepended = root.prepend_attribute(attr_name.c_str());
    if (prepended) {
        prepended.set_value("value1");
    }

    // Prepare attributes for subsequent tests
    // Use append_child_attribute equivalent: append_child followed by attribute manipulation or direct attribute creation
    // Since append_child_attribute doesn't exist, we create attributes using prepend_attribute or append_child_attribute logic
    // Actually, we can use append_child_attribute via the attribute() method on a new child or just use prepend_attribute again
    // Let's create attributes using prepend_attribute to ensure they exist
    std::string proto_name = sanitize_name(Data, Size, 10);
    std::string ref_name = sanitize_name(Data, Size, 20);
    
    pugi::xml_attribute proto_attr = root.prepend_attribute(proto_name.c_str());
    pugi::xml_attribute ref_attr = root.prepend_attribute(ref_name.c_str());

    if (!proto_attr || !ref_attr) {
        // Fallback if creation failed
        if (prepended) {
            proto_attr = prepended;
            ref_attr = prepended;
        } else {
            return 0;
        }
    }

    // Test 2: insert_copy_after
    pugi::xml_attribute copied_after = root.insert_copy_after(proto_attr, ref_attr);
    if (copied_after) {
        copied_after.set_value("copied_after_value");
    }

    // Test 3: insert_copy_before
    pugi::xml_attribute copied_before = root.insert_copy_before(proto_attr, ref_attr);
    if (copied_before) {
        copied_before.set_value("copied_before_value");
    }

    // Test 4: insert_attribute_before
    std::string name_before = sanitize_name(Data, Size, 30);
    pugi::xml_attribute inserted_before = root.insert_attribute_before(name_before.c_str(), ref_attr);
    if (inserted_before) {
        inserted_before.set_value("inserted_before_value");
    }

    // Test 5: insert_attribute_after
    std::string name_after = sanitize_name(Data, Size, 40);
    pugi::xml_attribute inserted_after = root.insert_attribute_after(name_after.c_str(), ref_attr);
    if (inserted_after) {
        inserted_after.set_value("inserted_after_value");
    }

    // Test 6: attribute
    std::string search_name = sanitize_name(Data, Size, 50);
    pugi::xml_attribute found = root.attribute(search_name.c_str());
    if (found) {
        found.set_value("found_value");
    }

    // Cleanup is handled automatically by pugi::xml_document destructor
    return 0;
}