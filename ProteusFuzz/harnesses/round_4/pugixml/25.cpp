// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5505:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5512:35 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_25(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_string("<root></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    // 1. pugi::xml_node::child
    // We need a valid name to search for. Use the first byte as a seed for a name.
    // Ensure the name is null-terminated.
    char child_name[256];
    size_t name_len = (Size > 255) ? 255 : Size;
    std::memcpy(child_name, Data, name_len);
    child_name[name_len] = '\0';

    // Try to find a child. It might not exist, which is fine (returns empty node).
    pugi::xml_node child_node = root.child(child_name);

    // If child_node is empty, we can't append attributes to it.
    // To ensure we exercise the append_attribute logic, we might need to create the child first
    // or use the root node if the child lookup fails.
    // However, the task specifies invoking child() first.
    // If child_node is empty, we will fallback to root for attribute operations to ensure coverage.
    pugi::xml_node target_node = child_node.empty() ? root : child_node;

    if (!target_node) {
        return 0;
    }

    // 2. pugi::xml_node::append_attribute
    // Create a unique attribute name based on input data
    char attr1_name[64];
    size_t attr1_len = (Size > 63) ? 63 : Size;
    std::memcpy(attr1_name, Data, attr1_len);
    attr1_name[attr1_len] = '\0';
    
    pugi::xml_attribute attr1 = target_node.append_attribute(attr1_name);
    if (!attr1) {
        return 0;
    }

    // 3. pugi::xml_attribute::set_value (int)
    int int_val = 0;
    if (Size >= 4) {
        std::memcpy(&int_val, Data, 4);
    }
    attr1.set_value(int_val);

    // 3. pugi::xml_attribute::set_value (unsigned int)
    unsigned int uint_val = 0;
    if (Size >= 4) {
        std::memcpy(&uint_val, Data, 4);
    }
    attr1.set_value(uint_val);

    // 3. pugi::xml_attribute::set_value (const char_t*)
    char str_val[128];
    size_t str_len = (Size > 127) ? 127 : Size;
    std::memcpy(str_val, Data, str_len);
    str_val[str_len] = '\0';
    attr1.set_value(str_val);

    // 4. pugi::xml_node::append_attribute
    char attr2_name[64];
    size_t attr2_len = (Size > 63) ? 63 : Size;
    // Use a different offset if possible to vary data
    size_t offset = (Size > 64) ? 64 : 0;
    if (Size - offset < attr2_len) {
        attr2_len = Size - offset;
    }
    std::memcpy(attr2_name, Data + offset, attr2_len);
    attr2_name[attr2_len] = '\0';

    pugi::xml_attribute attr2 = target_node.append_attribute(attr2_name);
    if (!attr2) {
        return 0;
    }

    // 5. pugi::xml_node::append_attribute
    char attr3_name[64];
    size_t attr3_len = (Size > 63) ? 63 : Size;
    offset = (Size > 128) ? 128 : 0;
    if (Size - offset < attr3_len) {
        attr3_len = Size - offset;
    }
    std::memcpy(attr3_name, Data + offset, attr3_len);
    attr3_name[attr3_len] = '\0';

    pugi::xml_attribute attr3 = target_node.append_attribute(attr3_name);
    if (!attr3) {
        return 0;
    }

    // 6. pugi::xml_attribute::set_value (int)
    int int_val2 = 0;
    if (Size >= 4) {
        std::memcpy(&int_val2, Data + 1, 4); // slight offset
    }
    attr3.set_value(int_val2);

    // 7. pugi::xml_node::append_attribute
    char attr4_name[64];
    size_t attr4_len = (Size > 63) ? 63 : Size;
    offset = (Size > 192) ? 192 : 0;
    if (Size - offset < attr4_len) {
        attr4_len = Size - offset;
    }
    std::memcpy(attr4_name, Data + offset, attr4_len);
    attr4_name[attr4_len] = '\0';

    pugi::xml_attribute attr4 = target_node.append_attribute(attr4_name);
    if (!attr4) {
        return 0;
    }

    // 8. pugi::xml_node::append_attribute
    char attr5_name[64];
    size_t attr5_len = (Size > 63) ? 63 : Size;
    offset = (Size > 256) ? 256 : 0;
    if (Size - offset < attr5_len) {
        attr5_len = Size - offset;
    }
    std::memcpy(attr5_name, Data + offset, attr5_len);
    attr5_name[attr5_len] = '\0';

    pugi::xml_attribute attr5 = target_node.append_attribute(attr5_name);
    if (!attr5) {
        return 0;
    }

    // 9. pugi::xml_attribute::set_value (unsigned int)
    unsigned int uint_val2 = 0;
    if (Size >= 4) {
        std::memcpy(&uint_val2, Data + 2, 4);
    }
    attr5.set_value(uint_val2);

    return 0;
}