#include <sstream>
#include <utility>
#include "bdb_serialization.hpp"
#include "bdb_tripthongs_dto.hpp"

// Bdb_text_id_occurence methods

[[nodiscard]] size_t Bdb_text_id_occurence::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(id);
  len += sizeof(tripthongs_count);
  len += sizeof(occurrence_count);
  return len;
}
void *Bdb_text_id_occurence::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(tripthongs_count, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(occurrence_count, p);
  return p;
}
void *Bdb_text_id_occurence::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(tripthongs_count, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(occurrence_count, p);
  return p;
}

// Bdb_text_id_occurrence_list methods

void Bdb_text_id_occurrence_list::add(const Bdb_text_id_occurence &bdb_text_id_occurence) {
  list.push_back(bdb_text_id_occurence);
}

// Bdb_text_tripthong_occurence methods

size_t Bdb_text_tripthong_occurence::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(tripthong);
  len += sizeof(occurrence_count);
  return len;
}
void *Bdb_text_tripthong_occurence::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(tripthong, p);
  p = (unsigned char *) Bdb_serialization::deserialize_int(occurrence_count, p);
  return p;
}
void *Bdb_text_tripthong_occurence::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(tripthong, p);
  p = (unsigned char *) Bdb_serialization::serialize_int(occurrence_count, p);
  return p;
}

// Bdb_text_tripthongs methods

Bdb_text_tripthongs::Bdb_text_tripthongs(const std::string &text) {
  auto tripthong_list = Bdb_tokens::tokenize_tripthongs(text, ' ');
  tripthongs_count = static_cast<int>(tripthong_list.size());
  std::map<std::string, int> tripthong_count_map;
  for (auto const &tripthong: tripthong_list)
    tripthong_count_map[tripthong]++;
  for (auto const &tripthong_occurence_entry: tripthong_count_map)
    list.emplace_back(tripthong_occurence_entry.first,
                      tripthong_occurence_entry.second);
}

// Bdb_tripthongs_DTO methods

Bdb_tripthongs_DTO::Bdb_tripthongs_DTO(void *buffer) {
  deserialize(buffer);
}

size_t Bdb_tripthongs_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(tripthong);
  len += sizeof(int);
  for (const auto &bdb_id_occurrence: list) {
    len += Bdb_serialization::buffer_len_string(bdb_id_occurrence.id);
    len += sizeof(bdb_id_occurrence.tripthongs_count);
    len += sizeof(bdb_id_occurrence.occurrence_count);
  }
  return len;
}

void *Bdb_tripthongs_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(tripthong, p);
  int n_id_tripthongs = static_cast<int>(list.size());
  p = (unsigned char *) Bdb_serialization::deserialize_int(n_id_tripthongs, p);
  for (int i = 0; i < n_id_tripthongs; i++) {
    Bdb_text_id_occurence bdb_text_id_occurence;
    p = (unsigned char *) bdb_text_id_occurence.deserialize(p);
    list.push_back(bdb_text_id_occurence);
  }
  return p;
}

void *Bdb_tripthongs_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(tripthong, p);
  int n_id_tripthongs = static_cast<int>(list.size());
  p = (unsigned char *) Bdb_serialization::serialize_int(n_id_tripthongs, p);
  for (const auto &id_tripthong_occurrence: list) {
    p = (unsigned char *) id_tripthong_occurrence.serialize(p);
  }
  return p;
}

// Bdb_tripthongs_DTO_key methods

Bdb_tripthongs_DTO_key::Bdb_tripthongs_DTO_key(const Bdb_tripthongs_DTO &bdb_tripthongs_dto) :
    tripthong(bdb_tripthongs_dto.tripthong) {
}

Bdb_tripthongs_DTO_key::Bdb_tripthongs_DTO_key(std::string tripthong_) :
    tripthong(std::move(tripthong_)) {
}

Bdb_tripthongs_DTO_key::Bdb_tripthongs_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Bdb_tripthongs_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(tripthong);
  return len;
}

void *Bdb_tripthongs_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(tripthong, p);
  return p;
}

void *Bdb_tripthongs_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(tripthong, p);
  return p;
}
