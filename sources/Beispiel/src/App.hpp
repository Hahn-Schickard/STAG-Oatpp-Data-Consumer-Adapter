#ifndef App_hpp
#define App_hpp
#include "../../Adapter/RestInterface.hpp"
#include "Information_Model/Device.hpp"
#include <oatpp/network/Server.hpp>

#include <thread>
#include <unordered_map>

using namespace std;

class RestSchnittstelle {
public:
  RestSchnittstelle();
  ~RestSchnittstelle();

  void start() override {}
  void stop() override {}
  void add(Information_Model::NonemptyDevicePtr device) override {}
  void remove(const std::string& id) override {}

private:
  void run();

  // Mitglieder
  bool stopFlag; // Flag zum Stoppen des Servers
  std::thread serverThread; // Der Thread, der den Server ausführt
  shared_ptr<oatpp::network::Server> server_;
  shared_ptr<unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
      devices_ = make_shared<
          unordered_map<std::string, Information_Model::NonemptyDevicePtr>>();
};
#endif