// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
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

// Custom walker to test begin and end callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker() : stop_next(false) {}

    bool begin(pugi::xml_node& node) override {
        // Test accessing parent and root during begin
        if (node) {
            pugi::xml_node p = node.parent();
            pugi::xml_node r = node.root();
            // Check validity to avoid undefined behavior
            if (p) {
                p.name(); 
            }
            if (r) {
                r.name();
            }
        }
        return !stop_next;
    }

    bool end(pugi::xml_node& node) override {
        // Test accessing parent and root during end
        if (node) {
            pugi::xml_node p = node.parent();
            pugi::xml_node r = node.root();
            if (p) {
                p.name();
            }
            if (r) {
                r.name();
            }
        }
        return !stop_next;
    }

    // Implement the pure virtual method for_each to make the class concrete
    // Default behavior: do nothing and return true to continue traversal
    bool for_each(pugi::xml_node& node) override {
        // Optional: perform additional checks here if needed
        if (node) {
            node.name();
        }
        return true;
    }

    void set_stop_next(bool val) { stop_next = val; }

private:
    bool stop_next;
};

// Helper to parse input data into a document
static bool parse_input(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) return false;
    
    // Ensure null termination for string conversion
    std::string str(reinterpret_cast<const char*>(data), size);
    
    // Try parsing as UTF-8
    pugi::xml_parse_result result = doc.load_string(str.c_str());
    return result.status == pugi::status_ok;
}

// Helper to write data to dummy file if needed
static bool write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (!f) return false;
    size_t written = fwrite(data, 1, size, f);
    fclose(f);
    return written == size;
}

extern "C" int LLVMFuzzerTestOneInput_89(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    FuzzWalker walker;

    // Strategy 1: Parse input as XML string
    if (parse_input(Data, Size, doc)) {
        pugi::xml_node root = doc.root();
        if (root) {
            // Test parent on root (should be null)
            pugi::xml_node p = root.parent();
            if (p) {
                p.name();
            }

            // Test root on root
            pugi::xml_node r = root.root();
            if (r) {
                r.name();
            }

            // Test next_sibling overloads on root
            pugi::xml_node ns1 = root.next_sibling();
            if (ns1) {
                ns1.name();
            }
            
            std::string_view sv(reinterpret_cast<const char*>(Data), Size);
            // Limit string_view size to avoid huge allocations
            if (sv.size() > 1024) sv = sv.substr(0, 1024);
            pugi::xml_node ns2 = root.next_sibling(sv);
            if (ns2) {
                ns2.name();
            }

            // Traverse with walker
            root.traverse(walker);
        }

        // Test on children if any
        for (pugi::xml_node node = root.first_child(); node; node = node.next_sibling()) {
            pugi::xml_node p = node.parent();
            if (p) {
                p.name();
            }
            pugi::xml_node r = node.root();
            if (r) {
                r.name();
            }
            
            pugi::xml_node ns1 = node.next_sibling();
            if (ns1) {
                ns1.name();
            }
            
            std::string_view sv(reinterpret_cast<const char*>(Data), Size);
            if (sv.size() > 1024) sv = sv.substr(0, 1024);
            pugi::xml_node ns2 = node.next_sibling(sv);
            if (ns2) {
                ns2.name();
            }
        }
    }

    // Strategy 2: Write to file and load
    write_dummy_file(Data, Size);
    pugi::xml_document doc2;
    if (doc2.load_file("./dummy_file")) {
        pugi::xml_node root2 = doc2.root();
        if (root2) {
            root2.traverse(walker);
            
            // Test parent/root on root
            root2.parent();
            root2.root();
            
            // Test next_sibling
            root2.next_sibling();
            std::string_view sv(reinterpret_cast<const char*>(Data), Size);
            if (sv.size() > 1024) sv = sv.substr(0, 1024);
            root2.next_sibling(sv);
        }
    }

    // Strategy 3: Test with empty/invalid nodes
    pugi::xml_node empty_node;
    empty_node.parent();
    empty_node.root();
    empty_node.next_sibling();
    std::string_view sv_empty;
    empty_node.next_sibling(sv_empty);

    // Strategy 4: Random walker behavior
    walker.set_stop_next(true);
    if (parse_input(Data, Size, doc)) {
        pugi::xml_node root = doc.root();
        if (root) {
            root.traverse(walker);
        }
    }

    return 0;
}