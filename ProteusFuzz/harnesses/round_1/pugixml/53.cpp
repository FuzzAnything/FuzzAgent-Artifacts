// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_document::load_buffer_inplace at pugixml.cpp:7756:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
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
#include <cstddef>
#include <cstring>
#include <vector>

static int fuzz(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    // Prepare buffer for in-place parsing
    std::vector<char> buffer(Size);
    std::memcpy(buffer.data(), Data, Size);
    
    pugi::xml_document doc;
    
    // 1. pugi::xml_document::load_buffer_inplace
    pugi::xml_parse_result result = doc.load_buffer_inplace(buffer.data(), Size);
    
    // If parsing failed, we can still try to proceed with an empty document structure
    // but many operations might return empty nodes. This is expected behavior for fuzzing.
    
    pugi::xml_node root = doc;
    
    // 2. pugi::xml_node::append_child
    pugi::xml_node child1 = root.append_child("test_element");
    
    // 3. pugi::xml_node::append_copy (requires a source attribute to copy)
    // Since we just created a new node, it has no attributes yet.
    // We try to append a copy of an empty attribute, which should return empty.
    pugi::xml_attribute attr_empty;
    pugi::xml_attribute copied_attr = child1.append_copy(attr_empty);
    
    // 4. pugi::xml_node::first_child
    pugi::xml_node first_child = root.first_child();
    
    // 5. pugi::xml_node::first_attribute
    pugi::xml_attribute first_attr = first_child.first_attribute();
    
    // 6. pugi::xml_node::append_child (on the first child if it exists)
    if (first_child) {
        pugi::xml_node child2 = first_child.append_child("nested_element");
        
        // 7. pugi::xml_node::append_copy (on the nested element)
        // Again, copying an empty attribute
        pugi::xml_attribute copied_attr2 = child2.append_copy(attr_empty);
        
        // 8. pugi::xml_node::first_attribute (on the nested element)
        pugi::xml_attribute first_attr2 = child2.first_attribute();
    }
    
    return 0;
}

extern "C" int LLVMFuzzerTestOneInput_53(const uint8_t *Data, size_t Size) {
    return fuzz(Data, Size);
}