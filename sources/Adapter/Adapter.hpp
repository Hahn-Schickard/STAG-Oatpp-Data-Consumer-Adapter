#include "../Beispiel/src/App.hpp"
#include "Data_Consumer_Adapter_Interface/DataConsumerAdapterInterface.hpp"
#include <string>
namespace Data_Consumer_Adapter {
class Adapter : public DCAI {
public:
  Adapter(ModelEventSourcePtr event_source)
      : DCAI(event_source, "Rest-Adapter") {
    server_ = std::make_shared<RestSchnittstelle>();
  }
  void start(std::vector<Information_Model::DevicePtr> devices = {}) override {
    server_->start();
  }
  void stop() override { server_->stop(); }

private:
  void registrate(Information_Model::NonemptyDevicePtr device) override {
    server_->add(device);
  }
  void deregistrate(const std::string& device_id) override {
    server_->remove(device_id);
  }
  std::shared_ptr<RestSchnittstelle> server_;
};
} // namespace Data_Consumer_Adapter