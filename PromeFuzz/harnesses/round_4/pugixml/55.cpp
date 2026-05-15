// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_attribute::next_attribute at pugixml.cpp:5289:44 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::first_attribute at pugixml.cpp:5951:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5826:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
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
#include <string>
#include <cstring>

static void initialize_test_document(pugi::xml_document& doc) {
    // Create a simple XML structure for testing
    pugi::xml_node root = doc.append_child("root");
    root.append_attribute("attr1") = "value1";
    root.append_attribute("attr2") = "value2";
    root.append_attribute("attr3") = "value3";
    
    // Add child nodes with attributes
    pugi::xml_node child = root.append_child("child");
    child.append_attribute("child_attr1") = "child_value1";
    child.append_attribute("child_attr2") = "child_value2";
}

static pugi::xml_attribute get_attribute_by_index(pugi::xml_node& node, size_t index) {
    size_t i = 0;
    for (pugi::xml_attribute attr = node.first_attribute(); attr; attr = attr.next_attribute()) {
        if (i == index) {
            return attr;
        }
        i++;
    }
    return pugi::xml_attribute();
}

extern "C" int LLVMFuzzerTestOneInput_55(const uint8_t *Data, size_t Size) {
    if (Size < 1) {
        return 0;
    }

    // Create test document
    pugi::xml_document doc;
    initialize_test_document(doc);
    
    // Get root node
    pugi::xml_node root = doc.child("root");
    if (!root) {
        return 0;
    }
    
    // Get child node
    pugi::xml_node child = root.child("child");
    
    // Use input data to determine which operations to perform
    uint8_t op_selector = Data[0] % 6;
    size_t data_offset = 1;
    
    // Extract a string from the input data for attribute names
    std::string attr_name;
    if (Size > data_offset) {
        size_t name_len = std::min(Size - data_offset, static_cast<size_t>(50));
        attr_name.assign(reinterpret_cast<const char*>(Data + data_offset), name_len);
        data_offset += name_len;
    } else {
        attr_name = "test_attr";
    }
    
    try {
        switch (op_selector) {
            case 0: {
                // Test pugi::xml_node::attribute with hint
                pugi::xml_attribute hint = root.first_attribute();
                if (hint) {
                    pugi::xml_attribute result = root.attribute(attr_name.c_str(), hint);
                    (void)result; // Use result to avoid unused variable warning
                }
                break;
            }
            
            case 1: {
                // Test pugi::xml_node::insert_attribute_before
                pugi::xml_attribute ref_attr = get_attribute_by_index(root, Size % 3);
                if (ref_attr) {
                    pugi::xml_attribute new_attr = root.insert_attribute_before(attr_name.c_str(), ref_attr);
                    (void)new_attr;
                }
                break;
            }
            
            case 2: {
                // Test pugi::xml_node::insert_copy_after
                pugi::xml_attribute proto = get_attribute_by_index(root, Size % 3);
                pugi::xml_attribute ref_attr = get_attribute_by_index(root, (Size + 1) % 3);
                if (proto && ref_attr) {
                    pugi::xml_attribute copy = root.insert_copy_after(proto, ref_attr);
                    (void)copy;
                }
                break;
            }
            
            case 3: {
                // Test pugi::xml_node::prepend_attribute
                pugi::xml_attribute new_attr = root.prepend_attribute(attr_name.c_str());
                (void)new_attr;
                break;
            }
            
            case 4: {
                // Test pugi::xml_node::insert_attribute_after
                pugi::xml_attribute ref_attr = get_attribute_by_index(root, Size % 3);
                if (ref_attr) {
                    pugi::xml_attribute new_attr = root.insert_attribute_after(attr_name.c_str(), ref_attr);
                    (void)new_attr;
                }
                break;
            }
            
            case 5: {
                // Test pugi::xml_node::insert_copy_before
                pugi::xml_attribute proto = get_attribute_by_index(root, Size % 3);
                pugi::xml_attribute ref_attr = get_attribute_by_index(root, (Size + 1) % 3);
                if (proto && ref_attr) {
                    pugi::xml_attribute copy = root.insert_copy_before(proto, ref_attr);
                    (void)copy;
                }
                break;
            }
        }
        
        // Also test on child node with different parameters
        if (child && Size > 10) {
            std::string child_attr_name;
            size_t child_name_len = std::min(Size - 5, static_cast<size_t>(30));
            child_attr_name.assign(reinterpret_cast<const char*>(Data + 5), child_name_len);
            
            pugi::xml_attribute child_ref = child.first_attribute();
            if (child_ref) {
                pugi::xml_attribute result = child.attribute(child_attr_name.c_str(), child_ref);
                (void)result;
            }
        }
        
    } catch (...) {
        // Catch any exceptions to prevent crashes
    }
    
    return 0;
}