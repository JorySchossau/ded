#pragma once

#include "../../components.h"

#include <algorithm>
#include <fstream>
#include <initializer_list>
#include <iostream>
#include <numeric>
#include <random>
#include <regex>
#include <set>
#include <string>
#include <string_view>
#include <vector>

class couter {

  bool message_ = true;

  std::string score_tag_ = "score,double";

public:
  couter() { configure(publish_configuration()); }

  life::configuration publish_configuration()
  {
    life::configuration c;
    c["parameters"]["message"] = message_;

    //  o:in:P must provide these tags
    c["pre-tags"]["score"] = score_tag_;

    // o:in:P' has no tags
    c["post-tags"] = nullptr;

    return c;
  }

  void configure(life::configuration con)
  {
    message_   = con["parameters"]["message"];
    score_tag_ = con["pre-tags"]["score"];
  }

  life::population evaluate(life::population);
};
