#ifndef _UNITTEST_H_
#define _UNITTEST_H_

/* parser */
void test_parse_one_number();
void test_parse_one_number2();
void test_parse_one_empty_should_return_END_OF_FILE();
void test_parse_one_executable_name();
void test_parse_one_literal_name();
void test_parse_one_open_curly();
void test_parse_one_close_curly();
void test_parse_one_unknown();

#endif
