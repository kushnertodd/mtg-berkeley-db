#include <cstring>
#include <sstream>
#include "bdb_file_io.hpp"
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "principals_dto.hpp"

Principals_DTO::Principals_DTO(void *buffer) {
  deserialize(buffer);
}

Principals_DTO::Principals_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Principals_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(id);
  len += Bdb_serialization::buffer_len_string(title_id);
  len += Bdb_serialization::buffer_len_string(name_id);
  len += Bdb_serialization::buffer_len_string(category);
  len += Bdb_serialization::buffer_len_string(job);
  len += Bdb_serialization::buffer_len_string(characters);
  return len;
}

void *Principals_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(category, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(job, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(characters, p);
  return p;
}

void Principals_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Principals_DTO::from_json", "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Principals_DTO::from_json", "2", "not class Principals_DTO");
  // parse: ' { "id": ... `
  id = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "1", jobj, "id", errors);
  if (!errors.has())
    // parse: ' { title_id": ... `
    title_id = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "2", jobj, "title_id", errors);
  if (!errors.has())
    // parse: ' { "name_id": ... `
    name_id = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "3", jobj, "name_id", errors);
  if (!errors.has())
    // parse: ' { "category": ... `
    category = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "4", jobj, "category", errors);
  if (!errors.has())
    // parse: ' { "job": ... `
    job = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "5", jobj, "job", errors);
  if (!errors.has())
    // parse: ' { "characters": ... `
    characters = Bdb_json_utils::get_json_string("Principals_DTO::from_json", "6", jobj, "characters", errors);
}

/*
 * Used to extract a principal name_id from a
 * principals database record. This function is used to create
 * keys for secondary database records.
 */
int Principals_DTO::get_principals_name_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Principals_DTO principals_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string name_id = principals_dto.name_id;
  size_t keylen = name_id.size() + 1;
  char *name_id_buf = (char *) malloc(keylen);
  std::strcpy(name_id_buf, name_id.c_str());
  // Now set the secondary key's data to be the name_id
  skey->set_data(name_id_buf);
  skey->set_size(keylen);
  return 0;
}

/*
 * Used to extract a principal title_id from a
 * principals database record. This function is used to create
 * keys for secondary database records.
 */
int Principals_DTO::get_principals_title_id(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Principals_DTO principals_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string title_id = principals_dto.title_id;
  size_t keylen = title_id.size() + 1;
  char *title_id_buf = (char *) malloc(keylen);
  std::strcpy(title_id_buf, title_id.c_str());
  // Now set the secondary key's data to be the title_id
  skey->set_data(title_id_buf);
  skey->set_size(keylen);
  return (0);
}

void Principals_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // tconst	ordering	nconst	category	job	characters
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, '\t');
  int i = 0;
  id = std::to_string(count);
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        title_id = token_str;
        if (title_id == "\\N")
          errors.add("Principals_DTO::create", "1", "required title_id == '\\N'");
        break;
      }
      case 1: {
        break;
      }
      case 2: {
        name_id = token_str;
        if (name_id == "\\N")
          errors.add("Principals_DTO::create", "2", "required name_id == '\\N'");
        break;
      }
      case 3: {
        category = token_str;
        // TODO: required?
        if (category == "\\N")
          errors.add("Principals_DTO::create", "3", "required category == '\\N'");
        break;
      }
      case 4: {
        job = (token_str == "\\N" ? "" : token_str);
        break;
      }
      case 5: {
        characters = (token_str == "\\N" ? "" : token_str);
        break;
      }
      default: {
        errors.add("Principals_DTO::create", "4", "too many principals_DTO fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on.
  if (i != 6) {
    errors.add("Principals_DTO::create", "5", "too few principals_DTO fields on line '"
        + Bdb_tokens::line_print(count, line));
  }
}

void *Principals_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(category, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(job, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(characters, p);
  return p;
}

json_object *Principals_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Principals_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "id", json_object_new_string(id.c_str()));
  json_object_object_add(root, "title_id", json_object_new_string(title_id.c_str()));
  json_object_object_add(root, "name_id", json_object_new_string(name_id.c_str()));
  json_object_object_add(root, "category", json_object_new_string(category.c_str()));
  json_object_object_add(root, "job", json_object_new_string(job.c_str()));
  json_object_object_add(root, "characters", json_object_new_string(characters.c_str()));
  return root;
}

std::string Principals_DTO::to_string() const {
  std::ostringstream os;
  os << "principals_DTO entry:" << std::endl;
  os << "\tid            " << id << std::endl;
  os << "\ttitle_id      " << title_id << std::endl;
  os << "\tname_id       " << name_id << std::endl;
  os << "\tcategory      " << category << std::endl;
  os << "\tjob           " << job << std::endl;
  os << "\tcharacters    " << characters << std::endl;
  return os.str();
}

Principals_DTO_key::Principals_DTO_key(const Principals_DTO &principals_dto) : id(principals_dto.id) {
}

Principals_DTO_key::Principals_DTO_key(std::string id_) : id(std::move(id_)) {
}

Principals_DTO_key::Principals_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Principals_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(id);
  return len;
}

void *Principals_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(id, p);
  return p;
}

void *Principals_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(id, p);
  return p;
}

std::string Principals_DTO_key::to_string() const {
  return "id: " + id;
}

json_object *Principals_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *principals_dto_list_json = json_object_new_array();
    json_object_object_add(root, "principals_dto_list", principals_dto_list_json);
    for (const auto &principals_dto: list) {
      json_object *principals_dto_json = principals_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(principals_dto_list_json, principals_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}

