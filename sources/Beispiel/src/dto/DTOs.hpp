#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(oatpp::String, name, "First-Name");
  DTO_FIELD(oatpp::String, surname, "Family-Name");
  DTO_FIELD(oatpp::Int32, age);
  DTO_FIELD(oatpp::Fields<oatpp::List<oatpp::Object<UserDto>>>,
      familyMembers); ///< Map<String, List<User>>
  DTO_FIELD(
      oatpp::Fields<oatpp::String>, additionalNotes); ///< Map<String, String>
};

class Named_Element_DTO : public oatpp::DTO {
  DTO_INIT(Named_Element_DTO, DTO)

  DTO_FIELD(oatpp::String, refID, "refID");
  DTO_FIELD(oatpp::String, name, "name");
  DTO_FIELD(oatpp::String, desc, "desc");
};

ENUM(ElementType, v_int32, VALUE(GROUP, 0, "Group"),
    VALUE(READABLE, 1, "Readable"), VALUE(WRITABLE, 2, "Writable"),
    VALUE(OBSERVABLE, 3, "Observable"), VALUE(FUNCTION, 4, "Function"))

class DeviceElement_DTO : public Named_Element_DTO {
  DTO_INIT(DeviceElement_DTO, Named_Element_DTO)
  DTO_FIELD(oatpp::List<oatpp::Object<DeviceElement_DTO>>, elements);
  DTO_FIELD(oatpp::String, elementtype);
};

class Metric_DTO : public Named_Element_DTO {
  DTO_INIT(Metric_DTO, Named_Element_DTO)
  DTO_FIELD(oatpp::String, value);
};

class Device_DTO : public Named_Element_DTO {
  DTO_INIT(Device_DTO, Named_Element_DTO)
  DTO_FIELD(oatpp::List<oatpp::Object<DeviceElement_DTO>>, elements);
};

#include OATPP_CODEGEN_END(DTO)
#endif
