// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::get at pugixml.cpp:6966:39 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5928:39 in pugixml.hpp
// pugi::xml_node::child_value at pugixml.cpp:5946:39 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_29(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure the input is null-terminated for load_string
    // We create a buffer with a null terminator if not present to avoid OOB reads
    // pugixml::char_t is either char or wchar_t depending on PUGIXML_WCHAR_MODE.
    // The fuzzer input is uint8_t*. If PUGIXML_WCHAR_MODE is defined, we need to handle wchar_t.
    // However, standard fuzzing usually targets the narrow (char) version unless specified.
    // We will assume the standard build where char_t is char.
    // To be safe against non-null-terminated input, we copy to a local buffer with a null terminator.
    
    // Check if the last byte is already null
    bool is_null_terminated = (Size > 0 && Data[Size - 1] == 0);
    
    const pugi::char_t* input_str;
    
    if (is_null_terminated) {
        input_str = reinterpret_cast<const pugi::char_t*>(Data);
    } else {
        // Create a temporary buffer with null terminator
        // We use a local vector to ensure safety and automatic cleanup
        std::vector<pugi::char_t> buffer(Size + 1);
        std::memcpy(buffer.data(), Data, Size);
        buffer[Size] = 0;
        input_str = buffer.data();
    }
    
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(input_str);
    
    // If parsing failed, we might still want to traverse to test error handling paths, 
    // but if the document is empty, child() will return empty node, which is safe.
    
    pugi::xml_node root = doc;
    
    // Sequence of calls as requested:
    // 1. child (1st)
    pugi::xml_node node1 = root.child("test1");
    // 2. child (2nd)
    pugi::xml_node node2 = node1.child("test2");
    // 3. child (3rd)
    pugi::xml_node node3 = node2.child("test3");
    // 4. child (4th)
    pugi::xml_node node4 = node3.child("test4");
    // 5. child (5th)
    pugi::xml_node node5 = node4.child("test5");
    // 6. child (6th)
    pugi::xml_node node6 = node5.child("test6");
    // 7. child (7th)
    pugi::xml_node node7 = node6.child("test7");
    // 8. child (8th)
    pugi::xml_node node8 = node7.child("test8");
    
    // 9. text
    pugi::xml_text text_node = node8.text();
    // Accessing text content safely
    const pugi::char_t* text_content = text_node.get();
    (void)text_content; // Suppress unused variable warning
    
    // 10. child (9th) - on node8
    pugi::xml_node node9 = node8.child("test9");
    
    // 11. child_value (no arg) - on node9
    const pugi::char_t* val1 = node9.child_value();
    (void)val1;
    
    // 12. child_value (with arg) - on node9
    const pugi::char_t* val2 = node9.child_value("test10");
    (void)val2;

    // Cleanup is automatic via RAII for pugi::xml_document
    
    return 0;
}