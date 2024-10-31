#include "AppComponent.hpp"
#include "controller/MyController.hpp"

#include "Information_Model/Device.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include <unordered_map>
using namespace std;
class RestSchnittstelle {
public:
  RestSchnittstelle() {
    oatpp::base::Environment::init();
    /* Register Components in scope of run() method */
    AppComponent components;

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;

    /* Create MyController and add all of its endpoints to router */
    auto myController = std::make_shared<MyController>();
    docEndpoints.append(router->addController(myController)->getEndpoints());
    router->addController(
        oatpp::swagger::Controller::createShared(docEndpoints));

    /* Get connection handler component */
    OATPP_COMPONENT(
        std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

    /* Get connection provider component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
        connectionProvider);
    /* Priny info about server port */
    OATPP_LOGI("MyApp", "Server running on port %s",
        connectionProvider->getProperty("port").getData());
    server_ = oatpp::network::Server::createShared(
        connectionProvider, connectionHandler);
  }

  ~RestSchnittstelle() { oatpp::base::Environment::destroy(); }
  void start() { run(); }
  void stop() {}
  void add(Information_Model::NonemptyDevicePtr device) {
    devices_.emplace(device->getElementId(), device);
  }
  void remove(const std::string& id) {}

private:
  void run() { server_->run(); }
  shared_ptr<oatpp::network::Server> server_;
  std::unordered_map<std::string, Information_Model::NonemptyDevicePtr>
      devices_;
};