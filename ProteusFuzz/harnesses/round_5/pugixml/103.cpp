// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
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
#include <pugixml.hpp>

static class CustomWalker : public pugi::xml_tree_walker {
public:
    CustomWalker() : pugi::xml_tree_walker() {}

    bool begin(pugi::xml_node& node) override {
        // Fuzz logic: sometimes skip children based on node name length
        if (node.name()) {
            size_t len = strlen(node.name());
            if (len % 2 == 0) {
                return false;
            }
        }
        return true;
    }

    bool end(pugi::xml_node& node) override {
        // Fuzz logic: sometimes stop traversal based on node type
        if (node.type() == pugi::node_element) {
            if (node.attribute("fuzz_stop")) {
                return false;
            }
        }
        return true;
    }

    // Required pure virtual method to make the class concrete
    bool for_each(pugi::xml_node& node) override {
        // Fuzz logic: sometimes skip processing based on node name
        if (node.name()) {
            size_t len = strlen(node.name());
            if (len % 3 == 0) {
                return false;
            }
        }
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput_103(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Create a document and load the fuzzer input
    pugi::xml_document doc;
    
    // Ensure the input data is null-terminated to prevent heap-buffer-overflow
    // when pugixml calls strlen internally. We create a buffer with a null terminator.
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    pugi::xml_parse_result result = doc.load_string(buffer.data());
    
    // Test with the root node if parsing succeeded or partially succeeded
    pugi::xml_node root = doc.root();

    // 1. Test pugi::xml_node::parent
    if (root) {
        pugi::xml_node parent = root.parent();
        (void)parent; 
        if (parent) {
            pugi::xml_node grandParent = parent.parent();
            (void)grandParent;
        }
    }

    // 2. Test pugi::xml_node::root
    if (root) {
        pugi::xml_node rootOfRoot = root.root();
        (void)rootOfRoot;
        
        if (root.first_child()) {
            pugi::xml_node child = root.first_child();
            pugi::xml_node childRoot = child.root();
            (void)childRoot;
        }
    }

    // 3. Test pugi::xml_node::next_sibling (both overloads)
    if (root) {
        pugi::xml_node nextSib1 = root.next_sibling();
        (void)nextSib1;
        
        const char* dummyName = "test";
        pugi::xml_node nextSib2 = root.next_sibling(dummyName);
        (void)nextSib2;
        
        for (pugi::xml_node child = root.first_child(); child; child = child.next_sibling()) {
            pugi::xml_node sib1 = child.next_sibling();
            (void)sib1;
            pugi::xml_node sib2 = child.next_sibling(dummyName);
            (void)sib2;
        }
    }

    // 4. Test pugi::xml_tree_walker::begin and end
    if (root) {
        CustomWalker walker;
        root.traverse(walker);
    }

    // Cleanup is automatic via RAII for pugi::xml_document
    return 0;
}