// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
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
#include <string>
#include <string_view>

static void initialize_test_document(pugi::xml_document& doc) {
    // Create a simple XML structure for testing
    pugi::xml_node root = doc.append_child("root");
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
    root.append_attribute("attr3") = "value3";
    
    // Add a child node with attributes
    pugi::xml_node child = root.append_child("child");
    child.append_attribute("child_attr1") = "child_value1";
    child.append_attribute("child_attr2") = "child_value2";
}

static std::string extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) {
        return "";
    }
    
    // Find null terminator or use remaining bytes
    size_t start = offset;
    while (offset < size && data[offset] != 0) {
        ++offset;
    }
    
    size_t length = offset - start;
    if (length == 0) {
        return "";
    }
    
    // Move past null terminator if found
    if (offset < size && data[offset] == 0) {
        ++offset;
    }
    
    return std::string(reinterpret_cast<const char*>(data + start), length);
}

extern "C" int LLVMFuzzerTestOneInput_93(const uint8_t* Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }
    
    // Initialize pugi::xml_document
    pugi::xml_document doc;
    initialize_test_document(doc);
    
    // Get root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // Get child node
    pugi::xml_node child = root.child("child");
    
    // Extract strings from fuzz data
    size_t offset = 0;
    std::string str1 = extract_string(Data, Size, offset);
    std::string str2 = extract_string(Data, Size, offset);
    std::string str3 = extract_string(Data, Size, offset);
    
    // Test 1: pugi::xml_node::attribute
    try {
        pugi::xml_attribute attr1 = root.attribute(str1.c_str());
        if (attr1) {
            const char* name = attr1.name();
            const char* value = attr1.value();
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 2: pugi::xml_node::insert_attribute_before
    try {
        pugi::xml_attribute first_attr = root.first_attribute();
        if (first_attr) {
            std::string_view sv(str2);
            pugi::xml_attribute new_attr = root.insert_attribute_before(sv, first_attr);
            if (new_attr) {
                new_attr = str3.c_str();
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 3: pugi::xml_node::insert_copy_after
    try {
        pugi::xml_attribute attr_to_copy = root.attribute("attr2");
        pugi::xml_attribute ref_attr = root.attribute("attr1");
        if (attr_to_copy && ref_attr) {
            pugi::xml_attribute copied_attr = root.insert_copy_after(attr_to_copy, ref_attr);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 4: pugi::xml_node::prepend_attribute
    try {
        pugi::xml_attribute prepended = root.prepend_attribute(str1.c_str());
        if (prepended) {
            prepended = "fuzz_value";
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 5: pugi::xml_node::insert_attribute_after
    try {
        pugi::xml_attribute ref_attr = root.attribute("attr3");
        if (ref_attr) {
            std::string_view sv(str2);
            pugi::xml_attribute new_attr = root.insert_attribute_after(sv, ref_attr);
            if (new_attr) {
                new_attr = str3.c_str();
            }
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test 6: pugi::xml_node::insert_copy_before
    try {
        pugi::xml_attribute attr_to_copy = child.attribute("child_attr1");
        pugi::xml_attribute ref_attr = child.attribute("child_attr2");
        if (attr_to_copy && ref_attr) {
            pugi::xml_attribute copied_attr = child.insert_copy_before(attr_to_copy, ref_attr);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Additional tests with empty/null strings
    try {
        // Test with empty string
        pugi::xml_attribute empty_attr = root.attribute("");
        
        // Test prepend with empty name
        pugi::xml_attribute empty_prepended = root.prepend_attribute("");
        
        // Test insert with empty name
        pugi::xml_attribute first = root.first_attribute();
        if (first) {
            std::string_view empty_sv("");
            pugi::xml_attribute empty_inserted = root.insert_attribute_before(empty_sv, first);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    // Test with child node
    try {
        pugi::xml_attribute child_attr = child.attribute(str1.c_str());
        
        // Try to insert in child node
        pugi::xml_attribute child_first = child.first_attribute();
        if (child_first) {
            std::string_view sv(str2);
            pugi::xml_attribute new_child_attr = child.insert_attribute_after(sv, child_first);
        }
    } catch (...) {
        // Ignore exceptions
    }
    
    return 0;
}