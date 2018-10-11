
#include <fmt/format.h>
#include <fmt/printf.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <random>
#include <regex>
#include <string>
#include <string_view>
#include <vector>

int main() {

  std::map<std::string, std::vector<std::string>> build_options;
  std::ifstream cfg("components.cfg");

  std::regex r(
      R"(^\s*(experiment|entity|selector|environment)\s*:\s*(\w+)\s*$)");
  for (std::string line; std::getline(cfg, line);) {
    std::smatch m;
    if (std::regex_match(line, m, r))
      build_options[m[1]].push_back(m[2]);
  }
  cfg.close();

  std::ofstream header("components.cpp");

  header << "\n// Auto generated by "
            "component_builder.cpp\n\n#include\"components.h\"\n\n";

  for (auto &[type, names] : build_options)
    for (auto &name : names)
      header << "#include\"user/" << type << "/" << name << ".h\"\n";

  header << "\n#include<string_view>\n\nnamespace life {\n\n";

  for (auto &[type, names] : build_options) {
    header << type << " make_" << type << "(std::string_view choice) {\n";
    for (auto &name : names)
      header << "  if (choice == \"" << name << "\")\n    return " << name
             << "();\n";
    header << "  std::cout << \"unknown-" << type
           << ": \" << choice;\n  exit(1);\n}\n\n";
  }

  for (auto &[type, names] : build_options) {
    header << "std::vector<std::string> " << type << "_list() {\n  return {";
    std::string vec_list;
    for (auto &name : names)
      vec_list += "\"" + name + "\",";
    vec_list.pop_back();
    header << vec_list << "};\n}\n\n";
  }
  header << "}\n";
  header.close();
  
  std::ofstream makefile("makefile");
  using namespace fmt::literals;
  makefile << "\n# Auto generated by component_builder.cpp"
              "\n\nflags = g++-7 -Wall -std=c++17 -O3 "
              "\n\ndebugflags = g++-7 -Wall -std=c++17 -O3 -g"
              "\n\nall : ded \n\n"_format();

  std::string core_headers(
      " core/experiment.h core/environment.h core/entity.h core/selector.h "
      "core/encoding.h core/configuration.h	core/eval_results.h "
      "core/signal.h ");

  for (auto &flags : {"", "debug"}) {
    makefile
        << "\n\n{0}components = {0}obj_files/main.o {0}obj_files/components.o "_format(
               flags);
    for (auto &[type, names] : build_options)
      for (auto &name : names)
        makefile << "{0}obj_files/{1}__{2}.o "_format(flags, type, name);

    makefile
        << "\n\n{0}ded : $({0}components)"
           "\n\t$({0}flags) $({0}components) -o {0}ded"
           "\n\n{0}obj_files/main.o : main.cpp"
           "\n\t$({0}flags) -c main.cpp -o {0}obj_files/main.o"
           "\n\n{0}obj_files/components.o : components.cpp components.h "_format(
               flags);

	makefile << core_headers;

    makefile
        << "\n\t$({0}flags) -c components.cpp -o {0}obj_files/components.o\n\n"_format(
               flags);

    for (auto &[type, names] : build_options)
      for (auto &name : names)
        makefile
            << "{0}obj_files/{1}__{2}.o : user/{1}/{2}.cpp user/{1}/{2}.h {3}\n"
               "\t$({0}flags) -c user/{1}/{2}.cpp -o {0}obj_files/{1}__{2}.o\n\n"_format(
                   flags, type, name,core_headers);

    makefile << "clean{0} : \n\trm  {0}obj_files/*.o {0}ded\n\n"_format(flags);
  }
  makefile.close();
}

