// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6594:30 in pugixml.hpp
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
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <string>
#include <cstring>
#include <cstdlib>

static std::string create_xml_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return "<root><a/><b/><c/><d/><e/><f/><g/><h/><i/><j/><k/><l/><m/><n/><o/></root>";
    }
    
    // Create a safe XML string from input data
    std::string result = "<root>";
    size_t max_len = std::min(size, (size_t)1000);
    
    for (size_t i = 0; i < max_len; ++i) {
        uint8_t byte = data[i];
        // Convert byte to a safe character for XML
        if (byte >= 'a' && byte <= 'z') {
            result += "<";
            result += static_cast<char>(byte);
            result += "/>";
        } else if (byte >= 'A' && byte <= 'Z') {
            result += "<";
            result += static_cast<char>(byte);
            result += "/>";
        } else {
            // Use a default tag for non-alphabetic bytes
            result += "<tag";
            result += static_cast<char>('0' + (byte % 10));
            result += "/>";
        }
    }
    
    result += "</root>";
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }
    
    pugi::xml_document doc;
    std::string xml_str = create_xml_from_input(Data, Size);
    
    // Parse the XML document
    if (!doc.load_string(xml_str.c_str())) {
        return 0;
    }
    
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    // Ensure we have enough children for the test sequence
    pugi::xml_node child_a = root.child("a");
    pugi::xml_node child_b = root.child("b");
    pugi::xml_node child_c = root.child("c");
    pugi::xml_node child_d = root.child("d");
    pugi::xml_node child_e = root.child("e");
    pugi::xml_node child_f = root.child("f");
    pugi::xml_node child_g = root.child("g");
    pugi::xml_node child_h = root.child("h");
    pugi::xml_node child_i = root.child("i");
    pugi::xml_node child_j = root.child("j");
    pugi::xml_node child_k = root.child("k");
    pugi::xml_node child_l = root.child("l");
    pugi::xml_node child_m = root.child("m");
    pugi::xml_node child_n = root.child("n");
    pugi::xml_node child_o = root.child("o");
    
    // If we don't have enough children, create some
    if (!child_a || !child_b || !child_c || !child_d || !child_e || 
        !child_f || !child_g || !child_h || !child_i || !child_j ||
        !child_k || !child_l || !child_m || !child_n || !child_o) {
        
        // Create default children
        root.append_child("a");
        root.append_child("b");
        root.append_child("c");
        root.append_child("d");
        root.append_child("e");
        root.append_child("f");
        root.append_child("g");
        root.append_child("h");
        root.append_child("i");
        root.append_child("j");
        root.append_child("k");
        root.append_child("l");
        root.append_child("m");
        root.append_child("n");
        root.append_child("o");
        
        child_a = root.child("a");
        child_b = root.child("b");
        child_c = root.child("c");
        child_d = root.child("d");
        child_e = root.child("e");
        child_f = root.child("f");
        child_g = root.child("g");
        child_h = root.child("h");
        child_i = root.child("i");
        child_j = root.child("j");
        child_k = root.child("k");
        child_l = root.child("l");
        child_m = root.child("m");
        child_n = root.child("n");
        child_o = root.child("o");
    }
    
    // Target API sequence:
    // remove_child -> remove_child -> child -> child -> remove_child -> remove_child -> child -> remove_child -> child -> remove_child -> remove_child -> remove_child -> remove_child -> remove_child -> remove_child
    
    // 1. remove_child (const char_t* name)
    root.remove_child("a");
    
    // 2. remove_child (const char_t* name)
    root.remove_child("b");
    
    // 3. child (const char_t* name)
    pugi::xml_node node1 = root.child("c");
    
    // 4. child (const char_t* name)
    pugi::xml_node node2 = root.child("d");
    
    // 5. remove_child (string_view_t name)
    std::string_view sv1("e");
    root.remove_child(sv1);
    
    // 6. remove_child (const xml_node& n)
    pugi::xml_node node_to_remove = root.child("f");
    if (node_to_remove) {
        root.remove_child(node_to_remove);
    }
    
    // 7. child (const char_t* name)
    pugi::xml_node node3 = root.child("g");
    
    // 8. remove_child (const char_t* name)
    root.remove_child("h");
    
    // 9. child (const char_t* name)
    pugi::xml_node node4 = root.child("i");
    
    // 10. remove_child (string_view_t name)
    std::string_view sv2("j");
    root.remove_child(sv2);
    
    // 11. remove_child (const xml_node& n)
    pugi::xml_node node_to_remove2 = root.child("k");
    if (node_to_remove2) {
        root.remove_child(node_to_remove2);
    }
    
    // 12. remove_child (const char_t* name)
    root.remove_child("l");
    
    // 13. remove_child (const char_t* name)
    root.remove_child("m");
    
    // 14. remove_child (string_view_t name)
    std::string_view sv3("n");
    root.remove_child(sv3);
    
    // 15. remove_child (const xml_node& n)
    pugi::xml_node node_to_remove3 = root.child("o");
    if (node_to_remove3) {
        root.remove_child(node_to_remove3);
    }
    
    return 0;
}