#include "option.hpp"

void print_err(const option_status_t status) {
  switch (status) {
  case option_status_t::invalid_argument:
    std::cerr << "Error: Invalid argument." << std::endl;
    break;
  case option_status_t::not_number:
    std::cerr << "Error: Argument must be a number." << std::endl;
    break;
  case option_status_t::path_not_found:
    std::cerr << "Error: Path not found." << std::endl;
    break;
  case option_status_t::out_of_range:
    std::cerr << "Error: Argument is out of range." << std::endl;
    break;
  case option_status_t::mode_not_selected:
    std::cerr << "Error: Argument \'--mode\' is required." << std::endl;
    break;
  case option_status_t::turn_not_selected:
    std::cerr << "Error: Argument \'--turn\' is required." << std::endl;
    break;
  case option_status_t::board_not_set:
    std::cerr << "Error: Argument \'--board\' is required." << std::endl;
    break;
  case option_status_t::path_not_selected:
    std::cerr << "Error: Argument \'--out_path\' is required." << std::endl;
    break;
  case option_status_t::not_directory:
    std::cerr << "Error: Argument must be a directory." << std::endl;
  case option_status_t::not_file:
    std::cerr << "Error: Argument must be a file." << std::endl;
  default:
    break;
  }
}

void print_help(const option_status_t status) {
  switch (status) {
  case option_status_t::help:
    std::cout << "help" << std::endl;
    break;

  default:
    break;
  }
}

void print_option_ga(const Option_ga &option_ga) {
  std::cout << "M: " << option_ga.M << std::endl;
  std::cout << "match_genetic: " << option_ga.match_genetic << std::endl;
  std::cout << "thresh: " << option_ga.thresh << std::endl;
  std::cout << "mutation_start: " << option_ga.mutation_start << std::endl;
  std::cout << "mutation_prob: " << option_ga.mutation_prob << std::endl;
  std::cout << "thread: " << option_ga.thread << std::endl;
  std::cout << "out_path: " << option_ga.out_path << std::endl;
}

void print_option_web(const Option_web &option_web) {
  std::cout << "board: " << option_web.board << std::endl;
  std::cout << "turn: " << option_web.turn << std::endl;
  std::cout << "is_turn_set: " << option_web.is_turn_set << std::endl;
  std::cout << "eval: " << option_web.eval << std::endl;
}

void print_option(const Option &option) {
  switch (option.mode) {
  case Mode::ga:
    std::cout << "Mode: ga" << std::endl;
    print_option_ga(option.option_ga);
    break;
  case Mode::web:
    std::cout << "Mode: web" << std::endl;
    print_option_web(option.option_web);
    break;
  }
  std::cout << "debug: " << option.debug << std::endl;
}

bool is_number(const std::string &x) {
  static const std::regex e1("[-+]?[0-9]*[.]?[0-9]+([eE][-+]?[0-9]+)?");
  static const std::regex e2("[-+]?[0-9]+[.]");
  return std::regex_match(x, e1) || std::regex_match(x, e2);
}

option_status_t to_int(const std::string &x, int &y) {
  if (!is_number(x))
    return option_status_t::not_number;

  y = std::stoi(x);
  return option_status_t::success;
}

option_status_t to_float(const std::string &x, float &y) {
  if (!is_number(x))
    return option_status_t::not_number;

  y = std::stof(x);
  return option_status_t::success;
}

option_status_t to_bool(const std::string &x, bool &y) {
  int num_int;
  if (!is_number(x))
    return option_status_t::not_number;

  num_int = std::stoi(x);

  if (num_int != 0 && num_int != 1)
    return option_status_t::invalid_argument;

  y = (num_int == 1);
  return option_status_t::success;
}

option_status_t parse_cmd(int argc, char **argv, Option &option) {
  int opt, option_index;
  std::string opt_arg, long_option_name;
  option_status_t option_status;

  while (true) {
    option_index = 0;
    if ((opt = getopt_long(argc, argv, OPT_STRING, long_options,
                           &option_index)) == -1) {
      break;
    }
    switch (opt) {
    case 0:
      long_option_name = std::string(long_options[option_index].name);

      if (optarg) {
        opt_arg = std::string(optarg);
      }

      if (long_option_name == "match_genetic") {
        option_status = to_int(opt_arg, option.option_ga.match_genetic);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "thresh") {
        option_status = to_float(opt_arg, option.option_ga.thresh);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "mutation_start") {
        option_status = to_int(opt_arg, option.option_ga.mutation_start);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "mutation_prob") {
        option_status = to_float(opt_arg, option.option_ga.mutation_prob);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "time_limit") {
        option_status = to_int(opt_arg, option.option_ga.time_limit);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "thread") {
        option_status = to_int(opt_arg, option.option_ga.thread);
        if (option_status != option_status_t::success)
          return option_status;
      } else if (long_option_name == "out_path") {
        option.option_ga.out_path = opt_arg;
      } else if (long_option_name == "eval") {
        option.option_web.eval = opt_arg;
      }
      break;
    case 'b':
      option.option_web.board = std::string(optarg);
      break;
    case 'd':
      option.debug = true;
      break;
    case 'e':
      option.option_web.eval = std::string(optarg);
      break;
    case 'h':
      return option_status_t::help;
      break;
    case 'm':
      opt_arg = std::string(optarg);
      if (opt_arg == "ga") {
        option.mode = Mode::ga;
      } else if (opt_arg == "web") {
        option.mode = Mode::web;
      } else {
        return option_status_t::invalid_argument;
      }
      break;
    case 'M':
      opt_arg = std::string(optarg);
      option_status = to_int(opt_arg, option.option_ga.M);
      if (option_status != option_status_t::success)
        return option_status;
      break;
    case 't':
      opt_arg = std::string(optarg);
      option_status = to_bool(opt_arg, option.option_web.turn);
      if (option_status != option_status_t::success)
        return option_status;
      option.option_web.is_turn_set = true;
      break;
    default:
      return option_status_t::invalid_argument;
      break;
    }
  }
  return option_status_t::success;
}

option_status_t validate_option_ga(Option_ga &option_ga) {
  if (option_ga.M <= 0)
    return option_status_t::out_of_range;
  if (option_ga.match_genetic <= 0)
    return option_status_t::out_of_range;
  if (option_ga.mutation_prob < 0 || option_ga.mutation_prob > 1)
    return option_status_t::out_of_range;
  if (option_ga.mutation_start < 0)
    return option_status_t::out_of_range;
  if (option_ga.thread <= 0) {
    return option_status_t::out_of_range;
  } else {
    option_ga.thread = std::min(option_ga.thread, 10);
  }
  if (option_ga.thresh < 0 || option_ga.thresh > 1)
    return option_status_t::out_of_range;
  if (option_ga.time_limit <= 0)
    return option_status_t::out_of_range;

  if (option_ga.out_path.empty()) {
    return option_status_t::path_not_selected;
  } else if (!std::filesystem::exists(option_ga.out_path)) {
    return option_status_t::path_not_found;
  } else if (!std::filesystem::is_directory(option_ga.out_path)) {
    return option_status_t::not_directory;
  }
  return option_status_t::success;
}

option_status_t validate_option_web(Option_web &option_web) {
  if (option_web.board.empty()) {
    return option_status_t::board_not_set;
  } else if (option_web.board.length() != 64) {
    return option_status_t::invalid_argument;
  } else {
    for (int i = 0; i < 64; ++i) {
      if (option_web.board[i] != '0' && option_web.board[i] != '1' &&
          option_web.board[i] != '2')
        return option_status_t::invalid_argument;
    }
  }
  if (!option_web.is_turn_set)
    return option_status_t::turn_not_selected;

  if (!option_web.eval.empty()) {
    if (!std::filesystem::exists(option_web.eval))
      return option_status_t::path_not_found;
    if (!std::filesystem::is_regular_file(option_web.eval))
      return option_status_t::not_file;
  }
  return option_status_t::success;
}

option_status_t validate_option(Option &option) {
  switch (option.mode) {
  case Mode::ga:
    return validate_option_ga(option.option_ga);
    break;
  case Mode::web:
    return validate_option_web(option.option_web);
  default:
    return option_status_t::mode_not_selected;
    break;
  }
  return option_status_t::success;
}

int set_option(int argc, char **argv, Option &option) {
  option_status_t option_status;

  // parse
  option_status = parse_cmd(argc, argv, option);
  print_option(option);
  if (option_status != option_status_t::success) {
    print_help(option_status);
    print_err(option_status);
    return -static_cast<int>(option_status);
  }

  // validation
  option_status = validate_option(option);
  if (option_status != option_status_t::success) {
    print_err(option_status);
    return -static_cast<int>(option_status);
  }

  return 0;
}