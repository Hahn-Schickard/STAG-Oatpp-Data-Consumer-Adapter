#ifndef __Rest_Adapter_hpp
#define __Rest_Adapter_hpp
#include "Data_Consumer_Adapter_Interface/DataConsumerAdapterInterface.hpp"
#include "RestInterface.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Data_Consumer_Adapter {

class Adapter : public DCAI {
public:
  Adapter(ModelEventSourcePtr event_source);
  void start(const DCAI::Devices& devices = {}) override;
  void stop() override;

private:
  void registrate(const Information_Model::NonemptyDevicePtr& device) override;
  void deregistrate(const std::string& device_id) override;
  std::shared_ptr<RestInterface> server_;
};

} // namespace Data_Consumer_Adapter
#endif //__Rest_Adapter_hpp