// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_46(const uint8_t *Data, size_t Size) {
    // Create a dummy XML document to work with
    pugi::xml_document doc;
    
    // Create some initial structure to ensure nodes exist for operations
    pugi::xml_node root = doc.append_child("root");
    pugi::xml_node child1 = root.append_child("child1");
    child1.append_child("subchild1").text().set("value1");
    child1.append_child("subchild2").text().set("value2");
    
    pugi::xml_node child2 = root.append_child("child2");
    child2.append_child("subchild3").text().set("value3");
    child2.append_child("subchild4").text().set("value4");
    
    pugi::xml_node child3 = root.append_child("child3");
    child3.append_child("subchild5").text().set("value5");
    
    // Create a string from fuzzer input for node names/values
    std::string input_str;
    if (Size > 0) {
        input_str.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(100)));
        // Ensure null termination for C-string APIs
        if (input_str.back() != '\0') {
            input_str.push_back('\0');
        }
    } else {
        input_str = "default";
    }
    
    const char* node_name = input_str.c_str();
    
    // Perform the sequence of API calls as specified
    pugi::xml_node result1 = root.child(node_name);
    pugi::xml_node result2 = root.child(node_name);
    pugi::xml_node result3 = root.insert_copy_after(child1, child2);
    pugi::xml_node result4 = root.child(node_name);
    pugi::xml_node result5 = root.insert_copy_after(child2, child3);
    pugi::xml_node result6 = root.child(node_name);
    pugi::xml_node result7 = root.child(node_name);
    pugi::xml_node result8 = root.child(node_name);
    pugi::xml_node result9 = root.child(node_name);
    pugi::xml_node result10 = root.insert_copy_after(child3, child1);
    pugi::xml_node result11 = root.child(node_name);
    pugi::xml_node result12 = root.first_child();
    const char* name_result = result12.name();
    pugi::xml_node result14 = root.child(node_name);
    pugi::xml_node result15 = root.insert_copy_after(child1, result3);
    pugi::xml_node result16 = root.child(node_name);
    pugi::xml_node result17 = root.first_child();
    pugi::xml_node result18 = root.child(node_name);
    pugi::xml_node result19 = root.last_child();
    const char* value_result = result19.value();
    pugi::xml_node result21 = root.child(node_name);
    pugi::xml_node result22 = root.insert_copy_after(child2, result5);
    pugi::xml_node result23 = root.child(node_name);
    pugi::xml_node result24 = root.first_child();
    pugi::xml_node result25 = root.child(node_name);
    pugi::xml_node result26 = root.first_child();
    const char* value_result2 = result26.value();
    
    // Use the results to avoid compiler optimizations
    (void)result1;
    (void)result2;
    (void)result3;
    (void)result4;
    (void)result5;
    (void)result6;
    (void)result7;
    (void)result8;
    (void)result9;
    (void)result10;
    (void)result11;
    (void)name_result;
    (void)result14;
    (void)result15;
    (void)result16;
    (void)result17;
    (void)result18;
    (void)value_result;
    (void)result21;
    (void)result22;
    (void)result23;
    (void)result24;
    (void)result25;
    (void)value_result2;
    
    return 0;
}