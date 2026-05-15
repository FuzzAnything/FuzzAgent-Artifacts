// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6129:39 in pugixml.hpp
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

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    // Write data to dummy file for potential use
    writeDummyFile(Data, Size);

    // Create XML document and node
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child(pugi::node_element);
    
    // Prepare strings from fuzzer input
    std::string str_data(reinterpret_cast<const char*>(Data), Size);
    std::string_view sv_data(str_data);
    
    // Ensure null-terminated string for const char* overloads
    std::string null_term_str = str_data;
    if (!null_term_str.empty()) {
        null_term_str.push_back('\0');
    }
    
    // Sequence of API calls as specified
    try {
        // 1. prepend_attribute (const char* overload)
        node.prepend_attribute(null_term_str.c_str());
        
        // 2. prepend_attribute (string_view overload)
        node.prepend_attribute(sv_data);
        
        // 3. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 4. prepend_attribute (const char* overload)
        node.prepend_attribute(null_term_str.c_str());
        
        // 5. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 6. prepend_attribute (string_view overload)
        node.prepend_attribute(sv_data);
        
        // 7. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 8. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 9. prepend_attribute (const char* overload)
        node.prepend_attribute(null_term_str.c_str());
        
        // 10. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 11. child (const char* overload)
        node.child(null_term_str.c_str());
        
        // 12. prepend_attribute (string_view overload)
        node.prepend_attribute(sv_data);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    // Cleanup - document destructor handles cleanup automatically
    return 0;
}