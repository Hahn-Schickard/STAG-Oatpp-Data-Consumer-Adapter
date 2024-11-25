#ifndef __Rest_Adapter_hpp
#define __Rest_Adapter_hpp
#include "../Beispiel/src/App.hpp"
#include "Data_Consumer_Adapter_Interface/DataConsumerAdapterInterface.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Data_Consumer_Adapter {

class Adapter : public DCAI {
public:
  Adapter(ModelEventSourcePtr event_source);
  void start(std::vector<Information_Model::DevicePtr> devices = {}) override;
  void stop() override;

private:
  void registrate(Information_Model::NonemptyDevicePtr device) override;
  void deregistrate(const std::string& device_id) override;
  std::shared_ptr<RestSchnittstelle> server_;
};

} // namespace Data_Consumer_Adapter
#endif //__Rest_Adapter_hpp