// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5519:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5526:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <pugixml.hpp>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    if (!doc.load_string("<root><child>test</child></root>")) {
        return 0;
    }

    // Write data to dummy file if needed
    write_dummy_file(Data, Size);

    // Step 1: pugi::xml_node::child
    // Create a safe, null-terminated string from input on the heap to avoid stack-use-after-scope
    // We use a static buffer or heap allocation to ensure lifetime exceeds the function call
    // Here we use a local std::string which manages its own memory safely
    std::string child_name_str;
    if (Size >= 1) {
        // Ensure null termination by taking min(Size, 255) and replacing any internal nulls if needed,
        // but for simplicity, we just copy up to 255 bytes and ensure termination.
        size_t len = (Size < 255) ? Size : 255;
        child_name_str.assign(reinterpret_cast<const char*>(Data), len);
        // Remove any null characters that might appear in the middle to ensure valid string behavior for strcmp
        size_t null_pos = child_name_str.find('\0');
        if (null_pos != std::string::npos) {
            child_name_str = child_name_str.substr(0, null_pos);
        }
    } else {
        child_name_str = "child";
    }

    pugi::xml_node node = doc.child("root");
    if (!node) {
        return 0;
    }
    
    // Pass the c_str() which is valid as long as child_name_str exists
    pugi::xml_node target_node = node.child(child_name_str.c_str());
    if (!target_node) {
        // Try to create the child if it doesn't exist to proceed with fuzzing
        target_node = node.append_child(child_name_str.c_str());
        if (!target_node) {
            return 0;
        }
    }

    // Step 2: pugi::xml_node::append_attribute (1st call)
    pugi::xml_attribute attr1 = target_node.append_attribute("attr1");
    if (!attr1) {
        return 0;
    }

    // Step 3: pugi::xml_node::append_attribute (2nd call)
    pugi::xml_attribute attr2 = target_node.append_attribute("attr2");
    if (!attr2) {
        return 0;
    }

    // Step 4: pugi::xml_attribute::set_value (unsigned long overload)
    unsigned long val_ul = 0;
    if (Size >= 4) {
        std::memcpy(&val_ul, Data, 4);
    }
    attr2.set_value(val_ul);

    // Step 5: pugi::xml_node::append_attribute (3rd call)
    pugi::xml_attribute attr3 = target_node.append_attribute("attr3");
    if (!attr3) {
        return 0;
    }

    // Step 6: pugi::xml_node::append_attribute (4th call)
    pugi::xml_attribute attr4 = target_node.append_attribute("attr4");
    if (!attr4) {
        return 0;
    }

    // Step 7: pugi::xml_attribute::set_value (long overload)
    long val_l = 0;
    if (Size >= 4) {
        // Use next 4 bytes if available, otherwise reuse first 4
        if (Size >= 8) {
            std::memcpy(&val_l, Data + 4, 4);
        } else {
            std::memcpy(&val_l, Data, Size);
        }
    }
    attr4.set_value(val_l);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}