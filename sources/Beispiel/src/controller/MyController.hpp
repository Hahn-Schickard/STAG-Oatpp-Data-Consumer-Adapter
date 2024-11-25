#ifndef MyController_hpp
#define MyController_hpp

#include "../dto/DTOs.hpp"
#include "Information_Model/Device.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/encoding/Base64.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
using namespace std;

#include OATPP_CODEGEN_BEGIN(ApiController)

class MyController : public oatpp::web::server::api::ApiController {
public:
  using Device_DTOPtr = oatpp::data::mapping::type::DTOWrapper<Device_DTO>;
  using DeviceElement_DTOPtr =
      oatpp::data::mapping::type::DTOWrapper<DeviceElement_DTO>;
  using Metric_DTOPtr = oatpp::data::mapping::type::DTOWrapper<Metric_DTO>;
  MyController(
      shared_ptr<
          unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
          devices_map,
      OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper),
        devices(devices_map) {}

  shared_ptr<unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
      devices;
  std::string decodeBase64(const oatpp::String& encoded) {
    auto decoded =
        oatpp::encoding::Base64::decode(encoded->c_str(), encoded->size());
    return std::string((const char*)decoded->data(), decoded->size());
  }

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

    auto metricValue = element->getMetricValue();

    switch (element->getDataType()) {
    case Information_Model::DataType::BOOLEAN:
      std::cout << "DataType is Boolean" << std::endl;
      if (std::holds_alternative<bool>(metricValue)) {
        bool boolValue = std::get<bool>(metricValue);
        std::cout << "Value: " << boolValue << std::endl;
        metricDto->value = to_string(boolValue);
      } else {
        std::cerr << "Error: Expected bool value, but got a different type."
                  << std::endl;
      }
      break;

    case Information_Model::DataType::INTEGER:
      std::cout << "DataType is Integer" << std::endl;
      if (std::holds_alternative<intmax_t>(metricValue)) {
        int intValue = std::get<intmax_t>(metricValue);
        std::cout << "Value: " << intValue << std::endl;
        metricDto->value = to_string(intValue);
      } else {
        std::cerr << "Error: Expected int value, but got a different type."
                  << std::endl;
      }
      break;

    case Information_Model::DataType::UNSIGNED_INTEGER:
      std::cout << "DataType is Unsigned Integer" << std::endl;
      if (std::holds_alternative<uintmax_t>(metricValue)) {
        unsigned int uintValue = std::get<uintmax_t>(metricValue);
        std::cout << "Value: " << uintValue << std::endl;
        metricDto->value = to_string(uintValue);
      } else {
        std::cerr
            << "Error: Expected unsigned int value, but got a different type."
            << std::endl;
      }
      break;

    case Information_Model::DataType::DOUBLE:
      std::cout << "DataType is Double" << std::endl;
      if (std::holds_alternative<double>(metricValue)) {
        double doubleValue = std::get<double>(metricValue);
        std::cout << "Value: " << doubleValue << std::endl;
        metricDto->value = to_string(doubleValue);
      } else {
        std::cerr << "Error: Expected double value, but got a different type."
                  << std::endl;
      }
      break;

    case Information_Model::DataType::STRING:
      std::cout << "DataType is String" << std::endl;
      if (std::holds_alternative<std::string>(metricValue)) {
        std::string stringValue = std::get<std::string>(metricValue);
        std::cout << "Value: " << stringValue << std::endl;
      } else {
        std::cerr << "Error: Expected string value, but got a different type."
                  << std::endl;
      }
      break;

    case Information_Model::DataType::OPAQUE:
      std::cout << "DataType is Opaque" << std::endl;
      if (std::holds_alternative<std::vector<uint8_t>>(metricValue)) {
        auto opaqueValue = std::get<std::vector<uint8_t>>(metricValue);
        std::cout << "Value (opaque data): ";
        for (auto byte : opaqueValue) {
          std::cout << static_cast<int>(byte) << " ";
        }
        std::cout << std::endl;
        /* metricDto->value = to_string(opaqueValue); */
      } else {
        std::cerr << "Error: Expected opaque data (vector<uint8_t>), but got a "
                     "different type."
                  << std::endl;
      }
      break;

    default:
      std::cout << "DataType is Unknown" << std::endl;
      break;
    }

    return metricDto;
  }
  ENDPOINT_INFO(getDeviceValue) {
    info->summary = "Get Device Value";
    info->pathParams.add<String>("DeviceValue").description =
        "Type in ID of your Device";
    info->addResponse<Object<Metric_DTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("GET", "/devices/{deviceId}/metric/{getValue}", getDeviceValue,
      PATH(String, deviceId), PATH(String, getValue)) {

    auto deviceIt = devices->find(deviceId);

    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {
        auto element = device_ptr->getDeviceElement(getValue);
        try {
          auto metric = std::get<Information_Model::NonemptyMetricPtr>(
              element->functionality);
          auto metricDto = getReadable(metric);

          return createDtoResponse(Status::CODE_200, metricDto);
        } catch (const exception& ex) {
          return createResponse(Status::CODE_400, "Incorrect Element Value");
        }
      } catch (const exception& ex) {
        return createResponse(Status::CODE_404, "Element not found");
      }
    } else {
      return createResponse(Status::CODE_404, "Device not found!");
    }
  }
  Metric_DTOPtr getWriteable(
      const Information_Model::NonemptyWritableMetricPtr& element) {
    auto metricDto = Metric_DTO::createShared();
    auto metricValue = element->getMetricValue();

    switch (element->getDataType()) {
    case Information_Model::DataType::BOOLEAN:
      if (std::holds_alternative<bool>(metricValue)) {
        bool boolValue = std::get<bool>(metricValue);
        metricDto->value = to_string(boolValue);
      }
      break;

    case Information_Model::DataType::INTEGER:
      if (std::holds_alternative<intmax_t>(metricValue)) {
        int intValue = std::get<intmax_t>(metricValue);
        metricDto->value = to_string(intValue);
      }
      break;

    case Information_Model::DataType::UNSIGNED_INTEGER:
      if (std::holds_alternative<uintmax_t>(metricValue)) {
        unsigned int uintValue = std::get<uintmax_t>(metricValue);
        metricDto->value = to_string(uintValue);
      }
      break;

    case Information_Model::DataType::DOUBLE:
      if (std::holds_alternative<double>(metricValue)) {
        double doubleValue = std::get<double>(metricValue);
        metricDto->value = to_string(doubleValue);
      }
      break;

    case Information_Model::DataType::STRING:
      if (std::holds_alternative<std::string>(metricValue)) {
        std::string stringValue = std::get<std::string>(metricValue);
        metricDto->value = stringValue;
      }
      break;

    case Information_Model::DataType::OPAQUE:
      if (std::holds_alternative<std::vector<uint8_t>>(metricValue)) {
        auto opaqueValue = std::get<std::vector<uint8_t>>(metricValue);
        metricDto->value = "Opaque data (Base64)";
      }
      break;

    default:
      break;
    }

    return metricDto;
  }
  ENDPOINT_INFO(getWritableMetric) {
    info->summary = "Get Writable Metric";
    info->pathParams.add<String>("WritableMetric").description =
        "Type in ID of your Device";
    info->addResponse<Object<Metric_DTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("GET", "/devices/{deviceId}/metrics/{metricId}", getWritableMetric,
      PATH(String, deviceId), PATH(String, metricId)) {

    auto deviceIt = devices->find(deviceId);
    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {
        auto element = device_ptr->getDeviceElement(metricId);
        if (element != nullptr) {
          auto metric = std::get<Information_Model::NonemptyWritableMetricPtr>(
              element->functionality);
          auto writableMetric = getWriteable(metric);
          return createDtoResponse(Status::CODE_200, writableMetric);
        } else {
          return createResponse(Status::CODE_404, "Metric not found!");
        }
      } catch (const std::exception& ex) {
        return createResponse(Status::CODE_500, "Error retrieving metric");
      }
    } else {
      return createResponse(Status::CODE_404, "Device not found");
    }
  }
  ENDPOINT_INFO(putWritableMetric) {
    info->summary = "Put Writabable Metric";
    info->pathParams.add<String>("WritableMetric").description =
        "Type in ID of your Device.";
    info->addResponse<Object<Device_DTO>>(Status::CODE_200, "application/json");
    info->addResponse<String>(Status::CODE_404, "text/plain");
  }
  ENDPOINT("PUT", "/devices/{deviceId}/metrics/{metricId}", putWritableMetric,
      PATH(String, deviceId), PATH(String, metricId)) {

    auto deviceIt = devices->find(deviceId);
    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {
        auto element = device_ptr->getDeviceElement(metricId);
        if (element != nullptr) {
          auto metric = std::get<Information_Model::NonemptyMetricPtr>(
              element->functionality);
          /* metric->setMetricValue(newValue);
           */
          return createResponse(
              Status::CODE_200, "Metric updated successfully");
        } else {
          return createResponse(Status::CODE_404, "Metric not found");
        }
      } catch (const std::exception& ex) {
        return createResponse(Status::CODE_500, "Error updating metric");
      }
    } else {
      return createResponse(Status::CODE_404, "Device not found");
    }
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
    info->pathParams.add<String>("GetDevices").description =
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

  ENDPOINT("GET", "/devices/{deviceId}/{encodedElementId}", getElement,
      PATH(String, deviceId), PATH(String, encodedElementId)) {
    auto elementId = decodeBase64(encodedElementId);
    auto deviceIt = devices->find(deviceId);
    if (deviceIt != devices->end()) {
      auto device_ptr = deviceIt->second;
      try {

        auto element = device_ptr->getDeviceElement(deviceId);
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
