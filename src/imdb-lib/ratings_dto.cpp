#include <sstream>
#include <utility>
#include "bdb_file_io.hpp"
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "ratings_dto.hpp"

Ratings_DTO::Ratings_DTO(void *buffer) {
  deserialize(buffer);
}

Ratings_DTO::Ratings_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Ratings_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(title_id);
  len += Bdb_serialization::buffer_len_string(averageRating);
  len += Bdb_serialization::buffer_len_string(numVotes);
  return len;
}

void *Ratings_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(averageRating, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(numVotes, p);
  return p;
}

void Ratings_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Ratings_DTO::from_json", "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Ratings_DTO::from_json", "2", "not class Ratings_DTO");
  // parse: ' { "title_id": ... `
  title_id = Bdb_json_utils::get_json_string("Ratings_DTO::from_json", "1", jobj, "title_id", errors);
  if (!errors.has())
    // parse: ' { averageRating": ... `
    averageRating = Bdb_json_utils::get_json_string("Ratings_DTO::from_json", "2", jobj, "averageRating", errors);
  if (!errors.has())
    // parse: ' { "numVotes": ... `
    numVotes = Bdb_json_utils::get_json_string("Ratings_DTO::from_json", "3", jobj, "numVotes", errors);
}

void Ratings_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // tconst     averageRating  numVotes
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, '\t');
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        title_id = token_str;
        if (title_id == "\\N")
          errors.add("Ratings_DTO::create", "1", "required title_id == '\\N'");
        break;
      }
      case 1: {
        averageRating = token_str;
        if (averageRating == "\\N")
          errors.add("Ratings_DTO::create", "2", "required averageRating == '\\N'");
        break;
      }
      case 2: {
        numVotes = token_str;
        if (numVotes == "\\N")
          errors.add("Ratings_DTO::create", "3", "required numVotes == '\\N'");
        break;
      }
      default: {
        errors.add("Ratings_DTO::create", "5", "too many ratings fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 3) {
    errors.add("Ratings_DTO::create", "6", "too few name fields on line"
        + Bdb_tokens::line_print(count, line));
  }
}

void *Ratings_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(averageRating, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(numVotes, p);
  return p;
}

json_object *Ratings_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Ratings_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "title_id", json_object_new_string(title_id.c_str()));
  json_object_object_add(root, "averageRating", json_object_new_string(averageRating.c_str()));
  json_object_object_add(root, "numVotes", json_object_new_string(numVotes.c_str()));
  return root;
}

std::string Ratings_DTO::to_string() const {
  std::ostringstream os;
  os << "ratings:" << std::endl;
  os << "\ttitle_id        " << title_id << std::endl;
  os << "\taverageRating       " << averageRating << std::endl;
  os << "\tnumVotes    " << numVotes << std::endl;
  return os.str();
}

Ratings_DTO_key::Ratings_DTO_key(const Ratings_DTO &ratings_dto) : title_id(ratings_dto.title_id) {
}

Ratings_DTO_key::Ratings_DTO_key(const Principals_DTO &principals_dto) : title_id(principals_dto.title_id) {
}

Ratings_DTO_key::Ratings_DTO_key(std::string title_id_) : title_id(std::move(title_id_)) {
}

Ratings_DTO_key::Ratings_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Ratings_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(title_id);
  return len;
}

void *Ratings_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_id, p);
  return p;
}

void *Ratings_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(title_id, p);
  return p;
}

std::string Ratings_DTO_key::to_string() const {
  return "title_id: " + title_id;
}

json_object *Ratings_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *ratings_dto_list_json = json_object_new_array();
    json_object_object_add(root, "ratings_dto_list", ratings_dto_list_json);
    for (const auto &ratings_dto: list) {
      json_object *ratings_dto_json = ratings_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(ratings_dto_list_json, ratings_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
