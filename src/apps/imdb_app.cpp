#include "oatpp/parser/json/mapping/ObjectMapper.hpp"
#include "oatpp/web/server/HttpConnectionHandler.hpp"
#include "oatpp/network/Server.hpp"
#include "oatpp/network/tcp/server/ConnectionProvider.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include <iostream>

#include "imdb_controller.hpp"
#include "imdb_component.hpp"
#include "imdb-dto.hpp"
#include "bdb_errors.hpp"

/* Begin DTO code-generation */
#include OATPP_CODEGEN_BEGIN(DTO)

/* End DTO code-generation */
#include OATPP_CODEGEN_END(DTO)
/**
 * Custom Request Handler
 */
class Handler : public oatpp::web::server::HttpRequestHandler {
 private:
  std::shared_ptr<oatpp::data::mapping::ObjectMapper> m_objectMapper;
 public:

  /**
 * Constructor with object mapper.
 * @param objectMapper - object mapper used to serialize objects.
 */
  Handler(const std::shared_ptr<oatpp::data::mapping::ObjectMapper> &objectMapper)
      : m_objectMapper(objectMapper) {}

  /**
   * Handle incoming request and return outgoing response.
   */
  std::shared_ptr<OutgoingResponse> handle(const std::shared_ptr<IncomingRequest> &request) override {
    auto message = Imdb_request_DTO::createShared();
    //message->statusCode = 1024;
    //message->message = "Hello DTO!";
    return ResponseFactory::createResponse(Status::CODE_200, message, m_objectMapper);
  }

};

void run() {
  /* Register Components in scope of run() method */
  Imdb_component components;

  /* Get router component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  /* Create MyController and add all of its endpoints to router */
  router->addController(std::make_shared<Imdb_controller>());

  /* Get connection handler component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);

  /* Get connection provider component */
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  /* Create server which takes provided TCP connections and passes them to HTTP connection handler */
  oatpp::network::Server server(connectionProvider, connectionHandler);

  /* Print info about server port */
  OATPP_LOGI("Imdb-app", "Server running on port %s", connectionProvider->getProperty("port").getData());

  /* Run server */
  server.run();
}

/**
 *  main
 */
int main(int argc, const char *argv[]) {

  Bdb_errors errors;
  oatpp::base::Environment::init();

  run();

  /* Print how much objects were created during app running, and what have left-probably leaked */
  /* Disable object counting for release builds using '-D OATPP_DISABLE_ENV_OBJECT_COUNTERS' flag for better performance */
  std::cout << "\nEnvironment:\n";
  std::cout << "objectsCount = " << oatpp::base::Environment::getObjectsCount() << "\n";
  std::cout << "objectsCreated = " << oatpp::base::Environment::getObjectsCreated() << "\n\n";

  oatpp::base::Environment::destroy();
  return 0;
}
