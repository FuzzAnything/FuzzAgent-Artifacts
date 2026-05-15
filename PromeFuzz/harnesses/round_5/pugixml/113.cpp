// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7040:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6397:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <fstream>

static pugi::xml_document create_test_document(const uint8_t* data, size_t size) {
    pugi::xml_document doc;
    
    // Try to parse the fuzzer input as XML
    pugi::xml_parse_result result = doc.load_buffer(data, size, 
        pugi::parse_default | pugi::parse_ws_pcdata | pugi::parse_escapes);
    
    // If parsing fails, create a minimal valid document
    if (!result) {
        doc.append_child("root");
        if (size > 0) {
            pugi::xml_node node = doc.child("root");
            std::string content(reinterpret_cast<const char*>(data), 
                               std::min(size, static_cast<size_t>(100)));
            node.append_child("child").text().set(content.c_str());
        }
    }
    
    return doc;
}

static class FuzzWalker : public pugi::xml_tree_walker {
public:
    bool for_each(pugi::xml_node& node) override {
        // Just continue traversal
        return true;
    }
    
    bool begin(pugi::xml_node& node) override {
        // Custom initialization logic
        traversal_count = 0;
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        // Custom cleanup logic
        return true;
    }
    
private:
    int traversal_count = 0;
} fuzz_walker;

extern "C" int LLVMFuzzerTestOneInput_113(const uint8_t* Data, size_t Size) {
    // Create test document from fuzzer input
    pugi::xml_document doc = create_test_document(Data, Size);
    
    // Test xml_node::append_copy
    pugi::xml_node root = doc.child("root");
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Get some nodes to work with
    pugi::xml_node child = root.first_child();
    if (child) {
        // Try to append a copy of child to root
        pugi::xml_node copied = root.append_copy(child);
        
        // Also try copying to different locations
        if (copied) {
            pugi::xml_node another_child = copied.append_child("nested");
            if (another_child) {
                root.append_copy(another_child);
            }
        }
    }
    
    // Test xml_node::parent
    if (child) {
        pugi::xml_node parent = child.parent();
        if (parent) {
            // Get parent of parent
            pugi::xml_node grandparent = parent.parent();
        }
    }
    
    // Test xml_node::root
    if (child) {
        pugi::xml_node doc_root = child.root();
    }
    
    // Test xml_node::traverse with xml_tree_walker
    if (root) {
        bool traversal_result = root.traverse(fuzz_walker);
        
        // Also test on child nodes
        for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
            node.traverse(fuzz_walker);
        }
    }
    
    // Test xml_tree_walker::begin and xml_tree_walker::end directly
    if (root) {
        fuzz_walker.begin(root);
        fuzz_walker.end(root);
    }
    
    // Additional test: create a file and parse from it
    const char* filename = "./dummy_file";
    std::ofstream outfile(filename, std::ios::binary);
    if (outfile.is_open()) {
        outfile.write(reinterpret_cast<const char*>(Data), Size);
        outfile.close();
        
        pugi::xml_document file_doc;
        pugi::xml_parse_result file_result = file_doc.load_file(filename);
        
        if (file_result) {
            pugi::xml_node file_root = file_doc.root();
            if (file_root) {
                file_root.append_copy(file_root.first_child());
                file_root.traverse(fuzz_walker);
            }
        }
        
        // Clean up file
        std::remove(filename);
    }
    
    // Test edge cases with empty nodes
    pugi::xml_node empty_node;
    empty_node.parent();
    empty_node.root();
    empty_node.append_copy(empty_node);
    
    // Test with newly created nodes
    pugi::xml_document doc2;
    pugi::xml_node new_root = doc2.append_child("test");
    pugi::xml_node new_child = new_root.append_child("child");
    new_root.append_copy(new_child);
    new_root.traverse(fuzz_walker);
    
    return 0;
}