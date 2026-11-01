// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <string>

static std::string create_null_terminated_string(const uint8_t* data, size_t size) {
    std::string result;
    if (size == 0) {
        result = "";
    } else {
        // Find the first null byte to truncate, or use the whole buffer if none
        size_t null_pos = size;
        for (size_t i = 0; i < size; ++i) {
            if (data[i] == 0) {
                null_pos = i;
                break;
            }
        }
        result.assign(reinterpret_cast<const char*>(data), null_pos);
    }
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_28(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    std::string input_str = create_null_terminated_string(Data, Size);
    
    // Ensure we have at least a minimal valid XML structure to parse if possible,
    // but allow the fuzzer to send garbage to test error handling.
    // We pass the string directly.
    pugi::xml_parse_result result = doc.load_string(input_str.c_str());

    // If parsing failed completely (e.g., no document element), we might not have a root to traverse.
    // However, load_string might still return a document with errors.
    // We proceed to traverse only if we have a root node.
    pugi::xml_node root = doc.document_element();
    if (!root) {
        return 0;
    }

    // Sequence: child -> child -> child -> child -> child -> child -> child
    pugi::xml_node node1 = root.child("a");
    if (!node1) return 0;
    
    pugi::xml_node node2 = node1.child("b");
    if (!node2) return 0;
    
    pugi::xml_node node3 = node2.child("c");
    if (!node3) return 0;
    
    pugi::xml_node node4 = node3.child("d");
    if (!node4) return 0;
    
    pugi::xml_node node5 = node4.child("e");
    if (!node5) return 0;
    
    pugi::xml_node node6 = node5.child("f");
    if (!node6) return 0;
    
    pugi::xml_node node7 = node6.child("g");
    if (!node7) return 0;

    // Sequence: text
    pugi::xml_text text_node = node7.text();
    // Accessing text is safe even if empty, but we can check if it's valid
    if (text_node) {
        // Just to ensure we exercise the text object
        const char* txt = text_node.get();
        (void)txt;
    }

    // Sequence: child
    pugi::xml_node node8 = node7.child("h");
    if (!node8) return 0;

    // Sequence: child_value (overload 1: no args)
    const pugi::char_t* val1 = node8.child_value();
    (void)val1;

    // Sequence: child_value (overload 2: with name)
    const pugi::char_t* val2 = node8.child_value("i");
    (void)val2;

    return 0;
}