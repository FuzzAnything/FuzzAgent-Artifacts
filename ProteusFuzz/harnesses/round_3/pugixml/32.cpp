// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
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
#include <string_view>

static void safe_fuzz_logic(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Prepare environment: Load a minimal valid XML to ensure we have a root node
    std::string xml_content = "<?xml version=\"1.0\"?><root/>";
    
    if (!doc.load_string(xml_content.c_str())) {
        return;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return;
    }

    // 1. pugi::xml_node::child
    const char* child_name = "child";
    // Only read if we have enough data
    if (size > 10) {
        char name_buf[20] = {0};
        size_t len = (size - 10 > 19) ? 19 : (size - 10);
        std::memcpy(name_buf, data + 10, len);
        child_name = name_buf;
    }
    
    pugi::xml_node child_node = root.child(child_name);
    
    // 2. pugi::xml_node::append_attribute (1st)
    const char* attr_name_1 = "attr1";
    if (size > 20) {
        char name_buf[20] = {0};
        size_t len = (size - 20 > 19) ? 19 : (size - 20);
        std::memcpy(name_buf, data + 20, len);
        attr_name_1 = name_buf;
    }
    pugi::xml_attribute attr1 = child_node.append_attribute(attr_name_1);

    // 3. pugi::xml_node::append_attribute (2nd)
    const char* attr_name_2 = "attr2";
    if (size > 30) {
        char name_buf[20] = {0};
        size_t len = (size - 30 > 19) ? 19 : (size - 30);
        std::memcpy(name_buf, data + 30, len);
        attr_name_2 = name_buf;
    }
    pugi::xml_attribute attr2 = child_node.append_attribute(attr_name_2);

    // 4. pugi::xml_attribute::set_value (long long)
    long long val_ll = 0;
    if (size >= sizeof(long long)) {
        std::memcpy(&val_ll, data, sizeof(long long));
    }
    attr1.set_value(val_ll);

    // 5. pugi::xml_node::append_attribute (3rd)
    const char* attr_name_3 = "attr3";
    if (size > 40) {
        char name_buf[20] = {0};
        size_t len = (size - 40 > 19) ? 19 : (size - 40);
        std::memcpy(name_buf, data + 40, len);
        attr_name_3 = name_buf;
    }
    pugi::xml_attribute attr3 = child_node.append_attribute(attr_name_3);

    // 6. pugi::xml_node::append_attribute (4th)
    const char* attr_name_4 = "attr4";
    if (size > 50) {
        char name_buf[20] = {0};
        size_t len = (size - 50 > 19) ? 19 : (size - 50);
        std::memcpy(name_buf, data + 50, len);
        attr_name_4 = name_buf;
    }
    pugi::xml_attribute attr4 = child_node.append_attribute(attr_name_4);

    // 7. pugi::xml_attribute::set_value (unsigned long long)
    unsigned long long val_ull = 0;
    // Ensure we have enough bytes for unsigned long long
    if (size >= sizeof(unsigned long long)) {
        // Read from the end of the buffer to maximize variety, ensuring bounds
        size_t offset = size - sizeof(unsigned long long);
        std::memcpy(&val_ull, data + offset, sizeof(unsigned long long));
    }
    attr4.set_value(val_ull);
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    try {
        safe_fuzz_logic(Data, Size);
    } catch (...) {
        return 0;
    }

    return 0;
}