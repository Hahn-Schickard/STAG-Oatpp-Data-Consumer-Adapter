#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/DTOs.hpp"
#include "Information_Model/Device.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include <stdexcept>
using namespace std;
using Metric_DTOPtr = oatpp::data::mapping::type::DTOWrapper<Metric_DTO>;
#include OATPP_CODEGEN_BEGIN(ApiController)

class MyController : public oatpp::web::server::api::ApiController {
public:
  using UserDtoPtr = oatpp::data::mapping::type::DTOWrapper<UserDto>;
  using Device_DTOPtr = oatpp::data::mapping::type::DTOWrapper<Device_DTO>;
  using DeviceElement_DTOPtr =
      oatpp::data::mapping::type::DTOWrapper<DeviceElement_DTO>;

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

  DeviceElement_DTOPtr getDeviceElement(
      const Information_Model::NonemptyDeviceElementPtr& element) {
    auto elementDto = DeviceElement_DTO::createShared();
    elementDto->elementtype = toString(element->getElementType());
    elementDto->refID = element->getElementId();
    elementDto->name = element->getElementName();
    elementDto->desc = element->getElementDescription();

    if (element->getElementType() == Information_Model::ElementType::GROUP) {
      auto subelements =
          std::get<Information_Model::NonemptyDeviceElementGroupPtr>(
              element->functionality);

      elementDto->elements = oatpp::List<DeviceElement_DTOPtr>::createShared();

      for (auto& subelement : subelements->getSubelements()) {
        auto subelementDto = getDeviceElement(subelement);
        elementDto->elements->push_back(subelementDto);
      }
    }
    return elementDto;
  }

  Metric_DTOPtr getReadable(
      const Information_Model::NonemptyMetricPtr& element) {
    auto metricDto = Metric_DTO::createShared();
  }

  Metric_DTOPtr getWriteable(
      const Information_Model::NonemptyWritableMetricPtr& element) {
    auto metricDto = Metric_DTO::createShared();
  }

  oatpp::List<oatpp::Object<DeviceElement_DTO>> getDeviceElements(
      const Information_Model::NonemptyDevicePtr& device_ptr) {
    auto elements =
        oatpp::List<oatpp::Object<DeviceElement_DTO>>::createShared();
    for (auto& element :
        device_ptr->getDeviceElementGroup()->getSubelements()) {
      elements->push_back(getDeviceElement(element));
    }
    return elements;
  }

  ENDPOINT_INFO(getDevices) {
    info->summary = "Get Devices";
    info->pathParams.add<String>("result").description =
        "Type in ID of your Device.";
    info->addResponse<Object<Device_DTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ENDPOINT("GET", "/devices", getDevices) {
    std::string result;
    for (const auto& device : *devices) {
      result += device.first + " ";
    }
    return createResponse(Status::CODE_200, result);
  }

  ENDPOINT_INFO(checkdevice) {
    info->summary = "Check Device";
    info->pathParams.add<String>("deviceId").description =
        "Unique identifier for the device.";
    info->addResponse<Object<Device_DTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }

  ENDPOINT("GET", "/devices/{deviceId}", checkdevice, PATH(String, deviceId)) {
    auto deviceIt = devices->find(deviceId);

    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      Device_DTOPtr deviceDto = Device_DTOPtr::createShared();
      deviceDto->refID = device_ptr->getElementId();
      deviceDto->name = oatpp::String(device_ptr->getElementName());
      deviceDto->desc = oatpp::String(device_ptr->getElementDescription());
      deviceDto->elements = getDeviceElements(device_ptr);
      return createDtoResponse(Status::CODE_200, deviceDto);
    }

    return createResponse(Status::CODE_404, "Device not found!");
  }

  ENDPOINT_INFO(getElementType) {
    info->summary = "Get Device Element Type";
    info->pathParams.add<String>("deviceId").description =
        "Unique identifier of the device containing the element.";
    info->pathParams.add<String>("elementId").description =
        "Unique identifier of the element within the device.";
    info->addResponse<Object<DeviceElement_DTO>>(
        Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
    info->addResponse<String>(Status::CODE_500, "text/plain");
  }

  ENDPOINT("GET", "/devices/{deviceId}/{elementId}/type", getElementType,
      PATH(String, deviceId), PATH(String, elementId)) {
    auto deviceIt = devices->find(deviceId);
    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {
        auto id = elementId.getValue("");
        auto element = device_ptr->getDeviceElement(id);

        if (element != nullptr) {
          return createDtoResponse(Status::CODE_200, getDeviceElement(element));
        } else {
          return createResponse(Status::CODE_404, "Element not found!");
        }
      } catch (const std::exception& ex) {
        return createResponse(
            Status::CODE_404, string("Exception Error ") + ex.what());
      }
    }
    return createResponse(Status::CODE_404, "Device not found!");
  }

  ENDPOINT("GET", "/devices/{deviceId}/{elementId}", getElement,
      PATH(String, deviceId), PATH(String, elementId)) {
    auto deviceIt = devices->find(deviceId);
    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {
        auto element = device_ptr->getDeviceElement(elementId);
        if (element != nullptr) {
          return createDtoResponse(Status::CODE_200, getDeviceElement(element));
        } else {
          return createResponse(Status::CODE_404, "Element not found!");
        }
      } catch (const std::exception& ex) {
        return createResponse(Status::CODE_404, "Device not found!");
      }
    } else {
      return createResponse(Status::CODE_404, "Device not found");
    }
  }
};

#include OATPP_CODEGEN_END(ApiController)
#endif
