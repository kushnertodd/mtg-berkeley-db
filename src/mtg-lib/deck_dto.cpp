#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "deck_dto.hpp"

Deck_DTO::Deck_DTO(void *buffer) {
  deserialize(buffer);
}

Deck_DTO::Deck_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Deck_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_id);
  len += Bdb_serialization::buffer_len_string(account_id);
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Deck_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void Deck_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_deck": ... `
  std::string jobj_class_deck =
      Bdb_json_utils::get_json_string("Deck_DTO::from_json", "1", jobj,
                                      "class_deck", errors);
  if (!errors.has() && jobj_class_deck != class_deck())
    errors.add("Deck_DTO::from_json", "2", "not class Deck_DTO");
  // parse: ' { "deck_id": ... `
  if (!errors.has())
    deck_id = Bdb_json_utils::get_json_string("Deck_DTO::from_json", "1", jobj, "deck_id", errors);
  if (!errors.has())
    account_id = Bdb_json_utils::get_json_string("Deck_DTO::from_json", "2", jobj, "account_id", errors);
  if (!errors.has())
    // parse: ' { primaryDeck": ... `
    name = Bdb_json_utils::get_json_string("Deck_DTO::from_json", "3", jobj, "name", errors);
}

int Deck_DTO::get_deck_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Deck_DTO deck_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string name = deck_dto.name;
  size_t keylen = name.size() + 1;
  char *deck_id_buf = (char *) malloc(keylen);
  std::strcpy(deck_id_buf, name.c_str());
  skey->set_data(deck_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Deck_DTO::get_deck_account_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Deck_DTO deck_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string account_id = deck_dto.account_id;
  size_t keylen = account_id.size() + 1;
  char *deck_id_buf = (char *) malloc(keylen);
  std::strcpy(deck_id_buf, account_id.c_str());
  skey->set_data(deck_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Deck_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        deck_id = token_str;
        break;
      }
      case 1: {
        account_id = token_str;
        break;
      }
      case 2: {
        name = token_str;
        break;
      }
      default: {
        errors.add("Deck_DTO::create", "3", "too many deck fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i < 3) {
    errors.add("Deck_DTO::create", "4", "too few deck fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Deck_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

json_object *Deck_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Deck_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck", json_object_new_string(class_deck().c_str()));
  json_object_object_add(root, "deck_id", json_object_new_string(deck_id.c_str()));
  json_object_object_add(root, "account_id", json_object_new_string(deck_id.c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  return root;
}

std::string Deck_DTO::to_string() const {
  std::ostringstream os;
  os << "deck:" << std::endl;
  os << "\tdeck_id        " << deck_id << std::endl;
  os << "\taccount_id     " << deck_id << std::endl;
  os << "\tname           " << name << std::endl;
  return os.str();
}

Deck_DTO_key::Deck_DTO_key(const Deck_DTO &deck_dto) : deck_id(deck_dto.deck_id) {}

Deck_DTO_key::Deck_DTO_key(std::string deck_id_) : deck_id(std::move(deck_id_)) {}

Deck_DTO_key::Deck_DTO_key(void *buffer) {
  deserialize(buffer);
}

Deck_DTO_key::Deck_DTO_key(const Deck_card_DTO &deck_card_dto) :
    deck_id(deck_card_dto.deck_id) {}

size_t Deck_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(deck_id);
  return len;
}

void *Deck_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(deck_id, p);
  return p;
}

void *Deck_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(deck_id, p);
  return p;
}

std::string Deck_DTO_key::to_string() const {
  return "deck_id: " + deck_id;
}

size_t Deck_DTO_account_id_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(account_id);
  return len;
}

void *Deck_DTO_account_id_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(account_id, p);
  return p;
}

void *Deck_DTO_account_id_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(account_id, p);
  return p;
}

std::string Deck_DTO_account_id_key::to_string() const {
  return "account_id: " + account_id;
}

size_t Deck_DTO_name_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Deck_DTO_name_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void *Deck_DTO_name_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

std::string Deck_DTO_name_key::to_string() const {
  return "name: " + name;
}

json_object *Deck_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_deck", json_object_new_string(class_deck().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *deck_dto_list_json = json_object_new_array();
    json_object_object_add(root, "deck_dto_list", deck_dto_list_json);
    for (const auto &deck_dto: list) {
      json_object *deck_dto_json = deck_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(deck_dto_list_json, deck_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
