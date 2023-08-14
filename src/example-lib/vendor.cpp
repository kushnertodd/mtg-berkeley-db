#include <cstring>
#include <db_cxx.h>
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "inventory.hpp"
#include "vendor.hpp"

Vendor_DTO::Vendor_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

void Vendor_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0:
        name = token_str;
        break;
      case 1:
        street = token_str;
        break;
      case 2:
        city = token_str;
        break;
      case 3:
        state = token_str;
        break;
      case 4:
        zipcode = token_str;
        break;
      case 5:
        phone_number = token_str;
        break;
      case 6:
        sales_rep = token_str;
        break;
      case 7:
        sales_rep_phone = token_str;
        break;
      default:
        errors.add("Name_DTO::create", "3", "too many name fields on line "
            + Bdb_tokens::line_print(count, line));
    }
    if (errors.has())
      break;
    i++;
  }
  // Store the tokens as per structure members , where (i==0) is first member and so on..
  if (i != 8 && !errors.has())
    errors.add("Name_DTO::create", "4", "too few name fields on line "
        + Bdb_tokens::line_print(count, line));
}

// Constructor from a void *
// For use with the data returned from a bdb get
Vendor_DTO::Vendor_DTO(void *buffer) {
  deserialize(buffer);
}

size_t Vendor_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  len += Bdb_serialization::buffer_len_string(street);
  len += Bdb_serialization::buffer_len_string(city);
  len += Bdb_serialization::buffer_len_string(state);
  len += Bdb_serialization::buffer_len_string(zipcode);
  len += Bdb_serialization::buffer_len_string(phone_number);
  len += Bdb_serialization::buffer_len_string(sales_rep);
  len += Bdb_serialization::buffer_len_string(sales_rep_phone);
  return len;
}

void *Vendor_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(street, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(city, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(state, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(zipcode, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(phone_number, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(sales_rep, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(sales_rep_phone, p);
  return p;
}

void *Vendor_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(street, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(city, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(state, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(zipcode, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(phone_number, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(sales_rep, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(sales_rep_phone, p);
  return p;
}

Vendor_DTO_key::Vendor_DTO_key(const Vendor_DTO &vendor_dto) : name(vendor_dto.name) {}

Vendor_DTO_key::Vendor_DTO_key(std::string name_) : name(std::move(name_)) {}

Vendor_DTO_key::Vendor_DTO_key(void *buffer) {
  deserialize(buffer);
}

size_t Vendor_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(name);
  return len;
}

void *Vendor_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(name, p);
  return p;
}

void *Vendor_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(name, p);
  return p;
}
