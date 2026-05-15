// This fuzz driver is generated for library pugixml, aiming to fuzz the following functions:
// pugi::xml_node::append_child at pugixml.cpp:6253:34 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_node::append_attribute at pugixml.cpp:6041:39 in pugixml.hpp
// pugi::xml_attribute::set_value at pugixml.cpp:5482:35 in pugixml.hpp
// pugi::xml_node::insert_attribute_after at pugixml.cpp:6075:39 in pugixml.hpp
// pugi::xml_node::insert_attribute_before at pugixml.cpp:6093:39 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::first_child at pugixml.cpp:5964:34 in pugixml.hpp
// pugi::xml_node::append_copy at pugixml.cpp:6183:39 in pugixml.hpp
// pugi::xml_node::prepend_copy at pugixml.cpp:6200:39 in pugixml.hpp
// pugi::xml_node::insert_copy_after at pugixml.cpp:6217:39 in pugixml.hpp
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
#include "src/pugixml.hpp"
#include <cstdint>
#include <cstring>
#include <string>

extern "C" int LLVMFuzzerTestOneInput_9(const uint8_t *Data, size_t Size) {
    // Use a small subset of data for attribute names to avoid excessive memory usage
    size_t name_len = Size > 100 ? 100 : Size;
    std::string attr_name(reinterpret_cast<const char*>(Data), name_len);
    
    // Create document and initial structure
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child(pugi::node_element);
    
    // Create initial attributes for reference
    pugi::xml_attribute ref_attr1 = root.append_attribute("ref1");
    pugi::xml_attribute ref_attr2 = root.append_attribute("ref2");
    pugi::xml_attribute proto_attr = root.append_attribute("proto");
    proto_attr.set_value("prototype_value");
    
    // 1. insert_attribute_after
    pugi::xml_attribute new_attr1 = root.insert_attribute_after(attr_name.c_str(), ref_attr1);
    
    // 2. insert_attribute_before  
    pugi::xml_attribute new_attr2 = root.insert_attribute_before(attr_name.c_str(), ref_attr2);
    
    // 3-6. first_child (four times)
    pugi::xml_node child1 = root.first_child();
    pugi::xml_node child2 = root.first_child();
    pugi::xml_node child3 = root.first_child();
    pugi::xml_node child4 = root.first_child();
    
    // 7. append_copy
    pugi::xml_attribute copied_attr1 = root.append_copy(proto_attr);
    
    // 8. prepend_copy
    pugi::xml_attribute copied_attr2 = root.prepend_copy(proto_attr);
    
    // 9. insert_copy_after
    pugi::xml_attribute copied_attr3 = root.insert_copy_after(proto_attr, ref_attr1);
    
    // 10. insert_copy_before
    pugi::xml_attribute copied_attr4 = root.insert_copy_before(proto_attr, ref_attr2);
    
    return 0;
}