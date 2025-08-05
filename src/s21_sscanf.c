#include "s21_string.h"

int equal(const char **str, const char **frm, fstr *form) {
  if (!*str || !**frm) return 0;
  while (1)
    if (**frm == **str) {
      if ((*frm)[0] == '%' && (*frm)[1] == '%') (*frm)++;
      form->count++, (*frm)++, (*str)++;
    } else if (isspace(**frm)) {
      SKIP(*str);
      SKIP(*frm);
    } else
      break;
  return *(*frm)++ == '%';
}
int error_handling(const char **frm, fstr *form, const char **str) {
  if (*str && s21_strchr("cdieEfgGosuxXpn]", **frm))
    form->spec = *(*frm)++;
  else
    return 0;
  if (form->spec != 'n' && **str == 0)
    perror("no input for specifier"), *str = 0;
  else if (form->spec == 'p' && form->length && form->length != '*')
    perror("sscanf: p specifier with length modifier"), *str = 0;
  else if (form->spec == ']' && form->length && form->length != '*')
    perror("sscanf: [] specifier with length modifier"), *str = 0;
  else if (form->spec == 'n' && form->length)
    perror("sscanf: n specifier with length modifier"), *str = 0;
  else if (s21_strchr("douixX", form->spec) && form->length == 'L')
    perror("sscanf: L with integer specifiers"), *str = 0;
  else if ((form->spec == 's' || form->spec == 'c') &&
           (form->length == 'L' || form->length == 'h'))
    perror("sscanf: s, c can't be short or Long"), *str = 0;
  return (*str != S21_NULL);
}

int s21_sscanf(const char *str, const char *frm, ...) {
  va_list ap;
  if (!str || !frm || !*str) return -1;
  va_start(ap, frm);
  fstr form = {0};
  for (unsigned char i = 0, w = 4; equal(&str, &frm, &form);
       form.width = 0, form.exc = 0, i = 0) {
    ANYNUM(frm, 10, form.width, w);
    form.length = LENGTH(frm);
    if (*frm == 'l' && frm[1] == 's' && form.length == '*') form.length = 'X';
    if (*frm == 'l' || *frm == 'h' || *frm == '*' || *frm == 'L') frm++;

    if (*frm == '[') {
      if (*++frm == '^') form.exc = 1, frm++;
      FINDBRACKET(frm, i);
      if (!frm[i] || !i)
        perror("sscanf: missing ] or invalid content of []"), str = 0;
      else
        s21_strncpy(form.set, frm, i)[i] = 0, frm += i;
    }
    if (error_handling(&frm, &form, &str)) {
      if (form.spec == 'n')
        *va_arg(ap, u *) = form.count;
      else if (s21_strchr("eEfgG", form.spec)) {
        if (form.spec != 'c' && form.spec != ']') SKIP(str), form.count++;
        ASDOUBLE(form, str, ap);
      } else {
        if (form.spec != 'c' && form.spec != ']') SKIP(str), form.count++;
        form.assigned += process_form(&form, &str, &ap);
      }
    }
  }
  va_end(ap);
  return form.assigned;
}
int process_form(fstr *form, const char **str, va_list *ap) {
  u i = 0, w = form->width ?: 1, n = form->width ?: MAX_WIDTH;
  const char *str1 = *str;
  if (s21_strchr("diuoxXp", form->spec)) {
    l lval = 0;
    if (form->spec == 'd')
      lval = strtol1(str, 10, n);
    else if (form->spec == 'i')
      lval = strtol1(str, 0, n);
    else
      lval = strtoul1(str,
                      form->spec == 'u' ? 10 : (form->spec == 'o' ? 8 : 16), n);
    ASINT(form, lval, ap);
  } else if (form->spec == 'c') {
    i = MIN(w, (form->length == 'l' || form->length == 'X')
                   ? s21_wstrlen((wchar_t *)*str) * sizeof(wchar_t)
                   : s21_strlen(*str));
    if (form->length != 'X' && form->length != '*')
      s21_memcpy(va_arg(*ap, s), *str, i);
    *str += i;
  } else if (form->spec == 's') {
    if (form->length == 'l' || form->length == 'X') {
      const wchar_t *ptr = (wchar_t *)*str;
      while (ptr[i] && !iswspace(ptr[i])) i++;
      if (form->length != 'X')
        s21_wcsncpy(va_arg(*ap, wchar_t *), ptr, MIN(i, n))[i] = L'\0';
    } else {
      while ((*str)[i] && !isspace((*str)[i])) i++;
      if (form->length != '*')
        s21_strncpy(va_arg(*ap, s), *str, MIN(i, n))[i] = 0;
    }
    *str += MIN(i, n);
  } else {
    if ((i = MIN(((form->exc) ? s21_strcspn(*str, form->set)
                              : s21_strspn(*str, form->set)),
                 w))) {
      if (form->length != '*') s21_memcpy(va_arg(*ap, s), *str, i);
      *str += i;
    } else
      *str = 0;
  }
  if (*str && str1 != *str) n = *str - str1, form->count += n;
  return form->length != '*' && *str;
}
