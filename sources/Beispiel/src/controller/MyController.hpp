#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/DTOs.hpp"

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"



#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
public:
  using UserDtoPtr = oatpp::data::mapping::type::DTOWrapper<UserDto>;

MyController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper) /* Inject object mapper */)
    : oatpp::web::server::api::ApiController(objectMapper) {}

std::unordered_map<long, UserDtoPtr> user_map;


ENDPOINT("GET", "/check/{userId}", checkId, PATH(Int32, userId)){
    if(auto it = user_map.find(userId); it != user_map.end()){
        return createResponse(Status::CODE_200, "ID already taken");
    }
    else{
        return createResponse(Status::CODE_400, "ID not found");
    }
}

ENDPOINT("PUT", "/users/{userId}", CreateUserById, PATH(Int32, userId)) {
    
    if (user_map.find(userId) == user_map.end()) {
        
        auto user = UserDto::createShared();
        user->name = "Robin";
        user->surname = "Weiler";
        user->age = 23;
        user->familyMembers = {}; 
        user->additionalNotes = {}; 

        
        auto brother = UserDto::createShared();
        brother->name = "Herbert";
        brother->surname = "Weiler";
        brother->age = 22;

        auto sister = UserDto::createShared();
        sister->name = "Laura";
        sister->surname = "Weiler";
        sister->age = 20;

        
        user_map.emplace(userId, user);  
        user_map.emplace(userId + 1, brother);  
        user_map.emplace(userId + 2, sister);  
        
        return createDtoResponse(Status::CODE_201, user);
    } else {
        
        return createResponse(Status::CODE_400, "ID already exists, user creation failed");
    }
}


ENDPOINT("POST", "/users", createUser, BODY_DTO(Object<UserDto>, userDto)) {
    
    return createDtoResponse(Status::CODE_201, userDto);
}
};
#include OATPP_CODEGEN_END(ApiController)
#endif