#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "cards_dto.hpp"

Cards_DTO::Cards_DTO(void *buffer) {
  deserialize(buffer);
}

Cards_DTO::Cards_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Cards_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(cards_id);
  len += Bdb_serialization::buffer_len_string(name);
  len += Bdb_serialization::buffer_len_string(type_id);
  return len;
}

void *Cards_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(cards_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(type_id, p);
  return p;
}

void Cards_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_cards": ... `
  std::string jobj_class_cards =
      Bdb_json_utils::get_json_string("Cards_DTO::from_json", "1", jobj,
                                      "class_cards", errors);
  if (!errors.has() && jobj_class_cards != class_cards())
    errors.add("Cards_DTO::from_json", "2", "not class Cards_DTO");
  // parse: ' { "cards_id": ... `
  cards_id = Bdb_json_utils::get_json_string("Cards_DTO::from_json", "1", jobj, "cards_id", errors);
  if (!errors.has())
    // parse: ' { primaryCards": ... `
    name = Bdb_json_utils::get_json_string("Cards_DTO::from_json", "4", jobj, "primaryCards", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    type_id = Bdb_json_utils::get_json_string("Cards_DTO::from_json", "5", jobj, "type_id", errors);
}

void Cards_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        cards_id = token_str;
        if (cards_id == "\\N")
          errors.add("Cards_DTO::create", "1", "required cards_id == '\\N'");
        break;
      }
      case 1: {
        name = token_str;
        if (name == "\\N")
          errors.add("Cards_DTO::create", "2", "required primaryCards_id == '\\N'");
        break;
      }
      case 2: {
        type_id = (token_str == "\\N" ? "" : token_str);
        break;
      }
      default: {
        errors.add("Cards_DTO::create", "3", "too many cards fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 3) {
    errors.add("Cards_DTO::create", "4", "too few cards fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Cards_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(cards_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(type_id, p);
  return p;
}

json_object *Cards_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Cards_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_cards", json_object_new_string(class_cards().c_str()));
  json_object_object_add(root, "cards_id", json_object_new_string(cards_id.c_str()));
  json_object_object_add(root, "primaryCards", json_object_new_string(name.c_str()));
  json_object_object_add(root, "birthYear", json_object_new_string(type_id.c_str()));
  return root;
}

std::string Cards_DTO::to_string() const {
  std::ostringstream os;
  os << "cards:" << std::endl;
  os << "\tcards_id        " << cards_id << std::endl;
  os << "\tprimaryCards    " << name << std::endl;
  os << "\tbirthYear      " << type_id << std::endl;
  return os.str();
}

Cards_DTO_key::Cards_DTO_key(const Cards_DTO &cards_DTO) : cards_id(cards_DTO.cards_id) {}

Cards_DTO_key::Cards_DTO_key(std::string cards_id_) : cards_id(std::move(cards_id_)) {}

Cards_DTO_key::Cards_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Cards_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(cards_id);
  return len;
}

void *Cards_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(cards_id, p);
  return p;
}

void *Cards_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(cards_id, p);
  return p;
}

std::string Cards_DTO_key::to_string() const {
  return "cards_id: " + cards_id;
}

json_object *Cards_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_cards", json_object_new_string(class_cards().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *cards_dto_list_json = json_object_new_array();
    json_object_object_add(root, "cards_dto_list", cards_dto_list_json);
    for (const auto &cards_dto: list) {
      json_object *cards_dto_json = cards_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(cards_dto_list_json, cards_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
