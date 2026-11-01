// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
#include <pugixml.hpp>
#include <cstdint>
#include <cstring>
#include <cstddef>

extern "C" int LLVMFuzzerTestOneInput_52(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Ensure minimum size for valid XML structure
    if (Size < 4) {
        return 0;
    }

    // Create a copy of the input data to use as a buffer for in-place parsing
    // We need a buffer that persists for the lifetime of the document
    char* buffer = new char[Size];
    std::memcpy(buffer, Data, Size);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer, Size, pugi::parse_default, pugi::encoding_auto);

    // If parsing failed, we can still try to exercise other paths if the document has a root
    // But for safety, if the document is completely empty, we clean up and return
    if (!doc.first_child()) {
        delete[] buffer;
        return 0;
    }

    // Step 2: pugi::xml_node::append_child
    pugi::xml_node root = doc.first_child();
    if (root) {
        pugi::xml_node new_child = root.append_child("fuzz_node");
        
        // Step 3: pugi::xml_node::append_copy (Attribute copy)
        // We need an attribute to copy. Let's try to get one from the root or the new child.
        pugi::xml_attribute attr_to_copy = root.first_attribute();
        if (attr_to_copy) {
            new_child.append_copy(attr_to_copy);
        } else {
            // If no attribute exists, we can't copy. 
            // We proceed to the next step regardless.
        }

        // Step 4: pugi::xml_node::first_child
        pugi::xml_node first_child_node = new_child.first_child();
        (void)first_child_node; // Suppress unused variable warning

        // Step 5: pugi::xml_node::first_attribute
        pugi::xml_attribute first_attr = new_child.first_attribute();
        (void)first_attr;

        // Step 6: pugi::xml_node::append_child (again)
        pugi::xml_node second_child = new_child.append_child("fuzz_node_2");

        // Step 7: pugi::xml_node::append_copy (again)
        // Try to copy an attribute from the second child or the parent
        pugi::xml_attribute attr_to_copy_2 = second_child.first_attribute();
        if (!attr_to_copy_2) {
            attr_to_copy_2 = new_child.first_attribute();
        }
        if (attr_to_copy_2) {
            second_child.append_copy(attr_to_copy_2);
        }

        // Step 8: pugi::xml_node::first_attribute (again)
        pugi::xml_attribute final_attr = second_child.first_attribute();
        (void)final_attr;
    }

    // Cleanup
    delete[] buffer;
    return 0;
}