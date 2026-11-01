// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xpath_query::evaluate_node_set at pugixml.cpp:13384:43 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::begin at pugixml.cpp:5618:44 in pugixml.hpp
// pugi::xml_node::end at pugixml.cpp:5623:44 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::size at pugixml.cpp:12869:38 in pugixml.hpp
// pugi::xpath_node_set::begin at pugixml.cpp:12885:62 in pugixml.hpp
// pugi::xpath_node_set::end at pugixml.cpp:12890:62 in pugixml.hpp
// pugi::xpath_node_set::empty at pugixml.cpp:12874:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xpath_node_set::sort at pugixml.cpp:12895:36 in pugixml.hpp
// pugi::xml_object_range::empty at pugixml.hpp:338:8 in pugixml.hpp
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
        return doc;
    }
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        return pugi::xml_document();
    }
    return doc;
}

static pugi::xpath_node_set create_xpath_node_set(const pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        return pugi::xpath_node_set();
    }
    
    std::string expr_str(reinterpret_cast<const char*>(data), size);
    pugi::xpath_variable_set vars;
    
    try {
        pugi::xpath_query query(expr_str.c_str(), &vars);
        if (!query) {
            return pugi::xpath_node_set();
        }
        pugi::xpath_node_set set = query.evaluate_node_set(doc);
        return set;
    } catch (...) {
        return pugi::xpath_node_set();
    }
}

static pugi::xml_object_range<pugi::xml_node_iterator> create_xml_object_range(const pugi::xml_document& doc) {
    pugi::xml_node root = doc.first_child();
    if (!root) {
        pugi::xml_node_iterator begin;
        pugi::xml_node_iterator end;
        return pugi::xml_object_range<pugi::xml_node_iterator>(begin, end);
    }
    pugi::xml_node_iterator begin = root.begin();
    pugi::xml_node_iterator end = root.end();
    return pugi::xml_object_range<pugi::xml_node_iterator>(begin, end);
}

extern "C" int LLVMFuzzerTestOneInput_57(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    size_t split_point = Size / 2;
    if (split_point == 0) split_point = 1;
    
    const uint8_t* xml_data = Data;
    size_t xml_size = split_point;
    
    const uint8_t* xpath_data = Data + split_point;
    size_t xpath_size = Size - split_point;
    
    pugi::xml_document doc = parse_document(xml_data, xml_size);
    
    // Test xpath_node_set functions
    pugi::xpath_node_set set = create_xpath_node_set(doc, xpath_data, xpath_size);
    
    bool is_empty = set.empty();
    (void)is_empty;
    
    size_t set_size = set.size();
    (void)set_size;
    
    auto begin_it = set.begin();
    auto end_it = set.end();
    
    if (!set.empty()) {
        for (auto it = begin_it; it != end_it; ++it) {
            (void)it;
        }
    }
    
    set.sort(false);
    set.sort(true);
    
    // Test xml_object_range functions
    pugi::xml_object_range<pugi::xml_node_iterator> range = create_xml_object_range(doc);
    
    bool range_empty = range.empty();
    (void)range_empty;
    
    if (!range_empty) {
        for (auto it = range.begin(); it != range.end(); ++it) {
            (void)it;
        }
    }
    
    return 0;
}