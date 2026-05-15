// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
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

static void test_xpath_node_set(const pugi::xpath_node_set& set) {
    // Test first() function
    pugi::xpath_node first_node = set.first();
    (void)first_node; // Use variable to avoid unused warning
    
    // Test type() function
    pugi::xpath_node_set::type_t set_type = set.type();
    (void)set_type; // Use variable to avoid unused warning
    
    // Test begin() and end() functions
    pugi::xpath_node_set::const_iterator begin_it = set.begin();
    pugi::xpath_node_set::const_iterator end_it = set.end();
    (void)begin_it; // Use variable to avoid unused warning
    (void)end_it; // Use variable to avoid unused warning
    
    // Test empty() function
    bool is_empty = set.empty();
    (void)is_empty; // Use variable to avoid unused warning
    
    // Test size() function
    size_t set_size = set.size();
    (void)set_size; // Use variable to avoid unused warning
    
    // Iterate through the set using begin() and end()
    for (pugi::xpath_node_set::const_iterator it = set.begin(); it != set.end(); ++it) {
        pugi::xpath_node node = *it;
        (void)node; // Use variable to avoid unused warning
    }
}

static pugi::xpath_node_set create_xpath_node_set_from_data(const uint8_t* data, size_t size) {
    // Create some dummy xpath_node objects
    static pugi::xml_attribute dummy_attr;
    static pugi::xml_node dummy_node;
    
    // Create a small array of xpath_node objects
    pugi::xpath_node nodes[3];
    
    // Initialize nodes with dummy data
    for (int i = 0; i < 3; i++) {
        nodes[i] = pugi::xpath_node(dummy_node);
    }
    
    // Determine the type based on input data
    pugi::xpath_node_set::type_t type = pugi::xpath_node_set::type_unsorted;
    if (size > 0) {
        uint8_t type_selector = data[0] % 3;
        switch (type_selector) {
            case 0: type = pugi::xpath_node_set::type_unsorted; break;
            case 1: type = pugi::xpath_node_set::type_sorted; break;
            case 2: type = pugi::xpath_node_set::type_sorted_reverse; break;
        }
    }
    
    // Create xpath_node_set with varying number of nodes based on input size
    size_t node_count = 0;
    if (size > 1) {
        node_count = data[1] % 4; // 0-3 nodes
    }
    
    return pugi::xpath_node_set(nodes, nodes + node_count, type);
}

extern "C" int LLVMFuzzerTestOneInput_87(const uint8_t* Data, size_t Size) {
    // Create xpath_node_set from fuzzer input
    pugi::xpath_node_set node_set = create_xpath_node_set_from_data(Data, Size);
    
    // Test all the target API functions
    test_xpath_node_set(node_set);
    
    // Create additional xpath_node_set with different configurations
    if (Size > 2) {
        // Create empty set
        pugi::xpath_node_set empty_set(nullptr, nullptr, pugi::xpath_node_set::type_unsorted);
        test_xpath_node_set(empty_set);
        
        // Create set with single node
        pugi::xml_node dummy_node;
        pugi::xpath_node single_node(dummy_node);
        pugi::xpath_node_set single_node_set(&single_node, &single_node + 1, 
                                            static_cast<pugi::xpath_node_set::type_t>(Data[2] % 3));
        test_xpath_node_set(single_node_set);
    }
    
    return 0;
}