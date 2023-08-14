#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "principals_dto.hpp"
#include "bdb_serializable.hpp"

/*
 * nconst     primaryName   birthYear  deathYear  primaryProfession     knownForTitle
 * nm0000001  Fred Astaire  1899       1987       soundtrack,actor,...  tt0053137,tt0045537,...
 *
 * name_id            nm0000001
 * primaryName        Fred Astaire
 * birthYear          1899
 * deathYear          1987
 * primaryProfession  soundtrack,actor,miscellaneous
 * knownForTitle      tt0053137,tt0045537,tt0072308,tt0050419
 *
 */


class Name_DTO { // use Imdb_DTO as pattern but do not inherit
 public:
  static std::string class_name() { return "Name_DTO"; }
  std::string name_id{};
  std::string primary_name{};
  std::string birth_year{};
  std::string death_year{};
  std::string user_rating{};
  std::string priority{};
  std::string score{};

  Name_DTO() = default;
  Name_DTO(const Name_DTO &name_) = default;
  Name_DTO(Name_DTO &&name_) = default;
  Name_DTO &operator=(const Name_DTO &name_) = default;
  Name_DTO &operator=(Name_DTO &&name_) = default;
  explicit Name_DTO(void *buffer);
  explicit Name_DTO(const Principals_DTO &principals_dto);
  Name_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return name_id; }
  [[nodiscard]] inline std::string get_text() const { return primary_name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Name_DTO_key {
 public:
  std::string name_id{};
  Name_DTO_key() = default;
  Name_DTO_key(const Name_DTO_key &name_key) = default;
  Name_DTO_key(Name_DTO_key &&name_key) noexcept = default;
  explicit Name_DTO_key(const Name_DTO &name_dto);
  explicit Name_DTO_key(const Principals_DTO &principals_dto);
  explicit Name_DTO_key(std::string name_id_);
  explicit Name_DTO_key(void *buffer);
  Name_DTO_key &operator=(const Name_DTO_key &name_key) = default;
  Name_DTO_key &operator=(Name_DTO_key &&name_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Name_DTO_list {
 public:
  static std::string class_name() { return "Name_DTO_list"; }
  std::list<Name_DTO> list;
  void add(Name_DTO &name_dto) { list.push_back(name_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Name_DTO_key_list {
 public:
  static std::string class_name() { return "Name_DTO_key_list"; }
  std::list<Name_DTO_key> list;
  void add(Name_DTO_key &name_dto_key) { list.push_back(name_dto_key); }
};