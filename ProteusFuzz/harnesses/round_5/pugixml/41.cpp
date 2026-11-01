// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
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

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_document* doc = new pugi::xml_document();
    pugi::xml_parse_result result = doc->load_string(xml_str.c_str());
    if (!result) {
        delete doc;
        return nullptr;
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_41(const uint8_t *Data, size_t Size) {
    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (root.empty()) {
        delete doc;
        return 0;
    }

    // Ensure there is at least one child to use as a reference for insert_child_after
    pugi::xml_node ref_node = root.first_child();
    if (ref_node.empty()) {
        // Try to create a child to use as reference
        ref_node = root.append_child("dummy");
        if (ref_node.empty()) {
            delete doc;
            return 0;
        }
    }

    // 1. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node1 = root.insert_child_after("child1", ref_node);
    
    // 2. pugi::xml_node::child
    pugi::xml_node found1 = root.child("child1");
    
    // 3. pugi::xml_node::child
    pugi::xml_node found2 = root.child("dummy");
    
    // 4. pugi::xml_node::child
    pugi::xml_node found3 = root.child("nonexistent");
    
    // 5. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node2 = root.insert_child_after("child2", found1.empty() ? ref_node : found1);
    
    // 6. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name1 = "child3";
    pugi::xml_node new_node3 = root.insert_child_after(sv_name1, found2.empty() ? ref_node : found2);
    
    // 7. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node4 = root.insert_child_after("child4", found3.empty() ? ref_node : found3);
    
    // 8. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name2 = "child5";
    pugi::xml_node new_node5 = root.insert_child_after(sv_name2, new_node1);
    
    // 9. pugi::xml_node::insert_child_after (char_t* overload)
    pugi::xml_node new_node6 = root.insert_child_after("child6", new_node2);
    
    // 10. pugi::xml_node::insert_child_after (string_view_t overload)
    std::string_view sv_name3 = "child7";
    pugi::xml_node new_node7 = root.insert_child_after(sv_name3, new_node3);

    delete doc;
    return 0;
}