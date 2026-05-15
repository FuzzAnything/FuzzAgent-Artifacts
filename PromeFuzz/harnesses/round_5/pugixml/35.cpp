// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_attribute::previous_attribute at pugixml.cpp:5295:44 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>

extern "C" int LLVMFuzzerTestOneInput_35(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // Only proceed if parsing succeeded
    if (!result) {
        return 0;
    }
    
    // Get the root node (document element)
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }
    
    // Prepare null-terminated strings for child/attribute names
    // Use fixed names to ensure we have valid null-terminated strings
    const char* child_name = "child";
    const char* attr_name = "attr";
    
    // 1. First child call
    pugi::xml_node child1 = root.child(child_name);
    
    // 2. First attribute call
    pugi::xml_attribute attr1 = root.attribute(attr_name);
    
    // 3. Second child call (on first child if it exists)
    pugi::xml_node child2 = child1.child(child_name);
    
    // 4. Second attribute call (on first child if it exists)
    pugi::xml_attribute attr2 = child1.attribute(attr_name);
    
    // 5. First next_attribute call
    pugi::xml_attribute attr3 = attr2.next_attribute();
    
    // 6. Second next_attribute call
    pugi::xml_attribute attr4 = attr3.next_attribute();
    
    // 7. First previous_attribute call
    pugi::xml_attribute attr5 = attr4.previous_attribute();
    
    // 8. Second previous_attribute call
    pugi::xml_attribute attr6 = attr5.previous_attribute();
    
    // 9. Third next_attribute call
    pugi::xml_attribute attr7 = attr6.next_attribute();
    
    // 10. Third previous_attribute call
    pugi::xml_attribute attr8 = attr7.previous_attribute();
    
    // Additional exploration: try different names if we have enough data
    if (Size > 0) {
        // Create a null-terminated string from fuzzer data
        size_t name_len = Size < 100 ? Size : 100;
        char* dynamic_name = new char[name_len + 1];
        memcpy(dynamic_name, Data, name_len);
        dynamic_name[name_len] = '\0';
        
        // Try with dynamic names
        pugi::xml_node dyn_child = root.child(dynamic_name);
        pugi::xml_attribute dyn_attr = root.attribute(dynamic_name);
        
        // Clean up
        delete[] dynamic_name;
    }
    
    return 0;
}