// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
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

static std::string extract_string(const uint8_t* data, size_t size, size_t& offset) {
    if (offset >= size) return "";
    size_t len = 0;
    while (offset + len < size && data[offset + len] != 0 && len < 1024) {
        len++;
    }
    std::string str(reinterpret_cast<const char*>(data + offset), len);
    offset += len + 1;
    return str;
}

static pugi::xml_document create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) return doc;
    
    // Ensure null termination for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Try to parse as XML
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    // If parsing fails, create a minimal valid document to ensure we have nodes to test
    if (!result) {
        doc.append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc = create_document_from_input(Data, Size);
    
    // Get a node to work with
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }

    // Helper to get a valid attribute name from input, stored in a static buffer to avoid dangling pointers
    static char name_buffer[1025];
    static char name_buffer2[1025];
    static char name_buffer3[1025];
    static char name_buffer4[1025];
    static char name_buffer5[1025];
    static char name_buffer6[1025];

    size_t offset = 0;
    
    // Test 1: prepend_attribute
    std::string s1 = extract_string(Data, Size, offset);
    if (s1.empty()) s1 = "test_attr";
    strncpy(name_buffer, s1.c_str(), sizeof(name_buffer) - 1);
    name_buffer[sizeof(name_buffer) - 1] = '\0';
    pugi::xml_attribute attr1 = root.prepend_attribute(name_buffer);
    
    // Test 2: attribute with hint
    pugi::xml_attribute hint = pugi::xml_attribute();
    std::string s2 = extract_string(Data, Size, offset);
    if (s2.empty()) s2 = "test_attr2";
    strncpy(name_buffer2, s2.c_str(), sizeof(name_buffer2) - 1);
    name_buffer2[sizeof(name_buffer2) - 1] = '\0';
    pugi::xml_attribute attr2 = root.attribute(name_buffer2, hint);
    
    // Test 3: insert_copy_after (requires existing attributes)
    if (attr1 && attr2) {
        pugi::xml_attribute copy_after = root.insert_copy_after(attr1, attr2);
        (void)copy_after; // Suppress unused warning
    } else if (attr1) {
        // If only one attr, try copying after itself (should fail gracefully or work depending on impl)
        pugi::xml_attribute copy_after = root.insert_copy_after(attr1, attr1);
        (void)copy_after;
    }
    
    // Test 4: insert_copy_before
    if (attr1 && attr2) {
        pugi::xml_attribute copy_before = root.insert_copy_before(attr1, attr2);
        (void)copy_before;
    } else if (attr1) {
        pugi::xml_attribute copy_before = root.insert_copy_before(attr1, attr1);
        (void)copy_before;
    }
    
    // Test 5: insert_attribute_after
    if (attr2) {
        std::string s3 = extract_string(Data, Size, offset);
        if (s3.empty()) s3 = "test_attr3";
        strncpy(name_buffer3, s3.c_str(), sizeof(name_buffer3) - 1);
        name_buffer3[sizeof(name_buffer3) - 1] = '\0';
        pugi::xml_attribute new_after = root.insert_attribute_after(name_buffer3, attr2);
        (void)new_after;
    }
    
    // Test 6: insert_attribute_before
    if (attr1) {
        std::string s4 = extract_string(Data, Size, offset);
        if (s4.empty()) s4 = "test_attr4";
        strncpy(name_buffer4, s4.c_str(), sizeof(name_buffer4) - 1);
        name_buffer4[sizeof(name_buffer4) - 1] = '\0';
        pugi::xml_attribute new_before = root.insert_attribute_before(name_buffer4, attr1);
        (void)new_before;
    }

    // Additional edge cases: try on non-element nodes if available
    pugi::xml_node text_node = doc.append_child(pugi::node_pcdata);
    text_node.set_value("test text");
    
    std::string s5 = extract_string(Data, Size, offset);
    if (s5.empty()) s5 = "test_attr5";
    strncpy(name_buffer5, s5.c_str(), sizeof(name_buffer5) - 1);
    name_buffer5[sizeof(name_buffer5) - 1] = '\0';
    pugi::xml_attribute attr_on_text = text_node.prepend_attribute(name_buffer5);
    (void)attr_on_text; // Should be empty

    std::string s6 = extract_string(Data, Size, offset);
    if (s6.empty()) s6 = "test_attr6";
    strncpy(name_buffer6, s6.c_str(), sizeof(name_buffer6) - 1);
    name_buffer6[sizeof(name_buffer6) - 1] = '\0';
    pugi::xml_attribute attr_on_text2 = text_node.attribute(name_buffer6, hint);
    (void)attr_on_text2;

    return 0;
}