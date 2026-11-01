// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5906:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::previous_sibling at pugixml.cpp:5811:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5797:34 in pugixml.hpp
// pugi::xml_node::next_sibling at pugixml.cpp:5749:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>

static void fuzzer_seed_xml_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    // Attempt to parse the fuzz input as XML
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    
    // If parsing fails or results in an empty document, create a minimal valid structure
    if (!result || doc.first_child() == nullptr) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        // Add some initial children to allow traversal testing
        root.append_child("child1");
        root.append_child("child2");
        root.append_child("child3");
    }
}

extern "C" int LLVMFuzzerTestOneInput_63(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document doc;
    fuzzer_seed_xml_document(doc, Data, Size);

    // Get a starting node (root or first child)
    pugi::xml_node start_node = doc.first_child();
    if (start_node == nullptr) {
        start_node = doc;
    }

    // Create a string_view from the input data for name parameters
    // Ensure we don't read past the buffer
    std::string_view name_view(reinterpret_cast<const char*>(Data), 
                               Size > 0 ? Size : 0);

    // 1. Test next_sibling with name
    pugi::xml_node ns1 = start_node.next_sibling(name_view);
    (void)ns1; // Suppress unused variable warning

    // 1. Test next_sibling without name
    pugi::xml_node ns2 = start_node.next_sibling();
    (void)ns2;

    // 2. Test append_child
    // We try to append to the start_node. 
    // Note: If start_node is the document root, this might fail if it's not an element, 
    // but the function handles it safely.
    pugi::xml_node new_child = start_node.append_child(name_view);
    (void)new_child;

    // 3. Test child with name
    pugi::xml_node c1 = start_node.child(name_view);
    (void)c1;

    // 3. Test child without name (not applicable based on signature, but we test with name_view)
    // The signature requires a name, so we use name_view.

    // 4. Test last_child
    pugi::xml_node lc = start_node.last_child();
    (void)lc;

    // 5. Test insert_copy_after
    // This requires a proto node and a node to insert after.
    // We use start_node as both proto and the reference node for simplicity,
    // though the logic might return null if they are not attributes or siblings in the expected way.
    // The function signature in the prompt says: xml_node insert_copy_after(const xml_node& proto, const xml_node& node);
    // However, standard pugixml insert_copy_after for nodes is: insert_copy_after(const xml_node& proto, const xml_node& node);
    // Wait, the prompt says "insert a deep copy of proto attribute after attr". 
    // But the signature provided is xml_node insert_copy_after(const xml_node& proto, const xml_node& node).
    // This signature matches the node version, not attribute. 
    // Let's assume the prompt's description of "attribute" is a slight confusion in the text, 
    // but the signature is for nodes. 
    // Actually, pugixml has:
    // xml_node insert_copy_after(const xml_node& proto, const xml_node& node);
    // This copies 'proto' and inserts it after 'node'.
    // If 'node' is invalid, it returns null.
    // Let's try with start_node as both.
    pugi::xml_node ic1 = start_node.insert_copy_after(start_node, start_node);
    (void)ic1;

    // Try with a child if it exists
    if (start_node.first_child()) {
        pugi::xml_node child_ref = start_node.first_child();
        pugi::xml_node ic2 = start_node.insert_copy_after(child_ref, child_ref);
        (void)ic2;
    }

    // 6. Test previous_sibling with name
    pugi::xml_node ps1 = start_node.previous_sibling(name_view);
    (void)ps1;

    // 6. Test previous_sibling without name
    pugi::xml_node ps2 = start_node.previous_sibling();
    (void)ps2;

    // Additional exploration: iterate siblings
    for (pugi::xml_node n = start_node.first_child(); n; n = n.next_sibling()) {
        (void)n.child(name_view);
        (void)n.last_child();
        (void)n.previous_sibling(name_view);
        (void)n.next_sibling(name_view);
        (void)n.append_child(name_view);
    }

    return 0;
}