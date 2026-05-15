// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_47(const uint8_t *Data, size_t Size) {
    // Parse XML from input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    if (!result) {
        return 0;
    }

    // Get root node
    pugi::xml_node root = doc.root();
    
    // Prepare a name string for child() calls - ensure null termination
    char name_buf[256] = {0};
    size_t name_len = Size < 255 ? Size : 255;
    if (name_len > 0) {
        memcpy(name_buf, Data, name_len);
        name_buf[name_len] = '\0';
    } else {
        name_buf[0] = 'd';
        name_buf[1] = '\0';
    }
    
    // Create a dummy node for copying if needed
    pugi::xml_document dummy_doc;
    pugi::xml_node dummy_node = dummy_doc.append_child("dummy");
    
    // Sequence of API calls
    pugi::xml_node node1 = root.child(name_buf);
    pugi::xml_node node2 = node1.child(name_buf);
    pugi::xml_node node3 = root.insert_copy_after(dummy_node, node2);
    pugi::xml_node node4 = node3.child(name_buf);
    pugi::xml_node node5 = root.insert_copy_after(dummy_node, node4);
    pugi::xml_node node6 = node5.child(name_buf);
    pugi::xml_node node7 = node6.child(name_buf);
    pugi::xml_node node8 = node7.child(name_buf);
    pugi::xml_node node9 = node8.child(name_buf);
    pugi::xml_node node10 = root.insert_copy_after(dummy_node, node9);
    pugi::xml_node node11 = node10.child(name_buf);
    pugi::xml_node node12 = node11.first_child();
    const char* name_result = node12.name();
    pugi::xml_node node13 = node12.child(name_buf);
    pugi::xml_node node14 = root.insert_copy_after(dummy_node, node13);
    pugi::xml_node node15 = node14.child(name_buf);
    pugi::xml_node node16 = node15.first_child();
    pugi::xml_node node17 = node16.child(name_buf);
    pugi::xml_node node18 = node17.last_child();
    const char* value_result = node18.value();
    pugi::xml_node node19 = node18.child(name_buf);
    pugi::xml_node node20 = root.insert_copy_after(dummy_node, node19);
    pugi::xml_node node21 = node20.child(name_buf);
    pugi::xml_node node22 = node21.first_child();
    pugi::xml_node node23 = node22.child(name_buf);
    pugi::xml_node node24 = node23.first_child();
    const char* final_value = node24.value();
    
    // Use the results to avoid optimization
    (void)name_result;
    (void)value_result;
    (void)final_value;
    
    return 0;
}