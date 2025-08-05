#include "s21_sprintf.h"

int s21_sprintf(char* buf, const char* format, ...) {
  va_list args;
  va_start(args, format);
  char* cur_buf = buf;
  while (format && *format) {
    if (*format == '%') {
      ActiveOpts state = {0};
      state.width = -1;
      state.precision = -1;
      format++;
      format = get_format(&state, &args, format);
      if (*format == '%') {
        *(cur_buf++) = *format;
      } else if (state.error_format) {
        format = S21_NULL;
      } else {
        cur_buf = apply_format_for_arg(cur_buf, &args, &state, buf, *format);
      }
    } else {
      *(cur_buf++) = *format;
    }
    if (format && *format) {
      format++;
    }
  }
  *cur_buf = '\0';
  va_end(args);
  return (int)(cur_buf - buf);
}

const char* get_format(ActiveOpts* state, va_list* args, const char* format) {
  while (!state->error_format && !state->end_format && *format) {
    if (*format == '%') {
      state->end_format = 1;
    } else if (!state->end_width && s21_strchr(ACCSES_FLAGS, *format)) {
      format = set_flags(state, format);
    } else if (!state->end_width &&
               ((*format > '0' && *format <= '9') || (*format == '*'))) {
      format = set_width(state, args, format);
    } else if (!state->end_precision && *format == '.') {
      format = set_precision(state, args, format);
    } else if (!state->end_length && s21_strchr(ACCSES_LENGTH, *format)) {
      format = set_length(state, format);
    } else if (s21_strchr(ACCSES_SPECIFICATOR, *format)) {
      state->end_format = 1;
    } else {
      state->error_format = 1;
    }
  }
  return format;
}

const char* set_flags(ActiveOpts* state, const char* format) {
  if (*format == '-') {
    state->fl_minus = 1;
  } else if (*format == '+') {
    state->fl_plus = 1;
  } else if (*format == ' ') {
    state->fl_space = 1;
  } else if (*format == '#') {
    state->fl_sharp = 1;
  } else if (*format == '0') {
    state->fl_zero = 1;
  }
  return ++format;
}

const char* set_width(ActiveOpts* state, va_list* args, const char* format) {
  state->width = 0;
  if (*format == '*') {
    state->width = va_arg(*args, int);
    format++;
  } else {
    while (*format >= '0' && *format <= '9') {
      state->width = state->width * 10 + (*format - '0');
      format++;
    }
  }
  if (state->width == 0) {
    state->width = -1;
    state->fl_minus = 1;
  } else if (state->width < 0) {
    state->fl_minus = 1;
    state->width *= -1;
  }
  state->end_width = 1;
  return format;
}

const char* set_precision(ActiveOpts* state, va_list* args,
                          const char* format) {
  state->precision = 0;
  format++;
  if (*format == '*') {
    state->precision = va_arg(*args, int);
    format++;
  } else {
    while (*format >= '0' && *format <= '9') {
      state->precision = state->precision * 10 + (*format - '0');
      format++;
    }
  }
  if (state->precision < 0) {
    state->precision = -1;
  }
  state->end_width = 1;
  state->end_precision = 1;
  return format;
}

const char* set_length(ActiveOpts* state, const char* format) {
  if (*format == 'h') {
    state->fl_h = 1;
  } else if (*format == 'l') {
    state->fl_l = 1;
  } else if (*format == 'L') {
    state->fl_L = 1;
  }
  state->end_width = 1;
  state->end_precision = 1;
  state->end_length = 1;
  return ++format;
}

char* apply_format_for_arg(char* cur_buf, va_list* args,
                           const ActiveOpts* state, const char* start_buf,
                           char specificator) {
  if (specificator == 'd' || specificator == 'i') {
    cur_buf = apply_d_i(cur_buf, args, state);
  } else if (specificator == 'o' || specificator == 'u' ||
             specificator == 'x' || specificator == 'X') {
    cur_buf = apply_o_u_x_X(cur_buf, args, state, specificator);
  } else if ((specificator == 'f') || (specificator == 'F') ||
             (specificator == 'e') || (specificator == 'E') ||
             (specificator == 'g') || (specificator == 'G')) {
    cur_buf = apply_f_F_e_E_g_G(cur_buf, args, state, specificator);
  } else if (specificator == 'c') {
    cur_buf = apply_c(cur_buf, args, state);
  } else if (specificator == 's') {
    cur_buf = apply_s(cur_buf, args, state);
  } else if (specificator == 'p') {
    cur_buf = apply_p(cur_buf, args, state);
  } else if (specificator == 'n') {
    cur_buf = apply_n(start_buf, cur_buf, args);
  }
  return cur_buf;
}

char* apply_d_i(char* cur_buf, va_list* args, const ActiveOpts* state) {
  long d = state->fl_l ? va_arg(*args, long) : va_arg(*args, int);
  char temp_buf[MAX_LEN_ST11] = {0};
  int len = 0;
  if (d == 0 && state->precision == 0) {
  } else {
    if (state->fl_h) {
      d = (short)d;
    } else if (!state->fl_l) {
      d = (int)d;
    }
    len = s21_itoa(temp_buf, d, state->precision);
  }
  cur_buf = do_opts_for_d_i(cur_buf, state, len, temp_buf);
  return cur_buf;
}

char* apply_o_u_x_X(char* cur_buf, va_list* args, const ActiveOpts* state,
                    char spe) {
  unsigned long d =
      state->fl_l ? va_arg(*args, unsigned long) : va_arg(*args, unsigned int);
  char temp_buf[MAX_LEN_ST11] = {0};
  int len = 0;
  if (d == 0 && state->precision == 0) {
  } else {
    if (state->fl_h) {
      d = (unsigned short)d;
    } else if (!state->fl_l) {
      d = (unsigned int)d;
    }
    int base =
        (spe == 'o') ? 8 : ((spe == 'u') ? 10 : ((spe == 'x') ? 16 : -16));
    len = s21_uitoa(temp_buf, d, base, state->precision);
  }
  cur_buf = do_opts_for_o_u_x_X(cur_buf, state, len, temp_buf, spe);
  return cur_buf;
}

char* apply_f_F_e_E_g_G(char* cur_buf, va_list* args, const ActiveOpts* state,
                        char spe) {
  long double f =
      state->fl_L ? va_arg(*args, long double) : va_arg(*args, double);
  char temp_buf[MAX_LEN_ST11] = {0};
  int len = 0;
  int inf_znak = isinf(f);
  if (inf_znak > 0) {
    s21_strncpy(temp_buf,
                (spe == 'f' || spe == 'e' || spe == 'g') ? "inf" : "INF", 10);
    len = 3;
  } else if (inf_znak < 0) {
    s21_strncpy(temp_buf,
                (spe == 'f' || spe == 'e' || spe == 'g') ? "-inf" : "-INF", 10);
    len = 4;
  } else if (isnan(f)) {
    if (!signbit(f)) {
      s21_strncpy(temp_buf,
                  (spe == 'f' || spe == 'e' || spe == 'g') ? "nan" : "NAN", 10);
      len = 3;
    } else {
      s21_strncpy(temp_buf,
                  (spe == 'f' || spe == 'e' || spe == 'g') ? "-nan" : "-NAN",
                  10);
      len = 4;
    }
  } else if (spe == 'f' || spe == 'F') {
    len = get_res_f_F(temp_buf, f, state->precision, state->fl_sharp, spe);
  } else {
    int e10 = 0;
    len =
        get_res_e_E(temp_buf, &e10, f, state->precision, state->fl_sharp, spe);
    if (spe == 'g' || spe == 'G') {
      int precision = (state->precision == -1)  ? 6
                      : (state->precision == 0) ? 1
                                                : state->precision;
      len = (precision > e10 && e10 > -4)
                ? get_res_f_F(temp_buf, f, precision - e10 - 1, state->fl_sharp,
                              spe)
                : get_res_e_E(temp_buf, &e10, f, precision - 1, state->fl_sharp,
                              spe);
    }
  }
  cur_buf = do_opts_for_f_F_e_E_g_G(cur_buf, state, len, temp_buf);
  return cur_buf;
}

char* apply_s(char* cur_buf, va_list* args, const ActiveOpts* state) {
  const char* str = S21_NULL;
  if (state->fl_l) {
    cur_buf = apply_ls(cur_buf, args, state);
  } else {
    str = va_arg(*args, const char*);
    if (!str) str = "(null)";
    cur_buf = do_opts_for_s_c(cur_buf, str, (int)s21_strlen(str), state);
  }
  return cur_buf;
}

char* apply_ls(char* cur_buf, va_list* args, const ActiveOpts* state) {
  const wchar_t* wstr = va_arg(*args, wchar_t*);
  char temp_buf[MAX_LEN_ST11] = {0};
  const char* str = S21_NULL;
  int str_len = 0;

  if (!wstr) {
    str = "(null)";
    str_len = 6;
  } else {
    s21_size_t conv = wcstombs(temp_buf, wstr, sizeof(temp_buf) - 1);
    if (conv == (s21_size_t)-1) {
      str = "";
      str_len = 0;
    } else {
      str = temp_buf;
      str_len = (int)conv;
    }
  }
  cur_buf = do_opts_for_s_c(cur_buf, str, str_len, state);
  return cur_buf;
}

char* apply_c(char* cur_buf, va_list* args, const ActiveOpts* state) {
  if (state->fl_l) {
    cur_buf = apply_lc(cur_buf, args, state);
  } else {
    char ch = (char)va_arg(*args, int);
    const char str[2] = {ch, '\0'};
    cur_buf = do_opts_for_s_c(cur_buf, str, 1, state);
  }
  return cur_buf;
}

char* apply_lc(char* cur_buf, va_list* args, const ActiveOpts* state) {
  wint_t wch = va_arg(*args, wint_t);
  char mbstr[5] = {0};
  int len = wctomb(mbstr, wch);
  if (len < 0) {
    mbstr[0] = '?';
    len = 1;
  }
  cur_buf = do_opts_for_s_c(cur_buf, mbstr, len, state);
  return cur_buf;
}

char* apply_p(char* cur_buf, va_list* args, const ActiveOpts* state) {
  char temp_buf[MAX_LEN_ST11] = {0};
  int len = 0;
  void* p = va_arg(*args, void*);
  if (p == S21_NULL) {
    s21_strncpy(temp_buf, "(nil)", 10);
    len = 5;
  } else {
    len = s21_uitoa(temp_buf, (unsigned long)p, 16, -1);
  }
  cur_buf = do_opts_for_p(cur_buf, state, len, temp_buf);
  return cur_buf;
}

char* apply_n(const char* start_buf, char* cur_buf, va_list* args) {
  int* n = va_arg(*args, int*);
  *n = cur_buf - start_buf;
  return cur_buf;
}

int s21_itoa(char* buf, long value, int precision) {
  int cur_indx = 0;
  bool isNegative = (value < 0);
  if (isNegative) {
    value = -value;
  }
  do {
    int remainder = value % 10;
    buf[cur_indx++] = remainder + '0';
    value /= 10;
  } while (value != 0);
  while (cur_indx < precision) {
    buf[cur_indx++] = '0';
  }
  if (isNegative) {
    buf[cur_indx++] = '-';
  }
  buf[cur_indx] = '\0';
  for (int start = 0, end = cur_indx - 1; start < end; ++start, --end) {
    char temp = buf[start];
    buf[start] = buf[end];
    buf[end] = temp;
  }
  return cur_indx;
}

int s21_uitoa(char* buf, unsigned long value, int base, int precision) {
  int cur_indx = 0;
  int A_or_a = (base == -16) ? 'A' : 'a';
  if (base < 0) {
    base *= -1;
  }
  do {
    int remainder = value % base;
    buf[cur_indx++] = remainder > 9 ? remainder - 10 + A_or_a : remainder + '0';
    value /= base;
  } while (value != 0);
  while (cur_indx < precision) {
    buf[cur_indx++] = '0';
  }
  buf[cur_indx] = '\0';
  for (int start = 0, end = cur_indx - 1; start < end; ++start, --end) {
    char temp = buf[start];
    buf[start] = buf[end];
    buf[end] = temp;
  }
  return cur_indx;
}

int get_res_f_F(char* buf, long double value, int precision, bool fl_sharp,
                char spe) {
  bool val_isNeg = (signbit(value));
  if (val_isNeg) {
    value = -value;
  }
  long double integer_part = 0.0L;
  long double fractional_part = modfl(value, &integer_part);
  int cur_indx = make_buf_integer_part(buf, integer_part, val_isNeg);
  if (!precision) {
    if (fl_sharp) {
      buf[cur_indx++] = '.';
    }
    if ((int)(fractional_part * 10.0L) > 4) {
      make_round_up(buf, &cur_indx, 1, 1 + fl_sharp, val_isNeg, -1);
    }
  } else {
    buf[cur_indx++] = '.';
    (precision == -1) && (precision = 6);
    bool round_up =
        make_buf_fractional_part(buf, &cur_indx, fractional_part, precision);
    if (round_up) {
      make_round_up(buf, &cur_indx, round_up, 2, val_isNeg, -1);
    }
  }
  if ((spe == 'g' || spe == 'G') && !fl_sharp && s21_strchr(buf, '.')) {
    cur_indx = delet_0_for_g_G(buf, cur_indx);
  }
  buf[cur_indx] = '\0';
  return cur_indx;
}

int get_res_e_E(char* buf, int* e10, long double value, int first_precision,
                bool fl_sharp, char spe) {
  bool val_isNeg = (signbit(value));
  if (val_isNeg) {
    value = -value;
  }
  long double integer_part = 0.0L;
  long double fractional_part = modfl(value, &integer_part);
  int cur_indx = make_buf_integer_part(buf, integer_part, val_isNeg);
  *e10 = cur_indx - 1 - val_isNeg;
  int precision = process_integer_part_for_e_E(
      buf, &cur_indx, e10, &fractional_part, integer_part, first_precision,
      val_isNeg, fl_sharp);
  if (precision) {
    bool round_up =
        make_buf_fractional_part(buf, &cur_indx, fractional_part, precision);
    if (round_up) {
      *e10 = make_round_up(buf, &cur_indx, round_up, 2, val_isNeg, *e10);
    }
  }
  if ((spe == 'g' || spe == 'G') && !fl_sharp && s21_strchr(buf, '.')) {
    cur_indx = delet_0_for_g_G(buf, cur_indx);
  }
  buf[cur_indx++] = (spe == 'e' || spe == 'g') ? 'e' : 'E';
  (*e10 >= 0) && (buf[cur_indx++] = '+');
  char temp_buf[64] = {0};
  int temp_indx = s21_itoa(temp_buf, *e10, 2);
  for (int i = 0; i < temp_indx; i++) {
    buf[cur_indx++] = temp_buf[i];
  }
  buf[cur_indx] = '\0';
  return cur_indx;
}

int make_buf_integer_part(char* buf, long double integer_part,
                          bool isNegative) {
  int cur_indx = 0;
  do {
    int remainder = (int)(integer_part - 10.0L * truncl(integer_part / 10.0L));
    buf[cur_indx++] = remainder + '0';
    integer_part = truncl(integer_part /= 10.0L);
  } while (integer_part > 0.1L && !isnan(integer_part) && !isinf(integer_part));
  if (isNegative) {
    buf[cur_indx++] = '-';
  }
  for (int start = 0, end = cur_indx - 1; start < end; ++start, --end) {
    char temp = buf[start];
    buf[start] = buf[end];
    buf[end] = temp;
  }
  return cur_indx;
}

bool make_buf_fractional_part(char* buf, int* cur_indx,
                              long double fractional_part, int precision) {
  bool round_up = 0;
  long double for_integer = 0;
  for (int k = 1; k <= precision; k++) {
    fractional_part *= 10.0L;
    int current_digit = (int)(fractional_part);
    if (k == precision) {
      round_up = 4 < ((int)(fractional_part * 10.0L) - current_digit * 10);
      current_digit += round_up;
      (current_digit == 10) ? (current_digit = 0) : (round_up = 0);
    }
    buf[(*cur_indx)++] = current_digit + '0';
    fractional_part = modfl(fractional_part, &for_integer);
  }
  return round_up;
}

int make_round_up(char* buf, int* cur_indx, bool round_up, int delta_indx,
                  bool value_isNegative, int e10) {
  int indx_point = -1;
  for (int i = *cur_indx - delta_indx; round_up && i >= value_isNegative; i--) {
    if (buf[i] != '.') {
      buf[i]++;
      (buf[i] == ('9' + 1)) ? (buf[i] = '0') : (round_up = 0);
    } else {
      indx_point = i;
    }
  }
  if (round_up && buf[value_isNegative] == '0') {
    *cur_indx = insert_chr(buf, *cur_indx, (int)value_isNegative, '1');
    e10++;
    if (indx_point != -1) {
      buf[indx_point + 1] = buf[indx_point];
      buf[indx_point] = '.';
    }
  }
  return e10;
}

int insert_chr(char* buf, int cur_indx, int insert_indx, char chr) {
  for (int i = cur_indx; i > insert_indx; --i) {
    buf[i] = buf[i - 1];
  }
  buf[insert_indx] = chr;
  cur_indx++;
  return cur_indx;
}

int process_integer_part_for_e_E(char* buf, int* cur_indx, int* e10,
                                 long double* fractional_part,
                                 long double integer_part, int first_precision,
                                 bool val_isNeg, bool fl_sharp) {
  int precision = (first_precision == -1) ? 6 : first_precision;
  if (integer_part > 0.1L) {
    if (precision <= *e10) {
      *cur_indx = precision + 1 + val_isNeg;
      if (precision < *e10 && buf[*cur_indx] > '4') {
        *e10 = make_round_up(buf, cur_indx, 1, 1, val_isNeg, *e10);
      } else if (precision == *e10 && (int)(*fractional_part * 10.0L) > 4) {
        *e10 = make_round_up(buf, cur_indx, 1, 1, val_isNeg, *e10);
      }
      precision = 0;
    } else {
      precision -= *e10;
    }
    *cur_indx = (!first_precision && !fl_sharp)
                    ? (1 + val_isNeg)
                    : insert_chr(buf, *cur_indx, 1 + (int)val_isNeg, '.');
  } else {
    int digit = (int)integer_part;
    while (!digit && *fractional_part > 1e-19) {
      (*e10)--;
      *fractional_part = modfl(*fractional_part * 10.0L, &integer_part);
      digit = (int)integer_part;
    }
    buf[val_isNeg] = digit + '0';
    if (!first_precision) {
      if (fl_sharp) {
        buf[(*cur_indx)++] = '.';
      }
      if ((int)(*fractional_part * 10.0L) > 4) {
        *e10 = make_round_up(buf, cur_indx, 1, 1 + fl_sharp, val_isNeg, *e10);
      }
    } else {
      buf[(*cur_indx)++] = '.';
    }
  }
  return precision;
}

int delet_0_for_g_G(char* buf, int cur_indx) {
  while (buf[cur_indx - 1] == '0') {
    buf[-1 + cur_indx--] = '\0';
  }
  if (buf[cur_indx - 1] == '.') {
    buf[-1 + cur_indx--] = '\0';
  }
  return cur_indx;
}

char* do_opts_for_d_i(char* cur_buf, const ActiveOpts* state, int len,
                      const char* temp_buf) {
  int delta = state->width - len;
  int cur_indx = 0;
  if (delta > 0 && !state->fl_minus &&
      (!state->fl_zero || state->precision != -1)) {
    bool fl_for_d_i =
        ((state->fl_plus || state->fl_space) && temp_buf[0] != '-');
    while (delta > fl_for_d_i) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  if (temp_buf[0] == '-') {
    *cur_buf++ = '-';
    cur_indx++;
  } else if (state->fl_plus) {
    *cur_buf++ = '+';
    delta--;
  } else if (state->fl_space) {
    *cur_buf++ = ' ';
    delta--;
  }
  if (delta > 0 && !state->fl_minus && state->fl_zero &&
      state->precision == -1) {
    while (delta) {
      *cur_buf++ = '0';
      delta--;
    }
  }
  while (cur_indx < len) {
    *cur_buf++ = temp_buf[cur_indx];
    cur_indx++;
  }
  if (state->fl_minus) {
    while (delta > 0) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  return cur_buf;
}

char* do_opts_for_o_u_x_X(char* cur_buf, const ActiveOpts* state, int len,
                          const char* temp_buf, char spe) {
  int cur_indx = 0;
  int delta = state->width - len;
  bool fl_for_xX = (len > 0 && temp_buf[len - 1] != '0');
  if (delta > 0 && !state->fl_minus &&
      (!state->fl_zero || state->precision != -1)) {
    bool fl_for_o =
        ((len == 0 && (state->precision == 0 || state->precision == -1)) ||
         (len > 0 && temp_buf[0] != '0'));
    while (delta > ((!state->fl_sharp || spe == 'u') ? 0
                    : (spe == 'o')                   ? fl_for_o
                                                     : fl_for_xX + fl_for_xX)) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  if (state->fl_sharp) {
    if (spe == 'o' && (len == 0 || temp_buf[0] != '0')) {
      *cur_buf++ = '0';
      delta--;
    } else if (spe != 'u' && spe != 'o' && fl_for_xX) {
      *cur_buf++ = '0';
      (spe == 'x') ? (*cur_buf++ = 'x') : (*cur_buf++ = 'X');
      delta -= 2;
    }
  }
  if (delta > 0 && !state->fl_minus && state->fl_zero) {
    while (delta) {
      *cur_buf++ = '0';
      delta--;
    }
  }
  while (cur_indx < len) {
    *cur_buf++ = temp_buf[cur_indx];
    cur_indx++;
  }
  if (state->fl_minus) {
    while (delta > 0) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  return cur_buf;
}

char* do_opts_for_f_F_e_E_g_G(char* cur_buf, const ActiveOpts* state, int len,
                              const char* temp_buf) {
  int delta = state->width - len;
  int cur_indx = 0;
  bool fl_for_nan_inf = (s21_strpbrk(temp_buf, "nN") != S21_NULL);
  if (delta > 0 && !state->fl_minus && (!state->fl_zero || fl_for_nan_inf)) {
    bool fl_for_fF =
        ((state->fl_plus || state->fl_space) && temp_buf[0] != '-');
    while (delta > fl_for_fF) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  if (temp_buf[0] == '-') {
    *cur_buf++ = '-';
    cur_indx++;
  } else if (state->fl_plus) {
    *cur_buf++ = '+';
    delta--;
  } else if (state->fl_space ||
             (delta > 0 && !state->fl_minus && !state->fl_zero)) {
    *cur_buf++ = ' ';
    delta--;
  }
  if (delta > 0 && !state->fl_minus && state->fl_zero && !fl_for_nan_inf) {
    while (delta) {
      *cur_buf++ = '0';
      delta--;
    }
  }
  while (cur_indx < len) {
    *cur_buf++ = temp_buf[cur_indx];
    cur_indx++;
  }
  if (state->fl_minus) {
    while (delta > 0) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  return cur_buf;
}

char* do_opts_for_s_c(char* cur_buf, const char* str, int str_len,
                      const ActiveOpts* state) {
  int out_len = str_len;
  if (state->precision >= 0 && state->precision < str_len) {
    out_len = state->precision;
  }
  int pad = (state->width > out_len) ? (state->width - out_len) : 0;
  if (!state->fl_minus) {
    for (int i = 0; i < pad; ++i) {
      *cur_buf++ = ' ';
    }
  }
  for (int i = 0; i < out_len; ++i) {
    *cur_buf++ = str[i];
  }
  if (state->fl_minus) {
    for (int i = 0; i < pad; ++i) {
      *cur_buf++ = ' ';
    }
  }
  return cur_buf;
}

char* do_opts_for_p(char* cur_buf, const ActiveOpts* state, int len,
                    const char* temp_buf) {
  int delta = state->width - len;
  bool fl_for_nil = (s21_strpbrk(temp_buf, "n") != S21_NULL);
  if (delta > 0 && !state->fl_minus) {
    bool fl_for_p = (state->fl_plus || state->fl_space);
    while (delta > ((!fl_for_nil) ? (2 + fl_for_p) : 0)) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  if (!fl_for_nil) {
    if (state->fl_plus) {
      *cur_buf++ = '+';
      delta--;
    } else if (state->fl_space) {
      *cur_buf++ = ' ';
      delta--;
    }
    *cur_buf++ = '0';
    *cur_buf++ = 'x';
    delta -= 2;
  }
  for (int i = 0; i < len; i++) {
    *cur_buf++ = temp_buf[i];
  }
  if (state->fl_minus) {
    while (delta > 0) {
      *cur_buf++ = ' ';
      delta--;
    }
  }
  return cur_buf;
}