#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "card_dto.hpp"
#include "deck_card_dto.hpp"

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
  card_id = Bdb_json_utils::get_json_string("Card_DTO::from_json", "1", jobj, "card_id", errors);
  if (!errors.has())
    // parse: ' { primaryCard": ... `
    name = Bdb_json_utils::get_json_string("Card_DTO::from_json", "4", jobj, "primaryCard", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    type_id = Bdb_json_utils::get_json_string("Card_DTO::from_json", "5", jobj, "type_id", errors);
}

void Card_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        card_id = token_str;
        if (card_id == "\\N")
          errors.add("Card_DTO::create", "1", "required card_id == '\\N'");
        break;
      }
      case 1: {
        name = token_str;
        if (name == "\\N")
          errors.add("Card_DTO::create", "2", "required primaryCard_id == '\\N'");
        break;
      }
      case 2: {
        type_id = (token_str == "\\N" ? "" : token_str);
        break;
      }
      default: {
        errors.add("Card_DTO::create", "3", "too many card fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 3) {
    errors.add("Card_DTO::create", "4", "too few card fields on line "
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
  json_object_object_add(root, "primaryCard", json_object_new_string(name.c_str()));
  json_object_object_add(root, "birthYear", json_object_new_string(type_id.c_str()));
  return root;
}

std::string Card_DTO::to_string() const {
  std::ostringstream os;
  os << "card:" << std::endl;
  os << "\tcard_id        " << card_id << std::endl;
  os << "\tprimaryCard    " << name << std::endl;
  os << "\tbirthYear      " << type_id << std::endl;
  return os.str();
}

Card_DTO_key::Card_DTO_key(const Card_DTO &card_DTO) : card_id(card_DTO.card_id) {}

Card_DTO_key::Card_DTO_key(std::string card_id_) : card_id(std::move(card_id_)) {}

Card_DTO_key::Card_DTO_key(void *buffer) {
  deserialize(buffer);
}

Card_DTO_key::Card_DTO_key(const Deck_card_DTO &deck_card_DTO) :
    card_id(deck_card_DTO.card_id) {}

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

json_object *Card_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
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
