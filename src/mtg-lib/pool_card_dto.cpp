#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "pool_card_dto.hpp"

Pool_card_DTO::Pool_card_DTO(void *buffer) {
  deserialize(buffer);
}

size_t Pool_card_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_card_id);
  len += Bdb_serialization::buffer_len_string(pool_id);
  len += Bdb_serialization::buffer_len_string(card_id);
  return len;
}

void *Pool_card_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_card_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(card_id, p);
  return p;
}

int Pool_card_DTO::get_pool_card_pool_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Pool_card_DTO pool_card_DTO(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string pool_id = pool_card_DTO.pool_id;
  size_t keylen = pool_id.size() + 1;
  char *pool_id_buf = (char *) malloc(keylen);
  std::strcpy(pool_id_buf, pool_id.c_str());
  skey->set_data(pool_id_buf);
  skey->set_size(keylen);
  return 0;
}

int Pool_card_DTO::get_pool_card_card_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Pool_card_DTO pool_card_DTO(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string card_id = pool_card_DTO.card_id;
  size_t keylen = card_id.size() + 1;
  char *card_id_buf = (char *) malloc(keylen);
  std::strcpy(card_id_buf, card_id.c_str());
  skey->set_data(card_id_buf);
  skey->set_size(keylen);
  return 0;
}

void Pool_card_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_pool_card": ... `
  std::string jobj_class_pool_card =
      Bdb_json_utils::get_json_string("Pool_card_DTO::from_json", "1", jobj,
                                      "class_pool_card", errors);
  if (!errors.has() && jobj_class_pool_card != class_pool_card())
    errors.add("Pool_card_DTO::from_json", "2", "not class Pool_card_DTO");
  // parse: ' { "pool_card_id": ... `
  pool_card_id = Bdb_json_utils::get_json_string("Pool_card_DTO::from_json", "1", jobj, "pool_card_id", errors);
  if (!errors.has())
    // parse: ' { primaryPool_card": ... `
    pool_id = Bdb_json_utils::get_json_string("Pool_card_DTO::from_json", "4", jobj, "primaryPool_card", errors);
  if (!errors.has())
    // parse: ' { "birthYear": ... `
    card_id = Bdb_json_utils::get_json_string("Pool_card_DTO::from_json", "5", jobj, "birthYear", errors);
}

void *Pool_card_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_card_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(card_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_id, p);
  return p;
}

json_object *Pool_card_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Pool_card_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool_card", json_object_new_string(class_pool_card().c_str()));
  json_object_object_add(root, "pool_card_id", json_object_new_string(pool_card_id.c_str()));
  json_object_object_add(root, "card_id", json_object_new_string(card_id.c_str()));
  json_object_object_add(root, "pool_id", json_object_new_string(pool_id.c_str()));
  return root;
}

std::string Pool_card_DTO::to_string() const {
  std::ostringstream os;
  os << "pool_card:" << std::endl;
  os << "\tpool_card_id        " << pool_card_id << std::endl;
  os << "\tcard_id    " << card_id << std::endl;
  os << "\tpool_id      " << pool_id << std::endl;
  return os.str();
}

Pool_card_DTO_key::Pool_card_DTO_key(const Pool_card_DTO &pool_card_DTO) : pool_card_id(pool_card_DTO.pool_card_id) {}

Pool_card_DTO_key::Pool_card_DTO_key(std::string pool_card_id_) : pool_card_id(std::move(pool_card_id_)) {}

Pool_card_DTO_key::Pool_card_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Pool_card_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(pool_card_id);
  return len;
}

void *Pool_card_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(pool_card_id, p);
  return p;
}

void *Pool_card_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(pool_card_id, p);
  return p;
}

std::string Pool_card_DTO_key::to_string() const {
  return "pool_card_id: " + pool_card_id;
}

json_object *Pool_card_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_pool_card", json_object_new_string(class_pool_card().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *pool_card_dto_list_json = json_object_new_array();
    json_object_object_add(root, "pool_card_dto_list", pool_card_dto_list_json);
    for (const auto &pool_card_dto: list) {
      json_object *pool_card_dto_json = pool_card_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(pool_card_dto_list_json, pool_card_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
