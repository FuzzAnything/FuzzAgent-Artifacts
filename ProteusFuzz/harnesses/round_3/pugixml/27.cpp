// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6666:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_node::find_child_by_attribute at pugixml.cpp:6691:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
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
#include <cstdlib>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // Ensure we have a null-terminated string for parsing
    if (size == 0) {
        return doc;
    }

    // Find the first null terminator or use the whole buffer if none exists
    size_t null_pos = size;
    for (size_t i = 0; i < size; ++i) {
        if (data[i] == 0) {
            null_pos = i;
            break;
        }
    }

    // Create a safe buffer with guaranteed null termination
    char* safe_buffer = static_cast<char*>(malloc(null_pos + 1));
    if (!safe_buffer) {
        delete doc;
        return nullptr;
    }

    if (null_pos > 0) {
        std::memcpy(safe_buffer, data, null_pos);
    }
    safe_buffer[null_pos] = '\0';

    pugi::xml_parse_result result = doc->load_string(safe_buffer);
    free(safe_buffer);

    // If parsing failed, we still return the document but it might be empty
    // The fuzzer will test the API on the resulting structure regardless
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_27(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->document_element();
    if (root.empty()) {
        // If no root element, create one from the document node
        root = *doc;
    }

    // 1. append_child (type)
    pugi::xml_node child1 = root.append_child(pugi::node_element);
    // 2. append_child (type)
    pugi::xml_node child2 = root.append_child(pugi::node_element);
    // 3. append_child (type)
    pugi::xml_node child3 = root.append_child(pugi::node_element);
    // 4. append_child (type)
    pugi::xml_node child4 = root.append_child(pugi::node_element);

    // 5. append_child (name) - using a safe name derived from input
    // Ensure null termination for name
    const char* name_ptr = reinterpret_cast<const char*>(Data);
    size_t name_len = 0;
    while (name_len < Size && Data[name_len] != 0) {
        name_len++;
    }
    // Limit name length to avoid huge allocations
    if (name_len > 100) name_len = 100;
    char name_buf[101];
    std::memcpy(name_buf, name_ptr, name_len);
    name_buf[name_len] = '\0';
    
    pugi::xml_node child5 = root.append_child(name_buf);

    // 6. append_attribute
    pugi::xml_attribute attr1 = root.append_attribute(name_buf);
    // 7. append_attribute
    pugi::xml_attribute attr2 = root.append_attribute(name_buf);
    // 8. append_attribute
    pugi::xml_attribute attr3 = root.append_attribute(name_buf);

    // Prepare attribute value for find_child_by_attribute
    // Use the same name buffer or a substring if possible
    const char* attr_val = name_len > 0 ? name_buf : "default";

    // 9. find_child_by_attribute (name, attr, val)
    pugi::xml_node found1 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 10. find_child_by_attribute (name, attr, val)
    pugi::xml_node found2 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 11. find_child_by_attribute (name, attr, val)
    pugi::xml_node found3 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 12. find_child_by_attribute (name, attr, val)
    pugi::xml_node found4 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 13. find_child_by_attribute (name, attr, val)
    pugi::xml_node found5 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 14. find_child_by_attribute (name, attr, val)
    pugi::xml_node found6 = root.find_child_by_attribute(name_buf, name_buf, attr_val);
    // 15. find_child_by_attribute (name, attr, val)
    pugi::xml_node found7 = root.find_child_by_attribute(name_buf, name_buf, attr_val);

    // 16. find_child_by_attribute (attr, val) - overload without name
    pugi::xml_node found8 = root.find_child_by_attribute(name_buf, attr_val);
    // 17. find_child_by_attribute (attr, val)
    pugi::xml_node found9 = root.find_child_by_attribute(name_buf, attr_val);
    // 18. find_child_by_attribute (attr, val)
    pugi::xml_node found10 = root.find_child_by_attribute(name_buf, attr_val);
    // 19. find_child_by_attribute (attr, val)
    pugi::xml_node found11 = root.find_child_by_attribute(name_buf, attr_val);
    // 20. find_child_by_attribute (attr, val)
    pugi::xml_node found12 = root.find_child_by_attribute(name_buf, attr_val);
    // 21. find_child_by_attribute (attr, val)
    pugi::xml_node found13 = root.find_child_by_attribute(name_buf, attr_val);
    // 22. find_child_by_attribute (attr, val)
    pugi::xml_node found14 = root.find_child_by_attribute(name_buf, attr_val);
    // 23. find_child_by_attribute (attr, val)
    pugi::xml_node found15 = root.find_child_by_attribute(name_buf, attr_val);

    // Cleanup
    delete doc;

    return 0;
}