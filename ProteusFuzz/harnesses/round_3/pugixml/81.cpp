// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5489:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <pugixml.hpp>

static void fuzz_insert_copy_after(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_attribute attr1 = root.append_attribute("attr1");
    attr1.set_value("value1");
    
    pugi::xml_attribute attr2 = root.append_attribute("attr2");
    attr2.set_value("value2");
    
    pugi::xml_attribute result = root.insert_copy_after(attr1, attr2);
    
    pugi::xml_attribute empty_attr;
    pugi::xml_attribute result2 = root.insert_copy_after(empty_attr, attr2);
    pugi::xml_attribute result3 = root.insert_copy_after(attr1, empty_attr);
    
    if (size > 10) {
        size_t len = 0;
        while (len < size - 1 && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        
        pugi::xml_attribute new_attr = root.append_attribute(name.c_str());
        size_t val_len = 0;
        size_t offset = 10;
        if (offset < size) {
            while (offset + val_len < size && data[offset + val_len] != '\0') {
                val_len++;
            }
            new_attr.set_value(reinterpret_cast<const char*>(data + offset), val_len);
        }
        
        if (root.attribute("attr1") && root.attribute("attr2")) {
            root.insert_copy_after(root.attribute("attr1"), root.attribute("attr2"));
        }
    }
}

static void fuzz_remove_attribute(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    root.append_attribute("id").set_value("123");
    root.append_attribute("name").set_value("test");
    
    bool removed = root.remove_attribute("id");
    bool not_removed = root.remove_attribute("nonexistent");
    
    if (size > 5) {
        size_t len = 0;
        while (len < size && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        root.remove_attribute(name.c_str());
    }
    
    root.remove_attribute("");
}

static void fuzz_insert_copy_before(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_attribute attr1 = root.append_attribute("first");
    attr1.set_value("1");
    
    pugi::xml_attribute attr2 = root.append_attribute("second");
    attr2.set_value("2");
    
    pugi::xml_attribute result = root.insert_copy_before(attr1, attr2);
    
    pugi::xml_attribute empty_attr;
    pugi::xml_attribute result2 = root.insert_copy_before(empty_attr, attr2);
    pugi::xml_attribute result3 = root.insert_copy_before(attr1, empty_attr);
    
    if (size > 10) {
        size_t len = 0;
        while (len < size - 1 && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        
        pugi::xml_attribute new_attr = root.append_attribute(name.c_str());
        size_t val_len = 0;
        size_t offset = 10;
        if (offset < size) {
            while (offset + val_len < size && data[offset + val_len] != '\0') {
                val_len++;
            }
            new_attr.set_value(reinterpret_cast<const char*>(data + offset), val_len);
        }
        
        if (root.attribute("first") && root.attribute("second")) {
            root.insert_copy_before(root.attribute("first"), root.attribute("second"));
        }
    }
}

static void fuzz_remove_child(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    root.append_child("child1").append_attribute("id").set_value("1");
    root.append_child("child2").append_attribute("id").set_value("2");
    root.append_child("child1").append_attribute("id").set_value("3");
    
    bool removed = root.remove_child("child1");
    bool not_removed = root.remove_child("nonexistent");
    
    if (size > 5) {
        size_t len = 0;
        while (len < size && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        root.remove_child(name.c_str());
    }
    
    root.remove_child("");
}

static void fuzz_insert_attribute_before(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 2) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_attribute ref_attr = root.append_attribute("ref");
    ref_attr.set_value("reference");
    
    pugi::xml_attribute result = root.insert_attribute_before("new_attr", ref_attr);
    if (result) {
        result.set_value("new_value");
    }
    
    pugi::xml_attribute empty_attr;
    pugi::xml_attribute result2 = root.insert_attribute_before("another", empty_attr);
    
    if (size > 5) {
        size_t len = 0;
        while (len < size && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        
        if (!name.empty()) {
            pugi::xml_attribute new_attr = root.insert_attribute_before(name.c_str(), ref_attr);
            if (new_attr) {
                size_t val_len = 0;
                size_t offset = 5;
                if (offset < size) {
                    while (offset + val_len < size && data[offset + val_len] != '\0') {
                        val_len++;
                    }
                    new_attr.set_value(reinterpret_cast<const char*>(data + offset), val_len);
                }
            }
        }
    }
    
    pugi::xml_attribute result3 = root.insert_attribute_before("", ref_attr);
}

static void fuzz_prepend_copy(pugi::xml_document& doc, const uint8_t* data, size_t size) {
    if (size < 1) return;
    
    pugi::xml_node root = doc.append_child("root");
    
    pugi::xml_attribute attr = root.append_attribute("source");
    attr.set_value("source_value");
    
    pugi::xml_attribute result = root.prepend_copy(attr);
    
    pugi::xml_attribute empty_attr;
    pugi::xml_attribute result2 = root.prepend_copy(empty_attr);
    
    if (size > 10) {
        size_t len = 0;
        while (len < size - 1 && data[len] != '\0') {
            len++;
        }
        std::string name(reinterpret_cast<const char*>(data), len);
        
        pugi::xml_attribute new_attr = root.append_attribute(name.c_str());
        size_t val_len = 0;
        size_t offset = 10;
        if (offset < size) {
            while (offset + val_len < size && data[offset + val_len] != '\0') {
                val_len++;
            }
            new_attr.set_value(reinterpret_cast<const char*>(data + offset), val_len);
        }
        
        if (new_attr) {
            root.prepend_copy(new_attr);
        }
    }
}

extern "C" int LLVMFuzzerTestOneInput_81(const uint8_t *Data, size_t Size) {
    if (Size == 0) return 0;
    
    pugi::xml_document doc;
    
    fuzz_insert_copy_after(doc, Data, Size);
    fuzz_remove_attribute(doc, Data, Size);
    fuzz_insert_copy_before(doc, Data, Size);
    fuzz_remove_child(doc, Data, Size);
    fuzz_insert_attribute_before(doc, Data, Size);
    fuzz_prepend_copy(doc, Data, Size);
    
    return 0;
}