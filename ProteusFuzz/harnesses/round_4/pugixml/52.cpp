// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::insert_child_before at pugixml.cpp:6287:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
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
#include <cstdint>
#include <cstddef>
#include <fstream>
#include <sstream>
#include <string>

static void write_dummy_file(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data), size);
        file.close();
    }
}

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare environment
    pugi::xml_document doc;
    
    // Create initial structure to allow insertions
    pugi::xml_node root = doc.append_child(pugi::node_element);
    if (!root) {
        return 0;
    }
    
    // Create a sibling to insert before
    pugi::xml_node sibling = root.append_child(pugi::node_element);
    if (!sibling) {
        return 0;
    }

    // 1. pugi::xml_node::insert_child_before
    // Insert a new child before the sibling
    pugi::xml_node new_node = root.insert_child_before(pugi::node_element, sibling);
    if (!new_node) {
        // Try inserting other node types if element fails
        new_node = root.insert_child_before(pugi::node_pcdata, sibling);
        if (!new_node) {
            new_node = root.insert_child_before(pugi::node_comment, sibling);
        }
    }

    // 2. pugi::xml_node::first_child
    pugi::xml_node first = root.first_child();
    (void)first; // Suppress unused variable warning

    // 3. pugi::xml_node::append_attribute
    pugi::xml_attribute attr = root.append_attribute("test_attr");
    if (attr) {
        // Set a value based on input data if possible
        if (Size > 0) {
            std::string val(reinterpret_cast<const char*>(Data), std::min(Size, size_t(100)));
            attr.set_value(val.c_str());
        }
    }

    // 4. pugi::xml_node::insert_child_before (Second invocation)
    // Try inserting before the first child if it exists
    pugi::xml_node target_sibling = root.first_child();
    if (target_sibling) {
        pugi::xml_node inserted = root.insert_child_before(pugi::node_cdata, target_sibling);
        (void)inserted;
    } else {
        // Fallback if no children
        root.insert_child_before(pugi::node_pcdata, sibling);
    }

    // 5. pugi::xml_node::first_child (Second invocation)
    pugi::xml_node first_again = root.first_child();
    if (first_again) {
        // 6. pugi::xml_node::set_value
        // Only set value if the node type supports it (not element)
        if (first_again.type() != pugi::node_element) {
            std::string val(reinterpret_cast<const char*>(Data), std::min(Size, size_t(100)));
            first_again.set_value(val.c_str());
        }
    }

    // 7. pugi::xml_document::save
    // Write to a file using the dummy file path
    write_dummy_file(Data, Size);
    FILE* f = fopen("./dummy_file", "wb");
    if (f) {
        pugi::xml_writer_file writer(f);
        doc.save(writer, "\t", pugi::format_write_bom, pugi::encoding_utf8);
        fclose(f);
    }

    // Cleanup is handled by RAII (doc destructor)
    return 0;
}