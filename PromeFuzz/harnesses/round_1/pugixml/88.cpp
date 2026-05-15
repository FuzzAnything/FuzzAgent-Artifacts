// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_node_set::first at pugixml.cpp:12900:42 in pugixml.hpp
// pugi::xpath_node_set::type at pugixml.cpp:12864:54 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
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
#include <vector>
#include <algorithm>

static pugi::xpath_node_set create_xpath_node_set_from_fuzz_data(const uint8_t* data, size_t size) {
    // Create a vector of xpath_node objects with default construction
    std::vector<pugi::xpath_node> nodes;
    
    // Use fuzz data to determine how many nodes to create (limit to reasonable size)
    size_t node_count = 0;
    if (size > 0) {
        node_count = std::min<size_t>(data[0] % 100, size);
    }
    
    nodes.resize(node_count);
    
    // Determine type from fuzz data
    pugi::xpath_node_set::type_t type = pugi::xpath_node_set::type_unsorted;
    if (size > 1) {
        switch (data[1] % 3) {
            case 0: type = pugi::xpath_node_set::type_unsorted; break;
            case 1: type = pugi::xpath_node_set::type_sorted; break;
            case 2: type = pugi::xpath_node_set::type_sorted_reverse; break;
        }
    }
    
    // Create xpath_node_set from pointer range (const_iterator is const xpath_node*)
    const pugi::xpath_node* begin_ptr = nodes.data();
    const pugi::xpath_node* end_ptr = nodes.data() + nodes.size();
    
    return pugi::xpath_node_set(begin_ptr, end_ptr, type);
}

extern "C" int LLVMFuzzerTestOneInput_88(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    // Create xpath_node_set from fuzz data
    pugi::xpath_node_set node_set = create_xpath_node_set_from_fuzz_data(Data, Size);
    
    // Test all target API functions
    // 1. first()
    pugi::xpath_node first_node = node_set.first();
    (void)first_node; // Use variable to avoid unused warning
    
    // 2. type()
    pugi::xpath_node_set::type_t set_type = node_set.type();
    (void)set_type;
    
    // 3. begin() and 4. end()
    pugi::xpath_node_set::const_iterator begin_it = node_set.begin();
    pugi::xpath_node_set::const_iterator end_it = node_set.end();
    
    // Iterate through the set using begin() and end()
    for (auto it = begin_it; it != end_it; ++it) {
        (void)*it;
    }
    
    // 5. empty()
    bool is_empty = node_set.empty();
    (void)is_empty;
    
    // 6. size()
    size_t set_size = node_set.size();
    (void)set_size;
    
    // Verify consistency between functions
    if (is_empty != (set_size == 0)) {
        // This should never happen for valid implementations
        return 1;
    }
    
    if (is_empty != (begin_it == end_it)) {
        // This should never happen for valid implementations
        return 1;
    }
    
    if (!is_empty && first_node != *begin_it) {
        // In unsorted sets, first() should return the first element in storage order
        // In sorted sets, first() should return the first element in document order
        // We can't verify document order without actual XML data, but we can
        // at least check that first() returns a valid node when set is not empty
        if (first_node.operator!() && begin_it->operator!()) {
            // Both are null nodes, this is OK
        } else if (first_node != *begin_it) {
            // For our test with default-constructed nodes, they should be equal
            return 1;
        }
    }
    
    return 0;
}