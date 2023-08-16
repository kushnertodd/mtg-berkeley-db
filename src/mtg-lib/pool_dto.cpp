#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "pool_dto.hpp"

Pool_DTO::Pool_DTO(void *buffer) {
  deserialize(buffer);
}

Pool_DTO::Pool_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Pool_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_id);
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Pool_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void Pool_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_pool": ... `
  std::string jobj_class_pool =
      Bdb_json_utils::get_json_string("Pool_DTO::from_json", "1", jobj,
                                      "class_pool", errors);
  if (!errors.has() && jobj_class_pool != class_pool())
    errors.add("Pool_DTO::from_json", "2", "not class Pool_DTO");
  // parse: ' { "pool_id": ... `
  pool_id = Bdb_json_utils::get_json_string("Pool_DTO::from_json", "1", jobj, "pool_id", errors);
  if (!errors.has())
    // parse: ' { name": ... `
    name = Bdb_json_utils::get_json_string("Pool_DTO::from_json", "4", jobj, "name", errors);
}

void Pool_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        pool_id = token_str;
        if (pool_id == "\\N")
          errors.add("Pool_DTO::create", "1", "required pool_id == '\\N'");
        break;
      }
      case 1: {
        name = token_str;
        if (name == "\\N")
          errors.add("Pool_DTO::create", "2", "required name_id == '\\N'");
        break;
      }
      default: {
        errors.add("Pool_DTO::create", "3", "too many pool fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 2) {
    errors.add("Pool_DTO::create", "4", "too few pool fields on line "
        + Bdb_tokens::line_print(count, line));
  }
}

void *Pool_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}

json_object *Pool_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Pool_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool", json_object_new_string(class_pool().c_str()));
  json_object_object_add(root, "pool_id", json_object_new_string(pool_id.c_str()));
  json_object_object_add(root, "name", json_object_new_string(name.c_str()));
  return root;
}

std::string Pool_DTO::to_string() const {
  std::ostringstream os;
  os << "pool:" << std::endl;
  os << "\tpool_id        " << pool_id << std::endl;
  os << "\tname    " << name << std::endl;
  return os.str();
}

Pool_DTO_key::Pool_DTO_key(const Pool_DTO &pool_DTO) : pool_id(pool_DTO.pool_id) {}

Pool_DTO_key::Pool_DTO_key(std::string pool_id_) : pool_id(std::move(pool_id_)) {}

Pool_DTO_key::Pool_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Pool_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_id);
  return len;
}

void *Pool_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_id, p);
  return p;
}

void *Pool_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_id, p);
  return p;
}

std::string Pool_DTO_key::to_string() const {
  return "pool_id: " + pool_id;
}

json_object *Pool_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool", json_object_new_string(class_pool().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *pool_dto_list_json = json_object_new_array();
    json_object_object_add(root, "pool_dto_list", pool_dto_list_json);
    for (const auto &pool_dto: list) {
      json_object *pool_dto_json = pool_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(pool_dto_list_json, pool_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
