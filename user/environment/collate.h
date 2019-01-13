
#pragma once

#include "../../components.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

class collate {

  std::string x1_tag_ = "x1,double";
  std::string x2_tag_ = "x2,double";
  std::string y_tag_ = "y,double";

  life::configuration x1_{"null_environment", {}};
  life::configuration x2_{"null_environment", {}};

public:
  collate() { configure(publish_configuration()); }

  life::configuration publish_configuration() {
    life::configuration ec;

    // o:in:P has no tags
    ec["pre-tags"] = nullptr;

    // o:in:P' has these tags
    ec["post-tags"]["y"] = y_tag_;

    ec["input-tags"] = nullptr;
    ec["output-tags"] = nullptr;

    // o:in:x1(P) must provide these tags
    ec["parameters"]["x1"] = {x1_[0], {}, {}, {x1_tag_}};

    // o:in:x2(P) must provide these tags
    ec["parameters"]["x2"] = {x2_[0], {}, {}, {x2_tag_}};

    return ec;
  }

  void configure(life::configuration con) {

    x1_ = con["parameters"]["x1"];
    x2_ = con["parameters"]["x2"];

    y_tag_ = con["post-tags"]["y"];
  }

  life::population evaluate(life::population);
};

