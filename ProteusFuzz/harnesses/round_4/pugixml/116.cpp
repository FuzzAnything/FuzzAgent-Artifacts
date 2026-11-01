// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5735:34 in pugixml.hpp
// pugi::xml_node::name at pugixml.cpp:5688:39 in pugixml.hpp
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

// Custom walker to test begin and end callbacks
// Must implement all pure virtual methods of xml_tree_walker
class FuzzWalker : public pugi::xml_tree_walker {
public:
    FuzzWalker(bool stop_begin, bool stop_end, bool stop_for_each) 
        : m_stop_begin(stop_begin), m_stop_end(stop_end), m_stop_for_each(stop_for_each) {}

    virtual bool begin(pugi::xml_node& node) override {
        (void)node;
        return !m_stop_begin;
    }

    virtual bool end(pugi::xml_node& node) override {
        (void)node;
        return !m_stop_end;
    }

    virtual bool for_each(pugi::xml_node& node) override {
        (void)node;
        return !m_stop_for_each;
    }

private:
    bool m_stop_begin;
    bool m_stop_end;
    bool m_stop_for_each;
};

static bool parse_xml_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Ensure the input is valid UTF-8 or ASCII for parsing
    std::string xml_str(reinterpret_cast<const char*>(data), size);
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    return result.status == pugi::status_ok;
}

static void fuzz_node_navigation(pugi::xml_node node) {
    if (node == nullptr) {
        return;
    }

    // Test parent()
    pugi::xml_node parent = node.parent();
    if (parent != nullptr) {
        // Traverse up a bit if possible
        pugi::xml_node grandparent = parent.parent();
        (void)grandparent;
    }

    // Test root()
    pugi::xml_node root = node.root();
    if (root != nullptr) {
        // Just ensure it's valid
        (void)root.name();
    }

    // Test next_sibling() - no argument
    pugi::xml_node next_sib = node.next_sibling();
    if (next_sib != nullptr) {
        (void)next_sib.name();
    }

    // Test next_sibling(name) - with argument
    // We try a few potential names based on the node's name or generic strings
    if (node.name()[0] == '\0') {
        node.next_sibling("test");
        node.next_sibling("");
    } else {
        node.next_sibling(node.name());
        node.next_sibling("nonexistent");
        node.next_sibling("");
    }
}

extern "C" int LLVMFuzzerTestOneInput_116(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the input data as XML
    // If parsing fails, we create a minimal valid XML to ensure we can test the API
    bool parsed = parse_xml_document(Data, Size, doc);
    
    if (!parsed) {
        // Fallback: create a minimal valid XML document to test API logic
        std::string fallback_xml = "<root><child>data</child><child2/></root>";
        doc.load_string(fallback_xml.c_str());
    }

    if (doc.empty()) {
        return 0;
    }

    // Get root node
    pugi::xml_node root_node = doc.root();
    if (root_node == nullptr) {
        return 0;
    }

    // Test navigation APIs on root
    fuzz_node_navigation(root_node);

    // Test navigation APIs on children
    for (pugi::xml_node child = root_node.first_child(); child != nullptr; child = child.next_sibling()) {
        fuzz_node_navigation(child);
    }

    // Test xml_tree_walker::begin and xml_tree_walker::end
    // We create walkers with different behaviors to explore states
    // Note: FuzzWalker now implements for_each as well
    FuzzWalker walker1(true, true, true);   // Stop at begin, stop at end, stop at for_each
    FuzzWalker walker2(false, true, false); // Continue at begin, stop at end, continue at for_each
    FuzzWalker walker3(true, false, true);  // Stop at begin, continue at end, stop at for_each
    FuzzWalker walker4(false, false, false); // Continue at begin, continue at end, continue at for_each

    // Traverse with different walkers
    root_node.traverse(walker1);
    root_node.traverse(walker2);
    root_node.traverse(walker3);
    root_node.traverse(walker4);

    // Additional stress test: traverse a specific child if it exists
    pugi::xml_node child_node = root_node.child("child");
    if (child_node != nullptr) {
        child_node.traverse(walker1);
        child_node.traverse(walker4);
    }

    return 0;
}