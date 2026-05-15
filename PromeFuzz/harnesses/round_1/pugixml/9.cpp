// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <string_view>

static pugi::xml_document create_test_document(const uint8_t* Data, size_t Size) {
    pugi::xml_document doc;
    std::string content(reinterpret_cast<const char*>(Data), Size);
    if (content.empty()) {
        content = "dummy";
    }
    
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    
    root.append_attribute("attr1").set_value(content.c_str());
    root.append_attribute("attr2").set_value(content.c_str());
    child1.append_attribute("childattr1").set_value(content.c_str());
    child2.append_attribute("childattr2").set_value(content.c_str());
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    std::string name(reinterpret_cast<const char*>(Data), Size);
    if (name.empty()) {
        name = "test";
    }
    
    try {
        pugi::xml_document doc = create_test_document(Data, Size);
        pugi::xml_node root = doc.child("root");
        
        if (!root.empty()) {
            pugi::xml_attribute first_attr = root.first_attribute();
            if (!first_attr.empty()) {
                // 1. insert_attribute_after
                root.insert_attribute_after(name.c_str(), first_attr);
                
                // 2. insert_attribute_before
                root.insert_attribute_before(name.c_str(), first_attr);
            }
            
            // 3-6. first_child (4 times)
            pugi::xml_node node1 = root.first_child();
            pugi::xml_node node2 = node1.first_child();
            pugi::xml_node node3 = root.first_child();
            pugi::xml_node node4 = node3.first_child();
            
            // Prepare attribute for copying
            pugi::xml_attribute copy_attr = root.first_attribute();
            if (!copy_attr.empty()) {
                // 7. append_copy
                root.append_copy(copy_attr);
                
                // 8. prepend_copy
                root.prepend_copy(copy_attr);
                
                // 9. insert_copy_after
                root.insert_copy_after(copy_attr, copy_attr);
                
                // 10. insert_copy_before
                root.insert_copy_before(copy_attr, copy_attr);
            }
        }
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }
    
    return 0;
}