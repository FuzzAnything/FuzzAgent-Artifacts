// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment: write data to dummy file and load document
    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("./dummy_file");

    // If parsing fails, try to parse from string directly to ensure we have some content
    if (!result) {
        // Ensure null termination for safety
        std::string str(reinterpret_cast<const char*>(Data), Size);
        if (str.back() != '\0') {
            str += '\0';
        }
        result = doc.load_string(str.c_str());
    }

    // Start traversal from root
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc; // Fallback to document root if no specific child found
    }

    // 1. pugi::xml_node::child
    pugi::xml_node node1 = root.child("child1");
    if (!node1) {
        node1 = root.child("node");
    }
    if (!node1) {
        node1 = root; // Fallback
    }

    // 2. pugi::xml_node::attribute
    pugi::xml_attribute attr1 = node1.attribute("attr1");
    if (!attr1) {
        attr1 = node1.attribute("id");
    }
    if (!attr1) {
        attr1 = node1.attribute("name");
    }

    // 3. pugi::xml_node::child
    pugi::xml_node node2 = node1.child("child2");
    if (!node2) {
        node2 = node1.child("item");
    }
    if (!node2) {
        node2 = node1; // Fallback
    }

    // 4. pugi::xml_node::attribute
    pugi::xml_attribute attr2 = node2.attribute("attr2");
    if (!attr2) {
        attr2 = node2.attribute("type");
    }
    if (!attr2) {
        attr2 = node2.attribute("value");
    }

    // 5. pugi::xml_attribute::next_attribute
    pugi::xml_attribute next1 = attr2.next_attribute();

    // 6. pugi::xml_attribute::next_attribute
    pugi::xml_attribute next2 = next1.next_attribute();

    // 7. pugi::xml_attribute::previous_attribute
    pugi::xml_attribute prev1 = next2.previous_attribute();

    // 8. pugi::xml_attribute::previous_attribute
    pugi::xml_attribute prev2 = prev1.previous_attribute();

    // 9. pugi::xml_attribute::next_attribute
    pugi::xml_attribute next3 = prev2.next_attribute();

    // 10. pugi::xml_attribute::previous_attribute
    pugi::xml_attribute prev3 = next3.previous_attribute();

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}