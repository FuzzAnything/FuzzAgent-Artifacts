// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
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
#include <string>
#include <string_view>

static void setup_document(pugi::xml_document& doc) {
    // Create a root node
    pugi::xml_node root = doc.append_child("root");
    
    // Add multiple child nodes with different names for testing
    root.append_child("child1");
    root.append_child("child2");
    root.append_child("child3");
    root.append_child("child4");
    root.append_child("child5");
    root.append_child("child6");
    root.append_child("child7");
    root.append_child("child8");
    root.append_child("child9");
    root.append_child("child10");
    
    // Add some duplicate named children
    root.append_child("duplicate");
    root.append_child("duplicate");
    root.append_child("duplicate");
}

extern "C" int LLVMFuzzerTestOneInput_32(const uint8_t *Data, size_t Size) {
    // Create a document and set it up with some nodes
    pugi::xml_document doc;
    setup_document(doc);
    
    // Get the root node
    pugi::xml_node root = doc.child("root");
    if (!root) return 0;
    
    // Ensure we have at least some data to work with
    if (Size == 0) return 0;
    
    // Create null-terminated strings from fuzzer input for node names
    // We'll use different portions of the input for different calls
    size_t chunk_size = Size / 16;
    if (chunk_size == 0) chunk_size = 1;
    
    // Create string views and strings for the various calls
    std::string_view sv1, sv2, sv3, sv4, sv5, sv6, sv7, sv8, sv9, sv10, sv11, sv12, sv13, sv14, sv15;
    std::string str1, str2, str3, str4, str5, str6, str7, str8, str9, str10, str11, str12, str13, str14, str15;
    
    // Helper function to create null-terminated strings
    auto create_string = [&](size_t offset, std::string& out_str, std::string_view& out_sv) {
        size_t start = (offset * chunk_size) % Size;
        size_t length = std::min(chunk_size, Size - start);
        
        out_str.assign(reinterpret_cast<const char*>(Data + start), length);
        // Ensure null termination
        out_str.push_back('\0');
        out_sv = std::string_view(out_str.data(), out_str.length() - 1);
    };
    
    // Create strings for all calls
    create_string(0, str1, sv1);
    create_string(1, str2, sv2);
    create_string(2, str3, sv3);
    create_string(3, str4, sv4);
    create_string(4, str5, sv5);
    create_string(5, str6, sv6);
    create_string(6, str7, sv7);
    create_string(7, str8, sv8);
    create_string(8, str9, sv9);
    create_string(9, str10, sv10);
    create_string(10, str11, sv11);
    create_string(11, str12, sv12);
    create_string(12, str13, sv13);
    create_string(13, str14, sv14);
    create_string(14, str15, sv15);
    
    // Execute the specified sequence of calls
    try {
        // 1. remove_child (const char*)
        root.remove_child(str1.c_str());
        
        // 2. remove_child (string_view)
        root.remove_child(sv2);
        
        // 3. child
        pugi::xml_node child3 = root.child(str3.c_str());
        
        // 4. child
        pugi::xml_node child4 = root.child(str4.c_str());
        
        // 5. remove_child (const xml_node&) - using child3 if valid
        if (child3) {
            root.remove_child(child3);
        }
        
        // 6. remove_child (const char*)
        root.remove_child(str6.c_str());
        
        // 7. child
        pugi::xml_node child7 = root.child(str7.c_str());
        
        // 8. remove_child (const xml_node&) - using child4 if valid
        if (child4) {
            root.remove_child(child4);
        }
        
        // 9. child
        pugi::xml_node child9 = root.child(str9.c_str());
        
        // 10. remove_child (string_view)
        root.remove_child(sv10);
        
        // 11. remove_child (const char*)
        root.remove_child(str11.c_str());
        
        // 12. remove_child (const xml_node&) - using child7 if valid
        if (child7) {
            root.remove_child(child7);
        }
        
        // 13. remove_child (string_view)
        root.remove_child(sv13);
        
        // 14. remove_child (const char*)
        root.remove_child(str14.c_str());
        
        // 15. remove_child (const xml_node&) - using child9 if valid
        if (child9) {
            root.remove_child(child9);
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}