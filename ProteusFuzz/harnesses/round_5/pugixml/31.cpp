// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
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
#include <cstddef>
#include <string_view>

static void fuzz_logic(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (!doc.load_string("<root><child/></root>")) {
        return;
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        return;
    }

    // 1. pugi::xml_node::child
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        return;
    }

    // 2. pugi::xml_node::append_attribute (first)
    pugi::xml_attribute attr1 = child_node.append_attribute("attr1");
    if (!attr1) {
        return;
    }

    // 3. pugi::xml_node::append_attribute (second)
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");
    if (!attr2) {
        return;
    }

    // 4. pugi::xml_attribute::set_value (unsigned long long)
    unsigned long long val1 = 0;
    if (size >= sizeof(unsigned long long)) {
        val1 = *reinterpret_cast<const unsigned long long*>(data);
    }
    attr2.set_value(val1);

    // 5. pugi::xml_node::append_attribute (third)
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");
    if (!attr3) {
        return;
    }

    // 6. pugi::xml_node::append_attribute (fourth)
    pugi::xml_attribute attr4 = child_node.append_attribute("attr4");
    if (!attr4) {
        return;
    }

    // 7. pugi::xml_attribute::set_value (long long)
    long long val2 = 0;
    size_t offset = 0;
    if (size >= sizeof(unsigned long long)) {
        offset = sizeof(unsigned long long);
    }
    if (size >= offset + sizeof(long long)) {
        val2 = *reinterpret_cast<const long long*>(data + offset);
    }
    attr4.set_value(val2);
}

extern "C" int LLVMFuzzerTestOneInput_31(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        fuzz_logic(Data, Size);
    } catch (...) {
        // Ignore exceptions to prevent fuzzer crashes
    }
    
    return 0;
}