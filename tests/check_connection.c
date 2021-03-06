/*
 * check_connection.c
 *
 *  Created on: Mar 17, 2015
 *      Author: jnevens
 */

#include <check.h>
#include <stdlib.h>
#include <unistd.h>

#include "../include/libvsb/connection.h"
#include "../src/connection.h"

const char *tmp_vsb_socket = "/tmp/vsb.ut.socket";

START_TEST(test_vsb_connection_create_destroy)
{
	vsb_conn_t *conn = vsb_conn_init(5, "lol");
	ck_assert_ptr_ne(conn, NULL);
	ck_assert_int_eq(vsb_conn_get_fd(conn), 5);
	ck_assert_str_eq((const char *)vsb_conn_get_arg(conn), "lol");
	vsb_conn_destroy(conn);
}END_TEST

START_TEST(test_vsb_connection_name)
{
	vsb_conn_t *conn = vsb_conn_init(5, "lol");
	vsb_conn_set_name(conn, "foobar");
	ck_assert_str_eq(vsb_conn_get_name(conn), "foobar");
	vsb_conn_destroy(conn);
}END_TEST

START_TEST(test_vsb_connection_frame_receiver)
{
	vsb_conn_t *conn = vsb_conn_init(5, "lol");
	vsb_frame_receiver_t *receiver = vsb_conn_get_frame_receiver(conn);
	ck_assert_ptr_ne(receiver, NULL);
	vsb_conn_destroy(conn);
}END_TEST

static char *disco_test = NULL;
static void disco_callback(void *arg)
{
	disco_test = (char *)arg;
}

START_TEST(test_vsb_connection_disco_callback)
{
	vsb_conn_t *conn = vsb_conn_init(5, "lol");
	vsb_conn_register_disconnect_cb(conn, disco_callback ,"bar");
	vsb_conn_disconnect(conn);
	ck_assert_ptr_ne(disco_test, NULL);
	ck_assert_str_eq(disco_test, "bar");
	vsb_conn_destroy(conn);
}END_TEST


Suite * vsb_suite(void)
{
	Suite *s;
	TCase *tc_core;

	s = suite_create("vsb connection");

	/* Core test case */
	tc_core = tcase_create("Core");

	tcase_add_test(tc_core, test_vsb_connection_create_destroy);
	tcase_add_test(tc_core, test_vsb_connection_name);
	tcase_add_test(tc_core, test_vsb_connection_frame_receiver);
	tcase_add_test(tc_core, test_vsb_connection_disco_callback);
	tcase_add_test(tc_core, test_vsb_connection_create_destroy);
	suite_add_tcase(s, tc_core);

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = vsb_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

