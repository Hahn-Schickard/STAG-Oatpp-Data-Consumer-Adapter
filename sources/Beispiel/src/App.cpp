#include "controller/MyController.hpp"
#include "AppComponent.hpp"

#include "oatpp/network/Server.hpp"
#include "oatpp-swagger/Controller.hpp"

void run() {


  /* Register Components in scope of run() method */
  AppComponent components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  oatpp::web::server::api::Endpoints docEndpoints;
  

  
  /* Create MyController and add all of its endpoints to router */
  auto myController = std::make_shared<MyController>();
  docEndpoints.append(router->addController(myController)->getEndpoints());
  router->addController(oatpp::swagger::Controller::createShared(docEndpoints));
  
  
  
 
  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Priny info about server port */
  OATPP_LOGI("MyApp", "Server running on port %s", connectionProvider->getProperty("port").getData());

 


  server.run();
  
}

int main(int argc, const char * argv[]) {

  /* Init oatpp Environment */
  oatpp::base::Environment::init();

  /* Run App */
  run();

  /* Destroy oatpp Environment */
  oatpp::base::Environment::destroy();

  return 0;
}
