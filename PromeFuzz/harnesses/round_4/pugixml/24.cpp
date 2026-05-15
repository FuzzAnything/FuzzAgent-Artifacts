// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::value at pugixml.cpp:5700:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6449:34 in pugixml.hpp
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
#include <cstring>
#include <string>

static const char* get_string_from_data(const uint8_t* Data, size_t Size, size_t& offset) {
    if (offset >= Size) {
        return "";
    }
    
    // Find null terminator or end of data
    size_t start = offset;
    while (offset < Size && Data[offset] != 0) {
        ++offset;
    }
    
    // Create a null-terminated string
    static char buffer[256];
    size_t len = offset - start;
    if (len > 255) len = 255;
    
    if (len > 0) {
        memcpy(buffer, Data + start, len);
    }
    buffer[len] = '\0';
    
    // Move past null terminator if found
    if (offset < Size && Data[offset] == 0) {
        ++offset;
    }
    
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_24(const uint8_t* Data, size_t Size) {
    static size_t call_counter = 0;
    
    // Parse XML document
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, pugi::parse_default | pugi::parse_ws_pcdata);
    
    // If parsing fails, create a minimal document to work with
    if (!result) {
        const char* minimal_xml = "<root><child1/><child2/><child3/></root>";
        doc.load_string(minimal_xml);
    }
    
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
    
    // Prepare string data for function calls
    size_t data_offset = 0;
    
    // Execute the sequence of API calls
    try {
        // 1. pugi::xml_node::child
        const char* str1 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node1 = root.child(str1);
        
        // 2. pugi::xml_node::child
        const char* str2 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node2 = node1.child(str2);
        
        // 3. pugi::xml_node::insert_copy_before
        if (node2) {
            pugi::xml_node proto1 = root.first_child();
            if (proto1) {
                node2.insert_copy_before(proto1, node2.first_child());
            }
        }
        
        // 4. pugi::xml_node::child
        const char* str3 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node3 = root.child(str3);
        
        // 5. pugi::xml_node::insert_copy_before
        if (node3) {
            pugi::xml_node proto2 = root.last_child();
            if (proto2) {
                node3.insert_copy_before(proto2, node3.first_child());
            }
        }
        
        // 6. pugi::xml_node::child
        const char* str4 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node4 = root.child(str4);
        
        // 7. pugi::xml_node::child
        const char* str5 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node5 = node4.child(str5);
        
        // 8. pugi::xml_node::child
        const char* str6 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node6 = root.child(str6);
        
        // 9. pugi::xml_node::child
        const char* str7 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node7 = node6.child(str7);
        
        // 10. pugi::xml_node::insert_copy_before
        if (node7) {
            pugi::xml_node proto3 = root.first_child();
            if (proto3) {
                node7.insert_copy_before(proto3, node7.last_child());
            }
        }
        
        // 11. pugi::xml_node::child
        const char* str8 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node8 = root.child(str8);
        
        // 12. pugi::xml_node::first_child
        pugi::xml_node node9 = node8.first_child();
        
        // 13. pugi::xml_node::name
        if (node9) {
            const char* name1 = node9.name();
            (void)name1; // Use result to avoid unused variable warning
        }
        
        // 14. pugi::xml_node::child
        const char* str9 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node10 = root.child(str9);
        
        // 15. pugi::xml_node::insert_copy_before
        if (node10) {
            pugi::xml_node proto4 = root.first_child();
            pugi::xml_node target1 = root.last_child();
            if (proto4 && target1) {
                node10.insert_copy_before(proto4, target1);
            }
        }
        
        // 16. pugi::xml_node::child
        const char* str10 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node11 = root.child(str10);
        
        // 17. pugi::xml_node::first_child
        pugi::xml_node node12 = node11.first_child();
        
        // 18. pugi::xml_node::child
        const char* str11 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node13 = node12.child(str11);
        
        // 19. pugi::xml_node::last_child
        pugi::xml_node node14 = root.last_child();
        
        // 20. pugi::xml_node::name
        if (node14) {
            const char* name2 = node14.name();
            (void)name2; // Use result to avoid unused variable warning
        }
        
        // 21. pugi::xml_node::child
        const char* str12 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node15 = root.child(str12);
        
        // 22. pugi::xml_node::insert_copy_before
        if (node15) {
            pugi::xml_node proto5 = root.first_child();
            pugi::xml_node target2 = root.last_child();
            if (proto5 && target2) {
                node15.insert_copy_before(proto5, target2);
            }
        }
        
        // 23. pugi::xml_node::child
        const char* str13 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node16 = root.child(str13);
        
        // 24. pugi::xml_node::first_child
        pugi::xml_node node17 = node16.first_child();
        
        // 25. pugi::xml_node::next_sibling
        pugi::xml_node node18 = node17.next_sibling();
        
        // 26. pugi::xml_node::child
        const char* str14 = get_string_from_data(Data, Size, data_offset);
        pugi::xml_node node19 = node18.child(str14);
        
        // 27. pugi::xml_node::first_child
        pugi::xml_node node20 = node19.first_child();
        
        // 28. pugi::xml_node::value
        if (node20) {
            const char* value = node20.value();
            (void)value; // Use result to avoid unused variable warning
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    ++call_counter;
    return 0;
}