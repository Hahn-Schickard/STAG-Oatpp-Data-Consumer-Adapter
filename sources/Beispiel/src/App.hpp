#ifndef App_hpp
#define App_hpp
#include "../../includes/RestInterface.hpp"
#include "Information_Model/Device.hpp"
#include <oatpp/network/Server.hpp>

#include <thread>
#include <unordered_map>

using namespace std;

class RestServer : public RestInterface {
public:
  RestServer();
  ~RestServer();

  void start() override;
  void stop() override;
  void add(Information_Model::NonemptyDevicePtr device) override;
  void remove(const std::string& id) override;

private:
  std::shared_ptr<oatpp::network::Server> server_;
  std::thread serverThread;
  bool stopFlag;
  std::shared_ptr<
      std::unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
      devices_ = std::make_shared<std::unordered_map<std::string,
          Information_Model::NonemptyDevicePtr>>();
  void run();
};
#endif