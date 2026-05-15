// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    // Create a document and parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_fragment);
    
    // If parsing fails, create a minimal document to work with
    if (!result) {
        doc.append_child("root");
    }
    
    // Get the root node or create one if document is empty
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Create some child nodes to work with
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Create a node with value for testing
    pugi::xml_node value_node = root.append_child(pugi::node_pcdata);
    value_node.set_value("test_value");
    
    // Create a node to copy
    pugi::xml_node copy_source = doc.append_child("copy_source");
    copy_source.append_attribute("attr") = "value";
    copy_source.append_child("nested");
    
    // Create string views for child() calls
    // Use fixed names since we need null-terminated strings
    const char* child_names[] = {"child1", "child2", "child3", "copy_source", "nonexistent"};
    
    // Execute the specified sequence of API calls
    // 1. prepend_copy
    root.prepend_copy(copy_source);
    
    // 2. child
    root.child(child_names[0]);
    
    // 3. prepend_copy
    root.prepend_copy(copy_source);
    
    // 4. prepend_copy
    root.prepend_copy(copy_source);
    
    // 5. child
    root.child(child_names[1]);
    
    // 6. prepend_copy
    root.prepend_copy(copy_source);
    
    // 7. child
    root.child(child_names[2]);
    
    // 8. first_child
    pugi::xml_node first = root.first_child();
    
    // 9. value
    if (first) {
        first.value();
    }
    
    // 10. child
    root.child(child_names[3]);
    
    // 11. prepend_copy
    root.prepend_copy(copy_source);
    
    // 12. child
    root.child(child_names[4]);
    
    // 13. child
    root.child(child_names[0]);
    
    // 14. name
    if (first) {
        first.name();
    }
    
    // 15. child
    root.child(child_names[1]);
    
    // 16. child
    root.child(child_names[2]);
    
    // 17. prepend_copy
    root.prepend_copy(copy_source);
    
    // 18. child
    root.child(child_names[3]);
    
    // 19. first_child
    pugi::xml_node first2 = root.first_child();
    
    // 20. next_sibling
    if (first2) {
        first2.next_sibling();
    }
    
    // 21. value
    if (first2) {
        first2.value();
    }
    
    return 0;
}