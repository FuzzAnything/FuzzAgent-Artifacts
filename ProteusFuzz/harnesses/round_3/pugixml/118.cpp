// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
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
#include <string_view>

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

static bool walk_tree(pugi::xml_node& node, int depth) {
    if (depth > 100) return false;
    
    pugi::xml_node root = node.root();
    if (root) {
        pugi::xml_node parent = root.parent();
        pugi::xml_node next = root.next_sibling();
        pugi::xml_node next_named = root.next_sibling("test");
        
        pugi::xml_node child = root.first_child();
        while (child) {
            if (!walk_tree(child, depth + 1)) return false;
            child = child.next_sibling();
        }
    }
    
    return true;
}

class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : pugi::xml_tree_walker() {}
    
    bool begin(pugi::xml_node& node) override {
        return true;
    }
    
    bool end(pugi::xml_node& node) override {
        return true;
    }
    
    bool for_each(pugi::xml_node& node) override {
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_118(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    
    // Try loading from buffer directly
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    if (result) {
        // Test root()
        pugi::xml_node root = doc.root();
        
        // Test parent() on root (should be null)
        pugi::xml_node parent = root.parent();
        
        // Test next_sibling() overloads on root
        pugi::xml_node next = root.next_sibling();
        std::string_view name_view = "test";
        pugi::xml_node next_named = root.next_sibling(name_view);
        
        // Test tree walker
        CustomWalker walker;
        doc.traverse(walker);
        
        // Test walk_tree which internally calls the target functions
        walk_tree(root, 0);
        
        // Test on children
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_root = child.root();
            pugi::xml_node child_parent = child.parent();
            pugi::xml_node child_next = child.next_sibling();
            pugi::xml_node child_next_named = child.next_sibling(name_view);
            
            if (child_parent == root) {
                // Valid parent-child relationship
            }
        }
    }
    
    // Also try loading from file
    write_dummy_file(Data, Size);
    pugi::xml_document doc2;
    if (doc2.load_file("./dummy_file")) {
        pugi::xml_node root2 = doc2.root();
        
        // Test all functions on file-loaded document
        pugi::xml_node parent2 = root2.parent();
        pugi::xml_node next2 = root2.next_sibling();
        std::string_view name_view2 = "file_test";
        pugi::xml_node next_named2 = root2.next_sibling(name_view2);
        
        CustomWalker walker2;
        doc2.traverse(walker2);
        
        walk_tree(root2, 0);
    }
    
    return 0;
}