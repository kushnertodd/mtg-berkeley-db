#pragma once

#include <json-c/json.h>
#include <list>
#include <utility>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"
#include "deck_card_dto.hpp"

class Card_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_card() { return "Card_DTO"; }
  std::string card_id{};
  std::string name{};
  std::string type_id{};
  std::string score{};

  Card_DTO() = default;
  Card_DTO(std::string card_id_,
           std::string name_,
           std::string type_id_);
  explicit Card_DTO(void *buffer);
  Card_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_card_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_card_type_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return card_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Card_DTO_key {
 public:
  std::string card_id{};
  Card_DTO_key() = default;
  explicit Card_DTO_key(const Card_DTO &card_dto);
  explicit Card_DTO_key(std::string card_id_);
  explicit Card_DTO_key(void *buffer);
  explicit Card_DTO_key(const Deck_card_DTO &deck_card_dto);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Card_DTO_name_key {
 public:
  std::string name;
  Card_DTO_name_key() = default;
  explicit Card_DTO_name_key(std::string name_) :
      name(std::move(name_)) {}

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Card_DTO_type_id_key {
 public:
  std::string type_id;
  Card_DTO_type_id_key() = default;
  explicit Card_DTO_type_id_key(std::string type_id_) :
      type_id(std::move(type_id_)) {}

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Card_DTO_list {
 public:
  static std::string class_card() { return "Card_DTO_list"; }
  std::list<Card_DTO> list;
  void add(Card_DTO &card_dto) { list.push_back(card_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Card_DTO_key_list {
 public:
  std::list<Card_DTO_key> list;
  void add(Card_DTO_key &card_dto_key) { list.push_back(card_dto_key); }
};