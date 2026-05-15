// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    if (Size < 1) return 0;

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Create some initial attributes for testing
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    
    // Create a reference attribute for insertion functions
    pugi::xml_attribute ref_attr = root.append_attribute("ref");
    ref_attr.set_value("reference");
    
    // Create some child nodes for testing
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    // Use fuzzer data to generate attribute names
    std::string attr_name;
    if (Size > 0) {
        size_t name_len = Data[0] % 16 + 1;
        if (name_len > Size - 1) name_len = Size - 1;
        if (name_len > 0) {
            attr_name.assign(reinterpret_cast<const char*>(Data + 1), name_len);
        }
    }
    
    if (attr_name.empty()) {
        attr_name = "fuzz_attr";
    }
    
    // 1. insert_attribute_after
    pugi::xml_attribute new_attr1 = root.insert_attribute_after(attr_name.c_str(), attr1);
    
    // 2. insert_attribute_before
    pugi::xml_attribute new_attr2 = root.insert_attribute_before(attr_name.c_str(), attr2);
    
    // 3-6. first_child (called 4 times as specified)
    pugi::xml_node first = root.first_child();
    first = root.first_child();
    first = root.first_child();
    first = root.first_child();
    
    // 7. append_copy
    pugi::xml_attribute appended = root.append_copy(ref_attr);
    
    // 8. prepend_copy
    pugi::xml_attribute prepended = root.prepend_copy(ref_attr);
    
    // 9. insert_copy_after
    pugi::xml_attribute inserted_after = root.insert_copy_after(ref_attr, attr1);
    
    // 10. insert_copy_before
    pugi::xml_attribute inserted_before = root.insert_copy_before(ref_attr, attr2);
    
    // Also test with child nodes
    if (child1) {
        pugi::xml_node copied_child = root.append_copy(child1);
        pugi::xml_node prepended_child = root.prepend_copy(child2);
    }
    
    // Clean up - let RAII handle it
    return 0;
}