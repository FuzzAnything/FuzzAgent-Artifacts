// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
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
#include <functional>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static bool attribute_predicate(const pugi::xml_attribute& attr) {
    return attr.name() != nullptr && attr.value() != nullptr;
}

static void traverse_nodes(pugi::xml_node node, std::vector<pugi::xml_node>& out) {
    if (node) {
        out.push_back(node);
        for (auto child = node.first_child(); child; child = child.next_sibling()) {
            traverse_nodes(child, out);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_107(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input data is null-terminated for string-based parsing
    // We create a buffer with an extra byte for the null terminator
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // If parsing fails, try loading from file as a fallback
    if (!result) {
        write_dummy_file(Data, Size);
        result = doc.load_file("./dummy_file");
    }

    if (!doc) {
        // Even if document is invalid, we can test with empty nodes
        pugi::xml_node empty_node;
        (void)empty_node.first_attribute();
        (void)empty_node.last_attribute();
        (void)empty_node.attribute("test");
        (void)empty_node.find_attribute(attribute_predicate);
        
        // These should return empty attributes on invalid nodes
        pugi::xml_attribute dummy_attr;
        // Note: insert_attribute_after/before are non-const, so we cannot call them on empty_node
        // unless we cast away const, but that's unsafe. We skip them for empty nodes.
        
        return 0;
    }

    // Test with document root and various nodes
    std::vector<pugi::xml_node> nodes_to_test;
    
    traverse_nodes(doc, nodes_to_test);

    // Ensure we have at least the document node
    if (nodes_to_test.empty()) {
        nodes_to_test.push_back(doc);
    }

    // Fuzz each node
    for (auto& node : nodes_to_test) {
        if (!node) continue;

        // 1. Test first_attribute
        pugi::xml_attribute first_attr = node.first_attribute();
        (void)first_attr;

        // 2. Test last_attribute
        pugi::xml_attribute last_attr = node.last_attribute();
        (void)last_attr;

        // 3. Test attribute with various names derived from input
        std::string test_name;
        if (Size > 0) {
            // Limit length to avoid excessively long strings
            size_t len = (Size > 10) ? 10 : Size;
            test_name.resize(len);
            for (size_t i = 0; i < len; ++i) {
                test_name[i] = static_cast<char>(Data[i]);
            }
            // Ensure null termination for safety, though std::string handles it
            test_name += '\0';
        } else {
            test_name = "test";
        }
        
        pugi::xml_attribute found_attr = node.attribute(test_name.c_str());
        (void)found_attr;

        // Test with empty string
        (void)node.attribute("");

        // 4. Test find_attribute with predicate
        pugi::xml_attribute found_pred = node.find_attribute(attribute_predicate);
        (void)found_pred;

        // Test find_attribute with lambda that always returns false
        pugi::xml_attribute found_false = node.find_attribute([](const pugi::xml_attribute&) { return false; });
        (void)found_false;

        // 5. Test insert_attribute_after
        // Try inserting after the first attribute if it exists
        if (first_attr) {
            std::string new_attr_name = "new_attr_after";
            if (Size > 0) {
                new_attr_name[0] = static_cast<char>(Data[0]);
            }
            pugi::xml_attribute inserted_after = node.insert_attribute_after(new_attr_name.c_str(), first_attr);
            (void)inserted_after;
            
            // Set value if insertion succeeded
            if (inserted_after) {
                inserted_after.set_value("value_after");
            }
        } else {
            // Try inserting after a non-existent attribute (should fail gracefully)
            pugi::xml_attribute dummy_attr;
            (void)node.insert_attribute_after("test", dummy_attr);
        }

        // 6. Test insert_attribute_before
        // Try inserting before the last attribute if it exists
        if (last_attr) {
            std::string new_attr_name_before = "new_attr_before";
            if (Size > 1) {
                new_attr_name_before[0] = static_cast<char>(Data[1]);
            }
            pugi::xml_attribute inserted_before = node.insert_attribute_before(new_attr_name_before.c_str(), last_attr);
            (void)inserted_before;
            
            // Set value if insertion succeeded
            if (inserted_before) {
                inserted_before.set_value("value_before");
            }
        } else {
            // Try inserting before a non-existent attribute
            pugi::xml_attribute dummy_attr;
            (void)node.insert_attribute_before("test", dummy_attr);
        }
    }

    return 0;
}