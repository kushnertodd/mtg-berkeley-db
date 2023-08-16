#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Deck_cards_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_deck_cards() { return "Deck_cards_DTO"; }
  std::string deck_cards_id{};
  std::string deck_id{};
  std::string card_id{};

  Deck_cards_DTO() = default;
  Deck_cards_DTO(const Deck_cards_DTO &deck_cards_) = default;
  Deck_cards_DTO(Deck_cards_DTO &&deck_cards_) = default;
  Deck_cards_DTO &operator=(const Deck_cards_DTO &deck_cards_) = default;
  Deck_cards_DTO &operator=(Deck_cards_DTO &&deck_cards_) = default;
  explicit Deck_cards_DTO(void *buffer);

  Deck_cards_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return deck_cards_id; }
  [[nodiscard]] inline std::string get_text() const { return deck_id; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_cards_DTO_key {
 public:
  std::string deck_cards_id{};
  Deck_cards_DTO_key() = default;
  Deck_cards_DTO_key(const Deck_cards_DTO_key &deck_cards_key) = default;
  Deck_cards_DTO_key(Deck_cards_DTO_key &&deck_cards_key) noexcept = default;
  explicit Deck_cards_DTO_key(const Deck_cards_DTO &deck_cards_dto);
  explicit Deck_cards_DTO_key(std::string deck_cards_id_);
  explicit Deck_cards_DTO_key(void *buffer);
  Deck_cards_DTO_key &operator=(const Deck_cards_DTO_key &deck_cards_key) = default;
  Deck_cards_DTO_key &operator=(Deck_cards_DTO_key &&deck_cards_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_cards_DTO_list {
 public:
  static std::string class_deck_cards() { return "Deck_cards_DTO_list"; }
  std::list<Deck_cards_DTO> list;
  void add(Deck_cards_DTO &deck_cards_dto) { list.push_back(deck_cards_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Deck_cards_DTO_key_list {
 public:
  static std::string class_deck_cards() { return "Deck_cards_DTO_key_list"; }
  std::list<Deck_cards_DTO_key> list;
  void add(Deck_cards_DTO_key &deck_cards_dto_key) { list.push_back(deck_cards_dto_key); }
};