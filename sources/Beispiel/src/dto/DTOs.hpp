#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public oatpp::DTO {
  DTO_INIT(UserDto, DTO)

  DTO_FIELD(String, name, "First-Name");
  DTO_FIELD(String, surname, "Family-Name");
  DTO_FIELD(Int32, age);
  DTO_FIELD(Fields<List<Object<UserDto>>>,
      familyMembers); ///< Map<String, List<User>>
  DTO_FIELD(Fields<String>, additionalNotes); ///< Map<String, String>
};

class Named_Element_DTO : public oatpp ::DTO {
  DTO_INIT(Named_Element_DTO, DTO)

  DTO_FIELD(String, refID, "refID");
  DTO_FIELD(String, name, "name");
  DTO_FIELD(String, desc, "desc");
};

class Device_DTO : public Named_Element_DTO {
  DTO_INIT(Device_DTO, Named_Element_DTO)
};

#include OATPP_CODEGEN_END(DTO)
#endif