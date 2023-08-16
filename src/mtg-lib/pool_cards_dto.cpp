#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "pool_cards_dto.hpp"

Pool_cards_DTO::Pool_cards_DTO(void *buffer) {
  deserialize(buffer);
}

Pool_cards_DTO::Pool_cards_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Pool_cards_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_cards_id);
  len += Bdb_serialization::buffer_len_string(primary_pool_cards);
  len += Bdb_serialization::buffer_len_string(birth_year);
  len += Bdb_serialization::buffer_len_string(death_year);
  len += Bdb_serialization::buffer_len_string(user_rating);
  len += Bdb_serialization::buffer_len_string(priority);
  len += Bdb_serialization::buffer_len_string(score);
  return len;
}

void *Pool_cards_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_cards_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(primary_pool_cards, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(birth_year, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(death_year, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(score, p);
  return p;
}

void Pool_cards_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_pool_cards": ... `
  std::string jobj_class_pool_cards =
      Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "1", jobj,
                                      "class_pool_cards", errors);
  if (!errors.has() && jobj_class_pool_cards != class_pool_cards())
    errors.add("Pool_cards_DTO::from_json", "2", "not class Pool_cards_DTO");
  // parse: ' { "pool_cards_id": ... `
  pool_cards_id = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "1", jobj, "pool_cards_id", errors);
  if (!errors.has())
    // parse: ' { primaryPool_cards": ... `
    primary_pool_cards = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "4", jobj, "primaryPool_cards", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    birth_year = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "5", jobj, "birthYear", errors);
  if (!errors.has())
    // parse: ' { "deathYear": ... `
    death_year = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "6", jobj, "deathYear", errors);
  if (!errors.has())
    // parse: ' { "userRating": ... `
    user_rating = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "7", jobj, "userRating", errors);
  if (!errors.has())
    // parse: ' { "priority": ... `
    priority = Bdb_json_utils::get_json_string("Pool_cards_DTO::from_json", "8", jobj, "priority", errors);
  if (!errors.has())
    // parse: ' { "score": ... `
    score = Bdb_json_utils::get_json_string("Title_DTO::from_json", "9", jobj, "score", errors);
}

void Pool_cards_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        pool_cards_id = token_str;
        if (pool_cards_id == "\\N")
          errors.add("Pool_cards_DTO::create", "1", "required pool_cards_id == '\\N'");
        break;
      }
      case 1: {
        primary_pool_cards = token_str;
        if (primary_pool_cards == "\\N")
          errors.add("Pool_cards_DTO::create", "2", "required primaryPool_cards_id == '\\N'");
        break;
      }
      case 2: {
        birth_year = (token_str == "\\N" ? "" : token_str);
        break;
      }
      case 3: {
        death_year = (token_str == "\\N" ? "" : token_str);
        break;
      }
      case 4: // drop through
      case 5: {
        break;
      }
      default: {
        errors.add("Pool_cards_DTO::create", "3", "too many pool_cards fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 6) {
    errors.add("Pool_cards_DTO::create", "4", "too few pool_cards fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Pool_cards_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_cards_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(primary_pool_cards, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(birth_year, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(death_year, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(score, p);
  return p;
}

json_object *Pool_cards_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Pool_cards_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool_cards", json_object_new_string(class_pool_cards().c_str()));
  json_object_object_add(root, "pool_cards_id", json_object_new_string(pool_cards_id.c_str()));
  json_object_object_add(root, "primaryPool_cards", json_object_new_string(primary_pool_cards.c_str()));
  json_object_object_add(root, "birthYear", json_object_new_string(birth_year.c_str()));
  json_object_object_add(root, "deathYear", json_object_new_string(death_year.c_str()));
  json_object_object_add(root, "userRating", json_object_new_string(user_rating.c_str()));
  json_object_object_add(root, "priority", json_object_new_string(priority.c_str()));
  json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Pool_cards_DTO::to_string() const {
  std::ostringstream os;
  os << "pool_cards:" << std::endl;
  os << "\tpool_cards_id        " << pool_cards_id << std::endl;
  os << "\tprimaryPool_cards    " << primary_pool_cards << std::endl;
  os << "\tbirthYear      " << birth_year << std::endl;
  os << "\tdeathYear      " << death_year << std::endl;
  os << "\tuserRating      " << user_rating << std::endl;
  os << "\tpriority        " << priority << std::endl;
  os << "\tscore           " << score << std::endl;
  return os.str();
}

Pool_cards_DTO_key::Pool_cards_DTO_key(const Pool_cards_DTO &pool_cards_DTO) : pool_cards_id(pool_cards_DTO.pool_cards_id) {}

Pool_cards_DTO_key::Pool_cards_DTO_key(std::string pool_cards_id_) : pool_cards_id(std::move(pool_cards_id_)) {}

Pool_cards_DTO_key::Pool_cards_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Pool_cards_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_cards_id);
  return len;
}

void *Pool_cards_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_cards_id, p);
  return p;
}

void *Pool_cards_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_cards_id, p);
  return p;
}

std::string Pool_cards_DTO_key::to_string() const {
  return "pool_cards_id: " + pool_cards_id;
}

json_object *Pool_cards_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool_cards", json_object_new_string(class_pool_cards().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *pool_cards_dto_list_json = json_object_new_array();
    json_object_object_add(root, "pool_cards_dto_list", pool_cards_dto_list_json);
    for (const auto &pool_cards_dto: list) {
      json_object *pool_cards_dto_json = pool_cards_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(pool_cards_dto_list_json, pool_cards_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
