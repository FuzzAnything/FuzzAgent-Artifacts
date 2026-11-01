// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_node::offset_debug at pugixml.cpp:6873:35 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6019:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5987:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
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

static bool parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        const char* minimal_xml = "<root/>";
        return doc.load_string(minimal_xml);
    }
    
    // Ensure the input is null-terminated for load_string which expects a C-string
    // We create a temporary buffer with a null terminator
    std::vector<char> buffer(size + 1);
    std::memcpy(buffer.data(), data, size);
    buffer[size] = '\0';
    
    return doc.load_string(buffer.data(), pugi::parse_default);
}

static void fuzz_xml_text_set(pugi::xml_text& text, const uint8_t* data, size_t size) {
    if (!text) return;
    
    // Test with null-terminated string from data
    if (size > 0) {
        std::string str(reinterpret_cast<const char*>(data), size);
        text.set(str.c_str());
    }
    
    text.set("");
    
    // Test with sized buffer
    if (size > 0) {
        text.set(reinterpret_cast<const char*>(data), size);
    }
}

static void fuzz_xml_node_offset_debug(pugi::xml_node& node) {
    if (!node) return;
    (void)node.offset_debug();
}

static void fuzz_xml_node_set_value(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (!node) return;
    
    // Test with null-terminated string
    if (size > 0) {
        std::string str(reinterpret_cast<const char*>(data), size);
        node.set_value(str.c_str());
    }
    node.set_value("");
    
    // Test with sized buffer
    if (size > 0) {
        node.set_value(reinterpret_cast<const char*>(data), size);
    }
    node.set_value("", 0);
}

static void fuzz_xml_node_set_name(pugi::xml_node& node, const uint8_t* data, size_t size) {
    if (!node) return;
    
    // Test with null-terminated string
    if (size > 0) {
        std::string str(reinterpret_cast<const char*>(data), size);
        node.set_name(str.c_str());
    }
    node.set_name("");
    
    // Test with sized buffer
    if (size > 0) {
        node.set_name(reinterpret_cast<const char*>(data), size);
    }
    node.set_name("", 0);
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    
    if (!parse_document(doc, Data, Size)) {
        // Fallback to a valid XML document if parsing fails
        if (!doc.load_string("<root><child/></root>")) {
            return 0;
        }
    }
    
    // Iterate through all nodes in the document
    for (pugi::xml_node node = doc.first_child(); node; node = node.next_sibling()) {
        fuzz_xml_node_offset_debug(node);
        fuzz_xml_node_set_value(node, Data, Size);
        fuzz_xml_node_set_name(node, Data, Size);
        
        // Process children
        for (pugi::xml_node child = node.first_child(); child; child = child.next_sibling()) {
            pugi::xml_text text = child.text();
            fuzz_xml_text_set(text, Data, Size);
        }
        
        // Process text of current node
        pugi::xml_text text = node.text();
        fuzz_xml_text_set(text, Data, Size);
    }
    
    // Test on document root if it has a name/value
    fuzz_xml_node_offset_debug(doc);
    fuzz_xml_node_set_value(doc, Data, Size);
    fuzz_xml_node_set_name(doc, Data, Size);
    
    return 0;
}