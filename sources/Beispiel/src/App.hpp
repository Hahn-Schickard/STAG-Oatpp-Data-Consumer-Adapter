#include "AppComponent.hpp"
#include "controller/MyController.hpp"

#include "Information_Model/Device.hpp"
#include "oatpp-swagger/Controller.hpp"
#include "oatpp/network/Server.hpp"
#include <thread> // Zum Hinzufügen von std::thread
#include <unordered_map>

using namespace std;

class RestSchnittstelle {
public:
  // Konstruktor
  RestSchnittstelle() : stopFlag(false) {
    oatpp::base::Environment::init();
    /* Register Components in scope of run() method */
    AppComponent components;

    /* Get router component */
    OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

    oatpp::web::server::api::Endpoints docEndpoints;

    /* Create MyController and add all of its endpoints to router */
    auto myController = std::make_shared<MyController>(devices_);
    docEndpoints.append(router->addController(myController)->getEndpoints());
    router->addController(
        oatpp::swagger::Controller::createShared(docEndpoints));

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

  // Destruktor
  ~RestSchnittstelle() {
    stop();
    oatpp::base::Environment::destroy();
  }

  // Startet den Server in einem eigenen Thread
  void start() {
    serverThread = std::thread([this]() { run(); });
  }

  // Stoppt den Server
  void stop() {
    stopFlag = true;
    if (serverThread.joinable()) {
      serverThread.join();
    }
  }

  // Fügt ein Gerät hinzu
  void add(Information_Model::NonemptyDevicePtr device) {
    devices_->emplace(device->getElementId(), device);
  }

  // Entfernt ein Gerät anhand der ID (funktioniert noch nicht)
  void remove(const std::string& id) { devices_->erase(id); }

private:
  // Startet den Server
  void run() { server_->run(); }

  // Mitglieder
  bool stopFlag; // Flag zum Stoppen des Servers
  std::thread serverThread; // Der Thread, der den Server ausführt
  shared_ptr<oatpp::network::Server> server_;
  shared_ptr<unordered_map<std::string, Information_Model::NonemptyDevicePtr>>
      devices_ = make_shared<
          unordered_map<std::string, Information_Model::NonemptyDevicePtr>>();
};
