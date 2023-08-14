#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "name_dto.hpp"

Name_DTO::Name_DTO(void *buffer) {
  deserialize(buffer);
}

Name_DTO::Name_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Name_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name_id);
  len += Bdb_serialization::buffer_len_string(primary_name);
  len += Bdb_serialization::buffer_len_string(birth_year);
  len += Bdb_serialization::buffer_len_string(death_year);
  len += Bdb_serialization::buffer_len_string(user_rating);
  len += Bdb_serialization::buffer_len_string(priority);
  len += Bdb_serialization::buffer_len_string(score);
  return len;
}

void *Name_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(primary_name, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(birth_year, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(death_year, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(score, p);
  return p;
}

Name_DTO::Name_DTO(const Principals_DTO &principals_dto) :
    name_id(principals_dto.name_id) {}

void Name_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Name_DTO::from_json", "1", jobj,
                                      "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Name_DTO::from_json", "2", "not class Name_DTO");
  // parse: ' { "name_id": ... `
  name_id = Bdb_json_utils::get_json_string("Name_DTO::from_json", "1", jobj, "name_id", errors);
  if (!errors.has())
    // parse: ' { primaryName": ... `
    primary_name = Bdb_json_utils::get_json_string("Name_DTO::from_json", "4", jobj, "primaryName", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    birth_year = Bdb_json_utils::get_json_string("Name_DTO::from_json", "5", jobj, "birthYear", errors);
  if (!errors.has())
    // parse: ' { "deathYear": ... `
    death_year = Bdb_json_utils::get_json_string("Name_DTO::from_json", "6", jobj, "deathYear", errors);
  if (!errors.has())
    // parse: ' { "userRating": ... `
    user_rating = Bdb_json_utils::get_json_string("Name_DTO::from_json", "7", jobj, "userRating", errors);
  if (!errors.has())
    // parse: ' { "priority": ... `
    priority = Bdb_json_utils::get_json_string("Name_DTO::from_json", "8", jobj, "priority", errors);
  if (!errors.has())
    // parse: ' { "score": ... `
    score = Bdb_json_utils::get_json_string("Title_DTO::from_json", "9", jobj, "score", errors);
}

void Name_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        name_id = token_str;
        if (name_id == "\\N")
          errors.add("Name_DTO::create", "1", "required name_id == '\\N'");
        break;
      }
      case 1: {
        primary_name = token_str;
        if (primary_name == "\\N")
          errors.add("Name_DTO::create", "2", "required primaryName_id == '\\N'");
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
        errors.add("Name_DTO::create", "3", "too many name fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 6) {
    errors.add("Name_DTO::create", "4", "too few name fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Name_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(primary_name, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(birth_year, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(death_year, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(score, p);
  return p;
}

json_object *Name_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Name_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "name_id", json_object_new_string(name_id.c_str()));
  json_object_object_add(root, "primaryName", json_object_new_string(primary_name.c_str()));
  json_object_object_add(root, "birthYear", json_object_new_string(birth_year.c_str()));
  json_object_object_add(root, "deathYear", json_object_new_string(death_year.c_str()));
  json_object_object_add(root, "userRating", json_object_new_string(user_rating.c_str()));
  json_object_object_add(root, "priority", json_object_new_string(priority.c_str()));
  json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Name_DTO::to_string() const {
  std::ostringstream os;
  os << "name:" << std::endl;
  os << "\tname_id        " << name_id << std::endl;
  os << "\tprimaryName    " << primary_name << std::endl;
  os << "\tbirthYear      " << birth_year << std::endl;
  os << "\tdeathYear      " << death_year << std::endl;
  os << "\tuserRating      " << user_rating << std::endl;
  os << "\tpriority        " << priority << std::endl;
  os << "\tscore           " << score << std::endl;
  return os.str();
}

Name_DTO_key::Name_DTO_key(const Name_DTO &name_DTO) : name_id(name_DTO.name_id) {}

Name_DTO_key::Name_DTO_key(const Principals_DTO &principals_dto) : name_id(principals_dto.name_id) {
}

Name_DTO_key::Name_DTO_key(std::string name_id_) : name_id(std::move(name_id_)) {}

Name_DTO_key::Name_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Name_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name_id);
  return len;
}

void *Name_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name_id, p);
  return p;
}

void *Name_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name_id, p);
  return p;
}

std::string Name_DTO_key::to_string() const {
  return "name_id: " + name_id;
}

json_object *Name_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *name_dto_list_json = json_object_new_array();
    json_object_object_add(root, "name_dto_list", name_dto_list_json);
    for (const auto &name_dto: list) {
      json_object *name_dto_json = name_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(name_dto_list_json, name_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
