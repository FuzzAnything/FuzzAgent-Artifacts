// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xpath_query::evaluate_node at pugixml.cpp:13406:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xpath_query::evaluate_boolean at pugixml.cpp:13289:33 in pugixml.hpp
// pugi::xpath_query::evaluate_number at pugixml.cpp:13310:35 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13332:37 in pugixml.hpp
// pugi::xpath_query::evaluate_string at pugixml.cpp:13354:35 in pugixml.hpp
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

static pugi::xml_document parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    if (size == 0) {
        doc.append_child(pugi::node_element).set_name("root");
        return doc;
    }
    
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    if (doc.load_string(xml_str.c_str()) == false) {
        doc.append_child(pugi::node_element).set_name("root");
    }
    return doc;
}

static pugi::xpath_query compile_xpath(const uint8_t* data, size_t size) {
    if (size == 0) {
        return pugi::xpath_query(".");
    }
    
    std::string xpath_str(reinterpret_cast<const char*>(data), size);
    try {
        return pugi::xpath_query(xpath_str.c_str());
    } catch (...) {
        return pugi::xpath_query(".");
    }
}

extern "C" int LLVMFuzzerTestOneInput_45(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    try {
        pugi::xml_document doc = parse_document(Data, Size);
        pugi::xpath_query query = compile_xpath(Data, Size);
        
        pugi::xml_node root = doc.child("root");
        if (!root) {
            root = doc.first_child();
            if (!root) {
                root = doc;
            }
        }
        
        // 1. pugi::xml_node::child
        pugi::xml_node child1 = root.child("child");
        
        // 2. pugi::xml_node::attribute
        pugi::xml_attribute attr = child1.attribute("id");
        if (!attr) {
            attr = root.attribute("id");
        }
        
        // 3. pugi::xml_node::child (again)
        pugi::xml_node child2 = child1.child("nested");
        if (!child2) {
            child2 = root.child("nested");
        }
        
        // Create context node
        pugi::xpath_node context_node;
        if (child2) {
            context_node = pugi::xpath_node(child2);
        } else if (attr) {
            context_node = pugi::xpath_node(attr, root);
        } else {
            context_node = pugi::xpath_node(root);
        }
        
        // 4. pugi::xpath_query::evaluate_boolean
        bool bool_result = query.evaluate_boolean(context_node);
        (void)bool_result;
        
        // 5. pugi::xpath_query::evaluate_number
        double num_result = query.evaluate_number(context_node);
        (void)num_result;
        
        // 6. pugi::xpath_query::evaluate_string (string_t overload)
        // Note: pugixml returns pugi::string_t which is typedef'd based on PUGIXML_WCHAR_MODE
        // We use pugi::string_t directly to avoid compilation errors
        pugi::string_t str_result1 = query.evaluate_string(context_node);
        (void)str_result1;
        
        // 6. pugi::xpath_query::evaluate_string (buffer overload)
        char buffer[256];
        size_t str_result2 = query.evaluate_string(buffer, sizeof(buffer), context_node);
        (void)str_result2;
        
        // 7. pugi::xpath_query::evaluate_string (string_t overload again)
        pugi::string_t str_result3 = query.evaluate_string(context_node);
        (void)str_result3;
        
        // 8. pugi::xpath_query::evaluate_node_set
        pugi::xpath_node_set node_set = query.evaluate_node_set(context_node);
        (void)node_set;
        
        // 9. pugi::xpath_query::evaluate_node
        pugi::xpath_node node_result = query.evaluate_node(context_node);
        (void)node_result;
        
    } catch (...) {
        // Ignore exceptions during fuzzing
    }
    
    return 0;
}