#include <cstring>
#include <sstream>
#include <utility>
#include "bdb_json_utils.hpp"
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "title_dto.hpp"

Title_DTO::Title_DTO(void *buffer) {
  deserialize(buffer);
}

Title_DTO::Title_DTO(const Principals_DTO &principals_dto) :
    title_id(principals_dto.title_id) {}

Title_DTO::Title_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Title_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(title_id);
  len += Bdb_serialization::buffer_len_string(title_type);
  len += Bdb_serialization::buffer_len_string(primary_title);
  len += Bdb_serialization::buffer_len_string(is_adult);
  len += Bdb_serialization::buffer_len_string(start_year);
  len += Bdb_serialization::buffer_len_string(runtime_minutes);
  len += Bdb_serialization::buffer_len_string(genres);
  len += Bdb_serialization::buffer_len_string(user_rating);
  len += Bdb_serialization::buffer_len_string(priority);
  len += Bdb_serialization::buffer_len_string(score);
  return len;
}

void *Title_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_type, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(primary_title, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(is_adult, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(start_year, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(runtime_minutes, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(genres, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(score, p);
  return p;
}

void Title_DTO::from_json(json_object *jobj, Bdb_errors &errors) {
  // parse: ' { "class_name": ... `
  std::string jobj_class_name =
      Bdb_json_utils::get_json_string("Title_DTO::from_json", "1", jobj, "class_name", errors);
  if (!errors.has() && jobj_class_name != class_name())
    errors.add("Title_DTO::from_json", "2", "not class Title_DTO");
  // parse: ' { "title_id": ... `
  title_id = Bdb_json_utils::get_json_string("Title_DTO::from_json", "1", jobj, "title_id", errors);
  if (!errors.has())
    // parse: ' { titleType": ... `
    title_type = Bdb_json_utils::get_json_string("Title_DTO::from_json", "2", jobj, "titleType", errors);
  if (!errors.has())
    // parse: ' { "primaryTitle": ... `
    primary_title = Bdb_json_utils::get_json_string("Title_DTO::from_json", "3", jobj, "primaryTitle", errors);
  if (!errors.has())
    // parse: ' { "isAdult": ... `
    is_adult = Bdb_json_utils::get_json_string("Title_DTO::from_json", "4", jobj, "isAdult", errors);
  if (!errors.has())
    // parse: ' { "startYear": ... `
    start_year = Bdb_json_utils::get_json_string("Title_DTO::from_json", "5", jobj, "startYear", errors);
  if (!errors.has())
    // parse: ' { "runtimeMinutes": ... `
    runtime_minutes = Bdb_json_utils::get_json_string("Title_DTO::from_json", "6", jobj, "runtime_minutes", errors);
  if (!errors.has())
    // parse: ' { "genres": ... `
    genres = Bdb_json_utils::get_json_string("Title_DTO::from_json", "7", jobj, "genres", errors);
  if (!errors.has())
    // parse: ' { "userRating": ... `
    user_rating = Bdb_json_utils::get_json_string("Title_DTO::from_json", "8", jobj, "userRating", errors);
  if (!errors.has())
    // parse: ' { "priority": ... `
    priority = Bdb_json_utils::get_json_string("Title_DTO::from_json", "9", jobj, "priority", errors);
  if (!errors.has())
    // parse: ' { "score": ... `
    score = Bdb_json_utils::get_json_string("Title_DTO::from_json", "9", jobj, "score", errors);
}

int Title_DTO::get_title_primary_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  Bdb_errors errors;
  Title_DTO title_dto(pdata->get_data());
  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string title_primary_title = title_dto.primary_title;
  size_t keylen = title_primary_title.size() + 1;
  char *title_primary_title_buf = (char *) malloc(keylen);
  strcpy(title_primary_title_buf, title_primary_title.c_str());
  // Now set the secondary key's data to be the primary_title
  skey->set_data(title_primary_title_buf);
  skey->set_size(keylen);
  return (0);
}

void Title_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // tconst	titleType	primaryTitle	originalTitle	isAdult	startYear	endYear	runtimeMinutes	genres
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, '\t');
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0: {
        title_id = token_str;
        if (title_id == "\\N")
          errors.add("Title_DTO::create", "1", "required title_id == '\\N'");
        break;
      }
      case 1: {
        title_type = token_str;
        if (title_type == "\\N")
          errors.add("Title_DTO::create", "2", "required titleType == '\\N'");
        break;
      }
      case 2: {
        primary_title = token_str;
        if (primary_title == "\\N")
          errors.add("Title_DTO::create", "3", "required primaryTitle == '\\N'");
        break;
      }
      case 3: {
        break;
      }
      case 4: {
        is_adult = token_str;
        // TODO: required?
        if (is_adult == "\\N")
          errors.add("Title_DTO::create", "4", "required isAdult == '\\N'");
        break;
      }
      case 5: {
        start_year = (token_str == "\\N" ? "" : token_str);
        break;
      }
      case 6: {
        break;
      }
      case 7: {
        runtime_minutes = (token_str == "\\N" ? "" : token_str);
        break;
      }
      case 8: {
        genres = (token_str == "\\N" ? "" : token_str);
        break;
      }
      default: {
        errors.add("Title_DTO::create", "5", "too many title fields on line "
            + Bdb_tokens::line_print(count, line));
      }
    }
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 9) {
    errors.add("Title_DTO::create", "6", "too few name fields on line"
        + Bdb_tokens::line_print(count, line));
  }
}

void *Title_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(title_id, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(title_type, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(primary_title, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(is_adult, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(start_year, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(runtime_minutes, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(genres, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(user_rating, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(priority, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(score, p);
  return p;
}

json_object *Title_DTO::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Title_DTO::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  json_object_object_add(root, "title_id", json_object_new_string(title_id.c_str()));
  json_object_object_add(root, "titleType", json_object_new_string(title_type.c_str()));
  json_object_object_add(root, "primaryTitle", json_object_new_string(primary_title.c_str()));
  json_object_object_add(root, "isAdult", json_object_new_string(is_adult.c_str()));
  json_object_object_add(root, "startYear", json_object_new_string(start_year.c_str()));
  json_object_object_add(root, "runtimeMinutes", json_object_new_string(runtime_minutes.c_str()));
  json_object_object_add(root, "genres", json_object_new_string(genres.c_str()));
  json_object_object_add(root, "averageRating", json_object_new_string(average_rating.c_str()));
  json_object_object_add(root, "numVotes", json_object_new_string(num_votes.c_str()));
  json_object_object_add(root, "userRating", json_object_new_string(user_rating.c_str()));
  json_object_object_add(root, "priority", json_object_new_string(priority.c_str()));
  json_object_object_add(root, "score", json_object_new_string(score.c_str()));
  return root;
}

std::string Title_DTO::to_string() const {
  std::ostringstream os;
  os << "title:" << std::endl;
  os << "\ttitle_id        " << title_id << std::endl;
  os << "\ttitleType       " << title_type << std::endl;
  os << "\tprimaryTitle    " << primary_title << std::endl;
  os << "\tisAdult         " << is_adult << std::endl;
  os << "\tstartYear       " << start_year << std::endl;
  os << "\truntimeMinutes  " << runtime_minutes << std::endl;
  os << "\tgenres          " << genres << std::endl;
  os << "\tuserRating      " << user_rating << std::endl;
  os << "\tpriority        " << priority << std::endl;
  os << "\tscore           " << score << std::endl;
  return os.str();
}
std::string Title_DTO::class_name() { return "Title_DTO"; }

Title_DTO_key::Title_DTO_key(const Title_DTO &title_dto) : title_id(title_dto.title_id) {
}

Title_DTO_key::Title_DTO_key(const Principals_DTO &principals_dto) : title_id(principals_dto.title_id) {
}

Title_DTO_key::Title_DTO_key(std::string title_id_) : title_id(std::move(title_id_)) {
}

Title_DTO_key::Title_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Title_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(title_id);
  return len;
}

void *Title_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(title_id, p);
  return p;
}

void *Title_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(title_id, p);
  return p;
}

std::string Title_DTO_key::to_string() const {
  return "title_id: " + title_id;
}

json_object *Title_DTO_list::to_json(Bdb_errors &errors) const {
  json_object *root = json_object_new_object();
  if (!root) {
    errors.add("Primary_database_config::to_json", "1", "json-c allocate error");
    return nullptr;
  }
  json_object_object_add(root, "class_name", json_object_new_string(class_name().c_str()));
  if (!errors.has() && !list.empty()) {
    json_object *title_dto_list_json = json_object_new_array();
    json_object_object_add(root, "title_dto_list", title_dto_list_json);
    for (const auto &title_dto: list) {
      json_object *title_dto_json = title_dto.to_json(errors);
      if (errors.has())
        break;
      json_object_array_add(title_dto_list_json, title_dto_json);
    }
  }
  if (!errors.has()) {
    return root;
  } else {
    json_object_put(root);
    return nullptr;
  }
}
