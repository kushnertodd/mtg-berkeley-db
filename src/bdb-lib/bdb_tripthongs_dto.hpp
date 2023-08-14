#pragma once

#include <json-c/json.h>
#include <list>
#include <utility>
#include "bdb_tokens.hpp"
#include "bdb_serializable.hpp"

class Bdb_text_id_occurence {
 public:
  std::string id{};
  int tripthongs_count{};
  int occurrence_count{};
  Bdb_text_id_occurence() = default;
  Bdb_text_id_occurence(std::string id_, int tripthongs_count_, int occurrence_count_)
      : id(std::move(id_)),
        tripthongs_count(tripthongs_count_),
        occurrence_count(occurrence_count_) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_text_id_occurrence_list {
 public:
  std::vector<Bdb_text_id_occurence> list;
  void add(const Bdb_text_id_occurence &bdb_text_id_occurence);
};

class Bdb_text_tripthong_occurence {
 public:
  std::string tripthong{};
  int occurrence_count{};
  Bdb_text_tripthong_occurence() = default;
  Bdb_text_tripthong_occurence(std::string tripthong_, int occurrence_count_)
      : tripthong(std::move(tripthong_)), occurrence_count(occurrence_count_) {}
  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_text_tripthongs {
 public:
  int tripthongs_count{};
  std::list<Bdb_text_tripthong_occurence> list;
  Bdb_text_tripthongs() = default;
  explicit Bdb_text_tripthongs(const std::string &text);
};

class Bdb_tripthongs_DTO { // use Imdb_DTO as pattern but do not inherit
 public:
  std::string tripthong{};
  std::list<Bdb_text_id_occurence> list;

  Bdb_tripthongs_DTO() = default;
  explicit Bdb_tripthongs_DTO(std::string tripthong_) : tripthong(std::move(tripthong_)) {}
  explicit Bdb_tripthongs_DTO(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Bdb_tripthongs_DTO_key {
 public:
  std::string tripthong{};
  Bdb_tripthongs_DTO_key() = default;

  explicit Bdb_tripthongs_DTO_key(const Bdb_tripthongs_DTO &bdb_tripthongs_dto);
  explicit Bdb_tripthongs_DTO_key(std::string bdb_tripthong_);
  explicit Bdb_tripthongs_DTO_key(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};
