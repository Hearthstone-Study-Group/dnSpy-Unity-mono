#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "test.h"

RESULT
test_list_length ()
{
	GList *list = g_list_prepend (NULL, "foo");

	if (g_list_length (list) != 1)
		return FAILED ("length failed. #1");

	list = g_list_prepend (list, "bar");
	if (g_list_length (list) != 2)
		return FAILED ("length failed. #2");

	list = g_list_append (list, "bar");
	if (g_list_length (list) != 3)
		return FAILED ("length failed. #3");

	g_list_free (list);
	return NULL;
}

RESULT
test_list_nth ()
{
	char *foo = "foo";
	char *bar = "bar";
	char *baz = "baz";
	GList *nth, *list;
	list = g_list_prepend (NULL, baz);
	list = g_list_prepend (list, bar);
	list = g_list_prepend (list, foo);

	nth = g_list_nth (list, 0);
	if (nth->data != foo)
		return FAILED ("nth failed. #0");

	nth = g_list_nth (list, 1);
	if (nth->data != bar)
		return FAILED ("nth failed. #1");
	
	nth = g_list_nth (list, 2);
	if (nth->data != baz)
		return FAILED ("nth failed. #2");

	nth = g_list_nth (list, 3);
	if (nth)
		return FAILED ("nth failed. #3: %s", nth->data);

	g_list_free (list);
	return OK;
}

RESULT
test_list_index ()
{
	int i;
	char *foo = "foo";
	char *bar = "bar";
	char *baz = "baz";
	GList *list;
	list = g_list_prepend (NULL, baz);
	list = g_list_prepend (list, bar);
	list = g_list_prepend (list, foo);

	i = g_list_index (list, foo);
	if (i != 0)
		return FAILED ("index failed. #0: %d", i);

	i = g_list_index (list, bar);
	if (i != 1)
		return FAILED ("index failed. #1: %d", i);
	
	i = g_list_index (list, baz);
	if (i != 2)
		return FAILED ("index failed. #2: %d", i);

	g_list_free (list);
	return OK;
}

RESULT
test_list_append ()
{
	GList *list = g_list_prepend (NULL, "first");
	if (g_list_length (list) != 1)
		return FAILED ("Prepend failed");

	list = g_list_append (list, "second");

	if (g_list_length (list) != 2)
		return FAILED ("Append failed");

	g_list_free (list);
	return OK;
}

RESULT
test_list_last ()
{
	GList *foo = g_list_prepend (NULL, "foo");
	GList *bar = g_list_prepend (NULL, "bar");
	GList *last;
	
	foo = g_list_concat (foo, bar);
	last = g_list_last (foo);

	if (last != bar)
		return FAILED ("last failed. #1");

	foo = g_list_concat (foo, g_list_prepend (NULL, "baz"));
	foo = g_list_concat (foo, g_list_prepend (NULL, "quux"));

	last = g_list_last (foo);	
	if (strcmp ("quux", last->data))
		return FAILED ("last failed. #2");

	g_list_free (foo);

	return OK;
}

RESULT
test_list_concat ()
{
	GList *foo = g_list_prepend (NULL, "foo");
	GList *bar = g_list_prepend (NULL, "bar");
	GList *list = g_list_concat (foo, bar);

	if (g_list_length (list) != 2)
		return FAILED ("Concat failed. #1");

	if (strcmp (list->data, "foo"))
		return FAILED ("Concat failed. #2");

	if (strcmp (list->next->data, "bar"))
		return FAILED ("Concat failed. #3");

	if (g_list_first (list) != foo)
		return FAILED ("Concat failed. #4");
	
	if (g_list_last (list) != bar)
		return FAILED ("Concat failed. #5");

	g_list_free (list);

	return OK;
}


static gint
compare (gconstpointer a, gconstpointer b)
{
	char *foo = (char *) a;
	char *bar = (char *) b;

	if (strlen (foo) < strlen (bar))
		return -1;

	return 1;
}

RESULT
test_list_insert_sorted ()
{
	GList *list = g_list_prepend (NULL, "a");
	list = g_list_append (list, "aaa");

	/* insert at the middle */
	list = g_list_insert_sorted (list, "aa", compare);
	if (strcmp ("aa", list->next->data))
		return FAILED ("insert_sorted failed. #1");

	/* insert at the beginning */
	list = g_list_insert_sorted (list, "", compare);
	if (strcmp ("", list->data))
		return FAILED ("insert_sorted failed. #2");		

	/* insert at the end */
	list = g_list_insert_sorted (list, "aaaa", compare);
	if (strcmp ("aaaa", g_list_last (list)->data))
		return FAILED ("insert_sorted failed. #3");

	g_list_free (list);
	return OK;
}

RESULT
test_list_copy ()
{
	int i, length;
	GList *list, *copy;
	list = g_list_prepend (NULL, "a");
	list = g_list_append  (list, "aa");
	list = g_list_append  (list, "aaa");
	list = g_list_append  (list, "aaaa");

	length = g_list_length (list);
	copy = g_list_copy (list);

	for (i = 0; i < length; i++)
		if (strcmp (g_list_nth (list, i)->data,
			    g_list_nth (copy, i)->data))
			return FAILED ("copy failed.");

	g_list_free (list);
	g_list_free (copy);	
	return OK;
}

RESULT
test_list_reverse ()
{
	guint i, length;
	GList *list, *reverse;
	list = g_list_prepend (NULL, "a");
	list = g_list_append  (list, "aa");
	list = g_list_append  (list, "aaa");
	list = g_list_append  (list, "aaaa");

	length  = g_list_length (list);
	reverse = g_list_reverse (g_list_copy (list));

	if (g_list_length (reverse) != length)
		return FAILED ("reverse failed #1");

	for (i = 0; i < length; i++){
		guint j = length - i - 1;
		if (strcmp (g_list_nth (list, i)->data,
			    g_list_nth (reverse, j)->data))
			return FAILED ("reverse failed. #2");
	}

	g_list_free (list);
	g_list_free (reverse);	
	return OK;
}

RESULT
test_list_remove ()
{
	GList *list = g_list_prepend (NULL, "three");
	char *one = "one";
	list = g_list_prepend (list, "two");
	list = g_list_prepend (list, one);

	list = g_list_remove (list, one);

	if (g_list_length (list) != 2)
		return FAILED ("Remove failed");

	if (strcmp ("two", list->data) != 0)
		return FAILED ("Remove failed");

	g_list_free (list);
	return OK;
}

RESULT
test_list_remove_link ()
{
	GList *foo = g_list_prepend (NULL, "a");
	GList *bar = g_list_prepend (NULL, "b");
	GList *baz = g_list_prepend (NULL, "c");
	GList *list = foo;

	foo = g_list_concat (foo, bar);
	foo = g_list_concat (foo, baz);	

	list = g_list_remove_link (list, bar);

	if (g_list_length (list) != 2)
		return FAILED ("remove_link failed #1");

	if (bar->next != NULL)
		return FAILED ("remove_link failed #2");

	g_list_free (list);	
	g_list_free (bar);
	return OK;
}

RESULT
test_list_insert_before ()
{
	GList *foo, *bar, *baz;

	foo = g_list_prepend (NULL, "foo");
	foo = g_list_insert_before (foo, NULL, "bar");
	bar = g_list_last (foo);

	if (strcmp (bar->data, "bar"))
		return FAILED ("1");

	baz = g_list_insert_before (foo, bar, "baz");
	if (foo != baz)
		return FAILED ("2");

	if (strcmp (g_list_nth_data (foo, 1), "baz"))
		return FAILED ("3: %s", g_list_nth_data (foo, 1));	

	g_list_free (foo);
	return OK;
}

#define N_ELEMS 101

static int intcompare (gconstpointer p1, gconstpointer p2)
{
	return GPOINTER_TO_INT (p1) - GPOINTER_TO_INT (p2);
}

static gboolean verify_sort (GList *list, int len)
{
	int prev;

	if (list->prev)
		return FALSE;

	prev = GPOINTER_TO_INT (list->data);
	len--;
	for (list = list->next; list; list = list->next) {
		int curr = GPOINTER_TO_INT (list->data);
		if (prev > curr)
			return FALSE;
		prev = curr;

		if (!list->prev || list->prev->next != list)
			return FALSE;

		if (len == 0)
			return FALSE;
		len--;
	}
	return len == 0;
}

RESULT
test_list_sort ()
{
	int i, j, mul;
	GList *list = NULL;

	for (i = 0; i < N_ELEMS; ++i)
		list = g_list_prepend (list, GINT_TO_POINTER (i));
	list = g_list_sort (list, intcompare);
	if (!verify_sort (list, N_ELEMS))
		return FAILED ("decreasing list");

	g_list_free (list);

	list = NULL;
	for (i = 0; i < N_ELEMS; ++i)
		list = g_list_prepend (list, GINT_TO_POINTER (-i));
	list = g_list_sort (list, intcompare);
	if (!verify_sort (list, N_ELEMS))
		return FAILED ("increasing list");

	g_list_free (list);

	list = g_list_prepend (NULL, GINT_TO_POINTER (0));
	for (i = 1; i < N_ELEMS; ++i) {
		list = g_list_prepend (list, GINT_TO_POINTER (i));
		list = g_list_prepend (list, GINT_TO_POINTER (-i));
	}
	list = g_list_sort (list, intcompare);
	if (!verify_sort (list, 2*N_ELEMS-1))
		return FAILED ("alternating list");

	g_list_free (list);

	list = NULL;
	mul = 1;
	for (i = 1; i < N_ELEMS; ++i) {
		mul = -mul;
		for (j = 0; j < i; ++j)
			list = g_list_prepend (list, GINT_TO_POINTER (mul * j));
	}
	list = g_list_sort (list, intcompare);
	if (!verify_sort (list, (N_ELEMS*N_ELEMS - N_ELEMS)/2))
		return FAILED ("wavering list");

	g_list_free (list);

	return OK;
}

static gint
find_custom (gconstpointer a, gconstpointer b)
{
	return(strcmp (a, b));
}

RESULT
test_list_find_custom ()
{
	GList *list = NULL, *found;
	char *foo = "foo";
	char *bar = "bar";
	char *baz = "baz";
	
	list = g_list_prepend (list, baz);
	list = g_list_prepend (list, bar);
	list = g_list_prepend (list, foo);
	
	found = g_list_find_custom (list, baz, find_custom);
	
	if (found == NULL)
		return FAILED ("Find failed");
	
	g_list_free (list);
	
	return OK;
}

static Test list_tests [] = {
	{       "length", test_list_length},
	{          "nth", test_list_nth},
	{        "index", test_list_index},	
	{         "last", test_list_last},	
	{       "append", test_list_append},
	{       "concat", test_list_concat},
	{"insert_sorted", test_list_insert_sorted},
	{"insert_before", test_list_insert_before},
	{         "copy", test_list_copy},
	{      "reverse", test_list_reverse},
	{       "remove", test_list_remove},
	{  "remove_link", test_list_remove_link},
	{  "remove_link", test_list_remove_link},
	{         "sort", test_list_sort},
	{  "find_custom", test_list_find_custom},
	{NULL, NULL}
};

DEFINE_TEST_GROUP_INIT(list_tests_init, list_tests)
