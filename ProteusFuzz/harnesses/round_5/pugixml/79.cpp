// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::last_attribute at pugixml.cpp:5957:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::name at pugixml.cpp:5365:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_attribute::value at pugixml.cpp:5372:44 in pugixml.hpp
// pugi::xml_node::find_attribute at pugixml.hpp:681:47 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6146:39 in pugixml.hpp
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
#include <string>
#include <vector>
#include <functional>

static void write_dummy_file(const uint8_t* data, size_t size) {
    // Placeholder for file I/O if required, though we parse in-memory.
    (void)data;
    (void)size;
}

extern "C" int LLVMFuzzerTestOneInput_79(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return 0;
    }

    pugi::xml_document doc;
    
    // Parse the input data as XML.
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    pugi::xml_node root = doc.root();
    if (!root) {
        root = doc.append_child("root");
    }

    // Pre-populate the node with some attributes to ensure we have references
    if (Size > 4) {
        std::string attrName1(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(10)));
        std::string attrName2(reinterpret_cast<const char*>(Data + 1), std::min(Size - 1, static_cast<size_t>(10)));
        
        if (attrName1.empty()) attrName1 = "attr1";
        if (attrName2.empty()) attrName2 = "attr2";

        root.append_attribute(attrName1.c_str()).set_value("val1");
        root.append_attribute(attrName2.c_str()).set_value("val2");
    }

    // 1. Test insert_attribute_before
    {
        pugi::xml_attribute ref_attr = root.first_attribute();
        if (ref_attr) {
            std::string_view new_name(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(20)));
            size_t null_pos = new_name.find('\0');
            if (null_pos != std::string_view::npos) {
                new_name = new_name.substr(0, null_pos);
            }
            if (new_name.empty()) new_name = "new_attr_before";
            
            pugi::xml_attribute new_attr = root.insert_attribute_before(new_name, ref_attr);
            (void)new_attr;
        }
        
        // Test with invalid reference
        pugi::xml_attribute empty_attr;
        std::string_view dummy_name = "dummy";
        root.insert_attribute_before(dummy_name, empty_attr);
    }

    // 2. Test insert_attribute_after
    {
        pugi::xml_attribute ref_attr = root.first_attribute();
        if (ref_attr) {
            std::string_view new_name(reinterpret_cast<const char*>(Data + 1), std::min(Size > 1 ? Size - 1 : 0, static_cast<size_t>(20)));
            size_t null_pos = new_name.find('\0');
            if (null_pos != std::string_view::npos) {
                new_name = new_name.substr(0, null_pos);
            }
            if (new_name.empty()) new_name = "new_attr_after";
            
            pugi::xml_attribute new_attr = root.insert_attribute_after(new_name, ref_attr);
            (void)new_attr;
        }
        
        // Test with invalid reference
        pugi::xml_attribute empty_attr;
        std::string_view dummy_name = "dummy";
        root.insert_attribute_after(dummy_name, empty_attr);
    }

    // 3. Test attribute
    {
        std::string_view name_view(reinterpret_cast<const char*>(Data), std::min(Size, static_cast<size_t>(20)));
        size_t null_pos = name_view.find('\0');
        if (null_pos != std::string_view::npos) {
            name_view = name_view.substr(0, null_pos);
        }
        if (name_view.empty()) name_view = "non_existent";
        
        std::string c_str(name_view);
        pugi::xml_attribute attr = root.attribute(c_str.c_str());
        (void)attr;
    }

    // 4. Test last_attribute
    {
        pugi::xml_attribute last = root.last_attribute();
        (void)last;
    }

    // 5. Test first_attribute
    {
        pugi::xml_attribute first = root.first_attribute();
        (void)first;
    }

    // 6. Test find_attribute with a predicate
    {
        // Predicate 1: Find attribute with name starting with 'a'
        root.find_attribute([](pugi::xml_attribute attr) {
            if (!attr) return false;
            const pugi::char_t* name = attr.name();
            return name && name[0] == 'a';
        });

        // Predicate 2: Find attribute with value length > 0
        root.find_attribute([](pugi::xml_attribute attr) {
            if (!attr) return false;
            const pugi::char_t* val = attr.value();
            return val && val[0] != '\0';
        });

        // Predicate 3: Always false
        root.find_attribute([](pugi::xml_attribute attr) {
            (void)attr;
            return false;
        });
    }

    return 0;
}