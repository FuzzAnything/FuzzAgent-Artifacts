// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::type at pugixml.cpp:5695:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
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

static bool try_parse_document(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size == 0) {
        doc.load_string("<root/>");
        return true;
    }
    std::string buffer(reinterpret_cast<const char*>(data), size);
    return doc.load_string(buffer.c_str());
}

extern "C" int LLVMFuzzerTestOneInput_67(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    if (!try_parse_document(doc, Data, Size)) {
        doc.load_string("<root/>");
    }

    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }

    if (root.type() != pugi::node_element) {
        pugi::xml_node element = root.first_child();
        if (!element || element.type() != pugi::node_element) {
            element = doc.append_child("test_element");
        }
        root = element;
    }

    std::vector<std::string> names;
    const char* raw_data = reinterpret_cast<const char*>(Data);
    size_t max_names = Size > 100 ? 100 : Size;
    
    for (size_t i = 0; i < max_names; ++i) {
        size_t len = 0;
        while (i + len < Size && raw_data[i + len] != '\0' && len < 100) {
            len++;
        }
        if (len > 0) {
            names.push_back(std::string(raw_data + i, len));
            i += len;
        } else {
            names.push_back("attr");
        }
    }

    if (names.empty()) {
        names.push_back("attr");
    }

    // 1. pugi::xml_node::prepend_attribute
    for (const auto& name : names) {
        pugi::xml_attribute attr = root.prepend_attribute(name.c_str());
        if (attr) {
            attr.set_value("value");
        }
    }

    // 2. pugi::xml_node::insert_copy_after
    pugi::xml_attribute first_attr = root.attribute(names[0].c_str());
    if (!first_attr) {
        first_attr = root.prepend_attribute(names[0].c_str());
        first_attr.set_value("val1");
    }
    
    pugi::xml_attribute second_attr = root.attribute(names[0].c_str());
    if (first_attr && second_attr) {
        pugi::xml_attribute copied = root.insert_copy_after(first_attr, second_attr);
        if (copied) {
            copied.set_value("copied_val");
        }
    }

    // 3. pugi::xml_node::insert_attribute_before
    if (second_attr) {
        for (const auto& name : names) {
            pugi::xml_attribute new_attr = root.insert_attribute_before(name.c_str(), second_attr);
            if (new_attr) {
                new_attr.set_value("before_val");
            }
        }
    }

    // 4. pugi::xml_node::insert_attribute_after
    if (second_attr) {
        for (const auto& name : names) {
            pugi::xml_attribute new_attr = root.insert_attribute_after(name.c_str(), second_attr);
            if (new_attr) {
                new_attr.set_value("after_val");
            }
        }
    }

    // 5. pugi::xml_node::insert_copy_before
    if (first_attr && second_attr) {
        pugi::xml_attribute copied_before = root.insert_copy_before(first_attr, second_attr);
        if (copied_before) {
            copied_before.set_value("copied_before_val");
        }
    }

    // 6. pugi::xml_node::attribute (with hint)
    pugi::xml_attribute hint = pugi::xml_attribute();
    for (const auto& name : names) {
        pugi::xml_attribute found = root.attribute(name.c_str(), hint);
        if (found) {
            found.set_value("found_val");
        }
    }

    return 0;
}