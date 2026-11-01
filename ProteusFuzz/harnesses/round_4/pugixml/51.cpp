// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5643:67 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_node::children at pugixml.cpp:5638:61 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::end at pugixml.hpp:336:6 in pugixml.hpp
// pugi::xml_object_range::begin at pugixml.hpp:335:6 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string_view>

static pugi::xml_document* parse_document(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (size == 0) {
        return doc;
    }
    // Ensure the input is null-terminated for parsing
    std::string str(reinterpret_cast<const char*>(data), size);
    // Append a null terminator if not present
    if (str.back() != '\0') {
        str += '\0';
    }
    pugi::xml_parse_result result = doc->load_string(str.c_str());
    if (!result) {
        // Even if parsing fails, we return the document to test error handling paths
        // The document might be partially parsed or empty
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document* doc = parse_document(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->root();
    if (!root) {
        // Try to test with an empty node
        pugi::xml_node empty_node;
        // Test child calls on empty node
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        (void)empty_node.child("test");
        
        auto range1 = empty_node.children();
        auto range2 = empty_node.children();
        auto range3 = empty_node.children();
        auto range4 = empty_node.children();
        
        (void)range1.end();
        (void)empty_node.first_child();
        (void)range2.end();
        (void)range3.end();
        (void)range4.end();
        (void)empty_node.first_child();
        (void)empty_node.last_child();
        (void)range1.end();
        (void)range2.end();
        (void)range3.end();
        (void)range4.end();
        (void)range1.end();
        (void)range2.end();
        (void)empty_node.first_child();
        (void)range1.end();
        (void)range2.end();
        (void)range1.begin();
        (void)range3.end();
        (void)range4.end();
        (void)range1.begin();
        
        delete doc;
        return 0;
    }

    // Extract a name from the input data if possible, otherwise use a default
    const char* name_str = "test";
    if (Size > 0) {
        // Try to find a null-terminated string in the input to use as a name
        // This is a heuristic to generate diverse names
        size_t null_pos = 0;
        while (null_pos < Size && Data[null_pos] != '\0') {
            null_pos++;
        }
        if (null_pos > 0 && null_pos < Size) {
            // We found a null terminator, use the string before it
            // We need to store it somewhere safe or just use a static buffer
            // For fuzzing, we can just use the raw pointer if it's safe, 
            // but let's be careful. We'll just use "test" or a simple char array.
            // To keep it simple and safe, we'll stick to "test" or derive from first byte.
            name_str = "node"; 
        }
    }

    // 1. pugi::xml_node::child -> 8 times
    pugi::xml_node n1 = root.child(name_str);
    pugi::xml_node n2 = n1.child(name_str);
    pugi::xml_node n3 = n2.child(name_str);
    pugi::xml_node n4 = n3.child(name_str);
    pugi::xml_node n5 = n4.child(name_str);
    pugi::xml_node n6 = n5.child(name_str);
    pugi::xml_node n7 = n6.child(name_str);
    pugi::xml_node n8 = n7.child(name_str);

    // 2. pugi::xml_node::children -> 4 times
    pugi::xml_object_range<pugi::xml_named_node_iterator> r1 = n8.children(name_str);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r2 = n8.children(name_str);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r3 = n8.children(name_str);
    pugi::xml_object_range<pugi::xml_named_node_iterator> r4 = n8.children(name_str);

    // 3. pugi::xml_object_range::end -> 1 time
    (void)r1.end();

    // 4. pugi::xml_node::first_child -> 1 time
    (void)n8.first_child();

    // 5. pugi::xml_object_range::end -> 3 times
    (void)r2.end();
    (void)r3.end();
    (void)r4.end();

    // 6. pugi::xml_node::first_child -> 1 time
    (void)n8.first_child();

    // 7. pugi::xml_node::last_child -> 1 time
    (void)n8.last_child();

    // 8. pugi::xml_object_range::end -> 6 times
    (void)r1.end();
    (void)r2.end();
    (void)r3.end();
    (void)r4.end();
    (void)r1.end();
    (void)r2.end();

    // 9. pugi::xml_node::first_child -> 1 time
    (void)n8.first_child();

    // 10. pugi::xml_object_range::end -> 2 times
    (void)r3.end();
    (void)r4.end();

    // 11. pugi::xml_object_range::begin -> 1 time
    (void)r1.begin();

    // 12. pugi::xml_object_range::end -> 2 times
    (void)r3.end();
    (void)r4.end();

    // 13. pugi::xml_object_range::begin -> 1 time
    (void)r2.begin();

    delete doc;
    return 0;
}