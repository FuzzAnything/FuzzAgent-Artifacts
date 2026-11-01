// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::insert_move_after at pugixml.cpp:6499:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::prepend_move at pugixml.cpp:6483:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::append_move at pugixml.cpp:6467:34 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
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
#include <string>
#include <cstring>

static pugi::xml_document* create_document_from_fuzz_data(const uint8_t* data, size_t size) {
    if (size == 0) return nullptr;
    
    // Ensure null termination for safety, though pugixml handles non-null-terminated input
    std::string input(reinterpret_cast<const char*>(data), size);
    
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc->load_string(input.c_str());
    
    // Even if parsing fails partially, we can still test with the document structure
    // as long as we have a root or some nodes to work with
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_12(const uint8_t *Data, size_t Size) {
    // Step 1: Prepare the environment
    if (Size == 0) return 0;
    
    pugi::xml_document* doc = create_document_from_fuzz_data(Data, Size);
    if (!doc) return 0;
    
    try {
        pugi::xml_node root = doc->root();
        
        // If no root exists, create a simple structure to work with
        if (!root) {
            root = doc->append_child("root");
        }
        
        // Create some child nodes to work with for move operations
        pugi::xml_node child1 = root.append_child("child1");
        pugi::xml_node child2 = root.append_child("child2");
        pugi::xml_node child3 = root.append_child("child3");
        
        // Ensure we have at least 3 children for move operations
        if (!child1 || !child2 || !child3) {
            delete doc;
            return 0;
        }
        
        // Step 2: Invoke target functions in specified order
        
        // 1. pugi::xml_node::child
        pugi::xml_node node1 = root.child("child1");
        
        // 2. pugi::xml_node::child (second call)
        pugi::xml_node node2 = root.child("child2");
        
        // 3. pugi::xml_node::first_child
        pugi::xml_node first = root.first_child();
        
        // 4. pugi::xml_node::last_child
        pugi::xml_node last = root.last_child();
        
        // 5. pugi::xml_node::prepend_move (first call)
        if (node1 && node2) {
            root.prepend_move(node2);
        }
        
        // 6. pugi::xml_node::prepend_move (second call)
        if (node1 && child3) {
            root.prepend_move(child3);
        }
        
        // 7. pugi::xml_node::append_move (first call)
        if (node1 && first) {
            root.append_move(first);
        }
        
        // 8. pugi::xml_node::append_move (second call)
        if (node1 && last) {
            root.append_move(last);
        }
        
        // 9. pugi::xml_node::insert_move_before (first call)
        if (node1 && node2) {
            root.insert_move_before(node1, node2);
        }
        
        // 10. pugi::xml_node::insert_move_before (second call)
        if (first && last) {
            root.insert_move_before(first, last);
        }
        
        // 11. pugi::xml_node::insert_move_after (first call)
        if (node1 && node2) {
            root.insert_move_after(node1, node2);
        }
        
        // 12. pugi::xml_node::insert_move_after (second call)
        if (first && last) {
            root.insert_move_after(first, last);
        }
        
        // 13. pugi::xml_node::append_move (final call)
        if (node1 && child3) {
            root.append_move(child3);
        }
        
    } catch (...) {
        // Handle any exceptions that might occur
    }
    
    // Step 3: Cleanup
    delete doc;
    
    return 0;
}