// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
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

static void initialize_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

static std::string extract_string(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) return "";
    size_t remaining = size - offset;
    size_t length = (remaining > 100) ? 100 : remaining;
    std::string result(reinterpret_cast<const char*>(data + offset), length);
    if (!result.empty() && result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t* data, size_t size) {
    // Initialize dummy file with fuzzer data
    initialize_dummy_file(data, size);
    
    // Load XML document from dummy file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) return 0;
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // Extract strings for child/attribute names from fuzzer data
    std::string child_name1 = extract_string(data, size, 0);
    std::string attr_name1 = extract_string(data, size, 100);
    std::string child_name2 = extract_string(data, size, 200);
    std::string attr_name2 = extract_string(data, size, 300);
    
    // Ensure null termination for C strings
    if (!child_name1.empty()) {
        pugi::xml_node child1 = root.child(child_name1.c_str());
        
        if (!attr_name1.empty()) {
            pugi::xml_attribute attr1 = child1.attribute(attr_name1.c_str());
            
            if (!child_name2.empty()) {
                pugi::xml_node child2 = child1.child(child_name2.c_str());
                
                if (!attr_name2.empty()) {
                    pugi::xml_attribute attr2 = child2.attribute(attr_name2.c_str());
                    
                    // Traverse attributes in both directions
                    pugi::xml_attribute attr3 = attr2.next_attribute();
                    pugi::xml_attribute attr4 = attr3.next_attribute();
                    pugi::xml_attribute attr5 = attr4.previous_attribute();
                    pugi::xml_attribute attr6 = attr5.previous_attribute();
                    pugi::xml_attribute attr7 = attr6.next_attribute();
                    pugi::xml_attribute attr8 = attr7.previous_attribute();
                    
                    // Touch the attributes to ensure they're used
                    (void)attr3; (void)attr4; (void)attr5;
                    (void)attr6; (void)attr7; (void)attr8;
                }
            }
        }
    }
    
    // Try alternative traversal starting from document element
    pugi::xml_node first_child = doc.first_child();
    if (first_child) {
        pugi::xml_attribute first_attr = first_child.first_attribute();
        if (first_attr) {
            pugi::xml_attribute next1 = first_attr.next_attribute();
            pugi::xml_attribute prev1 = first_attr.previous_attribute();
            (void)next1; (void)prev1;
        }
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}