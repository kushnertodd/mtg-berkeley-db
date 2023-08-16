#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "deck_cards_dto.hpp"

Deck_cards_DTO::Deck_cards_DTO(void *buffer) {
  deserialize(buffer);
}

Deck_cards_DTO::Deck_cards_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Deck_cards_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_cards_id);
  len += Bdb_serialization::buffer_len_string(deck_id);
  len += Bdb_serialization::buffer_len_string(card_id);
  return len;
}

void *Deck_cards_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_cards_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(card_id, p);
  return p;
}

void Deck_cards_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_deck_cards": ... `
  std::string jobj_class_deck_cards =
      Bdb_json_utils::get_json_string("Deck_cards_DTO::from_json", "1", jobj,
                                      "class_deck_cards", errors);
  if (!errors.has() && jobj_class_deck_cards != class_deck_cards())
    errors.add("Deck_cards_DTO::from_json", "2", "not class Deck_cards_DTO");
  // parse: ' { "deck_cards_id": ... `
  deck_cards_id = Bdb_json_utils::get_json_string("Deck_cards_DTO::from_json", "1", jobj, "deck_cards_id", errors);
  if (!errors.has())
    // parse: ' { primaryDeck_cards": ... `
    deck_id = Bdb_json_utils::get_json_string("Deck_cards_DTO::from_json", "4", jobj, "primaryDeck_cards", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    card_id = Bdb_json_utils::get_json_string("Deck_cards_DTO::from_json", "5", jobj, "birthYear", errors);
}

void Deck_cards_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        deck_cards_id = token_str;
        if (deck_cards_id == "\\N")
          errors.add("Deck_cards_DTO::create", "1", "required deck_cards_id == '\\N'");
        break;
      }
      case 1: {
        deck_id = token_str;
        if (deck_id == "\\N")
          errors.add("Deck_cards_DTO::create", "2", "required primaryDeck_cards_id == '\\N'");
        break;
      }
      case 2: {
        break;
      }
      default: {
        errors.add("Deck_cards_DTO::create", "3", "too many deck_cards fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 3) {
    errors.add("Deck_cards_DTO::create", "4", "too few deck_cards fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Deck_cards_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_cards_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(card_id, p);
  return p;
}

json_object *Deck_cards_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Deck_cards_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck_cards", json_object_new_string(class_deck_cards().c_str()));
  json_object_object_add(root, "deck_cards_id", json_object_new_string(deck_cards_id.c_str()));
  json_object_object_add(root, "primaryDeck_cards", json_object_new_string(deck_id.c_str()));
  json_object_object_add(root, "birthYear", json_object_new_string(card_id.c_str()));
  return root;
}

std::string Deck_cards_DTO::to_string() const {
  std::ostringstream os;
  os << "deck_cards:" << std::endl;
  os << "\tdeck_cards_id        " << deck_cards_id << std::endl;
  os << "\tdeck_id    " << deck_id << std::endl;
  os << "\tcard_id      " << card_id << std::endl;
  return os.str();
}

Deck_cards_DTO_key::Deck_cards_DTO_key(const Deck_cards_DTO &deck_cards_DTO) : deck_cards_id(deck_cards_DTO.deck_cards_id) {}

Deck_cards_DTO_key::Deck_cards_DTO_key(std::string deck_cards_id_) : deck_cards_id(std::move(deck_cards_id_)) {}

Deck_cards_DTO_key::Deck_cards_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Deck_cards_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_cards_id);
  return len;
}

void *Deck_cards_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_cards_id, p);
  return p;
}

void *Deck_cards_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_cards_id, p);
  return p;
}

std::string Deck_cards_DTO_key::to_string() const {
  return "deck_cards_id: " + deck_cards_id;
}

json_object *Deck_cards_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck_cards", json_object_new_string(class_deck_cards().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *deck_cards_dto_list_json = json_object_new_array();
    json_object_object_add(root, "deck_cards_dto_list", deck_cards_dto_list_json);
    for (const auto &deck_cards_dto: list) {
      json_object *deck_cards_dto_json = deck_cards_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(deck_cards_dto_list_json, deck_cards_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
