#pragma once

#include <cstdlib>
#include <cstring>
#include <list>
#include <string>
#include <json-c/json.h>
#include "bdb_errors.hpp"
#include "bdb_serializable.hpp"

class Vendor_DTO {
 public:
  std::string name;             /* Vendor name */
  std::string street;           /* Street name and number */
  std::string city;             /* City */
  std::string state;            /* Two-digit US state code */
  std::string zipcode;          /* US zipcode */
  std::string phone_number;     /* Vendor phone number */
  std::string sales_rep;        /* Name of sales representative */
  std::string sales_rep_phone;  /* Sales rep's phone number */

  Vendor_DTO() = default;
  explicit Vendor_DTO(void *buffer);
  Vendor_DTO(int count, const std::string &line, Bdb_errors &errors, char delimiter);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void parse(int count, const std::string &line, Bdb_errors &errors, char delimiter);
  void *serialize(void *buffer) const;
};

class Vendor_DTO_key {
 public:
  std::string name{};
  Vendor_DTO_key() = default;
  explicit Vendor_DTO_key(const Vendor_DTO &vendor_dto);
  explicit Vendor_DTO_key(std::string name_);
  explicit Vendor_DTO_key(void *buffer);

  [[nodiscard]] size_t buffer_size() const;
  void *deserialize(void *buffer);
  void *serialize(void *buffer) const;
};
