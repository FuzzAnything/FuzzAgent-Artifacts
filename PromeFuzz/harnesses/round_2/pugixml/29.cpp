// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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

static void create_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return;
    fwrite(data, 1, size, f);
    fclose(f);
}

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t* Data, size_t Size) {
    // Create dummy file if needed
    create_dummy_file(Data, Size);
    
    // Create XML document and parse from file
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Prepare string data from fuzzer input
    // We'll use the first part of input for string operations
    std::string_view input_view(reinterpret_cast<const char*>(Data), Size);
    
    // 1. pugi::xml_node::child
    if (!input_view.empty()) {
        // Ensure null-terminated string for child name
        std::string child_name(input_view.data(), std::min(input_view.size(), size_t(100)));
        if (!child_name.empty()) {
            pugi::xml_node child = root.child(child_name.c_str());
            (void)child; // Use result to avoid unused variable warning
        }
    }
    
    // 2. First pugi::xml_node::append_attribute
    std::string attr_name1;
    if (input_view.size() > 0) {
        size_t len1 = std::min(input_view.size(), size_t(50));
        attr_name1 = std::string(input_view.data(), len1);
    } else {
        attr_name1 = "attr1";
    }
    
    pugi::xml_attribute attr1 = root.append_attribute(attr_name1.c_str());
    
    // 3. Second pugi::xml_node::append_attribute
    std::string attr_name2;
    if (input_view.size() > 50) {
        size_t len2 = std::min(input_view.size() - 50, size_t(50));
        attr_name2 = std::string(input_view.data() + 50, len2);
    } else {
        attr_name2 = "attr2";
    }
    
    pugi::xml_attribute attr2 = root.append_attribute(attr_name2.c_str());
    
    // 4. First pugi::xml_attribute::set_value (with both overloads)
    if (attr2) {
        // Use parts of input data for numeric values
        unsigned long ul_val = 0;
        long l_val = 0;
        
        if (Size >= sizeof(unsigned long)) {
            memcpy(&ul_val, Data, sizeof(unsigned long));
        }
        if (Size >= sizeof(long)) {
            memcpy(&l_val, Data, sizeof(long));
        }
        
        attr2.set_value(ul_val);  // unsigned long overload
        attr2.set_value(l_val);   // long overload
    }
    
    // 5. Third pugi::xml_node::append_attribute
    std::string attr_name3;
    if (input_view.size() > 100) {
        size_t len3 = std::min(input_view.size() - 100, size_t(50));
        attr_name3 = std::string(input_view.data() + 100, len3);
    } else {
        attr_name3 = "attr3";
    }
    
    pugi::xml_attribute attr3 = root.append_attribute(attr_name3.c_str());
    
    // 6. Fourth pugi::xml_node::append_attribute
    std::string attr_name4;
    if (input_view.size() > 150) {
        size_t len4 = std::min(input_view.size() - 150, size_t(50));
        attr_name4 = std::string(input_view.data() + 150, len4);
    } else {
        attr_name4 = "attr4";
    }
    
    pugi::xml_attribute attr4 = root.append_attribute(attr_name4.c_str());
    
    // 7. Second pugi::xml_attribute::set_value (with both overloads)
    if (attr4) {
        // Use different parts of input data for numeric values
        unsigned long ul_val2 = 0;
        long l_val2 = 0;
        
        if (Size >= sizeof(unsigned long) * 2) {
            memcpy(&ul_val2, Data + sizeof(unsigned long), sizeof(unsigned long));
        }
        if (Size >= sizeof(long) * 2) {
            memcpy(&l_val2, Data + sizeof(long), sizeof(long));
        }
        
        attr4.set_value(ul_val2);  // unsigned long overload
        attr4.set_value(l_val2);   // long overload
    }
    
    // Clean up dummy file
    remove("./dummy_file");
    
    return 0;
}