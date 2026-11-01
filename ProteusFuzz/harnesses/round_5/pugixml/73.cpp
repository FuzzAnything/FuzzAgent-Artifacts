// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::find_child at pugixml.hpp:693:42 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_node at pugixml.hpp:705:42 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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
#include <cstring>
#include <string>
#include <vector>
#include <functional>

static bool predicate_by_name(pugi::xml_node node, const char* target_name) {
    if (!node) return false;
    return std::strcmp(node.name(), target_name) == 0;
}

static bool predicate_by_value(pugi::xml_node node, const char* target_value) {
    if (!node) return false;
    const char* val = node.value();
    if (!val) return false;
    return std::strcmp(val, target_value) == 0;
}

static bool predicate_by_attribute(pugi::xml_node node, const char* attr_name, const char* attr_value) {
    if (!node) return false;
    pugi::xml_attribute attr = node.attribute(attr_name);
    if (!attr) return false;
    return std::strcmp(attr.value(), attr_value) == 0;
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    
    // Ensure the input is null-terminated for parsing
    std::string xml_str(reinterpret_cast<const char*>(Data), Size);
    
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    
    // Even if parsing fails, we can test the API on the potentially partial document
    // or an empty document to ensure robustness.
    
    pugi::xml_node root = doc;
    
    // Test 1: parent() on root (should be null)
    pugi::xml_node parent_node = root.parent();
    (void)parent_node; // Suppress unused warning
    
    // Test 2: first_child()
    pugi::xml_node first = root.first_child();
    
    // Test 3: last_child()
    pugi::xml_node last = root.last_child();
    
    // Test 4: child() with string_view
    // We try a few potential names derived from the input or static strings
    const char* test_names[] = {"item", "root", "child", "test", nullptr};
    for (int i = 0; test_names[i]; ++i) {
        pugi::xml_node child_node = root.child(test_names[i]);
        (void)child_node;
        
        if (child_node) {
            // If found, test parent on child
            pugi::xml_node child_parent = child_node.parent();
            (void)child_parent;
        }
    }
    
    // Test 5: find_child with various predicates
    // Predicate: match name
    pugi::xml_node found_by_name = root.find_child([&](pugi::xml_node n) {
        return n && n.name() && std::strcmp(n.name(), "item") == 0;
    });
    (void)found_by_name;
    
    // Predicate: match value
    pugi::xml_node found_by_val = root.find_child([&](pugi::xml_node n) {
        return n && n.value() && std::strcmp(n.value(), "test") == 0;
    });
    (void)found_by_val;
    
    // Predicate: match attribute
    pugi::xml_node found_by_attr = root.find_child([&](pugi::xml_node n) {
        return n && n.attribute("id") && std::strcmp(n.attribute("id").value(), "1") == 0;
    });
    (void)found_by_attr;
    
    // Test 6: find_node (depth-first) with various predicates
    pugi::xml_node found_node_name = root.find_node([&](pugi::xml_node n) {
        return n && n.name() && std::strcmp(n.name(), "item") == 0;
    });
    (void)found_node_name;
    
    pugi::xml_node found_node_val = root.find_node([&](pugi::xml_node n) {
        return n && n.value() && std::strcmp(n.value(), "test") == 0;
    });
    (void)found_node_val;
    
    pugi::xml_node found_node_attr = root.find_node([&](pugi::xml_node n) {
        return n && n.attribute("id") && std::strcmp(n.attribute("id").value(), "1") == 0;
    });
    (void)found_node_attr;
    
    // Additional exploration: traverse the tree manually to trigger more states
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        // Test parent on child
        pugi::xml_node p = node.parent();
        (void)p;
        
        // Test first/last on child
        pugi::xml_node fc = node.first_child();
        pugi::xml_node lc = node.last_child();
        (void)fc;
        (void)lc;
        
        // Test child on child
        pugi::xml_node c = node.child("subitem");
        (void)c;
        
        // Test find_child on child
        pugi::xml_node fc_pred = node.find_child([](pugi::xml_node n) {
            return n && n.type() == pugi::node_element;
        });
        (void)fc_pred;
        
        // Test find_node on child
        // Fixed: Use pugi::node_text instead of pugi::node_text (which doesn't exist as a namespace member directly in some contexts if not careful, but actually pugi::node_text is correct enum value)
        // The error indicated 'node_text' not found in namespace 'pugi'. 
        // In pugixml, the enum values are pugi::node_element, pugi::node_pcdata, pugi::node_cdata, etc.
        // 'node_text' is NOT a standard enum value in pugixml. It is 'node_pcdata'.
        pugi::xml_node fn_pred = node.find_node([](pugi::xml_node n) {
            return n && n.type() == pugi::node_pcdata;
        });
        (void)fn_pred;
    }
    
    return 0;
}