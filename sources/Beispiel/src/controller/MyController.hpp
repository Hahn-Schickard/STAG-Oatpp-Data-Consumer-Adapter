#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/DTOs.hpp"

#include "Information_Model/Device.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
using namespace std;
#include OATPP_CODEGEN_BEGIN(ApiController) ///< Begin Codegen

class MyController : public oatpp::web::server::api::ApiController {
public:
  using UserDtoPtr = oatpp::data::mapping::type::DTOWrapper<UserDto>;
  using Device_DTOPtr = oatpp::data::mapping::type::DTOWrapper<Device_DTO>;

  MyController(
      shared_ptr<
          unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
          devices_map,
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        devices(devices_map) {}

  std::unordered_map<long, UserDtoPtr> user_map;
  shared_ptr<unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
      devices;

  ENDPOINT("GET", "/devices", getDevices) {
    std::string result;

    for (const auto& device : *devices) {
      result += device.first + " ";
    }

    return createResponse(Status::CODE_200, result);
  }

  ENDPOINT("GET", "/devices/{deviceId}", checkdevice, PATH(String, deviceId)) {

    auto deviceIt = devices->find(deviceId);

    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      Device_DTOPtr deviceDto = Device_DTOPtr::createShared();
      deviceDto->refID = device_ptr->getElementId();
      deviceDto->name = device_ptr->getElementName();
      deviceDto->desc = device_ptr->getElementDescription();

      return createDtoResponse(Status::CODE_201, deviceDto);
    }

    return createResponse(Status::CODE_404, "Device not found!");
  }
};
#include OATPP_CODEGEN_END(ApiController)
#endif