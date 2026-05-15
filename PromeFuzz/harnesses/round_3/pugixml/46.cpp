// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
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
#include <cstdint>
#include <cstring>

static void initialize_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Create a simple XML structure to work with
    pugi::xml_node root = doc.append_child("root");
    
    // Use fuzzer data to create some child nodes with names
    if (size > 0) {
        // Create first child with name based on first byte
        char name1[2] = {static_cast<char>('A' + (data[0] % 26)), '\0'};
        pugi::xml_node child1 = root.append_child(name1);
        child1.append_child(pugi::node_pcdata).set_value("value1");
        
        // Create second child if we have enough data
        if (size > 1) {
            char name2[2] = {static_cast<char>('A' + (data[1] % 26)), '\0'};
            pugi::xml_node child2 = root.append_child(name2);
            child2.append_child(pugi::node_pcdata).set_value("value2");
        }
        
        // Create third child if we have enough data
        if (size > 2) {
            char name3[2] = {static_cast<char>('A' + (data[2] % 26)), '\0'};
            pugi::xml_node child3 = root.append_child(name3);
            child3.append_child(pugi::node_pcdata).set_value("value3");
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    initialize_document(doc, Data, Size);
    
    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) return 0;
    
    // Create some child nodes to work with
    pugi::xml_node child_a = root.append_child("childA");
    child_a.append_child(pugi::node_pcdata).set_value("valueA");
    
    pugi::xml_node child_b = root.append_child("childB");
    child_b.append_child(pugi::node_pcdata).set_value("valueB");
    
    pugi::xml_node child_c = root.append_child("childC");
    child_c.append_child(pugi::node_pcdata).set_value("valueC");
    
    // Use fuzzer data to create node names for child lookups
    // Ensure null-terminated strings
    char lookup_name1[32] = {0};
    char lookup_name2[32] = {0};
    
    if (Size > 0) {
        size_t copy_len1 = Size < 31 ? Size : 31;
        memcpy(lookup_name1, Data, copy_len1);
        lookup_name1[copy_len1] = '\0';
    } else {
        strcpy(lookup_name1, "childA");
    }
    
    if (Size > 1) {
        // Use different part of input for second name
        size_t offset = Size > 16 ? 16 : 1;
        size_t copy_len2 = (Size - offset) < 31 ? (Size - offset) : 31;
        memcpy(lookup_name2, Data + offset, copy_len2);
        lookup_name2[copy_len2] = '\0';
    } else {
        strcpy(lookup_name2, "childB");
    }
    
    // Execute the sequence of API calls as specified
    // 1. pugi::xml_node::prepend_copy
    root.prepend_copy(child_a);
    
    // 2. pugi::xml_node::child
    root.child(lookup_name1);
    
    // 3. pugi::xml_node::prepend_copy
    root.prepend_copy(child_b);
    
    // 4. pugi::xml_node::prepend_copy
    root.prepend_copy(child_c);
    
    // 5. pugi::xml_node::child
    root.child(lookup_name2);
    
    // 6. pugi::xml_node::prepend_copy
    root.prepend_copy(child_a);
    
    // 7. pugi::xml_node::child
    root.child("childC");
    
    // 8. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    
    // 9. pugi::xml_node::value
    if (first) {
        first.value();
    }
    
    // 10. pugi::xml_node::child
    root.child(lookup_name1);
    
    // 11. pugi::xml_node::prepend_copy
    if (first) {
        root.prepend_copy(first);
    }
    
    // 12. pugi::xml_node::child
    root.child(lookup_name2);
    
    // 13. pugi::xml_node::child
    root.child("nonexistent");
    
    // 14. pugi::xml_node::name
    if (first) {
        first.name();
    }
    
    // 15. pugi::xml_node::child
    root.child(lookup_name1);
    
    // 16. pugi::xml_node::child
    root.child(lookup_name2);
    
    // 17. pugi::xml_node::prepend_copy
    if (child_b) {
        root.prepend_copy(child_b);
    }
    
    // 18. pugi::xml_node::child
    root.child("childC");
    
    // 19. pugi::xml_node::first_child
    pugi::xml_node first2 = root.first_child();
    
    // 20. pugi::xml_node::next_sibling
    if (first2) {
        first2.next_sibling();
    }
    
    // 21. pugi::xml_node::value
    if (first2) {
        pugi::xml_node next = first2.next_sibling();
        if (next) {
            next.value();
        }
    }
    
    return 0;
}