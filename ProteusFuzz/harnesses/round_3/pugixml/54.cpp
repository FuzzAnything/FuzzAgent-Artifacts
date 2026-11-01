// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
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
#include <vector>
#include <functional>

static pugi::xml_document parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        return doc;
    }
    // Attempt to parse as string, handling potential null bytes
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return doc;
}

static pugi::xml_node get_random_node(const pugi::xml_document& doc) {
    pugi::xml_node root = doc.root();
    if (!root) {
        return pugi::xml_node();
    }
    
    // Collect all nodes
    std::vector<pugi::xml_node> nodes;
    nodes.push_back(root);
    
    for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
        nodes.push_back(node);
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            nodes.push_back(child);
        }
    }
    
    if (nodes.empty()) {
        return root;
    }
    
    // Return a random node (using index based on size to avoid random dependency)
    size_t index = 0;
    if (!nodes.empty()) {
        index = nodes.size() > 1 ? 1 : 0;
    }
    return nodes[index];
}

extern "C" int LLVMFuzzerTestOneInput_54(const uint8_t* Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc = parse_document(Data, Size);
    
    // Test 1: first_attribute
    {
        pugi::xml_node node = get_random_node(doc);
        pugi::xml_attribute attr = node.first_attribute();
        (void)attr; // Suppress unused warning
    }
    
    // Test 2: find_attribute with various predicates
    {
        pugi::xml_node node = get_random_node(doc);
        
        // Predicate 1: Always true
        auto pred1 = [](pugi::xml_attribute) { return true; };
        pugi::xml_attribute attr1 = node.find_attribute(pred1);
        (void)attr1;
        
        // Predicate 2: Check name length
        auto pred2 = [](pugi::xml_attribute attr) { 
            return attr.name() && attr.name()[0] != '\0'; 
        };
        pugi::xml_attribute attr2 = node.find_attribute(pred2);
        (void)attr2;
        
        // Predicate 3: Check value
        auto pred3 = [](pugi::xml_attribute attr) { 
            return attr.value() && attr.value()[0] == 'a'; 
        };
        pugi::xml_attribute attr3 = node.find_attribute(pred3);
        (void)attr3;
    }
    
    // Test 3: attribute by name
    {
        pugi::xml_node node = get_random_node(doc);
        
        // Try with various names from input data
        std::vector<std::string> names = {"id", "class", "name", "value", "test"};
        for (const auto& name : names) {
            pugi::xml_attribute attr = node.attribute(name.c_str());
            (void)attr;
        }
        
        // Try with a name derived from input
        if (Size > 0) {
            char name_buf[32] = {0};
            size_t copy_len = std::min(Size, static_cast<size_t>(31));
            std::memcpy(name_buf, Data, copy_len);
            pugi::xml_attribute attr = node.attribute(name_buf);
            (void)attr;
        }
    }
    
    // Test 4: insert_copy_before
    {
        pugi::xml_node node = get_random_node(doc);
        if (!node) {
            // Skip if node is invalid
        } else {
            pugi::xml_attribute proto = node.first_attribute();
            pugi::xml_attribute attr = node.first_attribute();
            
            if (proto && attr) {
                pugi::xml_attribute new_attr = node.insert_copy_before(proto, attr);
                (void)new_attr;
            }
            
            // Try with second attribute if exists
            pugi::xml_attribute second_attr = proto.next_attribute();
            if (proto && second_attr) {
                pugi::xml_attribute new_attr2 = node.insert_copy_before(proto, second_attr);
                (void)new_attr2;
            }
        }
    }
    
    // Test 5: insert_attribute_after
    {
        pugi::xml_node node = get_random_node(doc);
        if (!node) {
            // Skip if node is invalid
        } else {
            pugi::xml_attribute attr = node.first_attribute();
            if (attr) {
                // Insert with various names
                const char* names[] = {"new_attr", "test", "id", "value"};
                for (const char* name : names) {
                    pugi::xml_attribute new_attr = node.insert_attribute_after(name, attr);
                    (void)new_attr;
                }
                
                // Try with name from input
                if (Size > 0) {
                    char name_buf[32] = {0};
                    size_t copy_len = std::min(Size, static_cast<size_t>(31));
                    std::memcpy(name_buf, Data, copy_len);
                    pugi::xml_attribute new_attr = node.insert_attribute_after(name_buf, attr);
                    (void)new_attr;
                }
            }
        }
    }
    
    // Test 6: last_attribute
    {
        pugi::xml_node node = get_random_node(doc);
        pugi::xml_attribute attr = node.last_attribute();
        (void)attr;
    }
    
    // Additional edge case tests
    {
        // Test on null node
        pugi::xml_node null_node;
        pugi::xml_attribute attr1 = null_node.first_attribute();
        pugi::xml_attribute attr2 = null_node.last_attribute();
        auto pred = [](pugi::xml_attribute) { return false; };
        pugi::xml_attribute attr3 = null_node.find_attribute(pred);
        pugi::xml_attribute attr4 = null_node.attribute("test");
        (void)attr1; (void)attr2; (void)attr3; (void)attr4;
    }
    
    return 0;
}