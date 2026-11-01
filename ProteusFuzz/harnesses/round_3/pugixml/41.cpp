// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    if (Size < 10) return 0;

    pugi::xml_document doc;
    if (!doc.load_string("<?xml version=\"1.0\"?><root><node>data</node></root>")) {
        return 0;
    }

    pugi::xml_node root = doc.root();
    if (!root) return 0;

    pugi::xml_node sibling = root.child("node");
    if (!sibling) return 0;

    // 1. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node1 = root.insert_child_after("new_node1", sibling);
    
    // 2. pugi::xml_node::child
    pugi::xml_node child1 = root.child("new_node1");
    
    // 3. pugi::xml_node::child
    pugi::xml_node child2 = root.child("node");
    
    // 4. pugi::xml_node::child
    pugi::xml_node child3 = root.child("nonexistent");
    
    // 5. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name = "new_node2";
    pugi::xml_node new_node2 = root.insert_child_after(sv_name, sibling);
    
    // 6. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node3 = root.insert_child_after("new_node3", sibling);
    
    // 7. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name2 = "new_node4";
    pugi::xml_node new_node4 = root.insert_child_after(sv_name2, sibling);
    
    // 8. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node5 = root.insert_child_after("new_node5", sibling);
    
    // 9. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name3 = "new_node6";
    pugi::xml_node new_node6 = root.insert_child_after(sv_name3, sibling);
    
    // 10. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node7 = root.insert_child_after("new_node7", sibling);

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}