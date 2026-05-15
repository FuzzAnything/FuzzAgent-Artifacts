// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_document::save at pugixml.cpp:7799:34 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

static void writeDummyFile(const uint8_t* data, size_t size) {
    std::ofstream file("./dummy_file", std::ios::binary);
    if (file) {
        file.write(reinterpret_cast<const char*>(data), size);
    }
}

static std::vector<std::string> splitData(const uint8_t* data, size_t size) {
    std::vector<std::string> parts;
    const char* start = reinterpret_cast<const char*>(data);
    const char* end = start + size;
    
    while (start < end) {
        const char* null_pos = start;
        while (null_pos < end && *null_pos != '\0') {
            ++null_pos;
        }
        
        parts.emplace_back(start, null_pos - start);
        start = null_pos + 1;
    }
    
    return parts;
}

extern "C" int LLVMFuzzerTestOneInput_100(const uint8_t* data, size_t size) {
    writeDummyFile(data, size);
    
    std::vector<std::string> parts = splitData(data, size);
    if (parts.empty()) {
        return 0;
    }
    
    try {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file("./dummy_file");
        
        if (!result) {
            doc.append_child("root");
        }
        
        pugi::xml_node root = doc.root();
        if (!root) {
            root = doc.append_child("root");
        }
        
        if (parts.size() < 2) {
            root.append_attribute("attr1");
            root.append_attribute("attr2");
        }
        
        if (!parts[0].empty()) {
            pugi::xml_attribute attr1 = root.attribute(parts[0].c_str());
            (void)attr1;
        }
        
        if (parts.size() > 1 && !parts[1].empty()) {
            std::string attr_name1(parts[1].data(), parts[1].length());
            pugi::xml_attribute new_attr = root.append_attribute(attr_name1.c_str());
            if (new_attr && parts.size() > 2 && !parts[2].empty()) {
                new_attr.set_value(parts[2].c_str());
            }
        }
        
        std::vector<pugi::xml_attribute> existing_attrs;
        for (pugi::xml_attribute attr = root.first_attribute(); attr; attr = attr.next_attribute()) {
            existing_attrs.push_back(attr);
        }
        
        if (parts.size() > 3 && !parts[3].empty() && !existing_attrs.empty()) {
            std::string attr_name2(parts[3].data(), parts[3].length());
            pugi::xml_attribute ref_attr = existing_attrs[0];
            pugi::xml_attribute inserted_after = root.insert_attribute_after(attr_name2.c_str(), ref_attr);
            (void)inserted_after;
        }
        
        if (parts.size() > 4 && !parts[4].empty() && !existing_attrs.empty()) {
            std::string attr_name3(parts[4].data(), parts[4].length());
            pugi::xml_attribute ref_attr = existing_attrs[0];
            pugi::xml_attribute inserted_before = root.insert_attribute_before(attr_name3.c_str(), ref_attr);
            (void)inserted_before;
        }
        
        if (parts.size() > 5 && existing_attrs.size() >= 2) {
            pugi::xml_attribute proto = existing_attrs[0];
            pugi::xml_attribute ref = existing_attrs[1];
            pugi::xml_attribute copied_before = root.insert_copy_before(proto, ref);
            (void)copied_before;
        }
        
        if (parts.size() > 6 && existing_attrs.size() >= 2) {
            pugi::xml_attribute proto = existing_attrs[0];
            pugi::xml_attribute ref = existing_attrs[1];
            pugi::xml_attribute copied_after = root.insert_copy_after(proto, ref);
            (void)copied_after;
        }
        
        std::ostringstream oss;
        doc.save(oss);
        
    } catch (...) {
    }
    
    return 0;
}