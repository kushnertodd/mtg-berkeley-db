#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "deck_card_dto.hpp"

Deck_card_DTO::Deck_card_DTO(std::string deck_id_, std::string card_id_) :
    deck_id(std::move(deck_id_)),
    card_id(std::move(card_id_)) {}

Deck_card_DTO::Deck_card_DTO(Deck_card_DTO deck_card_DTO, std::string &deck_card_id_) :
    deck_card_id(std::move(deck_card_id_)),
    deck_id(std::move(deck_card_DTO.deck_id)),
    card_id(std::move(deck_card_DTO.card_id)) {}

Deck_card_DTO::Deck_card_DTO(void *buffer) {
  deserialize(buffer);
}

Deck_card_DTO::Deck_card_DTO(int count,
                             const std::string &line,
                             Bdb_errors &errors,
                             char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Deck_card_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_card_id);
  len += Bdb_serialization::buffer_len_string(deck_id);
  len += Bdb_serialization::buffer_len_string(card_id);
  return len;
}

void Deck_card_DTO::create(const Deck_card_DTO &deck_card_dto,
                           const std::string &deck_card_id_) {
  deck_card_id = deck_card_id_;
  deck_id = deck_card_dto.deck_id;
  card_id = deck_card_dto.card_id;
}

void *Deck_card_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_card_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(card_id, p);
  return p;
}

void Deck_card_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_deck_card": ... `
  std::string jobj_class_deck_card =
      Bdb_json_utils::get_json_string("Deck_card_DTO::from_json", "1", jobj,
                                      "class_deck_card", errors);
  if (!errors.has() && jobj_class_deck_card != class_deck_card())
    errors.add("Deck_card_DTO::from_json", "2", "not class Deck_card_DTO");
  // parse: ' { "deck_card_id": ... `
  deck_card_id = Bdb_json_utils::get_json_string("Deck_card_DTO::from_json", "3", jobj, "deck_card_id", errors);
  if (!errors.has())
    // parse: ' { deck_id": ... `
    deck_id = Bdb_json_utils::get_json_string("Deck_card_DTO::from_json", "4", jobj, "deck_id", errors);
  if (!errors.has())
    // parse: ' { "card_id": ... `
    card_id = Bdb_json_utils::get_json_string("Deck_card_DTO::from_json", "5", jobj, "card_id", errors);
}

int Deck_card_DTO::get_deck_card_deck_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Deck_card_DTO deck_card_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string deck_id = deck_card_dto.deck_id;
  size_t keylen = deck_id.size() + 1;
  char *deck_id_buf = (char *) malloc(keylen);
  std::strcpy(deck_id_buf, deck_id.c_str());
  skey->set_data(deck_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Deck_card_DTO::get_deck_card_card_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Deck_card_DTO deck_card_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string card_id = deck_card_dto.card_id;
  size_t keylen = card_id.size() + 1;
  char *card_id_buf = (char *) malloc(keylen);
  std::strcpy(card_id_buf, card_id.c_str());
  skey->set_data(card_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Deck_card_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        deck_card_id = token_str;
        break;
      }
      case 1: {
        deck_id = token_str;
        break;
      }
      case 2: {
        card_id = token_str;
        break;
      }
      default: {
        errors.add("Deck_card_DTO::create", "1", "too many card fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i < 3) {
    errors.add("Deck_card_DTO::create", "2", "too few card fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Deck_card_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_card_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(card_id, p);
  return p;
}

json_object *Deck_card_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Deck_card_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck_card",
                         json_object_new_string(class_deck_card().c_str()));
  json_object_object_add(root, "deck_card_id",
                         json_object_new_string(deck_card_id.c_str()));
  json_object_object_add(root, "deck_id",
                         json_object_new_string(deck_id.c_str()));
  return root;
}

std::string Deck_card_DTO::to_string() const {
  std::ostringstream os;
  os << "deck_card:" << std::endl;
  os << "\tdeck_card_id " << deck_card_id << std::endl;
  os << "\tdeck_id      " << deck_id << std::endl;
  os << "\tcard_id      " << card_id << std::endl;
  return os.str();
}

Deck_card_DTO_key::Deck_card_DTO_key(const Deck_card_DTO &deck_card_dto) :
    deck_card_id(deck_card_dto.deck_card_id) {}

Deck_card_DTO_key::Deck_card_DTO_key(std::string deck_card_id_) :
    deck_card_id(std::move(deck_card_id_)) {}

Deck_card_DTO_key::Deck_card_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Deck_card_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_card_id);
  return len;
}

void *Deck_card_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_card_id, p);
  return p;
}

void *Deck_card_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_card_id, p);
  return p;
}

std::string Deck_card_DTO_key::to_string() const {
  return "deck_card_id: " + deck_card_id;
}

json_object *Deck_card_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Deck_card_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck_card",
                         json_object_new_string(class_deck_card().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *deck_card_dto_list_json = json_object_new_array();
    json_object_object_add(root, "deck_card_dto_list", deck_card_dto_list_json);
    for (const auto &deck_card_dto: list) {
      json_object *deck_card_dto_json = deck_card_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(deck_card_dto_list_json, deck_card_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
