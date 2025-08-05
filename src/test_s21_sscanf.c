#include <check.h>
#include <string.h>

#include "s21_string.h"
START_TEST(test_scanf_b1) {
  char *s3[] = {"134624563636345756563",   "-0xFDAFADA33FA", "-00000534FDADF",
                "-0xFFFFDFAFA.assfaf",     "FAFAFAF.FGF",    "01353456366787",
                "-0xAFAFAFFAFA.AFAFAFAFAG"};
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    int orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(s3[i], "%d", &orig), s21_sscanf(s3[i], "%d", &s21));
    ck_assert_int_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    lu orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(s3[i], "%lx", &orig),
                     s21_sscanf(s3[i], "%lx", &s21));
    ck_assert_int_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    int orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(s3[i], "%i", &orig), s21_sscanf(s3[i], "%i", &s21));
    ck_assert_int_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    f orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(s3[i], "%lf", &orig),
                     s21_sscanf(s3[i], "%lf", &s21));
    ck_assert_double_eq_tol(orig, s21, 1e-2);
  }
}
END_TEST

START_TEST(test_scanf_b2) {
  char *s3[] = {
      "inqeprjv498j5g24jv90h5gp3o4imgw9-ej8",
      "-0xFDAFrthwrthwhj35ue5net6u3by",
      "5yj46j5u46jtene56034FDADF",
      "-0xFFFFDDFA.erhwrthrt\t\nassfaf",
      "F\tAFAAF.FGF",
      "0j\n15j756h35ttu\nkyukyte566787",
      "-0x\t\t\t\\t\ttAgj,gu;y8lhjkyFAFAFA\n\nFAFAFA.AFAF\n\nAFAFAFAG"};
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    char orig[100], s21[100];
    ck_assert_int_eq(sscanf(s3[i], "%s", orig), s21_sscanf(s3[i], "%s", s21));
    ck_assert_str_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    char orig[100] = {0}, s21[100] = {0};
    int a = 0, b = 0;
    ck_assert_int_eq(sscanf(s3[i], "%100c%n", orig, &a),
                     s21_sscanf(s3[i], "%100c%n", s21, &b));
    ck_assert_int_eq(a, b);
    orig[a] = 0, s21[b] = 0;
    ck_assert_str_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(s3) / 8; i++) {
    char orig[100] = {0}, s21[100] = {0};
    int a = 0, b = 0;
    ck_assert_int_eq(sscanf(s3[i], "%15[qwertyuiop\';jhgdsaG]%n", orig, &a),
                     s21_sscanf(s3[i], "%15[qwertyuiop\';jhgdsaG]%n", s21, &b));
    ck_assert_int_eq(a, b);
    orig[a] = 0, s21[b] = 0;
    ck_assert_str_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_int) {
  int orig = 0, s21 = 0;
  char *str[] = {"0xFFFAFAD",
                 "-13345.3453",
                 "76543.9944",
                 "ABCDDD",
                 "02245346458",
                 "12345",
                 "0.45454",
                 "922337203685477583545"
                 ""};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%i", &orig),
                     s21_sscanf(str[i], "%i", &s21));
    ck_assert_int_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_double) {
  double orig = 0, s21 = 0;
  char *str[] = {"0.4564456e-2", "0xFFFF.afdda", "-0xFABC.CCCC",
                 "-00000000034656357.23e-2", "3.1415"};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%lf", &orig),
                     s21_sscanf(str[i], "%f", &s21));
    ck_assert_double_eq_tol(orig, s21, 1e-17);
  }
  char *s2[] = {"0X0.",
                "-.1",
                ".e-4",
                "0xFADFAFDADF.afafafaf8af987a",
                "23423e-",
                "24534.e+",
                ".234562362456356735722e+5",
                ".124523567567361346245735734563e+11",
                "1351345133656443453.",
                "23423534.E+5",
                "-0x8765fdaff.5af8a6s",
                "",
                "-01.e+2",
                ".123453543e-3",
                ".34532fdafaE+2"};
  for (u i = 0; i < sizeof(s2) / 8; i++) {
    double a = 0, b = 0;
    ck_assert_int_eq(s21_sscanf(s2[i], "%lf", &a), sscanf(s2[i], "%lf", &b));
    ck_assert_double_eq_tol(a, b, 1e-3);
  }
}
END_TEST

START_TEST(test_scanf_string) {
  char *str[] = {"0.456   4456e-2", "0         \na", "-0xFABC.CCCC",
                 "AASDASFDFGFGHFHF", " "};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    char orig[100] = {0}, s21[100] = {0};
    ck_assert_int_eq(sscanf(str[i], "%s", orig), s21_sscanf(str[i], "%s", s21));
    ck_assert_str_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_hex) {
  unsigned long orig = 0, s21 = 0;
  char *str[] = {"0.4564456e-2", "0xFFFF.afdda", "-0xFABC.C", "-056357",
                 "0xABB3423532"};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%lx", &orig),
                     s21_sscanf(str[i], "%lx", &s21));
    ck_assert_int_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_multiple) {
  long i1 = 0, i2 = 0, s21_i1 = 0, s21_i2 = 0;
  char *str[] = {"4564456e-2", "0xFFFF.afdda", "-0xFABCC",
                 "-00000000034656357.23e-2", "31415-----"};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%li %li", &i1, &i2),
                     s21_sscanf(str[i], "%li %li", &s21_i1, &s21_i2));
    ck_assert_int_eq(i1, s21_i1);
    ck_assert_int_eq(i2, s21_i2);
  }
}
END_TEST

START_TEST(test_scanf_octal) {
  unsigned orig = 0, s21 = 0;
  char *str[] = {"0755", "03452356456", "03124523646679", "0000000003423",
                 "-0"};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%o", &orig),
                     s21_sscanf(str[i], "%o", &s21));
    ck_assert_int_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_unsigned) {
  long unsigned orig = 0, s21 = 0;
  char *str[] = {"23635673578567474675467567567565jaspodifgafgp",
                 "0x23945345834534", "495873249853498",
                 "-42949672934645645647458734756473653475"};
  for (u i = 0; i < sizeof(*str) / 8; i++) {
    ck_assert_int_eq(sscanf(str[i], "%lu", &orig),
                     s21_sscanf(str[i], "%lu", &s21));
    ck_assert_int_eq(orig, s21);
  }
}
END_TEST

START_TEST(test_scanf_char) {
  char orig = 0, s21 = 0;
  char *S =
      "ABFdobierng\19843\29n8t\33vo\4m -3\5948ut=\6-235n\7-bsvmb =09 j3420\107 "
      " \nv-m\t4-9n\rg35\e8-g\v "
      "r-nfdsnfgl;wth\n";
  const char *str = S;
  do {
    int ret_orig = sscanf(str, "%c", &orig);
    int ret_s21 = s21_sscanf(str, "%c", &s21);
    ck_assert_int_eq(ret_orig, ret_s21);
    ck_assert_int_eq(orig, s21);
  } while (*str++);
}
END_TEST

START_TEST(test_scanf_set) {
  char orig[10] = {0}, oorig[10] = {0}, ss21[10] = {0}, s21[10] = {0};
  const char *str = "abc123ifj 000058hhgndfgeoinert";
  ck_assert_int_eq(sscanf(str, "%9[abcdefgz] %9[^akhdsfg498z]", orig, oorig),
                   s21_sscanf(str, "%9[abcdefgz] %9[^akhdsfg498z]", s21, ss21));
  ck_assert_str_eq(orig, s21);
  ck_assert_str_eq(oorig, ss21);
  s21_memset(orig, 0, 10), s21_memset(oorig, 0, 10), s21_memset(ss21, 0, 10),
      s21_memset(s21, 0, 10);
  ck_assert_int_eq(
      sscanf(str, "%9[^357khj] %9[a35bcdefghijkl 0]", orig, oorig),
      s21_sscanf(str, "%9[^357khj] %9[a35bcdefghijkl 0]", s21, ss21));
  ck_assert_str_eq(orig, s21);
  ck_assert_str_eq(oorig, ss21);
}
END_TEST

START_TEST(test_scanf_empty) {
  int orig = 42, s21 = 42;
  const char *str = "";
  int ret_orig = sscanf(str, "%d", &orig);
  int ret_s21 = s21_sscanf(str, "%d", &s21);
  ck_assert_int_eq(ret_orig, ret_s21);
}
END_TEST

START_TEST(test_scanf_gn) {
  char *str[] = {
      "       \
      -0xFFAFAB.DB",
      "0xFFAFAFAFA.FAF",
      "03516124641534"
      "0x1afa00.000"
      "03123454616716"
      "777"
      "hjeolihfgpif"
      "3-93hq3"
      "-000234245.78789E-2"
      "087=4"};
  // any integer in any base
  for (u i = 0; i < sizeof(str) / 8; i++) {
    long orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(str[i], "%li", &orig),
                     s21_sscanf(str[i], "%li", &s21));
    ck_assert_int_eq(orig, s21);
  }
  for (u i = 0; i < sizeof(str) / 8; i++) {
    char orig[4096] = {0}, s21[4096] = {0};
    ck_assert_int_eq(sscanf(str[i], "%s", orig), s21_sscanf(str[i], "%s", s21));
    ck_assert_str_eq(orig, s21);
  }
  // multiple nums int + double
  for (u i = 0; i < sizeof(str) / 8; i++) {
    double orig1 = 0, s211 = 0;
    long orig = 0, s21 = 0;
    ck_assert_int_eq(sscanf(str[i], "%li%lf", &orig, &orig1),
                     s21_sscanf(str[i], "%li%f", &s21, &s211));
    ck_assert_int_eq(orig, s21);
    ck_assert_double_eq_tol(orig1, s211, 1e-6);
  }
  /*
    wchar_t *s2[] =
    {L"JFOGHHEOOEGHOIEgfjfyHGOE!45",L"732q4g08\v937ohjtjty5ngq8bvpd;flmae",L"fjenrgbn0349587jtjhjnPUNP",L"49jvnw059h4wmvpdfopkjgj\t\b\v\n\\n\tgbms[pfg"};
    for(u i =0;i<sizeof(s2)/8;i++){
    char a[800] = {0}, e[800] = {0},g[800]={0};
    char g1[800] = {0},a1[800]={0},e1[800]={0};
    ck_assert_int_eq(s21_sscanf((s)s2[i],"%20lc%20ls%20ls",(wchar_t*)g,(wchar_t*)a,(wchar_t*)e),sscanf((s)s2[i],"%20lc%20ls%20ls",(wchar_t*)g1,(wchar_t*)a1,(wchar_t*)e1));
    ck_assert_str_eq(g,g1);
    ck_assert_str_eq(a,a1);
    ck_assert_str_eq(e,e1);
    }
   */
  // GEN multiple nums int + double
  long num1, num2, num3, num4, num5;
  double c, d, f;
  s str1 =
      "12345 -0x334 -04345 -0x5345 0xffffffffffff 0.234234 -0xAdadab 0.345 "
      "-0xff.ff";
  ck_assert_int_eq(s21_sscanf(str1, "%li%li%li%li%li%f%f%f", &num1, &num2,
                              &num3, &num4, &num5, &c, &d, &f),
                   sscanf(str1, "%li%li%li%li%li %lf%lf%lf", &num1, &num2,
                          &num3, &num4, &num5, &c, &d, &f));
}
END_TEST

Suite *test_s21_sscanf(void) {
  Suite *suite = suite_create("sscanf_tests");
  TCase *tc_core = tcase_create("base_functionality");
  tcase_add_test(tc_core, test_scanf_int);
  tcase_add_test(tc_core, test_scanf_double);
  tcase_add_test(tc_core, test_scanf_string);
  tcase_add_test(tc_core, test_scanf_hex);
  tcase_add_test(tc_core, test_scanf_multiple);
  tcase_add_test(tc_core, test_scanf_octal);
  tcase_add_test(tc_core, test_scanf_unsigned);
  tcase_add_test(tc_core, test_scanf_set);
  tcase_add_test(tc_core, test_scanf_char);
  tcase_add_test(tc_core, test_scanf_empty);
  tcase_add_test(tc_core, test_scanf_gn);
  tcase_add_test(tc_core, test_scanf_b1);
  tcase_add_test(tc_core, test_scanf_b2);
  suite_add_tcase(suite, tc_core);
  return suite;
}
