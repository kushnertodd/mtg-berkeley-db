#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "card_dto.hpp"
#include "deck_card_dto.hpp"

Card_DTO::Card_DTO(std::string card_id_,
                   std::string name_,
                   std::string type_id_) :
    card_id(std::move(card_id_)),
    name(std::move(name_)),
    type_id(std::move(type_id_)) {}

Card_DTO::Card_DTO(void *buffer) {
  deserialize(buffer);
}

Card_DTO::Card_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Card_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(card_id);
  len += Bdb_serialization::buffer_len_string(name);
  len += Bdb_serialization::buffer_len_string(type_id);
  return len;
}

void *Card_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(card_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(type_id, p);
  return p;
}

void Card_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_card": ... `
  std::string jobj_class_card =
      Bdb_json_utils::get_json_string("Card_DTO::from_json", "1", jobj,
                                      "class_card", errors);
  if (!errors.has() && jobj_class_card != class_card())
    errors.add("Card_DTO::from_json", "2", "not class Card_DTO");
  // parse: ' { "card_id": ... `
  card_id = Bdb_json_utils::get_json_string("Card_DTO::from_json", "3", jobj, "card_id", errors);
  if (!errors.has())
    // parse: ' { name": ... `
    name = Bdb_json_utils::get_json_string("Card_DTO::from_json", "4", jobj, "name", errors);
  if (!errors.has())
    // parse: ' { "type_id": ... `
    type_id = Bdb_json_utils::get_json_string("Card_DTO::type_id", "5", jobj, "type_id", errors);
}

int Card_DTO::get_card_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Card_DTO card_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string name = card_dto.name;
  size_t keylen = name.size() + 1;
  char *card_id_buf = (char *) malloc(keylen);
  std::strcpy(card_id_buf, name.c_str());
  skey->set_data(card_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Card_DTO::get_card_type_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Card_DTO card_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string type_id = card_dto.type_id;
  size_t keylen = type_id.size() + 1;
  char *card_id_buf = (char *) malloc(keylen);
  std::strcpy(card_id_buf, type_id.c_str());
  skey->set_data(card_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Card_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        card_id = token_str;
        break;
      }
      case 1: {
        name = token_str;
        break;
      }
      case 2: {
        type_id = token_str;
        break;
      }
      default: {
        errors.add("Card_DTO::create", "1", "too many card fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i < 3) {
    errors.add("Card_DTO::parse", "2", "too few card fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Card_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(card_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(type_id, p);
  return p;
}

json_object *Card_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Card_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_card", json_object_new_string(class_card().c_str()));
  json_object_object_add(root, "card_id", json_object_new_string(card_id.c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  json_object_object_add(root, "type_id", json_object_new_string(type_id.c_str()));
  json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Card_DTO::to_string() const {
  std::ostringstream os;
  os << "card:" << std::endl;
  os << "\tcard_id        " << card_id << std::endl;
  os << "\tname    " << name << std::endl;
  os << "\ttype_id      " << type_id << std::endl;
  return os.str();
}

Card_DTO_key::Card_DTO_key(const Card_DTO &card_DTO) :
    card_id(card_DTO.card_id) {}

Card_DTO_key::Card_DTO_key(std::string card_id_) :
    card_id(std::move(card_id_)) {}

Card_DTO_key::Card_DTO_key(void *buffer) {
  deserialize(buffer);
}

Card_DTO_key::Card_DTO_key(const Deck_card_DTO &deck_card_dto) :
    card_id(deck_card_dto.card_id) {}

size_t Card_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(card_id);
  return len;
}

void *Card_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(card_id, p);
  return p;
}

void *Card_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(card_id, p);
  return p;
}

std::string Card_DTO_key::to_string() const {
  return "card_id: " + card_id;
}

size_t Card_DTO_name_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Card_DTO_name_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void *Card_DTO_name_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

std::string Card_DTO_name_key::to_string() const {
  return "name: " + name;
}

size_t Card_DTO_type_id_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(type_id);
  return len;
}

void *Card_DTO_type_id_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(type_id, p);
  return p;
}

void *Card_DTO_type_id_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(type_id, p);
  return p;
}

std::string Card_DTO_type_id_key::to_string() const {
  return "type_id: " + type_id;
}

json_object *Card_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Card_DTO_list::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_card", json_object_new_string(class_card().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *card_dto_list_json = json_object_new_array();
    json_object_object_add(root, "card_dto_list", card_dto_list_json);
    for (const auto &card_dto: list) {
      json_object *card_dto_json = card_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(card_dto_list_json, card_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
