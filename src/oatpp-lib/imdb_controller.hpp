#pragma once

#include <iostream>
#include <json-c/json.h>
#include "DTOs.hpp"
#include "bdb_errors.hpp"
#include "bdb_unix_socket_client.hpp"
#include "imdb-dto.hpp"
#include "imdb_unix_request.hpp"

#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include "oatpp/parser/json/mapping/ObjectMapper.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class Imdb_controller : public oatpp::web::server::api::ApiController {
 public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  Imdb_controller(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
      : oatpp::web::server::api::ApiController(objectMapper) {}
 public:

  ENDPOINT("GET", "hello", root) {
    auto dto = Response_DTO::createShared();
    dto->statusCode = 200;
    dto->message = "Hello World!";
    return createDtoResponse(Status::CODE_200, dto);
  }

  // TODO Insert Your endpoints here !!!

  ENDPOINT("POST", "name-dto", postBodyDto,
           BODY_DTO(Object < Name_DTO > , name_dto)) {
    OATPP_LOGD("Test", "name_dto-name='%s'", name_dto->name_id.get());
    return createDtoResponse(Status::CODE_200, name_dto);
  }

  // https://oatpp.io/docs/components/dto/#serialize-deserialize
  ENDPOINT("POST", "imdb-request-dto", postImdbRequestDto,
           BODY_DTO(Object < Imdb_request_DTO > , body)) {
    auto mapper = oatpp::parser::json::mapping::ObjectMapper::createShared();
    //OATPP_LOGV(TAG, "json='%s'", result->c_str());
    mapper->getSerializer()->getConfig()->useBeautifier = true;
    auto result = mapper->writeToString(body);
    std::string str = result.getValue("none");
    std::cout << "imdb request dto response json = '"
              << str << "'" << std::endl;
    Bdb_errors errors;
    Bdb_unix_socket_client bdb_unix_socket_client(errors);
    if (!errors.has()) {
      std::cout << "test_client: sending '" << str << "'" << std::endl;
      bdb_unix_socket_client.write_socket(str.c_str(),
                                          str.length() + 1, errors);
    }
    std::string reply;
    if (!errors.has()) {
      int buffer_len = 65535;
      char buffer[buffer_len];
      if (bdb_unix_socket_client.read_socket(buffer, buffer_len, errors)) {
        std::cout << "test_client: received '" << buffer << "'" << std::endl;
        reply = buffer;
      }
    }
    std::string response_txt;
    if (errors.has())
      response_txt = errors.to_string();
    else
      response_txt = reply;
    auto dto = Response_DTO::createShared();
    dto->statusCode = 200;
    dto->message = response_txt;
    return createDtoResponse(Status::CODE_200, dto);
  }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen

