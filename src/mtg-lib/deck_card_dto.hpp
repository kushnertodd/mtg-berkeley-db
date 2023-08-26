#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Deck_card_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_deck_card() { return "Deck_card_DTO"; }
  std::string deck_card_id{};
  std::string deck_id{};
  std::string card_id{};
  Deck_card_DTO() = default;
  explicit Deck_card_DTO(void *buffer);
  Deck_card_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  static int get_deck_card_deck_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  static int get_deck_card_card_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  [[nodiscard]] inline std::string get_id() const { return deck_card_id; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_card_DTO_key {
 public:
  std::string deck_card_id{};
  Deck_card_DTO_key() = default;
  explicit Deck_card_DTO_key(const Deck_card_DTO &deck_card_dto);
  explicit Deck_card_DTO_key(std::string deck_card_id_);
  explicit Deck_card_DTO_key(void *buffer);
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_card_DTO_list {
 public:
  static std::string class_deck_card() { return "Deck_card_DTO_list"; }
  std::list<Deck_card_DTO> list;
  void add(Deck_card_DTO &deck_card_dto) { list.push_back(deck_card_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Deck_card_DTO_key_list {
 public:
  std::list<Deck_card_DTO_key> list;
  void add(Deck_card_DTO_key &deck_card_dto_key) { list.push_back(deck_card_dto_key); }
};