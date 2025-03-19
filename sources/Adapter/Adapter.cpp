#include "../../includes/Adapter.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Data_Consumer_Adapter {
Adapter::Adapter(ModelEventSourcePtr event_source)
    : DCAI(std::move(event_source), "Rest-Adapter") {
  server_ = makeRestInterface();
}

void Adapter::start(const DCAI::Devices& devices) { server_->start(); }

void Adapter::stop() { server_->stop(); }

void Adapter::registrate(const Information_Model::NonemptyDevicePtr& device) {
  server_->add(device);
}
void Adapter::deregistrate(const std::string& device_id) {
  server_->remove(device_id);
}
} // namespace Data_Consumer_Adapter