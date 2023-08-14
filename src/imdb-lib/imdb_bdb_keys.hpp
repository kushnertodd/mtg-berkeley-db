#pragma once

#include "bdb_key_extractor.hpp"

class Imdb_bdb_key_extractor : public Bdb_key_extractor {
 public:
  KEY_EXTRACTOR_FCT_TYPE key_extractor_fct(const char *key_extractor_name) override;
};
