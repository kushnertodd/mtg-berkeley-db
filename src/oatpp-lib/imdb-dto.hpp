#pragma once

#include "oatpp/core/Types.hpp"
#include "oatpp/core/macro/codegen.hpp"

#include OATPP_CODEGEN_BEGIN(DTO) ///< Begin DTO codegen section


/*
  {
    "class_name":"Bdb_errors",
    "errors":[
      "error1",
      "error2"
     ]
  }
*/
class Errors_DTO : public oatpp::DTO {
  DTO_INIT(Errors_DTO, DTO /* extends */)

  DTO_FIELD(String, class_name);
  DTO_FIELD(String, errors);
};

/*
{
  "class_name": "Imdb_request",
  "request": "lookup_name",
  "arguments": [
    "nm0000026"
  ]
}
*/
class Imdb_request_DTO : public oatpp::DTO {
  DTO_INIT(Imdb_request_DTO, DTO /* extends */)

  DTO_FIELD(String, class_name);
  DTO_FIELD(String, request);
  DTO_FIELD(List < String >, arguments);
};

class Imdb_response_DTO : public oatpp::DTO {
  DTO_INIT(Imdb_response_DTO, DTO /* extends */)
  DTO_FIELD(String, class_name);
  DTO_FIELD(String, text);
};

/*
  {
    "class_name":"Name",
    "name_id":"nm0000026",
    "primaryName":"Cary Grant",
    "birthYear":"1904",
    "deathYear":"1986",
    "line":"nm0000026\tCary Grant\t1904\t1986\tactor,soundtrack,producer\ttt0053125,tt0034248,tt0056923,tt0038787"
  }
*/
class Name_DTO : public oatpp::DTO {
  DTO_INIT(Name_DTO, DTO /* extends */)

  DTO_FIELD(String, class_name);
  DTO_FIELD(String, name_id);
  DTO_FIELD(String, primaryName);
  DTO_FIELD(String, birthYear);
  DTO_FIELD(String, deathYear);
  DTO_FIELD(String, line);
};

/*
  {
    "class_name":"Names",
    "names":[
      {
        "class_name":"Name",
        "name_id":"nm0000030",
        "primaryName":"Audrey Hepburn",
        "birthYear":"1929",
        "deathYear":"1993",
        "line":"nm0000030\tAudrey Hepburn\t1929\t1993\tactress,soundtrack\ttt0046250,tt0054698,tt0056923,tt0058385"
      }
    ]
  }
*/
class Names_DTO : public oatpp::DTO {
  DTO_INIT(Names_DTO, DTO);

  DTO_FIELD(String, class_name);
  DTO_FIELD(List < Object < Name_DTO >>, names);
};

/*
  {
    "class_name":"Title",
    "title_id":"tt0060522",
    "titleType":"movie",
    "primaryTitle":"How to Steal a Million",
    "isAdult":"0",
    "startYear":"1966",
    "runtimeMinutes":"123",
    "genres":"Comedy,Crime,Romance",
    "line":"tt0060522\tmovie\tHow to Steal a Million\tHow to Steal a Million\t0\t1966\t\\N\t123\tComedy,Crime,Romance"
  }
*/
class Title_DTO : public oatpp::DTO {
  DTO_INIT(Title_DTO, DTO /* extends */)

  DTO_FIELD(String, class_name);
  DTO_FIELD(String, title_id);
  DTO_FIELD(String, titleType);
  DTO_FIELD(String, primaryTitle);
  DTO_FIELD(String, isAdult);
  DTO_FIELD(String, startYear);
  DTO_FIELD(String, runtimeMinutes);
  DTO_FIELD(String, genres);
  DTO_FIELD(String, line);
};

/*
  {
    "class_name":"Titles",
    "titles":[
      {
        "class_name":"Title",
        "title_id":"tt0053125",
        "titleType":"movie",
        "primaryTitle":"North by Northwest",
        "isAdult":"0",
        "startYear":"1959",
        "runtimeMinutes":"136",
        "genres":"Action,Adventure,Mystery",
        "line":"tt0053125\tmovie\tNorth by Northwest\tNorth by Northwest\t0\t1959\t\\N\t136\tAction,Adventure,Mystery"
      },
      {
        "class_name":"Title",
        "title_id":"tt0056923",
        "titleType":"movie",
        "primaryTitle":"Charade",
        "isAdult":"0",
        "startYear":"1963",
        "runtimeMinutes":"113",
        "genres":"Comedy,Mystery,Romance",
        "line":"tt0056923\tmovie\tCharade\tCharade\t0\t1963\t\\N\t113\tComedy,Mystery,Romance"
      }
    ]
  }
*/
class Titles_DTO : public oatpp::DTO {
  DTO_INIT(Titles_DTO, DTO /* extends */)

  DTO_FIELD(String, class_name);
  DTO_FIELD(List < Object < Title_DTO >>, titles);
};

#include OATPP_CODEGEN_END(DTO) ///< End DTO codegen section
