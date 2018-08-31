
#pragma once 

#include "../../core/configuration.h"
#include "../../core/entity.h"

#include <algorithm>
#include <iostream>
#include <vector>


class elite_replacement {

		double strength_;
public:
  void configure(life::configuration c) { strength_ =c["st"]; }

life::configuration publish_configuration() const  {
  life::configuration c;
  c["st"] = 0.2;
  return c;
}
  elite_replacement() { configure(publish_configuration()); }

  std::vector<life::entity> select(std::vector<life::entity> &pop) const ;
};
