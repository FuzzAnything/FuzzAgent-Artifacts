// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::empty at pugixml.cpp:5360:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    // Parse XML from fuzzer input
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, early return
    if (!result) {
        return 0;
    }
    
    // Get root node
    pugi::xml_node root = doc.root();
    
    // 1. Call first_child
    pugi::xml_node child = root.first_child();
    
    // Prepare some attribute names using fuzzer data
    // Use modulo to avoid division by zero and ensure we have some data
    if (Size == 0) {
        return 0;
    }
    
    // Create string views from fuzzer data
    // We'll use different portions of the input for different attribute names
    size_t chunk_size = Size / 16;
    if (chunk_size == 0) chunk_size = 1;
    
    // 2-9. Call attribute multiple times (8 calls before set_name)
    // We'll use both overloads as required
    pugi::xml_attribute hint_attr;
    
    // First call with simple overload
    std::string_view name1(reinterpret_cast<const char*>(Data), 
                          std::min(chunk_size, Size));
    pugi::xml_attribute attr1 = child.attribute(name1.data());
    
    // Second call with hint overload
    std::string_view name2(reinterpret_cast<const char*>(Data) + chunk_size, 
                          std::min(chunk_size, Size - chunk_size));
    pugi::xml_attribute attr2 = child.attribute(name2, hint_attr);
    hint_attr = attr2;
    
    // Third call with simple overload
    std::string_view name3(reinterpret_cast<const char*>(Data) + 2*chunk_size, 
                          std::min(chunk_size, Size - 2*chunk_size));
    pugi::xml_attribute attr3 = child.attribute(name3.data());
    
    // Fourth call with hint overload
    std::string_view name4(reinterpret_cast<const char*>(Data) + 3*chunk_size, 
                          std::min(chunk_size, Size - 3*chunk_size));
    pugi::xml_attribute attr4 = child.attribute(name4, hint_attr);
    hint_attr = attr4;
    
    // Fifth call with simple overload
    std::string_view name5(reinterpret_cast<const char*>(Data) + 4*chunk_size, 
                          std::min(chunk_size, Size - 4*chunk_size));
    pugi::xml_attribute attr5 = child.attribute(name5.data());
    
    // Sixth call with hint overload
    std::string_view name6(reinterpret_cast<const char*>(Data) + 5*chunk_size, 
                          std::min(chunk_size, Size - 5*chunk_size));
    pugi::xml_attribute attr6 = child.attribute(name6, hint_attr);
    hint_attr = attr6;
    
    // Seventh call with simple overload
    std::string_view name7(reinterpret_cast<const char*>(Data) + 6*chunk_size, 
                          std::min(chunk_size, Size - 6*chunk_size));
    pugi::xml_attribute attr7 = child.attribute(name7.data());
    
    // Eighth call with hint overload (last before set_name)
    std::string_view name8(reinterpret_cast<const char*>(Data) + 7*chunk_size, 
                          std::min(chunk_size, Size - 7*chunk_size));
    pugi::xml_attribute attr8 = child.attribute(name8, hint_attr);
    
    // 10. Call set_name on an attribute
    // Use attr8 if it's valid, otherwise use attr1
    pugi::xml_attribute target_attr = attr8.empty() ? attr1 : attr8;
    if (!target_attr.empty()) {
        std::string_view new_name(reinterpret_cast<const char*>(Data) + 8*chunk_size, 
                                 std::min(chunk_size, Size - 8*chunk_size));
        target_attr.set_name(new_name);
    }
    
    // 11-14. Call attribute 4 more times after set_name
    // Reset hint
    hint_attr = pugi::xml_attribute();
    
    // Ninth call (first after set_name) with simple overload
    std::string_view name9(reinterpret_cast<const char*>(Data) + 9*chunk_size, 
                          std::min(chunk_size, Size - 9*chunk_size));
    pugi::xml_attribute attr9 = child.attribute(name9.data());
    
    // Tenth call with hint overload
    std::string_view name10(reinterpret_cast<const char*>(Data) + 10*chunk_size, 
                           std::min(chunk_size, Size - 10*chunk_size));
    pugi::xml_attribute attr10 = child.attribute(name10, hint_attr);
    hint_attr = attr10;
    
    // Eleventh call with simple overload
    std::string_view name11(reinterpret_cast<const char*>(Data) + 11*chunk_size, 
                           std::min(chunk_size, Size - 11*chunk_size));
    pugi::xml_attribute attr11 = child.attribute(name11.data());
    
    // Twelfth call with hint overload (final call)
    std::string_view name12(reinterpret_cast<const char*>(Data) + 12*chunk_size, 
                           std::min(chunk_size, Size - 12*chunk_size));
    pugi::xml_attribute attr12 = child.attribute(name12, hint_attr);
    
    return 0;
}