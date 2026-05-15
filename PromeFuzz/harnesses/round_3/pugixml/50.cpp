// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
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
#include <cstddef>
#include <string>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document with some structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    pugi::xml_node child2 = root.append_child("child2");
    pugi::xml_node child3 = root.append_child("child3");
    
    // Add some text nodes
    child1.append_child(pugi::node_pcdata).set_value("initial1");
    child2.append_child(pugi::node_pcdata).set_value("initial2");
    child3.append_child(pugi::node_pcdata).set_value("initial3");
    
    // Convert fuzzer input to a null-terminated string for API calls
    std::string input_str;
    if (Size > 0) {
        input_str.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        // Ensure null termination
        if (input_str.size() == 100) {
            input_str[99] = '\0';
        }
    } else {
        input_str = "default";
    }
    
    std::string_view str_view(input_str);
    
    // Execute the API calls in the specified order
    pugi::xml_node node1 = root.child(input_str.c_str());
    pugi::xml_text text1 = node1.text();
    bool set1 = text1.set(str_view.data(), str_view.size());
    
    pugi::xml_node node2 = root.first_child();
    pugi::xml_text text2 = node2.text();
    bool set2 = text2.set(str_view.data(), str_view.size());
    
    pugi::xml_node node3 = root.first_child();
    pugi::xml_node node4 = root.last_child();
    pugi::xml_text text3 = node4.text();
    bool set3 = text3.set(str_view.data(), str_view.size());
    
    pugi::xml_node node5 = root.first_child();
    pugi::xml_node node6 = root.last_child();
    pugi::xml_text text4 = node6.text();
    bool set4 = text4.set(str_view.data(), str_view.size());
    
    pugi::xml_node node7 = root.first_child();
    
    // Use the results to avoid optimization
    (void)node1;
    (void)node2;
    (void)node3;
    (void)node4;
    (void)node5;
    (void)node6;
    (void)node7;
    (void)set1;
    (void)set2;
    (void)set3;
    (void)set4;
    
    return 0;
}