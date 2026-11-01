// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::remove_child at pugixml.cpp:6582:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_document::load_string at pugixml.cpp:7712:46 in pugixml.hpp
// pugi::xml_node::root at pugixml.cpp:5918:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6535:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::remove_attribute at pugixml.cpp:6547:30 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
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
#include <pugixml.hpp>

static std::string generate_xml_from_fuzz_data(const uint8_t *Data, size_t Size) {
    if (Size == 0) {
        return "<root><child/><child2/></root>";
    }
    
    std::string result;
    result.reserve(Size + 100);
    
    result += "<root>";
    
    for (size_t i = 0; i < Size; ++i) {
        uint8_t byte = Data[i];
        if (byte == '<' || byte == '>' || byte == '&' || byte == '"' || byte == '\'') {
            result += "&";
            result += (char)byte;
            result += ";";
        } else if (byte >= 32 && byte < 127) {
            result += (char)byte;
        }
    }
    
    result += "<child attr1='value1' attr2='value2'/><child2 attr3='value3'/>";
    result += "</root>";
    
    return result;
}

extern "C" int LLVMFuzzerTestOneInput_76(const uint8_t *Data, size_t Size) {
    std::string xml_str = generate_xml_from_fuzz_data(Data, Size);
    
    pugi::xml_document doc;
    
    pugi::xml_parse_result result = doc.load_string(xml_str.c_str());
    if (!result) {
        doc.load_string("<root><child attr='val'/></root>");
    }
    
    pugi::xml_node root = doc.root();
    if (!root) {
        return 0;
    }
    
    pugi::xml_node child = root.child("child");
    pugi::xml_node child2 = root.child("child2");
    
    // Test insert_copy_after
    if (child) {
        pugi::xml_attribute attr1 = child.attribute("attr1");
        pugi::xml_attribute attr2 = child.attribute("attr2");
        
        if (attr1 && attr2) {
            child.insert_copy_after(attr1, attr2);
            child.insert_copy_after(attr2, attr1);
        }
        
        pugi::xml_attribute invalid_attr;
        child.insert_copy_after(attr1, invalid_attr);
        child.insert_copy_after(invalid_attr, attr2);
        child.insert_copy_after(invalid_attr, invalid_attr);
    }
    
    // Test remove_attribute
    if (child) {
        child.remove_attribute("attr1");
        child.remove_attribute("nonexistent");
        child.remove_attribute("");
        
        pugi::xml_attribute attr = child.attribute("attr2");
        if (attr) {
            child.remove_attribute(attr);
        }
    }
    
    // Test insert_copy_before
    if (child2) {
        pugi::xml_attribute attr3 = child2.attribute("attr3");
        
        if (attr3) {
            child2.insert_copy_before(attr3, attr3);
        }
        
        pugi::xml_attribute invalid_attr;
        child2.insert_copy_before(attr3, invalid_attr);
        child2.insert_copy_before(invalid_attr, attr3);
        child2.insert_copy_before(invalid_attr, invalid_attr);
    }
    
    // Test remove_child
    pugi::xml_node child_to_remove = root.child("child");
    if (child_to_remove) {
        root.remove_child("child");
        root.remove_child("nonexistent");
        root.remove_child("");
    }
    
    // Test insert_attribute_before
    if (child2) {
        pugi::xml_attribute attr3 = child2.attribute("attr3");
        
        if (attr3) {
            child2.insert_attribute_before("new_attr", attr3);
            child2.insert_attribute_before("", attr3);
        }
        
        pugi::xml_attribute invalid_attr;
        child2.insert_attribute_before("new_attr", invalid_attr);
    }
    
    // Test prepend_copy
    if (child2) {
        pugi::xml_attribute attr3 = child2.attribute("attr3");
        
        if (attr3) {
            child2.prepend_copy(attr3);
        }
        
        pugi::xml_attribute invalid_attr;
        child2.prepend_copy(invalid_attr);
    }
    
    return 0;
}