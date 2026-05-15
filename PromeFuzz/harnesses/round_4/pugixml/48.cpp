// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
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
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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

static void initialize_dummy_document(pugi::xml_document& doc) {
    const char* dummy_xml = 
        "<?xml version=\"1.0\"?>"
        "<root>"
        "<child1 attr=\"value1\">Text1</child1>"
        "<child2 attr=\"value2\">Text2</child2>"
        "<child3 attr=\"value3\">Text3</child3>"
        "<child4 attr=\"value4\">Text4</child4>"
        "<child5 attr=\"value5\">Text5</child5>"
        "</root>";
    doc.load_string(dummy_xml);
}

static void cleanup_dummy_document(pugi::xml_document& doc) {
    doc.reset();
}

extern "C" int LLVMFuzzerTestOneInput_48(const uint8_t* Data, size_t Size) {
    static pugi::xml_document doc;
    static bool initialized = false;
    
    if (!initialized) {
        initialize_dummy_document(doc);
        initialized = true;
    }
    
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // Use fuzz data to create null-terminated strings for child names
    char child_name1[256] = {0};
    char child_name2[256] = {0};
    char child_name3[256] = {0};
    char child_name4[256] = {0};
    char child_name5[256] = {0};
    char child_name6[256] = {0};
    char child_name7[256] = {0};
    char child_name8[256] = {0};
    char child_name9[256] = {0};
    char child_name10[256] = {0};
    char child_name11[256] = {0};
    char child_name12[256] = {0};
    char child_name13[256] = {0};
    char child_name14[256] = {0};
    
    size_t copy_size = Size;
    if (copy_size > 255) copy_size = 255;
    
    // Copy fuzz data to child names, ensuring null termination
    if (copy_size > 0) {
        memcpy(child_name1, Data, copy_size < 255 ? copy_size : 255);
        child_name1[255] = '\0';
    }
    if (copy_size > 1) {
        memcpy(child_name2, Data + 1, copy_size - 1 < 255 ? copy_size - 1 : 255);
        child_name2[255] = '\0';
    }
    if (copy_size > 2) {
        memcpy(child_name3, Data + 2, copy_size - 2 < 255 ? copy_size - 2 : 255);
        child_name3[255] = '\0';
    }
    if (copy_size > 3) {
        memcpy(child_name4, Data + 3, copy_size - 3 < 255 ? copy_size - 3 : 255);
        child_name4[255] = '\0';
    }
    if (copy_size > 4) {
        memcpy(child_name5, Data + 4, copy_size - 4 < 255 ? copy_size - 4 : 255);
        child_name5[255] = '\0';
    }
    if (copy_size > 5) {
        memcpy(child_name6, Data + 5, copy_size - 5 < 255 ? copy_size - 5 : 255);
        child_name6[255] = '\0';
    }
    if (copy_size > 6) {
        memcpy(child_name7, Data + 6, copy_size - 6 < 255 ? copy_size - 6 : 255);
        child_name7[255] = '\0';
    }
    if (copy_size > 7) {
        memcpy(child_name8, Data + 7, copy_size - 7 < 255 ? copy_size - 7 : 255);
        child_name8[255] = '\0';
    }
    if (copy_size > 8) {
        memcpy(child_name9, Data + 8, copy_size - 8 < 255 ? copy_size - 8 : 255);
        child_name9[255] = '\0';
    }
    if (copy_size > 9) {
        memcpy(child_name10, Data + 9, copy_size - 9 < 255 ? copy_size - 9 : 255);
        child_name10[255] = '\0';
    }
    if (copy_size > 10) {
        memcpy(child_name11, Data + 10, copy_size - 10 < 255 ? copy_size - 10 : 255);
        child_name11[255] = '\0';
    }
    if (copy_size > 11) {
        memcpy(child_name12, Data + 11, copy_size - 11 < 255 ? copy_size - 11 : 255);
        child_name12[255] = '\0';
    }
    if (copy_size > 12) {
        memcpy(child_name13, Data + 12, copy_size - 12 < 255 ? copy_size - 12 : 255);
        child_name13[255] = '\0';
    }
    if (copy_size > 13) {
        memcpy(child_name14, Data + 13, copy_size - 13 < 255 ? copy_size - 13 : 255);
        child_name14[255] = '\0';
    }
    
    try {
        // 1. pugi::xml_node::child
        pugi::xml_node node1 = root.child(child_name1);
        
        // 2. pugi::xml_node::child
        pugi::xml_node node2 = root.child(child_name2);
        
        // 3. pugi::xml_node::insert_copy_after
        pugi::xml_node inserted1 = root.insert_copy_after(node1, node2);
        
        // 4. pugi::xml_node::child
        pugi::xml_node node3 = root.child(child_name3);
        
        // 5. pugi::xml_node::insert_copy_after
        pugi::xml_node inserted2 = root.insert_copy_after(node2, node3);
        
        // 6. pugi::xml_node::child
        pugi::xml_node node4 = root.child(child_name4);
        
        // 7. pugi::xml_node::child
        pugi::xml_node node5 = root.child(child_name5);
        
        // 8. pugi::xml_node::child
        pugi::xml_node node6 = root.child(child_name6);
        
        // 9. pugi::xml_node::child
        pugi::xml_node node7 = root.child(child_name7);
        
        // 10. pugi::xml_node::insert_copy_after
        pugi::xml_node inserted3 = root.insert_copy_after(node6, node7);
        
        // 11. pugi::xml_node::child
        pugi::xml_node node8 = root.child(child_name8);
        
        // 12. pugi::xml_node::first_child
        pugi::xml_node first_child1 = root.first_child();
        
        // 13. pugi::xml_node::name
        const char* name1 = first_child1.name();
        (void)name1; // Avoid unused variable warning
        
        // 14. pugi::xml_node::child
        pugi::xml_node node9 = root.child(child_name9);
        
        // 15. pugi::xml_node::insert_copy_after
        pugi::xml_node inserted4 = root.insert_copy_after(node8, node9);
        
        // 16. pugi::xml_node::child
        pugi::xml_node node10 = root.child(child_name10);
        
        // 17. pugi::xml_node::first_child
        pugi::xml_node first_child2 = root.first_child();
        
        // 18. pugi::xml_node::child
        pugi::xml_node node11 = root.child(child_name11);
        
        // 19. pugi::xml_node::last_child
        pugi::xml_node last_child1 = root.last_child();
        
        // 20. pugi::xml_node::value
        const char* value1 = last_child1.value();
        (void)value1; // Avoid unused variable warning
        
        // 21. pugi::xml_node::child
        pugi::xml_node node12 = root.child(child_name12);
        
        // 22. pugi::xml_node::insert_copy_after
        pugi::xml_node inserted5 = root.insert_copy_after(node11, node12);
        
        // 23. pugi::xml_node::child
        pugi::xml_node node13 = root.child(child_name13);
        
        // 24. pugi::xml_node::first_child
        pugi::xml_node first_child3 = root.first_child();
        
        // 25. pugi::xml_node::child
        pugi::xml_node node14 = root.child(child_name14);
        
        // 26. pugi::xml_node::first_child
        pugi::xml_node first_child4 = root.first_child();
        
        // 27. pugi::xml_node::value
        const char* value2 = first_child4.value();
        (void)value2; // Avoid unused variable warning
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Reset document for next iteration
    cleanup_dummy_document(doc);
    initialize_dummy_document(doc);
    
    return 0;
}