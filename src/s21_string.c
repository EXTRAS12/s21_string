#include "s21_string.h"

s21_size_t s21_strspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  if (str1 && str2)
    while (s21_strchr(str2, *str1++)) count++;
  return count;
}

s21_size_t s21_wstrlen(const wchar_t *str) {
  s21_size_t len = 0;
  while (str[len] != L'\0') len++;
  return len;
}

wchar_t *s21_wcsncpy(wchar_t *dest, const wchar_t *src, s21_size_t n) {
  wchar_t *save = dest;
  while ((*dest++ = *src++) != L'\0' && n--);
  return save;
}

long strtol1(const char **number, u base, unsigned width) {
  const char *num = *number;
  int sign = SIGN(num);
  long val = 0;
  if (*num == '0' && base != 10) {
    if (num[1] == 'x' && (base == 16 || base == 0)) {
      base = 16;
      (num) += 2;
      width -= 2;
    } else if (base == 0 || base == 8) {
      base = 8;
      num++;
      width--;
    }
  } else {
    base = base ? base : 10;
  }
  if (width && width <= MAX_WIDTH && (base > 1 && base < 17) &&
      conv(num) < base)
    do {
      val *= base;
      val += conv(num);
      (num)++;
    } while (--width && conv(num) < base && val >= 0);
  else
    num = 0;
  if (val < 0) {
    val = LONG_MAX;
    while (--width && conv(num) < base) num++;
  }
  *number = num;
  return sign ? -val : val;
}

lu strtoul1(const char **number, u base, unsigned width) {
  const char *num = *number;
  if (width == 0) width = 128;

  int sign = SIGN(num);
  if (*num == '0') {
    if (num[1] == 'x' && (base == 16 || base == 0)) {
      base = 16;
      num += 2;
      width -= 2;
    } else if (base == 0 || base == 8) {
      base = 8;
      num++;
      width--;
    }
  } else {
    base = base ? base : 10;
  }
  lu val = 0;
  if (width && width <= MAX_WIDTH && (base > 1 && base < 17) &&
      conv(num) < base) {
    do {
      lu val1 = val * base + conv(num);
      val = (val1 < val) ? ULONG_MAX : val1;
      num++;
    } while (--width && conv(num) < base && val != ULONG_MAX);
  } else
    num = 0;
  if (val == ULONG_MAX)
    while (--width && conv(num) < base) num++;
  *number = num;
  return sign ? -val : val;
}

f strtod1(const char **num, unsigned width) {
  f val = 0;
  if (width == 0) width = 128;
  const char *num1 = *num;
  int sign = SIGN(num1);
  if (!s21_strncmp(num1, "nan", 3) || !s21_strncmp(num1, "NAN", 3))
    val = NAN;
  else if (!s21_strncmp(num1, "inf", 3) || !s21_strncmp(num1, "INF", 3))
    val = INFINITY;
  else {
    unsigned base =
        (*num1 == '0' && (num1[1] == 'x' || num1[1] == 'X') && (num1 += 2))
            ? 16
            : 10;
    width -= MIN(width, num1 - *num);
    ANYNUM(num1, base, val, width);
    if (*num1 == '.' && width--) {
      num1++;
      for (u w = 1; conv(num1) < base && width--; w++, num1++)
        val += conv(num1) / powl(base, w);
      if (num1 - *num < 2) num1 = 0;
    }
    if (num1 && (*num1 == 'e' || *num1 == 'E') && width--) {
      num1++;
      f v = 0;
      int sg = SIGN(num1);
      ANYNUM(num1, 10, v, width);
      if (sg)
        val /= powl(10, v);
      else
        val *= powl(10, v);
    }
    if (num1 == *num)
      *num = 0;
    else
      *num = num1;
  }
  return sign ? -val : val;
}

Lf strtold1(const char **num, unsigned width) {
  Lf val = 0;
  if (width == 0) width = 128;
  const char *num1 = *num;
  int sign = SIGN(num1);
  if (!s21_strncmp(num1, "nan", 3) || !s21_strncmp(num1, "NAN", 3))
    val = NAN;
  else if (!s21_strncmp(num1, "inf", 3) || !s21_strncmp(num1, "INF", 3))
    val = INFINITY;
  else {
    unsigned base =
        (*num1 == '0' && (num1[1] == 'x' || num1[1] == 'X') && (num1 += 2))
            ? 16
            : 10;
    width -= MIN(width, num1 - *num);
    ANYNUM(num1, base, val, width);
    if (num1 && *num1 == '.' && width--) {
      num1++;
      for (u w = 1; conv(num1) < base && width--; w++, num1++)
        val += conv(num1) / pow(base, w);
      if (num1 - *num < 2) num1 = 0;
    }
    if (num1 && (*num1 == 'e' || *num1 == 'E') && width--) {
      num1++;
      Lf v = 0;
      int sg = SIGN(num1);
      ANYNUM(num1, 10, v, width);
      if (sg)
        val /= powl(10, v);
      else
        val *= powl(10, v);
    }
    if (num1 == *num)
      *num = 0;
    else
      *num = num1;
  }
  return sign ? -val : val;
}

#if defined(__APPLE__)
#define CNT 106
#define UNKNOWN "Unknown error: "
static const char *err[] = {"Undefined error: 0",
                            "Operation not permitted",
                            "No such file or directory",
                            "No such process",
                            "Interrupted system call",
                            "Input/output error",
                            "Device not configured",
                            "Argument list too long",
                            "Exec format error",
                            "Bad file descriptor",
                            "No child processes",
                            "Resource deadlock avoided",
                            "Cannot allocate memory",
                            "Permission denied",
                            "Bad address",
                            "Block device required",
                            "Resource busy",
                            "File exists",
                            "Cross-device link",
                            "Operation not supported by device",
                            "Not a directory",
                            "Is a directory",
                            "Invalid argument",
                            "Too many open files in system",
                            "Too many open files",
                            "Inappropriate ioctl for device",
                            "Text file busy",
                            "File too large",
                            "No space left on device",
                            "Illegal seek",
                            "Read-only file system",
                            "Too many links",
                            "Broken pipe",
                            "Numerical argument out of domain",
                            "Result too large",
                            "Resource temporarily unavailable",
                            "Operation now in progress",
                            "Operation already in progress",
                            "Socket operation on non-socket",
                            "Destination address required",
                            "Message too long",
                            "Protocol wrong type for socket",
                            "Protocol not available",
                            "Protocol not supported",
                            "Socket type not supported",
                            "Operation not supported",
                            "Protocol family not supported",
                            "Address family not supported by protocol family",
                            "Address already in use",
                            "Can't assign requested address",
                            "Network is down",
                            "Network is unreachable",
                            "Network dropped connection on reset",
                            "Software caused connection abort",
                            "Connection reset by peer",
                            "No buffer space available",
                            "Socket is already connected",
                            "Socket is not connected",
                            "Can't send after socket shutdown",
                            "Too many references: can't splice",
                            "Operation timed out",
                            "Connection refused",
                            "Too many levels of symbolic links",
                            "File name too long",
                            "Host is down",
                            "No route to host",
                            "Directory not empty",
                            "Too many processes",
                            "Too many users",
                            "Disc quota exceeded",
                            "Stale NFS file handle",
                            "Too many levels of remote in path",
                            "RPC struct is bad",
                            "RPC version wrong",
                            "RPC prog. not avail",
                            "Program version wrong",
                            "Bad procedure for program",
                            "No locks available",
                            "Function not implemented",
                            "Inappropriate file type or format",
                            "Authentication error",
                            "Need authenticator",
                            "Device power is off",
                            "Device error",
                            "Value too large to be stored in data type",
                            "Bad executable (or shared library)",
                            "Bad CPU type in executable",
                            "Shared library version mismatch",
                            "Malformed Mach-o file",
                            "Operation canceled",
                            "Identifier removed",
                            "No message of desired type",
                            "Illegal byte sequence",
                            "Attribute not found",
                            "Bad message",
                            "EMULTIHOP (Reserved)",
                            "No message available on STREAM",
                            "ENOLINK (Reserved)",
                            "No STREAM resources",
                            "Not a STREAM",
                            "Protocol error",
                            "STREAM ioctl timeout",
                            "Operation not supported on socket",
                            "Policy not found",
                            "State not recoverable",
                            "Previous owner died",
                            "Interface output queue is full"};

#elif defined(__linux__)
#define CNT 133
#define UNKNOWN "Unknown error "

static const char *err[] = {"Success",
                            "Operation not permitted",
                            "No such file or directory",
                            "No such process",
                            "Interrupted system call",
                            "Input/output error",
                            "No such device or address",
                            "Argument list too long",
                            "Exec format error",
                            "Bad file descriptor",
                            "No child processes",
                            "Resource temporarily unavailable",
                            "Cannot allocate memory",
                            "Permission denied",
                            "Bad address",
                            "Block device required",
                            "Device or resource busy",
                            "File exists",
                            "Invalid cross-device link",
                            "No such device",
                            "Not a directory",
                            "Is a directory",
                            "Invalid argument",
                            "Too many open files in system",
                            "Too many open files",
                            "Inappropriate ioctl for device",
                            "Text file busy",
                            "File too large",
                            "No space left on device",
                            "Illegal seek",
                            "Read-only file system",
                            "Too many links",
                            "Broken pipe",
                            "Numerical argument out of domain",
                            "Numerical result out of range",
                            "Resource deadlock avoided",
                            "File name too long",
                            "No locks available",
                            "Function not implemented",
                            "Directory not empty",
                            "Too many levels of symbolic links",
                            "Unknown error 41",
                            "No message of desired type",
                            "Identifier removed",
                            "Channel number out of range",
                            "Level 2 not synchronized",
                            "Level 3 halted",
                            "Level 3 reset",
                            "Link number out of range",
                            "Protocol driver not attached",
                            "No CSI structure available",
                            "Level 2 halted",
                            "Invalid exchange",
                            "Invalid request descriptor",
                            "Exchange full",
                            "No anode",
                            "Invalid request code",
                            "Invalid slot",
                            "Unknown error 58",
                            "Bad font file format",
                            "Device not a stream",
                            "No data available",
                            "Timer expired",
                            "Out of streams resources",
                            "Machine is not on the network",
                            "Package not installed",
                            "Object is remote",
                            "Link has been severed",
                            "Advertise error",
                            "Srmount error",
                            "Communication error on send",
                            "Protocol error",
                            "Multihop attempted",
                            "RFS specific error",
                            "Bad message",
                            "Value too large for defined data type",
                            "Name not unique on network",
                            "File descriptor in bad state",
                            "Remote address changed",
                            "Can not access a needed shared library",
                            "Accessing a corrupted shared library",
                            ".lib section in a.out corrupted",
                            "Attempting to link in too many shared libraries",
                            "Cannot exec a shared library directly",
                            "Invalid or incomplete multibyte or wide character",
                            "Interrupted system call should be restarted",
                            "Streams pipe error",
                            "Too many users",
                            "Socket operation on non-socket",
                            "Destination address required",
                            "Message too long",
                            "Protocol wrong type for socket",
                            "Protocol not available",
                            "Protocol not supported",
                            "Socket type not supported",
                            "Operation not supported",
                            "Protocol family not supported",
                            "Address family not supported by protocol",
                            "Address already in use",
                            "Cannot assign requested address",
                            "Network is down",
                            "Network is unreachable",
                            "Network dropped connection on reset",
                            "Software caused connection abort",
                            "Connection reset by peer",
                            "No buffer space available",
                            "Transport endpoint is already connected",
                            "Transport endpoint is not connected",
                            "Cannot send after transport endpoint shutdown",
                            "Too many references: cannot splice",
                            "Connection timed out",
                            "Connection refused",
                            "Host is down",
                            "No route to host",
                            "Operation already in progress",
                            "Operation now in progress",
                            "Stale file handle",
                            "Structure needs cleaning",
                            "Not a XENIX named type file",
                            "No XENIX semaphores available",
                            "Is a named type file",
                            "Remote I/O error",
                            "Disk quota exceeded",
                            "No medium found",
                            "Wrong medium type",
                            "Operation canceled",
                            "Required key not available",
                            "Key has expired",
                            "Key has been revoked",
                            "Key was rejected by service",
                            "Owner died",
                            "State not recoverable",
                            "Operation not possible due to RF-kill",
                            "Memory page has hardware error"};

#endif

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *result = S21_NULL;
  unsigned char *buffer = (unsigned char *)str;
  int flag = 0;
  for (s21_size_t i = 0; i < n && !flag; i++) {
    if (buffer[i] == c) {
      result = &buffer[i];
      flag = 1;
    }
  }
  return result;
}

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const unsigned char *s1 = (const unsigned char *)str1;
  const unsigned char *s2 = (const unsigned char *)str2;
  int res = 0;
  int flag = 0;
  for (s21_size_t i = 0; i < n && !flag; i++) {
    if (s1[i] != s2[i]) {
      res = s1[i] - s2[i];
      flag = 1;
    }
  }
  return res;
}

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *dst = (char *)dest;
  const char *s = (const char *)src;

  for (s21_size_t i = 0; i < n; i++) {
    dst[i] = s[i];
  }
  return dest;
}

s21_size_t s21_strlen(const char *str) {
  s21_size_t len = 0;
  if (str) {
    for (s21_size_t i = 0; str[i]; i++) {
      len++;
    }
  }
  return len;
}

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *buffer = (unsigned char *)str;
  for (s21_size_t i = 0; i < n; i++) {
    buffer[i] = c;
  }
  return str;
}

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  s21_size_t len = 0;
  s21_size_t cnt = 0;
  while (dest[len] != '\0') {
    len++;
  }
  for (; cnt < n && src[cnt] != '\0'; cnt++) {
    dest[len + cnt] = src[cnt];
  }
  dest[len + cnt] = '\0';
  return dest;
}

char *s21_strchr(const char *str, int c) {
  if (!str) return S21_NULL;
  s21_size_t len = s21_strlen(str);
  char *ptr = S21_NULL;
  int flag = 0;
  for (s21_size_t i = 0; i < len + 1 && !flag; i++) {
    if (str[i] == c) {
      ptr = (char *)&str[i];
      flag = 1;
    }
  }
  return ptr;
}

int s21_strncmp(const char *str1, const char *str2, size_t n) {
  s21_size_t i = 0;
  int diff = 0;
  int flag = 0;
  while ((str1[i] != '\0' || str2[i] != '\0') && !flag && i < n) {
    diff = str1[i] - str2[i];
    if (diff != 0) {
      flag = 1;
    }
    i++;
  }
  return diff;
}

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  char *ptr = dest;
  s21_size_t i;

  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  for (; i < n; i++) {
    dest[i] = '\0';
  }
  return ptr;
}

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  if (str1 && str2) {
    int found = 0;
    while (str1[count] && !found) {
      const char *ptr = str2;
      while (*ptr && !found) {
        if (str1[count] == *ptr) {
          found = 1;
        }
        ptr++;
      }
      if (!found) count++;
    }
  }
  return count;
}

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = S21_NULL;
  for (; *str1 && !res; str1++) {
    if (s21_strchr(str2, *str1)) {
      res = (char *)str1;
    }
  }
  return res;
}

char *s21_strrchr(const char *str, int c) {
  char *res = S21_NULL;
  do {
    if (*str == (char)c) {
      res = (char *)str;
    }
  } while (*str++);
  return res;
}

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = S21_NULL;
  if (!*needle) {
    res = (char *)haystack;
  } else {
    for (; *haystack && !res; haystack++) {
      const char *hay = haystack;
      const char *need = needle;
      while (*hay && *need && *hay == *need) {
        hay++;
        need++;
      }
      if (!*need) {
        res = (char *)haystack;
      }
    }
  }
  return res;
}

char *s21_strtok(char *str, const char *delim) {
  static char *tok = S21_NULL;
  char *start_tok = S21_NULL;
  if (str) tok = str;
  if (tok && *tok) {
    while (*tok && s21_strchr(delim, *tok)) tok++;
    if (*tok) {
      start_tok = tok;
      while (*tok && !s21_strchr(delim, *tok)) tok++;
      if (*tok) {
        *tok = '\0';
        tok++;
      } else {
        tok = S21_NULL;
      }
    }
  }
  return start_tok;
}

char *s21_strerror(int errnum) {
  static char buffer[100];
  const char *msg = S21_NULL;
  if (errnum >= 0 && errnum <= CNT) {
    msg = err[errnum];
  } else {
#if defined(__APPLE__)
    s21_sprintf(buffer, "%s%d", UNKNOWN, errnum);
    msg = buffer;
#elif defined(__linux__)
    s21_sprintf(buffer, "%s%d", UNKNOWN, errnum);
    msg = buffer;
#else
    s21_sprintf(buffer, "Unknown error: %d", errnum);
    msg = buffer;
#endif
  }
  return (char *)msg;
}

void *s21_to_upper(const char *str) {
  if (!str) return S21_NULL;
  int len = s21_strlen(str);
  char *str_ptr = (char *)calloc(len + 1, sizeof(char));
  s21_strncpy(str_ptr, str, len);

  for (int i = 0; i < len; i++) {
    if (str[i] >= 97 && str[i] <= 122) str_ptr[i] = str[i] - 32;
  }

  return str_ptr;
}

void *s21_to_lower(const char *str) {
  if (!str) return S21_NULL;
  int len = s21_strlen(str);
  char *str_ptr = (char *)calloc(len + 1, sizeof(char));
  s21_strncpy(str_ptr, str, len);

  for (int i = 0; i < len; i++) {
    if (str[i] >= 65 && str[i] <= 90) str_ptr[i] = str[i] + 32;
  }

  return str_ptr;
}

void *s21_insert(const char *src, const char *str, size_t start_index) {
  if (!src || !str) return S21_NULL;
  int len_src = s21_strlen(src);
  int len_str = s21_strlen(str);
  char *res_str = (char *)calloc(len_str + len_src + 1, sizeof(char));
  s21_memcpy(res_str, src, start_index);
  s21_memcpy(res_str + start_index, str, len_str);
  s21_memcpy(res_str + start_index + len_str, src + start_index,
             len_src - start_index);

  return res_str;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (!src || !trim_chars) return S21_NULL;

  const char *begin = src;
  int src_len = s21_strlen(src);
  const char *end = begin + src_len - 1;

  while (*begin && s21_strchr(trim_chars, *begin)) begin++;
  while (end >= begin && s21_strchr(trim_chars, *end)) end--;

  int new_len = (end >= begin) ? (end - begin + 1) : 0;
  char *new_str = (char *)calloc(new_len + 1, sizeof(char));
  if (new_len > 0) s21_memcpy(new_str, begin, new_len);

  return new_str;
}