// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
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

static void initialize_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t* Data, size_t Size) {
    // Initialize dummy file with fuzzer data
    initialize_dummy_file(Data, Size);
    
    try {
        // Create XML document and parse from dummy file
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file("./dummy_file");
        
        // Get root node or create one if parsing failed
        pugi::xml_node root = doc.root();
        if (!root) {
            root = doc.append_child("root");
        }
        
        // Create a child node to work with
        pugi::xml_node child_node = root.append_child("child_node");
        
        // 1. Call pugi::xml_node::child
        // Extract child name from fuzzer data (ensure null termination)
        std::string child_name;
        if (Size > 0) {
            size_t name_len = std::min(Size, static_cast<size_t>(100));
            child_name = std::string(reinterpret_cast<const char*>(Data), name_len);
        } else {
            child_name = "test";
        }
        pugi::xml_node found_child = root.child(child_name.c_str());
        
        // 2. Call pugi::xml_node::append_attribute (first time)
        std::string attr_name1;
        if (Size > 1) {
            size_t attr_len1 = std::min(Size - 1, static_cast<size_t>(100));
            attr_name1 = std::string(reinterpret_cast<const char*>(Data + 1), attr_len1);
        } else {
            attr_name1 = "attr1";
        }
        pugi::xml_attribute attr1 = child_node.append_attribute(attr_name1.c_str());
        
        // 3. Call pugi::xml_node::append_attribute (second time)
        std::string attr_name2;
        if (Size > 2) {
            size_t attr_len2 = std::min(Size - 2, static_cast<size_t>(100));
            attr_name2 = std::string(reinterpret_cast<const char*>(Data + 2), attr_len2);
        } else {
            attr_name2 = "attr2";
        }
        pugi::xml_attribute attr2 = child_node.append_attribute(attr_name2.c_str());
        
        // 4. Call pugi::xml_attribute::set_value (first time - unsigned long overload)
        if (attr1) {
            unsigned long ul_val = 0;
            if (Size >= sizeof(unsigned long)) {
                memcpy(&ul_val, Data, sizeof(unsigned long));
            }
            attr1.set_value(ul_val);
        }
        
        // 5. Call pugi::xml_node::append_attribute (third time)
        std::string attr_name3;
        if (Size > 3) {
            size_t attr_len3 = std::min(Size - 3, static_cast<size_t>(100));
            attr_name3 = std::string(reinterpret_cast<const char*>(Data + 3), attr_len3);
        } else {
            attr_name3 = "attr3";
        }
        pugi::xml_attribute attr3 = child_node.append_attribute(attr_name3.c_str());
        
        // 6. Call pugi::xml_node::append_attribute (fourth time)
        std::string attr_name4;
        if (Size > 4) {
            size_t attr_len4 = std::min(Size - 4, static_cast<size_t>(100));
            attr_name4 = std::string(reinterpret_cast<const char*>(Data + 4), attr_len4);
        } else {
            attr_name4 = "attr4";
        }
        pugi::xml_attribute attr4 = child_node.append_attribute(attr_name4.c_str());
        
        // 7. Call pugi::xml_attribute::set_value (second time - long overload)
        if (attr3) {
            long l_val = 0;
            if (Size >= sizeof(long)) {
                memcpy(&l_val, Data, sizeof(long));
            }
            attr3.set_value(l_val);
        }
        
        // Also call set_value on other attributes to explore more states
        if (attr2) {
            unsigned long ul_val2 = Size;
            attr2.set_value(ul_val2);
        }
        
        if (attr4) {
            long l_val2 = static_cast<long>(Size);
            attr4.set_value(l_val2);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}