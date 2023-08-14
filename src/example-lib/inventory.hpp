#pragma once

#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <json-c/json.h>
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Inventory_DTO {
 public:
  inline double &getPrice() { return (price_); }
  inline long &getQuantity() { return (quantity_); }
  inline std::string &getCategory() { return (category_); }
  inline std::string &getName() { return (name_); }
  inline std::string &getVendor() { return (vendor_); }
  inline std::string &getSKU() { return (sku_); }

  Inventory_DTO() = default;
  explicit Inventory_DTO(void *buffer);
  Inventory_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  void show();

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  static int get_item_name(Db *dbp, const Dbt *pkey, const Dbt *pdata, Dbt *skey);
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;

 private:
  /* Data members */
  std::string category_{};
  std::string name_{};
  std::string vendor_{};
  std::string sku_{};
  double price_{};
  long quantity_{};
};

class Inventory_DTO_key {
 public:
  std::string sku_{};
  Inventory_DTO_key() = default;
  explicit Inventory_DTO_key(Inventory_DTO &name_dto);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};

class Inventory_DTO_list {
 public:
  std::list<Inventory_DTO> list;
  void add(Inventory_DTO &name_dto) { list.push_back(name_dto); }
};

class Inventory_DTO_key_list {
 public:
  std::list<Inventory_DTO_key> list;
  void add(Inventory_DTO_key &name_dto_key) { list.push_back(name_dto_key); }
};

class Inventory_item_name_key {
 public:
  std::string item_name_{};
  Inventory_item_name_key() = default;
  explicit Inventory_item_name_key(std::string item_name);
  explicit Inventory_item_name_key(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};
