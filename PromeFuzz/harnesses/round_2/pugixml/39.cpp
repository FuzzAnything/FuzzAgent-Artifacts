// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6341:34 in pugixml.hpp
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

static void initialize_document(pugi::xml_document& doc, pugi::xml_node& root, pugi::xml_node& sibling) {
    // Create a root node
    root = doc.append_child("root");
    
    // Create an initial child to use as sibling reference
    sibling = root.append_child("initial_child");
    
    // Create some additional nodes for the tree structure
    root.append_child("child1");
    root.append_child("child2");
    sibling.append_child("grandchild1");
}

static std::string create_null_terminated_string(const uint8_t* data, size_t size, size_t offset, size_t length) {
    if (offset >= size) return "default";
    
    size_t actual_length = std::min(length, size - offset);
    std::string result(reinterpret_cast<const char*>(data + offset), actual_length);
    
    // Ensure null termination
    if (result.empty() || result.back() != '\0') {
        result.push_back('\0');
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* Data, size_t Size) {
    if (Size < 10) return 0;
    
    try {
        pugi::xml_document doc;
        pugi::xml_node root;
        pugi::xml_node sibling;
        
        // Initialize document with some structure
        initialize_document(doc, root, sibling);
        
        // Create strings from fuzzer input for node names
        std::string str1 = create_null_terminated_string(Data, Size, 0, 5);
        std::string str2 = create_null_terminated_string(Data, Size, 5, 5);
        std::string str3 = create_null_terminated_string(Data, Size, 10, 5);
        std::string str4 = create_null_terminated_string(Data, Size, 15, 5);
        std::string str5 = create_null_terminated_string(Data, Size, 20, 5);
        std::string str6 = create_null_terminated_string(Data, Size, 25, 5);
        std::string str7 = create_null_terminated_string(Data, Size, 30, 5);
        std::string str8 = create_null_terminated_string(Data, Size, 35, 5);
        std::string str9 = create_null_terminated_string(Data, Size, 40, 5);
        std::string str10 = create_null_terminated_string(Data, Size, 45, 5);
        
        // Create string_view versions
        std::string_view sv1(str1.c_str(), str1.size() - 1); // -1 to exclude null terminator
        std::string_view sv2(str2.c_str(), str2.size() - 1);
        std::string_view sv3(str3.c_str(), str3.size() - 1);
        std::string_view sv4(str4.c_str(), str4.size() - 1);
        std::string_view sv5(str5.c_str(), str5.size() - 1);
        std::string_view sv6(str6.c_str(), str6.size() - 1);
        std::string_view sv7(str7.c_str(), str7.size() - 1);
        
        // 1. First insert_child_after (string_view overload)
        pugi::xml_node new_node1 = root.insert_child_after(sv1, sibling);
        
        // 2. First child call
        pugi::xml_node child1 = root.child(str2.c_str());
        
        // 3. Second child call
        pugi::xml_node child2 = root.child(str3.c_str());
        
        // 4. Third child call
        pugi::xml_node child3 = root.child(str4.c_str());
        
        // 5. Second insert_child_after (const char* overload)
        pugi::xml_node new_node2 = root.insert_child_after(str5.c_str(), new_node1);
        
        // 6. Third insert_child_after (string_view overload)
        pugi::xml_node new_node3 = root.insert_child_after(sv2, new_node2);
        
        // 7. Fourth insert_child_after (const char* overload)
        pugi::xml_node new_node4 = root.insert_child_after(str6.c_str(), new_node3);
        
        // 8. Fifth insert_child_after (string_view overload)
        pugi::xml_node new_node5 = root.insert_child_after(sv3, new_node4);
        
        // 9. Sixth insert_child_after (const char* overload)
        pugi::xml_node new_node6 = root.insert_child_after(str7.c_str(), new_node5);
        
        // 10. Seventh insert_child_after (string_view overload)
        pugi::xml_node new_node7 = root.insert_child_after(sv4, new_node6);
        
        // Additional calls with remaining strings to explore more states
        if (child1) {
            child1.insert_child_after(sv5, child1.first_child());
            child1.insert_child_after(str8.c_str(), child1.last_child());
        }
        
        if (child2) {
            child2.insert_child_after(sv6, pugi::xml_node());
            child2.insert_child_after(str9.c_str(), pugi::xml_node());
        }
        
        if (child3) {
            child3.insert_child_after(sv7, child3);
            child3.insert_child_after(str10.c_str(), child3);
        }
        
    } catch (...) {
        // Catch all exceptions to prevent crashes
    }
    
    return 0;
}