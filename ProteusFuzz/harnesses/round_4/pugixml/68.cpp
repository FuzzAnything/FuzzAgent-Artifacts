// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
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
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>
#include <string_view>

// Custom walker to test begin and end callbacks
class FuzzWalker : public pugi::xml_tree_walker {
public:
    explicit FuzzWalker(bool stop_begin, bool stop_end)
        : m_stop_begin(stop_begin), m_stop_end(stop_end) {}

    bool begin(pugi::xml_node& node) override {
        (void)node;
        return !m_stop_begin;
    }

    bool end(pugi::xml_node& node) override {
        (void)node;
        return !m_stop_end;
    }

    // Required pure virtual method implementation
    bool for_each(pugi::xml_node& node) override {
        (void)node;
        // Default behavior: continue traversal
        return true;
    }

private:
    bool m_stop_begin;
    bool m_stop_end;
};

static void test_node_navigation(pugi::xml_node node) {
    // Test parent()
    pugi::xml_node parent = node.parent();
    (void)parent;

    // Test root()
    pugi::xml_node root = node.root();
    (void)root;

    // Test next_sibling()
    pugi::xml_node next = node.next_sibling();
    (void)next;

    // Test next_sibling(name)
    std::string_view name_str = "test_sibling";
    pugi::xml_node next_named = node.next_sibling(name_str);
    (void)next_named;
}

static void test_traversal(pugi::xml_node node, bool stop_begin, bool stop_end) {
    FuzzWalker walker(stop_begin, stop_end);
    node.traverse(walker);
}

extern "C" int LLVMFuzzerTestOneInput_68(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Ensure the input is null-terminated for load_string
    // We create a temporary buffer with a null terminator if the last byte is not null
    // However, load_string expects a null-terminated string. 
    // If the fuzzer provides raw bytes without a null terminator, we must handle it.
    // A safe approach is to copy the data to a buffer that we know is null-terminated.
    
    pugi::xml_document doc;
    
    // Check if the last byte is already null
    bool has_null_terminator = (Size > 0 && Data[Size - 1] == '\0');
    
    pugi::xml_parse_result result;
    
    if (has_null_terminator) {
        result = doc.load_string(reinterpret_cast<const char*>(Data));
    } else {
        // If not null-terminated, we need to create a temporary buffer
        // We allocate Size + 1 bytes to ensure space for the null terminator
        std::vector<char> buffer(Size + 1);
        std::memcpy(buffer.data(), Data, Size);
        buffer[Size] = '\0';
        result = doc.load_string(buffer.data());
    }
    
    // Even if parsing fails, we can still test some functions on the document or null nodes
    pugi::xml_node root_node = doc.root();
    
    // Test navigation on root
    test_node_navigation(root_node);

    // Test traversal with different walker configurations
    // Case 1: Continue both begin and end
    test_traversal(root_node, false, false);
    
    // Case 2: Stop at begin
    test_traversal(root_node, true, false);
    
    // Case 3: Stop at end
    test_traversal(root_node, false, true);
    
    // Case 4: Stop both
    test_traversal(root_node, true, true);

    // Test on children if they exist
    for (pugi::xml_node child = root_node.first_child(); child; child = child.next_sibling()) {
        test_node_navigation(child);
        test_traversal(child, false, false);
        test_traversal(child, true, false);
        test_traversal(child, false, true);
        test_traversal(child, true, true);
    }

    // Test on a null node to ensure safety
    pugi::xml_node null_node;
    test_node_navigation(null_node);
    test_traversal(null_node, false, false);

    return 0;
}