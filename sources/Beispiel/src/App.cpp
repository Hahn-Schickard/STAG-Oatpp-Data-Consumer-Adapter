#include "App.hpp"
#include "AppComponent.hpp"
#include "RestInterface.hpp"
#include "controller/MyController.hpp"

#include "oatpp-swagger/Controller.hpp"

using namespace std;

RestInterfacePtr makeRestInterface() {
  return std::make_shared<RestSchnittstelle>();
}
RestSchnittstelle::RestSchnittstelle() : stopFlag(false) {
  oatpp::base::Environment::init();
  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;

  /* Create MyController and add all of its endpoints to router */
  auto myController = std::make_shared<MyController>(devices_);
  docEndpoints.append(router->addController(myController)->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));

  /* Get connection handler component */
  OATPP_COMPONENT(
      std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      connectionProvider);

  /* Print info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s",
      connectionProvider->getProperty("port").getData());

  server_ = oatpp::network::Server::createShared(
      connectionProvider, connectionHandler);
}

RestSchnittstelle::~RestSchnittstelle() {
  stop();
  oatpp::base::Environment::destroy();
}

void RestSchnittstelle::start() {
  serverThread = std::thread([this]() { run(); });
}

void RestSchnittstelle::stop() {
  stopFlag = true;
  if (server_) {
    server_->stop();
  }
  if (serverThread.joinable()) {
    serverThread.join();
  }
}

void RestSchnittstelle::add(Information_Model::NonemptyDevicePtr device) {
  devices_->emplace(device->getElementId(), device);
}

void RestSchnittstelle::remove(const std::string& id) { devices_->erase(id); }

void RestSchnittstelle::run() { server_->run(); }
