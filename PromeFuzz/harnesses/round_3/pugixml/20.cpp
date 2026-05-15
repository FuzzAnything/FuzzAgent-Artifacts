// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6541:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <string_view>

static void initialize_xml_node(pugi::xml_node& node, const uint8_t* data, size_t size) {
    // Create some dummy attributes with names derived from fuzz data
    const char* attr_names[] = {"attr1", "attr2", "attr3", "attr4", "attr5"};
    for (const char* name : attr_names) {
        node.append_attribute(name);
    }
    
    // Create some child nodes
    const char* child_names[] = {"child1", "child2", "child3"};
    for (const char* name : child_names) {
        node.append_child(name);
    }
}

extern "C" int LLVMFuzzerTestOneInput_20(const uint8_t* Data, size_t Size) {
    // Create a document and node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("root");
    
    // Initialize the node with some attributes and children
    initialize_xml_node(node, Data, Size);
    
    // Prepare strings from fuzz data for function parameters
    // Ensure null-terminated strings
    std::string str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13;
    
    if (Size > 0) {
        // Use first bytes for string parameters, ensuring null termination
        size_t str_len = Size > 100 ? 100 : Size;
        str1.assign(reinterpret_cast<const char*>(Data), str_len);
        str1.push_back('\0');
        
        // Create additional strings with different offsets
        if (Size > 10) {
            str2.assign(reinterpret_cast<const char*>(Data) + 5, str_len - 5);
            str2.push_back('\0');
        } else {
            str2 = "test2\0";
        }
        
        if (Size > 20) {
            str3.assign(reinterpret_cast<const char*>(Data) + 10, str_len - 10);
            str3.push_back('\0');
        } else {
            str3 = "test3\0";
        }
        
        str4 = "attr1\0";
        str5 = "attr2\0";
        str6 = "attr3\0";
        str7 = "attr4\0";
        str8 = "child1\0";
        str9 = "child2\0";
        str10 = "attr5\0";
        str11 = "nonexistent1\0";
        str12 = "nonexistent2\0";
        str13 = "nonexistent3\0";
    } else {
        // Default strings if no data
        str1 = "test1\0";
        str2 = "test2\0";
        str3 = "test3\0";
        str4 = "attr1\0";
        str5 = "attr2\0";
        str6 = "attr3\0";
        str7 = "attr4\0";
        str8 = "child1\0";
        str9 = "child2\0";
        str10 = "attr5\0";
        str11 = "nonexistent1\0";
        str12 = "nonexistent2\0";
        str13 = "nonexistent3\0";
    }
    
    // Get string views for string_view_t parameters
    std::string_view sv1(str1.c_str());
    std::string_view sv2(str2.c_str());
    std::string_view sv3(str3.c_str());
    std::string_view sv4(str4.c_str());
    std::string_view sv5(str5.c_str());
    std::string_view sv6(str6.c_str());
    std::string_view sv7(str7.c_str());
    std::string_view sv8(str8.c_str());
    std::string_view sv9(str9.c_str());
    std::string_view sv10(str10.c_str());
    std::string_view sv11(str11.c_str());
    std::string_view sv12(str12.c_str());
    std::string_view sv13(str13.c_str());
    
    try {
        // Call 1: remove_attribute (const char* overload)
        node.remove_attribute(str4.c_str());
        
        // Call 2: remove_attribute (string_view_t overload)
        node.remove_attribute(sv5);
        
        // Call 3: child (const char*)
        pugi::xml_node child1 = node.child(str8.c_str());
        (void)child1; // Use variable to avoid unused warning
        
        // Call 4: child (const char*)
        pugi::xml_node child2 = node.child(str9.c_str());
        (void)child2; // Use variable to avoid unused warning
        
        // Call 5: remove_attribute (const char* overload)
        node.remove_attribute(str6.c_str());
        
        // Call 6: remove_attribute (string_view_t overload)
        node.remove_attribute(sv7);
        
        // Call 7: attribute (const char*)
        pugi::xml_attribute attr1 = node.attribute(str10.c_str());
        
        // Call 8: remove_attribute (xml_attribute& overload)
        if (attr1) {
            node.remove_attribute(attr1);
        }
        
        // Call 9: attribute (const char*)
        pugi::xml_attribute attr2 = node.attribute(str11.c_str());
        
        // Call 10: remove_attribute (xml_attribute& overload)
        if (attr2) {
            node.remove_attribute(attr2);
        }
        
        // Call 11: remove_attribute (const char* overload)
        node.remove_attribute(str12.c_str());
        
        // Call 12: remove_attribute (string_view_t overload)
        node.remove_attribute(sv13);
        
        // Call 13: remove_attribute (xml_attribute& overload)
        // Get another attribute first
        pugi::xml_attribute attr3 = node.attribute(str1.c_str());
        if (attr3) {
            node.remove_attribute(attr3);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}