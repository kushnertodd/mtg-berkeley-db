#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Card_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_card() { return "Card_DTO"; }
  std::string card_id{};
  std::string name{};
  std::string type_id{};

  Card_DTO() = default;
  Card_DTO(const Card_DTO &card_) = default;
  Card_DTO(Card_DTO &&card_) = default;
  Card_DTO &operator=(const Card_DTO &card_) = default;
  Card_DTO &operator=(Card_DTO &&card_) = default;
  explicit Card_DTO(void *buffer);
  Card_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
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
  Card_DTO_key(const Card_DTO_key &card_key) = default;
  Card_DTO_key(Card_DTO_key &&card_key) noexcept = default;
  explicit Card_DTO_key(const Card_DTO &card_dto);
  explicit Card_DTO_key(std::string card_id_);
  explicit Card_DTO_key(void *buffer);
  Card_DTO_key &operator=(const Card_DTO_key &card_key) = default;
  Card_DTO_key &operator=(Card_DTO_key &&card_key) noexcept = default;

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
  static std::string class_card() { return "Card_DTO_key_list"; }
  std::list<Card_DTO_key> list;
  void add(Card_DTO_key &card_dto_key) { list.push_back(card_dto_key); }
};