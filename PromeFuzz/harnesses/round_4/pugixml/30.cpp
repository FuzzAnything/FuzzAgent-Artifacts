// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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

static pugi::xml_document create_test_document() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Add some child nodes with various names to allow removal operations
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    root.append_child("child4");
    root.append_child("child5");
    root.append_child("test");
    root.append_child("data");
    root.append_child("node");
    
    // Add some nested structure
    pugi::xml_node child = root.child("child1");
    child.append_child("nested1");
    child.append_child("nested2");
    
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_30(const uint8_t *Data, size_t Size) {
    // Create a test document
    pugi::xml_document doc = create_test_document();
    pugi::xml_node root = doc.child("root");
    
    // Prepare strings from fuzzer input for node names
    // Ensure null-termination for const char* parameters
    std::string name1, name2;
    if (Size > 0) {
        size_t half = Size / 2;
        name1.assign(reinterpret_cast<const char*>(Data), std::min(half, Size));
        name2.assign(reinterpret_cast<const char*>(Data) + half, Size - half);
    } else {
        name1 = "test";
        name2 = "data";
    }
    
    // Ensure null termination
    name1.push_back('\0');
    name2.push_back('\0');
    
    // Create string_view versions
    std::string_view sv1(name1.c_str(), name1.size() - 1); // Exclude null terminator
    std::string_view sv2(name2.c_str(), name2.size() - 1);
    
    // Get some child nodes for the remove_child(xml_node) overload
    pugi::xml_node child1 = root.child("child1");
    pugi::xml_node child2 = root.child("child2");
    pugi::xml_node child3 = root.child("child3");
    pugi::xml_node child4 = root.child("child4");
    pugi::xml_node child5 = root.child("child5");
    
    // Execute the sequence of API calls
    // 1. remove_child (const char*)
    root.remove_child(name1.c_str());
    
    // 2. remove_child (string_view)
    root.remove_child(sv1);
    
    // 3. child (const char*)
    root.child(name1.c_str());
    
    // 4. child (const char*)
    root.child(name2.c_str());
    
    // 5. remove_child (const char*)
    root.remove_child(name2.c_str());
    
    // 6. remove_child (xml_node&) - using child1
    if (child1) {
        root.remove_child(child1);
    }
    
    // 7. child (const char*)
    root.child("test");
    
    // 8. remove_child (xml_node&) - using child2
    if (child2) {
        root.remove_child(child2);
    }
    
    // 9. child (const char*)
    root.child("data");
    
    // 10. remove_child (string_view)
    root.remove_child(sv2);
    
    // 11. remove_child (const char*)
    root.remove_child("node");
    
    // 12. remove_child (string_view) - using sv1 again
    root.remove_child(sv1);
    
    // 13. remove_child (xml_node&) - using child3
    if (child3) {
        root.remove_child(child3);
    }
    
    // 14. remove_child (xml_node&) - using child4
    if (child4) {
        root.remove_child(child4);
    }
    
    // 15. remove_child (xml_node&) - using child5
    if (child5) {
        root.remove_child(child5);
    }
    
    // Additional calls to explore more states
    // Try removing non-existent nodes
    root.remove_child("nonexistent");
    root.remove_child(std::string_view("nonexistent"));
    
    // Try getting children after removals
    root.child("child1");
    root.child("child2");
    root.child("child3");
    
    // Try removing already removed nodes
    if (child1) {
        root.remove_child(child1);
    }
    
    return 0;
}