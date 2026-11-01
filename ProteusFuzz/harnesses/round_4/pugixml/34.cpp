// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7084:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7077:30 in pugixml.hpp
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

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    std::string xml_str(reinterpret_cast<const char*>(data), size);
    // Ensure the input is valid XML by wrapping it if necessary or using a default
    // For fuzzing, we try to parse directly. If it fails, we create a minimal valid document.
    if (!doc->load_string(xml_str.c_str())) {
        // If parsing fails, create a minimal valid document to ensure we have a root to work with
        // pugixml xml_document does not have a clear() method in older versions or specific builds,
        // so we just append a root to the existing (potentially empty) document.
        doc->append_child("root");
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_34(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    try {
        // 1. pugi::xml_node::child
        // We need a node to call child on. Let's try to get a child named "root" from the document.
        pugi::xml_node node = doc->child("root");
        
        // If "root" wasn't found, try to get the first child of the document (which is the root element)
        if (!node) {
            node = doc->first_child();
        }

        if (!node) {
            // If we still don't have a valid node, append a root to the document to have something to work with
            node = doc->append_child("root");
        }

        // 2. pugi::xml_node::append_child (First call)
        pugi::xml_node child1 = node.append_child("child1");
        if (!child1) {
            delete doc;
            return 0;
        }

        // 3. pugi::xml_node::append_child (Second call)
        pugi::xml_node child2 = node.append_child("child2");
        if (!child2) {
            delete doc;
            return 0;
        }

        // 4. pugi::xml_text::set (First overload: unsigned long)
        // We need a text node. append_child returns an element node. 
        // We can get the text node of an element by calling text().
        pugi::xml_text text1 = child1.text();
        // Invoke set(unsigned long)
        text1.set(static_cast<unsigned long>(12345));

        // 5. pugi::xml_node::append_child (Third call)
        pugi::xml_node child3 = node.append_child("child3");
        if (!child3) {
            delete doc;
            return 0;
        }

        // 6. pugi::xml_node::append_child (Fourth call)
        pugi::xml_node child4 = node.append_child("child4");
        if (!child4) {
            delete doc;
            return 0;
        }

        // 7. pugi::xml_text::set (Second overload: long)
        pugi::xml_text text2 = child2.text();
        // Invoke set(long)
        text2.set(static_cast<long>(-67890));

        // Cleanup
        delete doc;
    } catch (...) {
        if (doc) delete doc;
        return 0;
    }

    return 0;
}