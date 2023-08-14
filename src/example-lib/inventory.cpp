#include <iostream>
#include "bdb_serialization.hpp"
#include "bdb_tokens.hpp"
#include "inventory.hpp"

int string_to_int(const std::string &int_str, Bdb_errors &errors) {
  try {
    int i = std::stoi(int_str);
    return i;
  }
  catch (std::exception &err) {
    errors.add("Misc_utils::string_to_int",
               "1",
               "invalid int '" + std::string(int_str) + "': " + std::string(err.what()));
  }
  return 0;
}

double string_to_double(const std::string &real_str, Bdb_errors &errors) {
  try {
    double d = std::stod(real_str);
    return d;
  }
  catch (std::exception &err) {
    errors.add("Misc_utils::string_to_real",
               "1",
               "invalid real '" + std::string(real_str) + "': " + std::string(err.what()));
  }
  return 0;
}

// Constructor from a void *
// For use with the data returned from a bdb get
Inventory_DTO::Inventory_DTO(void *buffer) {
  deserialize(buffer);
}

Inventory_DTO::Inventory_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  parse(count, line, errors, delimiter);
}

size_t Inventory_DTO::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(category_);
  len += Bdb_serialization::buffer_len_string(name_);
  len += Bdb_serialization::buffer_len_string(vendor_);
  len += Bdb_serialization::buffer_len_string(sku_);
  len += sizeof(price_);
  len += sizeof(quantity_);
  return len;
}

void *Inventory_DTO::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(category_, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(name_, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(vendor_, p);
  p = (unsigned char *) Bdb_serialization::deserialize_string(sku_, p);
  p = (unsigned char *) Bdb_serialization::serialize_double(price_, p);
  p = (unsigned char *) Bdb_serialization::serialize_long(quantity_, p);
  return p;
}

void *Inventory_DTO::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(category_, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(name_, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(vendor_, p);
  p = (unsigned char *) Bdb_serialization::serialize_string(sku_, p);
  p = (unsigned char *) Bdb_serialization::serialize_double(price_, p);
  p = (unsigned char *) Bdb_serialization::serialize_long(quantity_, p);
  return p;
}

// Used to extract an inventory item's name from an
// inventory database record. This function is used to create
// keys for secondary database records.
int Inventory_DTO::get_item_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey) {
  /*
   * First, obtain the buffer location where we placed the item's name. In
   * this example, the item's name is located in the primary data. It is the
   * first string in the buffer after the price (a double) and the quantity
   * (a long).
   */
  Inventory_DTO inventory_dto(pdata->get_data());

  // key memory is malloc()'d, berkeley db will free
  std::memset((void *) skey, 0, sizeof(Dbt));
  skey->set_flags(DB_DBT_APPMALLOC);
  std::string item_name = inventory_dto.getName();
  size_t keylen = item_name.size() + 1;
  char *itemname_buf = (char *) malloc(keylen);
  std::strcpy(itemname_buf, item_name.c_str());

  /* Now set the secondary key's data to be the item name */
  skey->set_data(itemname_buf);
  skey->set_size(keylen);

  return (0);
}

void Inventory_DTO::parse(int count, const std::string &line, Bdb_errors &errors, char delimiter) {
  // nconst	primaryPrincipals	birthYear	deathYear	primaryProfession	knownForTitle
  std::vector<std::string> token_list = Bdb_tokens::tokenize(line, delimiter);
  int i = 0;
  for (const std::string &token_str: token_list) {
    switch (i) {
      case 0:
        name_ = token_str;
        break;
      case 1:
        sku_ = token_str;
        break;
      case 2:
        price_ = string_to_double(token_str, errors);
        break;
      case 3:
        quantity_ = string_to_int(token_str, errors);
        break;
      case 4:
        category_ = token_str;
        break;
      case 5:
        vendor_ = token_str;
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
  if (i != 6 && !errors.has())
    errors.add("Inventory_DTO::create", "4", "too few name fields on line "
        + Bdb_tokens::line_print(count, line));
}

/* Utility function used to show the contents of this class */
void Inventory_DTO::show() {
  std::cout << "\nName:           " << name_ << std::endl;
  std::cout << "    SKU:        " << sku_ << std::endl;
  std::cout << "    Price:      " << price_ << std::endl;
  std::cout << "    Quantity:   " << quantity_ << std::endl;
  std::cout << "    Category:   " << category_ << std::endl;
  std::cout << "    Vendor:     " << vendor_ << std::endl;
}

Inventory_DTO_key::Inventory_DTO_key(Inventory_DTO &inventory_dto) : sku_(inventory_dto.getSKU()) {}

size_t Inventory_DTO_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(sku_);
  return len;
}

void *Inventory_DTO_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(sku_, p);
  return p;
}

void *Inventory_DTO_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(sku_, p);
  return p;
}

Inventory_item_name_key::Inventory_item_name_key(std::string item_name) : item_name_(std::move(item_name)) {}

Inventory_item_name_key::Inventory_item_name_key(void *buffer) {
  deserialize(buffer);
}

size_t Inventory_item_name_key::buffer_size() const {
  size_t len = 0;
  len += Bdb_serialization::buffer_len_string(item_name_);
  return len;
}

void *Inventory_item_name_key::deserialize(void *buffer) {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::deserialize_string(item_name_, p);
  return p;
}

void *Inventory_item_name_key::serialize(void *buffer) const {
  auto *p = (unsigned char *) buffer;
  p = (unsigned char *) Bdb_serialization::serialize_string(item_name_, p);
  return p;
}



