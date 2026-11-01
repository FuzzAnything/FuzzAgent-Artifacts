// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::text at pugixml.cpp:5923:34 in pugixml.hpp
// pugi::xml_text::set at pugixml.cpp:7047:30 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
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
#include <string>
#include <string_view>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc) return nullptr;

    // If data is empty or too small, create a minimal valid XML
    if (size == 0) {
        if (!doc->load_string("<root/>")) {
            delete doc;
            return nullptr;
        }
    } else {
        // Try to load as XML, if fails, wrap in root to ensure validity
        std::string str(reinterpret_cast<const char*>(data), size);
        if (!doc->load_string(str.c_str())) {
            // Fallback: create a simple document and try to parse content as text
            doc->reset();
            if (!doc->load_string("<root/>")) {
                delete doc;
                return nullptr;
            }
            // Attempt to load original data as text content if it was invalid XML
            pugi::xml_node root = doc->document_element();
            if (root) {
                root.text().set(reinterpret_cast<const char*>(data), size);
            }
        }
    }
    return doc;
}

extern "C" int LLVMFuzzerTestOneInput_85(const uint8_t* Data, size_t Size) {
    if (Size == 0) return 0;

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) return 0;

    pugi::xml_node root = doc->document_element();
    if (!root) {
        delete doc;
        return 0;
    }

    // Helper to generate attribute names from input data
    auto get_attr_name = [&](size_t offset, size_t len) -> const char* {
        if (offset >= Size) return "attr";
        size_t actual_len = (offset + len > Size) ? (Size - offset) : len;
        // Ensure null termination and valid chars
        static char buf[256];
        size_t copy_len = (actual_len > 255) ? 255 : actual_len;
        memcpy(buf, Data + offset, copy_len);
        buf[copy_len] = '\0';
        // Sanitize: replace non-printable chars
        for (size_t i = 0; i < copy_len; ++i) {
            if (buf[i] < 32 || buf[i] > 126) buf[i] = 'x';
        }
        return buf;
    };

    // Create initial attributes to use as references
    pugi::xml_attribute attr1 = root.append_attribute(get_attr_name(0, 10));
    pugi::xml_attribute attr2 = root.append_attribute(get_attr_name(10, 10));
    pugi::xml_attribute attr3 = root.append_attribute(get_attr_name(20, 10));

    // Test 1: insert_attribute_before
    {
        const char* name = get_attr_name(30, 10);
        pugi::xml_attribute ref = (Size % 3 == 0) ? attr1 : (Size % 3 == 1) ? attr2 : attr3;
        if (!ref) ref = attr1; // Fallback
        root.insert_attribute_before(name, ref);
    }

    // Test 2: attribute with hint
    {
        pugi::xml_attribute hint;
        const char* name = get_attr_name(40, 10);
        root.attribute(name, hint);
        // Try again with updated hint
        root.attribute(name, hint);
    }

    // Test 3: insert_attribute_after
    {
        const char* name = get_attr_name(50, 10);
        pugi::xml_attribute ref = (Size % 3 == 0) ? attr1 : (Size % 3 == 1) ? attr2 : attr3;
        if (!ref) ref = attr1;
        root.insert_attribute_after(name, ref);
    }

    // Test 4: insert_copy_after
    {
        pugi::xml_attribute proto = (Size % 3 == 0) ? attr1 : (Size % 3 == 1) ? attr2 : attr3;
        pugi::xml_attribute after = (Size % 3 == 0) ? attr2 : (Size % 3 == 1) ? attr3 : attr1;
        if (!proto || !after) {
            proto = attr1;
            after = attr2;
        }
        root.insert_copy_after(proto, after);
    }

    // Test 5: insert_copy_before
    {
        pugi::xml_attribute proto = (Size % 3 == 0) ? attr1 : (Size % 3 == 1) ? attr2 : attr3;
        pugi::xml_attribute before = (Size % 3 == 0) ? attr2 : (Size % 3 == 1) ? attr3 : attr1;
        if (!proto || !before) {
            proto = attr1;
            before = attr2;
        }
        root.insert_copy_before(proto, before);
    }

    // Test 6: prepend_attribute
    {
        const char* name = get_attr_name(60, 10);
        root.prepend_attribute(name);
    }

    // Additional edge cases: invalid references
    {
        pugi::xml_attribute invalid_attr;
        root.insert_attribute_before("test", invalid_attr);
        root.insert_attribute_after("test", invalid_attr);
        root.insert_copy_after(invalid_attr, attr1);
        root.insert_copy_before(invalid_attr, attr1);
        root.insert_copy_after(attr1, invalid_attr);
        root.insert_copy_before(attr1, invalid_attr);
    }

    // Test with text node (which cannot have attributes)
    // Note: xml_text does not have insert_attribute methods, so we skip those calls.
    // We can still test attribute retrieval on text nodes if needed, but insertion is invalid.
    pugi::xml_text text_node = root.text();
    if (text_node) {
        // These operations are not supported on xml_text, so we do not call them.
        // The library handles this by not exposing these methods on xml_text.
    }

    delete doc;
    return 0;
}