// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_attribute::set_name at pugixml.cpp:5474:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::empty at pugixml.cpp:5683:30 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5866:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
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

static bool try_parse_document(const uint8_t* data, size_t size, pugi::xml_document& doc) {
    if (size == 0) {
        return false;
    }
    // Ensure the data is null-terminated for string conversion if needed, 
    // but pugixml can parse from buffer directly.
    // We use load_buffer to avoid modifying the fuzz input buffer.
    pugi::xml_parse_result result = doc.load_buffer(data, size);
    return result.status == pugi::status_ok;
}

extern "C" int LLVMFuzzerTestOneInput_38(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Attempt to parse the input as XML
    // If parsing fails, we might still want to test with an empty document or skip
    // However, to trigger the specific API chain, we need a valid structure.
    // We will try to parse. If it fails, we return early or try to construct a minimal valid one.
    bool parsed = doc.load_buffer(Data, Size);
    
    // If parsing failed, we can try to load a minimal valid XML to ensure the API chain runs
    // This helps in exploring states even if the fuzzer generates garbage.
    if (!parsed) {
        // Fallback to a simple valid XML to ensure the test logic executes
        doc.load_string("<root><child attr1=\"val\" attr2=\"val\" attr3=\"val\" attr4=\"val\" attr5=\"val\" attr6=\"val\" attr7=\"val\" attr8=\"val\"/></root>");
    }

    // 1. pugi::xml_node::first_child
    pugi::xml_node root = doc.first_child();
    if (root.empty()) {
        // If no root, try to find any node or return
        // In a valid document, first_child should return the root element
        // If the document is empty, we can't proceed with the chain safely
        return 0;
    }

    // Get the first child of the root to traverse deeper if needed, 
    // but the prompt says invoke first_child. Let's assume we operate on the root or its child.
    // The chain implies: first_child -> attribute -> ...
    // Let's get the first child of the root. If root is the document, first_child is the root element.
    // If the input was <root>...</root>, doc.first_child() is <root>.
    // Let's try to get a child node to work on.
    pugi::xml_node node = root.first_child();
    
    // If node is empty (e.g. root has no children), we might need to use root itself or stop.
    // To maximize coverage, if node is empty, let's try to use 'root' as the node if it has attributes.
    if (node.empty()) {
        node = root;
    }

    if (node.empty()) {
        return 0;
    }

    // Prepare hint for the hint overload
    pugi::xml_attribute hint;
    hint = node.first_attribute();

    // 2. pugi::xml_node::attribute (overload 1: with hint)
    pugi::xml_attribute attr1 = node.attribute(pugi::string_view_t("attr1"), hint);
    
    // 3. pugi::xml_node::attribute (overload 2: without hint)
    pugi::xml_attribute attr2 = node.attribute("attr2");
    
    // 4. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute attr3 = node.attribute(pugi::string_view_t("attr3"), hint);
    
    // 5. pugi::xml_node::attribute (without hint)
    pugi::xml_attribute attr4 = node.attribute("attr4");
    
    // 6. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute attr5 = node.attribute(pugi::string_view_t("attr5"), hint);
    
    // 7. pugi::xml_node::attribute (without hint)
    pugi::xml_attribute attr6 = node.attribute("attr6");
    
    // 8. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute attr7 = node.attribute(pugi::string_view_t("attr7"), hint);
    
    // 9. pugi::xml_node::attribute (without hint)
    pugi::xml_attribute attr8 = node.attribute("attr8");

    // 10. pugi::xml_attribute::set_name
    // We need a valid attribute to set the name. 
    // If attr1 is valid, we try to rename it.
    // If not, we try others.
    if (attr1) {
        std::string_view new_name = "new_name_1";
        attr1.set_name(new_name);
    } else if (attr2) {
        std::string_view new_name = "new_name_2";
        attr2.set_name(new_name);
    } else if (attr3) {
        std::string_view new_name = "new_name_3";
        attr3.set_name(new_name);
    } else if (attr4) {
        std::string_view new_name = "new_name_4";
        attr4.set_name(new_name);
    } else if (attr5) {
        std::string_view new_name = "new_name_5";
        attr5.set_name(new_name);
    } else if (attr6) {
        std::string_view new_name = "new_name_6";
        attr6.set_name(new_name);
    } else if (attr7) {
        std::string_view new_name = "new_name_7";
        attr7.set_name(new_name);
    } else if (attr8) {
        std::string_view new_name = "new_name_8";
        attr8.set_name(new_name);
    }

    // 11. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute attr9 = node.attribute(pugi::string_view_t("new_name_1"), hint);
    
    // 12. pugi::xml_node::attribute (without hint)
    pugi::xml_attribute attr10 = node.attribute("new_name_2");
    
    // 13. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute attr11 = node.attribute(pugi::string_view_t("new_name_3"), hint);
    
    // 14. pugi::xml_node::attribute (without hint)
    pugi::xml_attribute attr12 = node.attribute("new_name_4");

    return 0;
}