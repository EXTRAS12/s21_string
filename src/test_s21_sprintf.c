#include "test_s21_sprintf.h"

static const char *flags[] = {"-", "+", " ", "#", "0"};
static const char *widths[] = {"", "1", "8", "15", "*"};
static const char *precisions[] = {"", ".0", ".1", ".8", ".*"};
static const char *lengths[] = {"", "h", "l", "L"};
#ifdef __APPLE__
static const char *specifiers[] = {"d", "i", "u", "o", "x", "X", "f", "F",
                                   "e", "E", "g", "G", "c", "s", "p", "%"};
#else
static const char *specifiers[] = {"d", "i", "u", "o", "x", "X", "f", "F", "e",
                                   "E", "g", "G", "c", "s", "p", "n", "%"};
#endif
static int int_values[] = {0, 1, -1, 123, -123, INT_MAX, INT_MIN};
static unsigned int uint_values[] = {0, 1, 123, UINT_MAX};
static double double_values[] = {0,           -0.0,     1.0,      -1.0,
                                 123.456,     -123.456, DBL_MAX,  DBL_MIN,
                                 DBL_EPSILON, NAN,      -INFINITY};
static const char *string_values[] = {"", "abc", "longer string", "123",
                                      "veryveryverylongstringthatexceedslimit"};
static const char char_values[] = {'\0', 'A', 'z', 127, 32};

START_TEST(test_cases_count) {
  ck_assert_msg(cases.count > 0, "No test cases generated (cases.count == 0)");
}

flag_combs comb_flags = {0};
format_test_cases cases = {0};

int is_ub_combination(const char *flag, const char *width,
                      const char *precision, const char *length,
                      const char *specifier_str) {
  char spec = specifier_str[0];

  int has_precision = (precision[0] == '.' || strcmp(precision, ".*") == 0);
  int has_star_width = strcmp(width, "*") == 0;

  int is_flag_plus = strchr(flag, '+') != NULL;
  int is_flag_space = strchr(flag, ' ') != NULL;
  int is_flag_minus = strchr(flag, '-') != NULL;
  int is_flag_zero = strchr(flag, '0') != NULL;
  int is_flag_hash = strchr(flag, '#') != NULL;

  return spec == '\0' || (is_flag_hash && strchr("diucspn", spec)) ||
         (is_flag_zero && strchr("cspn", spec)) ||
         (has_precision && strchr("cpn", spec)) ||
         ((is_flag_space || is_flag_plus) && strchr("p", spec)) ||
         (strchr(length, 'h') && strchr("fFeEgGaAcspn", spec)) ||
         (strchr(length, 'l') && strchr("cspn", spec)) ||
         (strchr(length, 'L') && strchr("diouxXcspn", spec)) ||
         (strchr(length, 'L') && strchr("aA", spec)) ||
         (strchr(length, 'j') || strchr(length, 'z') || strchr(length, 't')) ||
         ((spec == '%' || spec == 'n') &&
          (*flag || *width || *precision || *length)) ||
         (has_star_width && strchr("c%", spec)) ||
         (is_flag_plus && is_flag_space) || (is_flag_minus && is_flag_zero) ||
         (!strchr("diuoxXfFeEgGaAcspn%", spec));
}

void get_all_flags(void) {
  comb_flags.count = 0;
  comb_flags.str[comb_flags.count++][0] = '\0';

  unsigned int num_flags = sizeof(flags) / sizeof(flags[0]);
  unsigned int max_mask = 1u << num_flags;

  for (unsigned int mask = 1; mask < max_mask; ++mask) {
    char buf[FLAGS + 1] = {0};
    int pos = 0;

    for (unsigned int bit = 0; bit < num_flags; ++bit) {
      if (mask & (1u << bit)) {
        buf[pos++] = flags[bit][0];
      }
    }
    buf[pos] = '\0';

    strcpy(comb_flags.str[comb_flags.count++], buf);
  }
}

void get_all_forms(size_t flag_index) {
  size_t w_count = sizeof(widths) / sizeof(widths[0]);
  size_t p_count = sizeof(precisions) / sizeof(precisions[0]);
  size_t l_count = sizeof(lengths) / sizeof(lengths[0]);
  size_t s_count = sizeof(specifiers) / sizeof(specifiers[0]);

  for (size_t w = 0; w < w_count; ++w) {
    for (size_t p = 0; p < p_count; ++p) {
      for (size_t l = 0; l < l_count; ++l) {
        for (size_t s = 0; s < s_count; ++s) {
          const char *f = comb_flags.str[flag_index];
          const char *wd = widths[w];
          const char *pr = precisions[p];
          const char *ln = lengths[l];
          const char *sp = specifiers[s];

          if (is_ub_combination(f, wd, pr, ln, sp)) {
            continue;
          }

          char fmt[LIMIT_STR_FORMAT] = {0};
          if (pr[0] && sp[0] != '%') {
            snprintf(fmt, sizeof(fmt), "%%%s%s%s%s%s", f, wd, pr, ln, sp);
          } else {
            snprintf(fmt, sizeof(fmt), "%%%s%s%s%s", f, wd, ln, sp);
          }

          test_case_t *tc = &cases.test[cases.count];
          strcpy(tc->str_format, fmt);
          tc->fl_star_width = (strchr(wd, '*') != NULL);
          tc->fl_star_precision = (strchr(pr, '*') != NULL);
          tc->specifier = sp[0];

          tc->not_d_i_u = !strchr("diu", sp[0]);
          tc->not_o_x_X = !strchr("oxX", sp[0]);
          tc->not_f_F_e_E_g_G = !strchr("fFeEgG", sp[0]);
          tc->not_c_p_n = !strchr("cpn", sp[0]);
          tc->not_s = (sp[0] != 's');

          cases.count++;
        }
      }
    }
  }
}

START_TEST(test_one_format) {
  size_t i = _i;
  ck_assert_msg(i < cases.count, "Index out of range: %zu >= %zu", i,
                cases.count);

  char buf_std[LIMIT_STRING], buf_s21[LIMIT_STRING];
  const char *fmt = cases.test[i].str_format;
  char spec = cases.test[i].specifier;

  for (int v = 0; v < 3; ++v) {
    int res_std = 0, res_s21 = 0;

    if (!cases.test[i].not_d_i_u) {
      int val = int_values[v % (sizeof(int_values) / sizeof(int_values[0]))];
      if (cases.test[i].fl_star_width && cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 10, 2, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 10, 2, val);
      } else if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 10, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 10, val);
      } else if (cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 2, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 2, val);
      } else {
        res_std = sprintf(buf_std, fmt, val);
        res_s21 = s21_sprintf(buf_s21, fmt, val);
      }
    } else if (!cases.test[i].not_o_x_X) {
      unsigned int val =
          uint_values[v % (sizeof(uint_values) / sizeof(uint_values[0]))];
      if (cases.test[i].fl_star_width && cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 10, 2, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 10, 2, val);
      } else if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 10, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 10, val);
      } else if (cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 2, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 2, val);
      } else {
        res_std = sprintf(buf_std, fmt, val);
        res_s21 = s21_sprintf(buf_s21, fmt, val);
      }
    } else if (!cases.test[i].not_f_F_e_E_g_G) {
      double val =
          double_values[v % (sizeof(double_values) / sizeof(double_values[0]))];
      if (cases.test[i].fl_star_width && cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 16, 6, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 16, 6, val);
      } else if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 16, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 16, val);
      } else if (cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 6, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 6, val);
      } else {
        res_std = sprintf(buf_std, fmt, val);
        res_s21 = s21_sprintf(buf_s21, fmt, val);
      }
    } else if (!cases.test[i].not_c_p_n && spec == 'c') {
      char val =
          char_values[v % (sizeof(char_values) / sizeof(char_values[0]))];
      if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 6, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 6, val);
      } else {
        res_std = sprintf(buf_std, fmt, val);
        res_s21 = s21_sprintf(buf_s21, fmt, val);
      }
    } else if (!cases.test[i].not_s) {
      const char *val =
          string_values[v % (sizeof(string_values) / sizeof(string_values[0]))];
      if (cases.test[i].fl_star_width && cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 8, 3, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 8, 3, val);
      } else if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 8, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 8, val);
      } else if (cases.test[i].fl_star_precision) {
        res_std = sprintf(buf_std, fmt, 3, val);
        res_s21 = s21_sprintf(buf_s21, fmt, 3, val);
      } else {
        res_std = sprintf(buf_std, fmt, val);
        res_s21 = s21_sprintf(buf_s21, fmt, val);
      }
    } else if (!cases.test[i].not_c_p_n && spec == 'p') {
      int for_p = 0;
      if (cases.test[i].fl_star_width) {
        res_std = sprintf(buf_std, fmt, 16, (void *)&for_p);
        res_s21 = s21_sprintf(buf_s21, fmt, 16, (void *)&for_p);
      } else {
        res_std = sprintf(buf_std, fmt, (void *)&for_p);
        res_s21 = s21_sprintf(buf_s21, fmt, (void *)&for_p);
      }
    } else if (!cases.test[i].not_c_p_n && spec == 'n') {
      int n_std = 0, n_s21 = 0;
      res_std = sprintf(buf_std, fmt, &n_std);
      res_s21 = s21_sprintf(buf_s21, fmt, &n_s21);
      ck_assert_msg(n_std == n_s21, "[FAIL n write] fmt='%s', std=%d, s21=%d",
                    fmt, n_std, n_s21);
    } else if (spec == '%') {
      char ch = '%';
      res_std = sprintf(buf_std, fmt, ch);
      res_s21 = s21_sprintf(buf_s21, fmt, ch);
    }

    ck_assert_msg(res_std == res_s21, "[FAIL return] fmt='%s', std=%d, s21=%d",
                  fmt, res_std, res_s21);
    ck_assert_msg(strcmp(buf_std, buf_s21) == 0,
                  "[FAIL output] fmt='%s', std_out='%s', s21_out='%s'", fmt,
                  buf_std, buf_s21);
  }
}
END_TEST

Suite *test_s21_printf(void) {
  get_all_flags();

  cases.count = 0;
  for (size_t i = 0; i < comb_flags.count; ++i) {
    get_all_forms(i);
  }

  Suite *suite = suite_create("S21_sprintf_FULL");
  TCase *tc = tcase_create("FullCoverage");

  if (cases.count == 0) {
    ck_abort_msg("No test cases generated!");
  }

  tcase_add_test(tc, test_cases_count);
  tcase_add_loop_test(tc, test_one_format, 0, cases.count);

  suite_add_tcase(suite, tc);
  return suite;
}