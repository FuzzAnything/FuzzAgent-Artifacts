// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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

static class TestWalker : public pugi::xml_tree_walker {
public:
    virtual bool begin(pugi::xml_node& node) override {
        // Test root() and parent() on the traversal starting node
        (void)node.root();
        (void)node.parent();
        return true; // Continue traversal
    }
    
    virtual bool for_each(pugi::xml_node& node) override {
        // Test functions on each node during traversal
        (void)node.root();
        (void)node.parent();
        (void)node.next_sibling();
        return true; // Continue traversal
    }
    
    virtual bool end(pugi::xml_node& node) override {
        // Test functions on the root node at traversal end
        (void)node.root();
        (void)node.parent();
        return true;
    }
} g_walker;

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t* Data, size_t Size) {
    // 1. Create XML document and parse input data
    pugi::xml_document doc;
    
    // Try to parse the input as XML
    pugi::xml_parse_result result = doc.load_buffer(Data, Size, 
        pugi::parse_default | pugi::parse_trim_pcdata);
    
    // 2. Test API functions on various nodes
    pugi::xml_node root = doc.root();
    
    // Test root() function
    (void)root.root();
    
    // Test parent() function on root and children
    (void)root.parent();
    
    // Test next_sibling() on root and traverse children
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.root();
        (void)child.parent();
        (void)child.next_sibling();
        
        // Test next_sibling() with string_view parameter
        std::string_view dummy_name = "dummy";
        (void)child.next_sibling(dummy_name.data());
    }
    
    // 3. Test traversal with custom walker
    if (result) { // Only traverse if parsing succeeded
        // Test traverse() function
        (void)root.traverse(g_walker);
        
        // Test on child nodes too
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            (void)child.traverse(g_walker);
        }
    }
    
    // 4. Test functions on empty/default constructed nodes
    pugi::xml_node empty_node;
    (void)empty_node.root();
    (void)empty_node.parent();
    (void)empty_node.next_sibling();
    
    // 5. Test with string_view parameter for next_sibling
    std::string_view name_filter;
    if (Size > 0) {
        // Use first byte as length hint for name (capped at 16)
        size_t name_len = Data[0] % 16;
        if (name_len > 0 && name_len <= Size - 1) {
            name_filter = std::string_view(reinterpret_cast<const char*>(Data + 1), name_len);
        }
    }
    
    // Apply name filter to next_sibling calls
    for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
        (void)child.next_sibling(name_filter.data());
    }
    
    // 6. Test with file input (using dummy file as requested)
    const char* filename = "./dummy_file";
    FILE* fp = fopen(filename, "wb");
    if (fp) {
        fwrite(Data, 1, Size, fp);
        fclose(fp);
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file(filename);
        
        if (file_result) {
            pugi::xml_node file_root = file_doc.root();
            (void)file_root.root();
            (void)file_root.parent();
            
            // Test traversal on file-loaded document
            (void)file_root.traverse(g_walker);
        }
        
        // Clean up dummy file
        remove(filename);
    }
    
    return 0;
}