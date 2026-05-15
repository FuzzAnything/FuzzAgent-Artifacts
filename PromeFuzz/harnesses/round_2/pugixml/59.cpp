// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::prepend_attribute at pugixml.cpp:6058:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_document::load_file at pugixml.cpp:7729:46 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::load_buffer at pugixml.cpp:7749:46 in pugixml.hpp
// pugi::xml_document::reset at pugixml.cpp:7490:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_document::document_element at pugixml.cpp:7830:38 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::child at pugixml.cpp:5707:34 in pugixml.hpp
// pugi::xml_node::append_child at pugixml.cpp:6323:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::attribute at pugixml.cpp:5721:39 in pugixml.hpp
// pugi::xml_node::insert_copy_before at pugixml.cpp:6235:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6164:39 in pugixml.hpp
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
#include "pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>
#include <fstream>

extern "C" int LLVMFuzzerTestOneInput_59(const uint8_t *Data, size_t Size) {
    // Create a dummy file for potential file operations
    static const char* dummy_file = "./dummy_file";
    
    // Write input data to dummy file
    std::ofstream file(dummy_file, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(Data), Size);
        file.close();
    }
    
    // Try to parse XML from the input data
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_buffer(Data, Size);
    
    // If parsing fails, create a minimal valid document to work with
    if (!result) {
        doc.reset();
        pugi::xml_node root = doc.append_child("root");
        root.append_attribute("attr1") = "value1";
        root.append_attribute("attr2") = "value2";
        root.append_child("child").append_attribute("child_attr") = "child_value";
    }
    
    // Get the root node and some attributes to work with
    pugi::xml_node root = doc.document_element();
    if (!root) {
        root = doc.append_child("root");
    }
    
    // Get existing attributes or create some
    pugi::xml_attribute attr1 = root.attribute("attr1");
    if (!attr1) {
        attr1 = root.append_attribute("attr1");
        attr1.set_value("value1");
    }
    
    pugi::xml_attribute attr2 = root.attribute("attr2");
    if (!attr2) {
        attr2 = root.append_attribute("attr2");
        attr2.set_value("value2");
    }
    
    // Create a child node with attributes for more testing scenarios
    pugi::xml_node child = root.child("child");
    if (!child) {
        child = root.append_child("child");
        child.append_attribute("child_attr1") = "child_value1";
        child.append_attribute("child_attr2") = "child_value2";
    }
    
    // Extract strings from fuzzer input for attribute names/values
    std::string_view input_str(reinterpret_cast<const char*>(Data), Size);
    
    // Test 1: insert_copy_before
    if (Size >= 1) {
        try {
            // Try to insert copy of attr2 before attr1
            pugi::xml_attribute new_attr = root.insert_copy_before(attr2, attr1);
            
            // Also test on child node
            pugi::xml_attribute child_attr1 = child.attribute("child_attr1");
            pugi::xml_attribute child_attr2 = child.attribute("child_attr2");
            if (child_attr1 && child_attr2) {
                child.insert_copy_before(child_attr2, child_attr1);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 2: insert_attribute_before
    if (Size >= 2) {
        try {
            // Create attribute name from input data
            size_t name_len = Size > 10 ? 10 : Size;
            std::string attr_name(input_str.substr(0, name_len));
            
            pugi::xml_attribute new_attr = root.insert_attribute_before(attr_name, attr1);
            
            // Try with child node
            if (child) {
                pugi::xml_attribute child_attr = child.attribute("child_attr1");
                if (child_attr) {
                    child.insert_attribute_before(attr_name + "_child", child_attr);
                }
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 3: insert_copy_after
    if (Size >= 3) {
        try {
            pugi::xml_attribute new_attr = root.insert_copy_after(attr1, attr2);
            
            // Test on child node
            pugi::xml_attribute child_attr1 = child.attribute("child_attr1");
            pugi::xml_attribute child_attr2 = child.attribute("child_attr2");
            if (child_attr1 && child_attr2) {
                child.insert_copy_after(child_attr1, child_attr2);
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 4: insert_attribute_after
    if (Size >= 4) {
        try {
            // Create attribute name from input data
            size_t name_len = Size > 8 ? 8 : Size;
            std::string attr_name(reinterpret_cast<const char*>(Data), name_len);
            
            pugi::xml_attribute new_attr = root.insert_attribute_after(attr_name.c_str(), attr2);
            
            // Try with child node
            if (child) {
                pugi::xml_attribute child_attr = child.attribute("child_attr2");
                if (child_attr) {
                    child.insert_attribute_after((attr_name + "_child2").c_str(), child_attr);
                }
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 5: prepend_attribute
    if (Size >= 5) {
        try {
            // Create attribute name from input data
            size_t name_len = Size > 6 ? 6 : Size;
            std::string attr_name(reinterpret_cast<const char*>(Data + (Size % 2)), name_len);
            
            pugi::xml_attribute new_attr = root.prepend_attribute(attr_name.c_str());
            
            // Try with child node
            if (child) {
                child.prepend_attribute((attr_name + "_prepend").c_str());
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Test 6: attribute (retrieval)
    if (Size >= 6) {
        try {
            // Try to retrieve attributes with various names
            size_t name_len = Size > 12 ? 12 : Size;
            std::string search_name(reinterpret_cast<const char*>(Data + (Size % 3)), name_len);
            
            pugi::xml_attribute found_attr = root.attribute(search_name.c_str());
            
            // Try with different variations
            found_attr = root.attribute("attr1");
            found_attr = root.attribute("attr2");
            found_attr = root.attribute("nonexistent");
            
            // Try with child node
            if (child) {
                child.attribute("child_attr1");
                child.attribute("child_attr2");
                child.attribute(search_name.c_str());
            }
        } catch (...) {
            // Ignore exceptions
        }
    }
    
    // Additional testing: try to load from dummy file
    if (Size > 0) {
        pugi::xml_document doc2;
        pugi::xml_parse_result result2 = doc2.load_file(dummy_file);
        
        // If successful, test some operations on this document too
        if (result2) {
            pugi::xml_node root2 = doc2.document_element();
            if (root2) {
                pugi::xml_attribute test_attr = root2.attribute("test");
                if (!test_attr) {
                    root2.append_attribute("test") = "test_value";
                }
            }
        }
    }
    
    // Clean up dummy file
    std::remove(dummy_file);
    
    return 0;
}