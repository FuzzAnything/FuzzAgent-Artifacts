// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <sstream>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_73(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    write_dummy_file(Data, Size);

    pugi::xml_document doc;
    
    // Try loading from file
    pugi::xml_parse_result result = doc.load_file("./dummy_file");
    if (!result) {
        // If file load fails, try parsing the raw data directly as a string
        std::string str(reinterpret_cast<const char*>(Data), Size);
        result = doc.load_string(str.c_str());
    }

    if (!doc) {
        return 0;
    }

    // Test pugi::xml_node::root
    pugi::xml_node root_node = doc.root();
    if (root_node) {
        pugi::xml_node root_check = root_node.root();
        (void)root_check; // Ensure root is accessible
    }

    // Test pugi::xml_node::parent
    pugi::xml_node current = root_node;
    if (current) {
        pugi::xml_node parent = current.parent();
        (void)parent; // Check parent (will be null for root)
        
        // Traverse children to test parent on non-root nodes
        for (pugi::xml_node child = current.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node child_parent = child.parent();
            (void)child_parent;
        }
    }

    // Test pugi::xml_node::next_sibling overloads
    if (root_node) {
        // Overload 1: next_sibling()
        pugi::xml_node next_sib = root_node.next_sibling();
        (void)next_sib;

        // Overload 2: next_sibling(name)
        // Use a simple name from the input or a default
        std::string_view a_str(reinterpret_cast<const char*>(Data), Size);
        if (a_str.size() > 0) {
            // Ensure null termination for char_t* if needed, though pugixml handles string_view internally in some versions
            // Here we construct a safe C-string from the view to pass to next_sibling(const char_t*)
            // We take a small chunk to avoid huge allocations or invalid chars if input is binary
            size_t len = (a_str.size() < 100) ? a_str.size() : 100;
            std::string name_str(a_str.data(), len);
            name_str.resize(len); // Ensure no nulls in middle if we want to be safe, though pugixml handles it
            // Replace potential nulls to ensure valid C-string for the API
            for (size_t i = 0; i < name_str.size(); ++i) {
                if (name_str[i] == '\0') name_str[i] = ' ';
            }
            pugi::xml_node next_sib_named = root_node.next_sibling(name_str.c_str());
            (void)next_sib_named;
        } else {
            pugi::xml_node next_sib_named = root_node.next_sibling("default");
            (void)next_sib_named;
        }
    }

    // Test pugi::xml_tree_walker::begin and end
    struct CustomWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            // Logic to start traversal
            // Return true to visit children, false to skip
            return true;
        }

        bool end(pugi::xml_node& node) override {
            // Logic to end traversal
            // Return true to continue, false to stop
            return true;
        }

        bool for_each(pugi::xml_node& node) override {
            // Logic for each node
            // Return true to continue, false to stop
            return true;
        }
    };

    CustomWalker walker;
    if (root_node) {
        root_node.traverse(walker);
    }

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}