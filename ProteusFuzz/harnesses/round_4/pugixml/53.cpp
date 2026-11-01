// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
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
#include <cstdlib>

static const char* dummy_file = "./dummy_file";

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure minimum size for a valid XML-like structure
    if (Size < 4) {
        return 0;
    }

    // Create a buffer that persists for the lifetime of the document
    // We allocate slightly more to handle potential internal modifications
    size_t alloc_size = Size + 16;
    char* buffer = static_cast<char*>(std::malloc(alloc_size));
    if (!buffer) {
        return 0;
    }

    // Copy input data to buffer and ensure null termination for safety
    std::memcpy(buffer, Data, Size);
    buffer[Size] = '\0';

    pugi::xml_document doc;
    
    // 1. pugi::xml_document::load_buffer_inplace
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size, pugi::parse_default, pugi::encoding_auto);
    
    // Proceed even if parsing failed to test error handling paths, 
    // but we need a root to work with for subsequent calls.
    pugi::xml_node root = doc.child("root");
    if (!root) {
        // If no root found, try to get document element or create one
        root = doc.document_element();
        if (!root) {
            // Attempt to append a root if the document is empty or invalid
            root = doc.append_child("root");
        }
    }

    if (!root) {
        // If we still don't have a valid node, clean up and exit
        std::free(buffer);
        return 0;
    }

    // 2. pugi::xml_node::append_child
    // Use a static name to avoid buffer issues, or derive from input if safe
    pugi::xml_node child1 = root.append_child("child1");

    // 3. pugi::xml_node::append_copy
    // We need an attribute to copy. Let's add one first.
    pugi::xml_attribute attr1 = child1.append_attribute("attr1");
    attr1.set_value("value1");
    
    // Now copy the attribute to the same node or another
    if (child1) {
        pugi::xml_attribute copied_attr = child1.append_copy(attr1);
        (void)copied_attr; // Suppress unused warning
    }

    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    (void)first_child; // Suppress unused warning

    // 5. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = root.first_attribute();
    (void)first_attr; // Suppress unused warning

    // 6. pugi::xml_node::append_child (again)
    pugi::xml_node child2 = root.append_child("child2");

    // 7. pugi::xml_node::append_copy (again)
    if (child2) {
        pugi::xml_attribute attr2 = child2.append_attribute("attr2");
        attr2.set_value("value2");
        pugi::xml_attribute copied_attr2 = child2.append_copy(attr2);
        (void)copied_attr2;
    }

    // 8. pugi::xml_node::first_attribute (again)
    pugi::xml_attribute first_attr2 = child2.first_attribute();
    (void)first_attr2;

    // Cleanup: free the buffer allocated for load_buffer_inplace
    std::free(buffer);

    return 0;
}