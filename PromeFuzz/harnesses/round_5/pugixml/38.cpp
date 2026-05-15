// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string_view>

static pugi::xml_node create_test_node(pugi::xml_document& doc) {
    // Create a document and add an element node that supports attributes
    pugi::xml_node root = doc.append_child(pugi::node_element);
    return root;
}

static std::string_view get_string_view(const uint8_t* data, size_t size, size_t offset) {
    if (offset >= size) {
        return std::string_view();
    }
    return std::string_view(reinterpret_cast<const char*>(data + offset), size - offset);
}

static const char* get_null_terminated_string(const uint8_t* data, size_t size, size_t offset, char* buffer, size_t buffer_size) {
    if (offset >= size || buffer_size == 0) {
        buffer[0] = '\0';
        return buffer;
    }
    
    size_t copy_size = std::min(size - offset, buffer_size - 1);
    std::memcpy(buffer, data + offset, copy_size);
    buffer[copy_size] = '\0';
    return buffer;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t* Data, size_t Size) {
    // Initialize pugixml document
    pugi::xml_document doc;
    pugi::xml_node node = create_test_node(doc);
    
    // Buffer for null-terminated strings
    char buffer[256];
    
    // Calculate offsets for different string parameters
    size_t offset1 = 0;
    size_t offset2 = Size > 0 ? Size / 4 : 0;
    size_t offset3 = Size > 0 ? Size / 2 : 0;
    size_t offset4 = Size > 0 ? (3 * Size) / 4 : 0;
    
    try {
        // 1. First prepend_attribute (string_view_t overload)
        std::string_view sv1 = get_string_view(Data, Size, offset1);
        node.prepend_attribute(sv1);
        
        // 2. Second prepend_attribute (const char_t* overload)
        const char* str2 = get_null_terminated_string(Data, Size, offset2, buffer, sizeof(buffer));
        node.prepend_attribute(str2);
        
        // 3. First child call
        const char* str3 = get_null_terminated_string(Data, Size, offset3, buffer, sizeof(buffer));
        node.child(str3);
        
        // 4. Third prepend_attribute (string_view_t overload)
        std::string_view sv4 = get_string_view(Data, Size, offset4);
        node.prepend_attribute(sv4);
        
        // 5. Second child call
        const char* str5 = get_null_terminated_string(Data, Size, offset1, buffer, sizeof(buffer));
        node.child(str5);
        
        // 6. Fourth prepend_attribute (const char_t* overload)
        const char* str6 = get_null_terminated_string(Data, Size, offset2, buffer, sizeof(buffer));
        node.prepend_attribute(str6);
        
        // 7. Third child call
        const char* str7 = get_null_terminated_string(Data, Size, offset3, buffer, sizeof(buffer));
        node.child(str7);
        
        // 8. Fourth child call
        const char* str8 = get_null_terminated_string(Data, Size, offset4, buffer, sizeof(buffer));
        node.child(str8);
        
        // 9. Fifth prepend_attribute (string_view_t overload)
        std::string_view sv9 = get_string_view(Data, Size, offset1);
        node.prepend_attribute(sv9);
        
        // 10. Fifth child call
        const char* str10 = get_null_terminated_string(Data, Size, offset2, buffer, sizeof(buffer));
        node.child(str10);
        
        // 11. Sixth child call
        const char* str11 = get_null_terminated_string(Data, Size, offset3, buffer, sizeof(buffer));
        node.child(str11);
        
        // 12. Sixth prepend_attribute (const char_t* overload)
        const char* str12 = get_null_terminated_string(Data, Size, offset4, buffer, sizeof(buffer));
        node.prepend_attribute(str12);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup happens automatically when doc goes out of scope
    return 0;
}