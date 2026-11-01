// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6431:34 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6360:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5769:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5998:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_move_before at pugixml.cpp:6517:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::last_child at pugixml.cpp:5970:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
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

extern "C" int LLVMFuzzerTestOneInput_72(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    if (!doc.load_buffer(Data, Size)) {
        // If parsing fails, try to create a minimal document manually to test APIs
        doc.append_child(pugi::node_element);
        if (doc.root().empty()) {
            return 0;
        }
    }

    pugi::xml_node root = doc.root();
    if (root.empty()) {
        root = doc;
    }

    // Extract potential names from the input data to use in API calls
    std::string_view input_view(reinterpret_cast<const char*>(Data), Size);
    std::string_view name1 = input_view.substr(0, std::min(Size, static_cast<size_t>(32)));
    std::string_view name2 = input_view.substr(std::min(Size, static_cast<size_t>(10)), std::min(Size - std::min(Size, static_cast<size_t>(10)), static_cast<size_t>(32)));
    std::string_view name3 = input_view.substr(Size > 20 ? 20 : 0, std::min(Size - (Size > 20 ? 20 : 0), static_cast<size_t>(32)));

    // 1. Test set_name
    // Ensure we have a valid element node to rename
    pugi::xml_node target_node = root;
    if (target_node.type() != pugi::node_element) {
        target_node = root.append_child(pugi::node_element);
        if (target_node.empty()) {
            target_node = root;
        }
    }
    // Convert string_view to const char* for set_name if needed, but pugi accepts string_view_t
    // Note: In older pugixml versions string_view_t might be const char_t*, assuming modern version with std::string_view support or compatible typedef
    target_node.set_name(name1);

    // 2. Test append_child
    pugi::xml_node new_child = root.append_child(name2);
    if (!new_child.empty()) {
        new_child.set_value("test_value");
    }

    // 3. Test child
    pugi::xml_node found_child = root.child(name3);
    (void)found_child; // Suppress unused warning

    // 4. Test last_child
    pugi::xml_node last = root.last_child();
    (void)last;

    // 5. Test insert_move_before
    // Need two valid nodes in the same document
    pugi::xml_node node_a = root.child(name2);
    if (node_a.empty()) {
        node_a = root.append_child(pugi::node_element);
        if (!node_a.empty()) node_a.set_name(name2);
    }
    
    pugi::xml_node node_b = root.last_child();
    if (node_b.empty()) {
        node_b = root.append_child(pugi::node_element);
    }

    if (!node_a.empty() && !node_b.empty() && node_a != node_b) {
        node_b.insert_move_before(node_a, node_b);
    }

    // 6. Test insert_copy_after
    // The signature provided is xml_node insert_copy_after(const xml_node& proto, const xml_node& node);
    // In pugixml, this function typically copies the node 'proto' and inserts it after 'node'.
    // The prompt description mentions attributes, but the signature takes xml_node.
    // We will use element nodes to satisfy the signature and avoid constructor errors.
    
    pugi::xml_node proto_node = root.append_child(pugi::node_element);
    if (!proto_node.empty()) {
        proto_node.set_name("proto_elem");
        proto_node.set_value("proto_value");
    }
    
    pugi::xml_node target_node_wrapper = root.last_child();
    if (target_node_wrapper.empty()) {
        target_node_wrapper = root.append_child(pugi::node_element);
    }

    if (!proto_node.empty() && !target_node_wrapper.empty()) {
        // Attempt to copy the node after the target node
        root.insert_copy_after(proto_node, target_node_wrapper);
    }

    return 0;
}