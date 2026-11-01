// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6378:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::parent at pugixml.cpp:5913:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::traverse at pugixml.cpp:6789:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6588:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
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
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string_view>

static pugi::xml_document* g_doc = nullptr;

extern "C" int LLVMFuzzerTestOneInput_80(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    // Initialize document if not already initialized
    if (g_doc == nullptr) {
        g_doc = new pugi::xml_document();
    }

    // Ensure the input is null-terminated for safe parsing
    // We create a buffer that is guaranteed to be null-terminated
    // to prevent heap-buffer-overflow in load_string which expects a C-string
    std::vector<char> buffer(Size + 1);
    std::memcpy(buffer.data(), Data, Size);
    buffer[Size] = '\0';

    // Load XML from fuzz input
    pugi::xml_parse_result result = g_doc->load_string(buffer.data());
    if (!result) {
        // Parse failed, but we can still test with an empty document or skip
        // We proceed to test with the current state of the document (which might be empty)
    }

    pugi::xml_node root = g_doc->root();

    // Helper to get a random node for testing
    auto get_random_node = [&]() -> pugi::xml_node {
        if (root.empty()) return pugi::xml_node();
        // Simple traversal to pick a random node based on input data
        size_t idx = 0;
        for (pugi::xml_node node = root; !node.empty(); node = node.next_sibling()) {
            if (idx++ == (Size % 10)) return node;
        }
        return root;
    };

    pugi::xml_node test_node = get_random_node();
    if (test_node.empty()) test_node = root;

    // 1. Test traverse
    struct TestWalker : public pugi::xml_tree_walker {
        bool begin(pugi::xml_node& node) override {
            return true;
        }
        bool for_each(pugi::xml_node& node) override {
            return true;
        }
        bool end(pugi::xml_node& node) override {
            return true;
        }
    };
    TestWalker walker;
    if (!test_node.empty()) {
        test_node.traverse(walker);
    }

    // 2. Test next_sibling
    if (!test_node.empty()) {
        std::string_view name_sv = "";
        // Try with empty name
        (void)test_node.next_sibling(name_sv);
        // Try with a name derived from input if possible (ensure null termination if needed, but string_view handles size)
        if (Size > 0) {
            // Use the buffer which is null-terminated
            std::string_view name_from_input(buffer.data(), Size);
            (void)test_node.next_sibling(name_from_input);
        }
    }

    // 3. Test remove_child
    if (!test_node.empty()) {
        std::string_view name_sv = "";
        (void)test_node.remove_child(name_sv);
        if (Size > 0) {
            std::string_view name_from_input(buffer.data(), Size);
            (void)test_node.remove_child(name_from_input);
        }
    }

    // 4. Test insert_child_after
    if (!test_node.empty()) {
        std::string_view name_sv = "";
        pugi::xml_node sibling = test_node.next_sibling();
        if (sibling.empty()) sibling = test_node;
        (void)test_node.insert_child_after(name_sv, sibling);
        if (Size > 0) {
            std::string_view name_from_input(buffer.data(), Size);
            (void)test_node.insert_child_after(name_from_input, sibling);
        }
    }

    // 5. Test previous_sibling
    if (!test_node.empty()) {
        std::string_view name_sv = "";
        (void)test_node.previous_sibling(name_sv);
        if (Size > 0) {
            std::string_view name_from_input(buffer.data(), Size);
            (void)test_node.previous_sibling(name_from_input);
        }
    }

    // 6. Test parent
    if (!test_node.empty()) {
        (void)test_node.parent();
    }

    return 0;
}