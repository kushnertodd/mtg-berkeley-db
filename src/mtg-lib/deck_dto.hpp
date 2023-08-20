#pragma once

#include <json-c/json.h>
#include <list>
#include "bdb_key_extractor.hpp"
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"
#include "deck_card_dto.hpp"

class Deck_DTO { // use Mtg_DTO as pattern but do not inherit
 public:
  static std::string class_deck() { return "Deck_DTO"; }
  std::string deck_id{};
  std::string account_id{};
  std::string name{};
  std::string score{};

  Deck_DTO() = default;
  Deck_DTO(const Deck_DTO &deck_) = default;
  Deck_DTO(Deck_DTO &&deck_) = default;
  Deck_DTO &operator=(const Deck_DTO &deck_) = default;
  Deck_DTO &operator=(Deck_DTO &&deck_) = default;
  explicit Deck_DTO(void *buffer);
  Deck_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void from_json(json_object *jobj, Bdb_errors &errors);
  [[nodiscard]] inline std::string get_id() const { return deck_id; }
  [[nodiscard]] inline std::string get_text() const { return name; }
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
  json_object *to_json(Bdb_errors &errors) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_DTO_key {
 public:
  std::string deck_id{};
  Deck_DTO_key() = default;
  Deck_DTO_key(const Deck_DTO_key &deck_key) = default;
  Deck_DTO_key(Deck_DTO_key &&deck_key) noexcept = default;
  explicit Deck_DTO_key(const Deck_DTO &deck_dto);
  explicit Deck_DTO_key(std::string deck_id_);
  explicit Deck_DTO_key(void *buffer);
  explicit Deck_DTO_key(const Deck_card_DTO &deck_card_DTO);
  Deck_DTO_key &operator=(const Deck_DTO_key &deck_key) = default;
  Deck_DTO_key &operator=(Deck_DTO_key &&deck_key) noexcept = default;

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
  [[nodiscard]] std::string to_string() const;
};

class Deck_DTO_list {
 public:
  static std::string class_deck() { return "Deck_DTO_list"; }
  std::list<Deck_DTO> list;
  void add(Deck_DTO &deck_dto) { list.push_back(deck_dto); }
  json_object *to_json(Bdb_errors &errors) const;
};

class Deck_DTO_key_list {
 public:
  std::list<Deck_DTO_key> list;
  void add(Deck_DTO_key &deck_dto_key) { list.push_back(deck_dto_key); }
};
