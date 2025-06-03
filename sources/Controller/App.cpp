#include "App.hpp"
#include "AppComponent.hpp"
#include "RestInterface.hpp"
#include "MyController.hpp"
#include "oatpp-swagger/Controller.hpp"

RestInterfacePtr makeRestInterface() { return std::make_shared<RestServer>(); }

RestServer::RestServer() : stopFlag(false) {
  oatpp::base::Environment::init();
  AppComponent components;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints endpoints;

  /* Create MyController and add all of its endpoints to router */
  auto my_controller = std::make_shared<MyController>(devices_);
  endpoints.append(router->addController(my_controller)->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(endpoints));

  /* Get connection handler component */
  OATPP_COMPONENT(
      std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>,
      connectionProvider);

  server_ = oatpp::network::Server::createShared(
        connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGI("STAG_REST_DCA", "Swagger UI available at http://%s:%s/swagger/ui",
        connectionProvider->getProperty("host").getData(),
        connectionProvider->getProperty("port").getData());
}

RestServer::~RestServer() {
  stop();
  oatpp::base::Environment::destroy();
}

void RestServer::start() {
  serverThread = std::thread([this]() { run(); });
}

void RestServer::stop() {
  stopFlag = true;
  if (server_) {
    server_->stop();
  }
  if (serverThread.joinable()) {
    serverThread.join();
  }
}

void RestServer::add(Information_Model::NonemptyDevicePtr device) {
  devices_->emplace(device->getElementId(), device);
}

void RestServer::remove(const std::string& id) { devices_->erase(id); }

void RestServer::run() { server_->run(); }
