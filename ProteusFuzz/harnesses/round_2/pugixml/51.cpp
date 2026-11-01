// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_element_by_path at pugixml.cpp:6748:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::set_value at pugixml.cpp:6009:30 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7770:34 in pugixml.hpp
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

static void write_dummy_file(const uint8_t* data, size_t size) {
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        fwrite(data, 1, size, f);
        fclose(f);
    }
}

extern "C" int LLVMFuzzerTestOneInput_51(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Load data if it looks like XML, otherwise create a minimal document
    if (Size > 4 && (Data[0] == '<' || Data[0] == 0xEF)) {
        // Try to load as UTF-8
        if (!doc.load_string(reinterpret_cast<const char*>(Data))) {
            // If load fails, create a minimal document
            doc.append_child(pugi::node_element);
        }
    } else {
        // Create a minimal document with a root element
        doc.append_child(pugi::node_element);
    }

    pugi::xml_node root = doc.child("root");
    if (!root) {
        // Get the first child element if "root" doesn't exist
        root = doc.first_element_by_path("/");
        if (!root) {
            // Fallback: create a new element if document is empty
            root = doc.append_child(pugi::node_element);
        }
    }

    // 1. pugi::xml_node::insert_child_before
    pugi::xml_node sibling = root.first_child();
    if (!sibling) {
        // Create a sibling if none exists
        sibling = root.append_child(pugi::node_element);
    }
    pugi::xml_node new_child = root.insert_child_before(pugi::node_element, sibling);

    // 2. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();

    // 3. pugi::xml_node::append_attribute
    pugi::xml_attribute attr = first.append_attribute("fuzz_attr");
    if (attr) {
        attr.set_value("fuzz_value");
    }

    // 4. pugi::xml_node::insert_child_before (again)
    pugi::xml_node sibling2 = first.first_child();
    if (!sibling2) {
        sibling2 = first.append_child(pugi::node_pcdata);
    }
    pugi::xml_node new_child2 = first.insert_child_before(pugi::node_pcdata, sibling2);

    // 5. pugi::xml_node::first_child (again)
    pugi::xml_node first2 = first.first_child();

    // 6. pugi::xml_node::set_value
    if (!first2.empty() && (first2.type() == pugi::node_pcdata || first2.type() == pugi::node_cdata || 
        first2.type() == pugi::node_comment || first2.type() == pugi::node_pi)) {
        first2.set_value("fuzzed_text");
    } else if (!first.empty()) {
        // If not a text node, try to set value on a text node child or create one
        pugi::xml_node text_node = first.append_child(pugi::node_pcdata);
        text_node.set_value("fuzzed_text");
    }

    // 7. pugi::xml_document::save
    // Write input to dummy file first to ensure it exists
    write_dummy_file(Data, Size);
    
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        doc.save(writer, "\t", pugi::format_write_bom, pugi::encoding_utf8);
        fclose(f);
    }

    // Cleanup is handled by RAII (xml_document destructor)
    return 0;
}