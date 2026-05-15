// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
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
#include <fstream>

static std::string create_temp_xml(const uint8_t* data, size_t size) {
    // Create a dummy XML file with some structure to work with
    std::string xml_content = R"(<?xml version="1.0"?>
<root>
    <child1 attr="value1">Text1</child1>
    <child2 attr="value2">Text2</child2>
    <child3 attr="value3">Text3</child3>
    <child4 attr="value4">Text4</child4>
    <child5 attr="value5">Text5</child5>
    <nested>
        <inner>InnerText</inner>
        <inner2>InnerText2</inner2>
    </nested>
</root>)";
    
    // If we have enough data, replace some characters to create variation
    if (size > 0) {
        for (size_t i = 0; i < std::min(size, xml_content.size()); i++) {
            if (xml_content[i] == 'c' && data[i % size] % 2 == 0) {
                xml_content[i] = 'd';
            }
        }
    }
    
    return xml_content;
}

extern "C" int LLVMFuzzerTestOneInput_50(const uint8_t *Data, size_t Size) {
    // Create XML content
    std::string xml_content = create_temp_xml(Data, Size);
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(xml_content.c_str());
    
    if (!result) {
        // If parsing fails, create a minimal valid XML
        doc.load_string("<root><child>text</child></root>");
    }
    
    // Get root node
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
        root.append_child("child1").append_child(pugi::node_pcdata).set_value("value1");
        root.append_child("child2").append_child(pugi::node_pcdata).set_value("value2");
    }
    
    // Prepare string parameters from fuzzer input
    std::string child_name1 = "child1";
    std::string child_name2 = "child2";
    std::string child_name3 = "child3";
    std::string child_name4 = "child4";
    
    // Use fuzzer data to modify string parameters if we have enough data
    if (Size >= 4) {
        if (Data[0] % 2 == 0) child_name1 = "child" + std::to_string(Data[0] % 5 + 1);
        if (Data[1] % 2 == 0) child_name2 = "child" + std::to_string(Data[1] % 5 + 1);
        if (Data[2] % 2 == 0) child_name3 = "nested";
        if (Data[3] % 2 == 0) child_name4 = "inner";
    }
    
    // Ensure null termination
    child_name1.push_back('\0');
    child_name2.push_back('\0');
    child_name3.push_back('\0');
    child_name4.push_back('\0');
    
    // Execute the sequence of API calls
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node node1 = root.child(child_name1.c_str());
        
        // 2. pugi::xml_node::child
        pugi::xml_node node2 = node1.child(child_name2.c_str());
        
        // 3. pugi::xml_node::insert_copy_after
        pugi::xml_node node3 = root.insert_copy_after(node1, node2);
        
        // 4. pugi::xml_node::child
        pugi::xml_node node4 = node3.child(child_name3.c_str());
        
        // 5. pugi::xml_node::insert_copy_after
        pugi::xml_node node5 = root.insert_copy_after(node2, node4);
        
        // 6. pugi::xml_node::child
        pugi::xml_node node6 = node5.child(child_name4.c_str());
        
        // 7. pugi::xml_node::child
        pugi::xml_node node7 = node6.child(child_name1.c_str());
        
        // 8. pugi::xml_node::child
        pugi::xml_node node8 = node7.child(child_name2.c_str());
        
        // 9. pugi::xml_node::child
        pugi::xml_node node9 = node8.child(child_name3.c_str());
        
        // 10. pugi::xml_node::insert_copy_after
        pugi::xml_node node10 = root.insert_copy_after(node3, node9);
        
        // 11. pugi::xml_node::child
        pugi::xml_node node11 = node10.child(child_name4.c_str());
        
        // 12. pugi::xml_node::first_child
        pugi::xml_node node12 = node11.first_child();
        
        // 13. pugi::xml_node::name
        const char* name13 = node12.name();
        (void)name13; // Avoid unused variable warning
        
        // 14. pugi::xml_node::child
        pugi::xml_node node14 = node12.child(child_name1.c_str());
        
        // 15. pugi::xml_node::insert_copy_after
        pugi::xml_node node15 = root.insert_copy_after(node4, node14);
        
        // 16. pugi::xml_node::child
        pugi::xml_node node16 = node15.child(child_name2.c_str());
        
        // 17. pugi::xml_node::first_child
        pugi::xml_node node17 = node16.first_child();
        
        // 18. pugi::xml_node::child
        pugi::xml_node node18 = node17.child(child_name3.c_str());
        
        // 19. pugi::xml_node::last_child
        pugi::xml_node node19 = node18.last_child();
        
        // 20. pugi::xml_node::value
        const char* value20 = node19.value();
        (void)value20; // Avoid unused variable warning
        
        // 21. pugi::xml_node::child
        pugi::xml_node node21 = node19.child(child_name4.c_str());
        
        // 22. pugi::xml_node::insert_copy_after
        pugi::xml_node node22 = root.insert_copy_after(node5, node21);
        
        // 23. pugi::xml_node::child
        pugi::xml_node node23 = node22.child(child_name1.c_str());
        
        // 24. pugi::xml_node::first_child
        pugi::xml_node node24 = node23.first_child();
        
        // 25. pugi::xml_node::child
        pugi::xml_node node25 = node24.child(child_name2.c_str());
        
        // 26. pugi::xml_node::first_child
        pugi::xml_node node26 = node25.first_child();
        
        // 27. pugi::xml_node::value
        const char* value27 = node26.value();
        (void)value27; // Avoid unused variable warning
        
    } catch (...) {
        // Catch any exceptions to prevent crashes from propagating
    }
    
    return 0;
}