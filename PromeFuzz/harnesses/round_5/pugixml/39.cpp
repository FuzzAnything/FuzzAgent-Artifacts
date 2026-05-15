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

static void fuzz_prepend_child_and_child(const uint8_t *Data, size_t Size) {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");
    
    // Split input into segments for different function calls
    const size_t segment_count = 9;
    const size_t segment_size = Size / segment_count;
    
    // Prepare string data for function calls
    std::string string_data[segment_count];
    for (size_t i = 0; i < segment_count; i++) {
        size_t start = i * segment_size;
        size_t length = (i == segment_count - 1) ? (Size - start) : segment_size;
        if (start < Size) {
            string_data[i] = std::string(reinterpret_cast<const char*>(Data + start), 
                                         std::min(length, Size - start));
        }
    }
    
    // Ensure null termination for char* parameters
    for (auto& str : string_data) {
        str.push_back('\0');
    }
    
    // Call sequence as specified
    // 1. prepend_child (both overloads)
    root.prepend_child(string_data[0].c_str());
    root.prepend_child(std::string_view(string_data[0]));
    
    // 2. child
    root.child(string_data[1].c_str());
    
    // 3. prepend_child (both overloads)
    root.prepend_child(string_data[2].c_str());
    root.prepend_child(std::string_view(string_data[2]));
    
    // 4. child
    root.child(string_data[3].c_str());
    
    // 5. child
    root.child(string_data[4].c_str());
    
    // 6. prepend_child (both overloads)
    root.prepend_child(string_data[5].c_str());
    root.prepend_child(std::string_view(string_data[5]));
    
    // 7. child
    root.child(string_data[6].c_str());
    
    // 8. prepend_child (both overloads)
    root.prepend_child(string_data[7].c_str());
    root.prepend_child(std::string_view(string_data[7]));
    
    // 9. prepend_child (both overloads)
    root.prepend_child(string_data[8].c_str());
    root.prepend_child(std::string_view(string_data[8]));
}

extern "C" int LLVMFuzzerTestOneInput_39(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    fuzz_prepend_child_and_child(Data, Size);
    
    return 0;
}