// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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

static void initializeTestDocument(pugi::xml_document& doc) {
    // Create a root node with some initial structure
    pugi::xml_node root = doc.append_child("root");
    root.append_child("child1").append_child(pugi::node_pcdata).set_value("value1");
    root.append_child("child2").append_child(pugi::node_pcdata).set_value("value2");
    root.append_child("child3").append_child(pugi::node_pcdata).set_value("value3");
    
    // Add some attributes for variety
    root.append_attribute("attr1") = "attr_value1";
    root.first_child().append_attribute("child_attr") = "child_attr_value";
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Create XML document
    pugi::xml_document doc;
    initializeTestDocument(doc);
    
    // Get root node
    pugi::xml_node root = doc.root();
    if (!root) return 0;
    
    // Get a prototype node for copying
    pugi::xml_node proto = root.first_child();
    
    // Prepare string for child() calls - ensure null termination
    std::string child_name;
    if (Size > 0) {
        child_name.assign(reinterpret_cast<const char*>(Data), std::min(Size, (size_t)100));
        // Ensure null termination
        if (child_name.size() > 0) {
            child_name[child_name.size() - 1] = '\0';
        }
    } else {
        child_name = "child1";
    }
    
    // Execute the sequence of API calls
    try {
        // 1. pugi::xml_node::prepend_copy
        pugi::xml_node node1 = root.prepend_copy(proto);
        
        // 2. pugi::xml_node::child
        pugi::xml_node node2 = root.child(child_name.c_str());
        
        // 3. pugi::xml_node::prepend_copy
        pugi::xml_node node3 = root.prepend_copy(proto);
        
        // 4. pugi::xml_node::prepend_copy
        pugi::xml_node node4 = root.prepend_copy(proto);
        
        // 5. pugi::xml_node::child
        pugi::xml_node node5 = root.child(child_name.c_str());
        
        // 6. pugi::xml_node::prepend_copy
        pugi::xml_node node6 = root.prepend_copy(proto);
        
        // 7. pugi::xml_node::child
        pugi::xml_node node7 = root.child(child_name.c_str());
        
        // 8. pugi::xml_node::first_child
        pugi::xml_node node8 = root.first_child();
        
        // 9. pugi::xml_node::value
        if (node8) {
            const char* value9 = node8.value();
            (void)value9; // Use to avoid unused variable warning
        }
        
        // 10. pugi::xml_node::child
        pugi::xml_node node10 = root.child(child_name.c_str());
        
        // 11. pugi::xml_node::prepend_copy
        pugi::xml_node node11 = root.prepend_copy(proto);
        
        // 12. pugi::xml_node::child
        pugi::xml_node node12 = root.child(child_name.c_str());
        
        // 13. pugi::xml_node::child
        pugi::xml_node node13 = root.child(child_name.c_str());
        
        // 14. pugi::xml_node::name
        if (node13) {
            const char* name14 = node13.name();
            (void)name14; // Use to avoid unused variable warning
        }
        
        // 15. pugi::xml_node::child
        pugi::xml_node node15 = root.child(child_name.c_str());
        
        // 16. pugi::xml_node::child
        pugi::xml_node node16 = root.child(child_name.c_str());
        
        // 17. pugi::xml_node::prepend_copy
        pugi::xml_node node17 = root.prepend_copy(proto);
        
        // 18. pugi::xml_node::child
        pugi::xml_node node18 = root.child(child_name.c_str());
        
        // 19. pugi::xml_node::first_child
        pugi::xml_node node19 = root.first_child();
        
        // 20. pugi::xml_node::next_sibling
        pugi::xml_node node20 = node19.next_sibling();
        
        // 21. pugi::xml_node::value
        if (node20) {
            const char* value21 = node20.value();
            (void)value21; // Use to avoid unused variable warning
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup happens automatically when doc goes out of scope
    return 0;
}