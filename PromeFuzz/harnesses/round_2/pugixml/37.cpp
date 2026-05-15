// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
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

static void fuzz_pugixml(const uint8_t* data, size_t size) {
    // Create a document and a node to work with
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Prepare string data from fuzzer input
    // We'll use the entire input as a string, ensuring null termination
    std::string input_str;
    if (size > 0) {
        input_str.assign(reinterpret_cast<const char*>(data), size);
    } else {
        input_str = "";
    }
    
    // Ensure null termination for C-style string usage
    const char* cstr = input_str.c_str();
    std::string_view sv(input_str);
    
    // Sequence of API calls as specified
    // 1. pugi::xml_node::prepend_attribute (const char_t* overload)
    pugi::xml_attribute attr1 = node.prepend_attribute(cstr);
    
    // 2. pugi::xml_node::prepend_attribute (string_view_t overload)
    pugi::xml_attribute attr2 = node.prepend_attribute(sv);
    
    // 3. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child1 = node.child(cstr);
    
    // 4. pugi::xml_node::prepend_attribute (string_view_t overload)
    pugi::xml_attribute attr3 = node.prepend_attribute(sv);
    
    // 5. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child2 = node.child(cstr);
    
    // 6. pugi::xml_node::prepend_attribute (const char_t* overload)
    pugi::xml_attribute attr4 = node.prepend_attribute(cstr);
    
    // 7. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child3 = node.child(cstr);
    
    // 8. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child4 = node.child(cstr);
    
    // 9. pugi::xml_node::prepend_attribute (string_view_t overload)
    pugi::xml_attribute attr5 = node.prepend_attribute(sv);
    
    // 10. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child5 = node.child(cstr);
    
    // 11. pugi::xml_node::child (const char_t* overload)
    pugi::xml_node child6 = node.child(cstr);
    
    // 12. pugi::xml_node::prepend_attribute (const char_t* overload)
    pugi::xml_attribute attr6 = node.prepend_attribute(cstr);
    
    // The results are intentionally ignored to allow the fuzzer to explore
    // different states through the API calls themselves
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t *Data, size_t Size) {
    fuzz_pugixml(Data, Size);
    return 0;
}