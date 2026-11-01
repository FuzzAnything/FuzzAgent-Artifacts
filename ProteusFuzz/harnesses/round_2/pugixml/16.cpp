// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_after at pugixml.cpp:6305:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::prepend_child at pugixml.cpp:6270:34 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::set_name at pugixml.cpp:5977:30 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstddef>

static void cleanup_document(pugi::xml_document& doc) {
    // pugixml xml_document does not have reset_memory. 
    // The document is automatically cleaned up when it goes out of scope or reset() is called.
    doc.reset();
}

extern "C" int LLVMFuzzerTestOneInput_16(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Initialize document
    // Try to load the fuzzed data. If it fails, create a minimal valid document to ensure we have a root.
    if (!doc.load_buffer(Data, Size)) {
        if (!doc.load_string("<root/>")) {
            return 0;
        }
    }

    pugi::xml_node root = doc.document_element();
    if (root.empty()) {
        // Ensure we have a root to work with
        pugi::xml_node new_root = doc.append_child(pugi::node_element);
        if (!new_root.empty()) {
            new_root.set_name("root");
            root = new_root;
        } else {
            return 0;
        }
    }

    // 1. append_child
    pugi::xml_node node1 = root.append_child(pugi::node_element);
    if (!node1.empty()) {
        node1.set_name("child1");
    }

    // 2. first_child
    pugi::xml_node first1 = root.first_child();
    
    // 3. first_child (again)
    pugi::xml_node first2 = root.first_child();

    // 4. insert_child_after
    // Need a sibling to insert after. Use node1 if it exists.
    pugi::xml_node node2;
    if (!node1.empty()) {
        node2 = root.insert_child_after(pugi::node_element, node1);
        if (!node2.empty()) {
            node2.set_name("child2");
        }
    } else {
        // Fallback if node1 failed
        node2 = root.append_child(pugi::node_element);
        if (!node2.empty()) {
            node2.set_name("child2");
        }
    }

    // 5. first_child
    pugi::xml_node first3 = root.first_child();

    // 6. insert_child_before
    // Need a sibling to insert before. Use node2 if it exists.
    pugi::xml_node node3;
    if (!node2.empty()) {
        node3 = root.insert_child_before(pugi::node_element, node2);
        if (!node3.empty()) {
            node3.set_name("child3");
        }
    } else {
        // Fallback
        node3 = root.append_child(pugi::node_element);
        if (!node3.empty()) {
            node3.set_name("child3");
        }
    }

    // 7. first_child
    pugi::xml_node first4 = root.first_child();

    // 8. prepend_child
    pugi::xml_node node4 = root.prepend_child(pugi::node_element);
    if (!node4.empty()) {
        node4.set_name("child4");
    }

    // Cleanup
    cleanup_document(doc);

    return 0;
}