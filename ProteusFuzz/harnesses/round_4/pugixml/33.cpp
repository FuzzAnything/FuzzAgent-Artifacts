// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_attribute::set_value at pugixml.cpp:5576:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5569:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
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
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

static pugi::xml_document* create_document_from_input(const uint8_t* data, size_t size) {
    if (size == 0) {
        return nullptr;
    }
    
    // Create a simple XML document with a root and a child to ensure we have nodes to work with
    // We use a fixed template and try to inject some fuzz data as text content if possible
    std::string xml_str = "<root><child/></root>";
    
    // If input is large enough, try to append some data as text content to vary the document
    if (size > 10) {
        // Take a portion of input, ensure it's safe (replace potential problematic chars)
        std::string safe_data;
        safe_data.reserve(size);
        for (size_t i = 0; i < size && i < 1000; ++i) {
            uint8_t c = data[i];
            if (c == '<' || c == '>' || c == '&' || c == '"' || c == '\'') {
                safe_data += '_';
            } else {
                safe_data += static_cast<char>(c);
            }
        }
        xml_str = "<root><child>" + safe_data + "</child></root>";
    }
    
    pugi::xml_document* doc = new pugi::xml_document();
    if (!doc->load_string(xml_str.c_str())) {
        delete doc;
        return nullptr;
    }
    return doc;
}

static void cleanup_document(pugi::xml_document* doc) {
    if (doc) {
        delete doc;
    }
}

extern "C" int LLVMFuzzerTestOneInput_33(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document* doc = create_document_from_input(Data, Size);
    if (!doc) {
        return 0;
    }

    pugi::xml_node root = doc->document_element();
    if (!root) {
        cleanup_document(doc);
        return 0;
    }

    // Step 1: pugi::xml_node::child
    // We try to get the "child" node which we know exists in our template
    pugi::xml_node child_node = root.child("child");
    if (!child_node) {
        // If child not found, try to create it or use root
        child_node = root;
    }

    // Step 2: pugi::xml_node::append_attribute (first call)
    pugi::xml_attribute attr1 = child_node.append_attribute("attr1");
    if (!attr1) {
        cleanup_document(doc);
        return 0;
    }

    // Step 3: pugi::xml_node::append_attribute (second call)
    pugi::xml_attribute attr2 = child_node.append_attribute("attr2");
    if (!attr2) {
        cleanup_document(doc);
        return 0;
    }

    // Step 4: pugi::xml_attribute::set_value (unsigned long long overload)
    // Use data from input to generate a value
    unsigned long long val_ull = 0;
    if (Size >= 8) {
        val_ull = 0;
        for (int i = 0; i < 8; ++i) {
            val_ull = (val_ull << 8) | Data[i];
        }
    } else if (Size > 0) {
        val_ull = Data[0];
    }
    attr1.set_value(val_ull);

    // Step 5: pugi::xml_node::append_attribute (third call)
    pugi::xml_attribute attr3 = child_node.append_attribute("attr3");
    if (!attr3) {
        cleanup_document(doc);
        return 0;
    }

    // Step 6: pugi::xml_node::append_attribute (fourth call)
    pugi::xml_attribute attr4 = child_node.append_attribute("attr4");
    if (!attr4) {
        cleanup_document(doc);
        return 0;
    }

    // Step 7: pugi::xml_attribute::set_value (long long overload)
    // Use data from input to generate a value
    long long val_ll = 0;
    if (Size >= 8) {
        val_ll = 0;
        for (int i = 0; i < 8; ++i) {
            val_ll = (val_ll << 8) | Data[i];
        }
    } else if (Size > 0) {
        val_ll = static_cast<int8_t>(Data[0]);
    }
    attr2.set_value(val_ll);

    // Cleanup
    cleanup_document(doc);
    return 0;
}