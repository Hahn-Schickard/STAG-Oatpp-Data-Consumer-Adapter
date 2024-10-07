#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"


/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

class UserDto : public  oatpp::DTO
{
  DTO_INIT(UserDto, DTO /* extends */)

  DTO_FIELD(String, name, "First-Name");
  DTO_FIELD(String, surname, "Family-Name");
  DTO_FIELD(Int32, age);
  DTO_FIELD(Fields<List<Object<UserDto>>>, familyMembers); ///< Map<String, List<User>>
  DTO_FIELD(Fields<String>, additionalNotes); ///< Map<String, String>
  
  

};

#include OATPP_CODEGEN_END(DTO)
#endif