// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6414:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <string>
#include <vector>

static std::vector<std::string> split_strings(const uint8_t* data, size_t size) {
    std::vector<std::string> result;
    const char* p = reinterpret_cast<const char*>(data);
    const char* end = p + size;
    
    while (p < end) {
        const char* start = p;
        while (p < end && *p != '\0') {
            ++p;
        }
        result.emplace_back(start, p - start);
        if (p < end && *p == '\0') {
            ++p;
        }
    }
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t* Data, size_t Size) {
    if (Size < 1) return 0;
    
    auto strings = split_strings(Data, Size);
    if (strings.empty()) return 0;
    
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_attribute("attr1") = "value1";
    child1.text().set("text1");
    
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_attribute("attr2") = "value2";
    child2.text().set("text2");
    
    pugi::xml_node child3 = root.append_child("child3");
    child3.append_attribute("attr3") = "value3";
    
    size_t str_idx = 0;
    
    auto get_string = [&]() -> const char* {
        if (str_idx < strings.size()) {
            return strings[str_idx++].c_str();
        }
        return "";
    };
    
    try {
        pugi::xml_node result1 = root.prepend_copy(child1);
        (void)result1;
        
        pugi::xml_node result2 = root.child(get_string());
        (void)result2;
        
        pugi::xml_node result3 = root.prepend_copy(child2);
        (void)result3;
        
        pugi::xml_node result4 = root.prepend_copy(child3);
        (void)result4;
        
        pugi::xml_node result5 = root.child(get_string());
        (void)result5;
        
        pugi::xml_node result6 = root.prepend_copy(child1);
        (void)result6;
        
        pugi::xml_node result7 = root.child(get_string());
        (void)result7;
        
        pugi::xml_node result8 = root.first_child();
        (void)result8;
        
        const char* result9 = root.value();
        (void)result9;
        
        pugi::xml_node result10 = root.child(get_string());
        (void)result10;
        
        pugi::xml_node result11 = root.prepend_copy(child2);
        (void)result11;
        
        pugi::xml_node result12 = root.child(get_string());
        (void)result12;
        
        pugi::xml_node result13 = root.child(get_string());
        (void)result13;
        
        const char* result14 = root.name();
        (void)result14;
        
        pugi::xml_node result15 = root.child(get_string());
        (void)result15;
        
        pugi::xml_node result16 = root.child(get_string());
        (void)result16;
        
        pugi::xml_node result17 = root.prepend_copy(child3);
        (void)result17;
        
        pugi::xml_node result18 = root.child(get_string());
        (void)result18;
        
        pugi::xml_node result19 = root.first_child();
        (void)result19;
        
        pugi::xml_node result20 = result19.next_sibling();
        (void)result20;
        
        const char* result21 = result20.value();
        (void)result21;
        
    } catch (...) {
    }
    
    return 0;
}