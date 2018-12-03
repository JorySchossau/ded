#pragma once

#include "../../core/configuration.h"
#include "../../core/encoding.h"
#include "../../core/entity.h"
#include "../../core/signal.h"
#include "../../core/utilities.h"

#include <algorithm>
#include <set>
#include <vector>

class linear {

  std::set<life::entity> bottled_;

  bool track_lineage_{false};
  std::vector<life::entity> pop_;
  std::string entity_name_{"null_entity"};
  life::configuration entity_config_;
  long size_{0};

public:
  linear() { configure(publish_configuration()); }

  void configure(life::configuration con) {
    size_ = con["size"];
    track_lineage_ = con["track-lineage"];
    entity_name_ = std::string(con["entity"][0]);
    entity_config_ = con["entity"][1];
    pop_.clear();
    util::rv3::generate_n(util::rv3::back_inserter(pop_), size_, [&] {
      auto org = life::make_entity(entity_name_);
      org.configure(entity_config_);
      return org;
    });
    if (track_lineage_)
      bottled_.insert(std::begin(pop_), std::end(pop_));
  }

  life::configuration publish_configuration() {
    life::configuration con;
    con["track-lineage"] = track_lineage_;
    con["size"] = size_;
    con["entity"] = {entity_name_, {}};
    return con;
  }

  std::vector<life::entity> get_as_vector();
  void merge(std::vector<life::entity>);
  void clear();
  void snapshot(std::ofstream&)const;
  life::configuration get_stats();
  std::vector<life::entity> prune_lineage();
};
