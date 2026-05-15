// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6332:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6369:34 in pugixml.hpp
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

static void initialize_dummy_file(const uint8_t* data, size_t size) {
    FILE* file = fopen("./dummy_file", "wb");
    if (file) {
        fwrite(data, 1, size, file);
        fclose(file);
    }
}

extern "C" int LLVMFuzzerTestOneInput_37(const uint8_t* Data, size_t Size) {
    // Initialize dummy file with fuzzer data
    initialize_dummy_file(Data, Size);
    
    try {
        // Create XML document and root node
        pugi::xml_document doc;
        pugi::xml_node root = doc.append_child("root");
        
        // Prepare string data from fuzzer input
        // Ensure null termination for char* parameters
        std::string str_data;
        if (Size > 0) {
            str_data.assign(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(256)));
            if (!str_data.empty() && str_data.back() != '\0') {
                str_data.push_back('\0');
            }
        } else {
            str_data = "default";
        }
        
        const char* cstr = str_data.c_str();
        std::string_view sv(cstr, str_data.size() - 1); // Exclude null terminator for string_view
        
        // Sequence of API calls as specified
        // 1. First prepend_child (both overloads)
        root.prepend_child(cstr);
        root.prepend_child(sv);
        
        // 2. First child
        root.child(cstr);
        
        // 3. Second prepend_child (both overloads)
        root.prepend_child(cstr);
        root.prepend_child(sv);
        
        // 4. Second child
        root.child(cstr);
        
        // 5. Third child
        root.child(cstr);
        
        // 6. Third prepend_child (both overloads)
        root.prepend_child(cstr);
        root.prepend_child(sv);
        
        // 7. Fourth child
        root.child(cstr);
        
        // 8. Fourth prepend_child (both overloads)
        root.prepend_child(cstr);
        root.prepend_child(sv);
        
        // 9. Fifth prepend_child (both overloads)
        root.prepend_child(cstr);
        root.prepend_child(sv);
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}