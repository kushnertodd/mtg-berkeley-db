#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "principals_dto.hpp"
#include "bdb_serializable.hpp"

/*
 * tconst     averageRating  numVotes
 * tt0053125  8.3            330190
 *
 * ratings_DTO

 * 0 tconst         tt0053125  
 * 1 averageRating  8.3            
 * 2 numVotes       330190
 *
 */

class Ratings_DTO { // use Imdb_DTO as pattern but do not inherit
 public:
  static std::string class_name() { return "Ratings_DTO"; }
  std::string title_id{};
  std::string averageRating{};
  std::string numVotes{};

  Ratings_DTO() = default;
  Ratings_DTO(const Ratings_DTO &ratings_) = default;
  Ratings_DTO(Ratings_DTO &&ratings_) = default;
  Ratings_DTO &operator=(const Ratings_DTO &ratings_) = default;
  Ratings_DTO &operator=(Ratings_DTO &&ratings_) = default;
  explicit Ratings_DTO(void *buffer);
  Ratings_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return title_id; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Ratings_DTO_key {
 public:
  std::string title_id{};
  Ratings_DTO_key() = default;
  Ratings_DTO_key(const Ratings_DTO_key &ratings_key) = default;
  Ratings_DTO_key(Ratings_DTO_key &&ratings_key) noexcept = default;
  explicit Ratings_DTO_key(const Ratings_DTO &ratings_dto);
  explicit Ratings_DTO_key(const Principals_DTO &principals_dto);
  explicit Ratings_DTO_key(std::string ratings_id_);
  explicit Ratings_DTO_key(void *buffer);
  Ratings_DTO_key &operator=(const Ratings_DTO_key &ratings_key) = default;
  Ratings_DTO_key &operator=(Ratings_DTO_key &&ratings_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Ratings_DTO_list {
 public:
  static std::string class_name() { return "Ratings_DTO_list"; }
  std::list<Ratings_DTO> list;
  void add(Ratings_DTO &ratings_dto) { list.push_back(ratings_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};
