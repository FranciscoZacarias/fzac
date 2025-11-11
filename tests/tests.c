#include "../fzac.h"

function void string_test(Test_Result* test);

int main(void)
{
  Test_Result test = test_init();

  string_test(&test);

  return 0;
}

function void
string_test(Test_Result* test)
{
  Arena* arena = arena_alloc();
  test_section("Strings");  

  // === string_copy ===
  {
    String src = S("Hello");
    String dst = string_copy(arena, src);
    test_equal(test, src.size, dst.size);
    test_memory_equal(test, src.str, dst.str, (u32)src.size);
  }

  // === string_range ===
  {
    u8 buf[] = "abcdef";
    String s = string_range(buf + 1, buf + 4); // "bcd"
    test_equal(test, s.size, 3);
    test_true(test, memory_match(s.str, "bcd", 3));
  }

  // === string_concat ===
  {
    String a = S("foo");
    String b = S("bar");
    String c = string_concat(arena, a, b); // "foobar"
    test_equal(test, c.size, 6);
    test_true(test, memory_match(c.str, "foobar", 6));
  }

  // === string_slice ===
  {
    String s = S("testing");
    String sub = string_slice(s, 1, 4); // "est"
    test_equal(test, sub.size, 3);
    test_true(test, memory_match(sub.str, "est", 3));
  }

  // === string_trim ===
  {
    String s = S("   trim me\t\n");
    String trimmed = string_trim(s);
    test_true(test, string_match(trimmed, S("trim me"), true));
  }

  // === string_contains ===
  {
    String s = S("abcdefg");
    test_true(test, string_contains(s, S("cde")));
    test_false(test, string_contains(s, S("xyz")));
  }

  // === string_find_first / string_find_last ===
  {
    String s = S("abcabcabc");
    u64 index = 0;
    test_true(test, string_find_first(s, S("abc"), &index));
    test_equal(test, index, 0);
    test_true(test, string_find_last(s, S("abc"), &index));
    test_equal(test, index, 6);
    test_false(test, string_find_first(s, S("zzz"), &index));
  }

  // === string_match ===
  {
    String a = S("Hello");
    String b = S("Hello");
    String c = S("hello");
    test_true(test, string_match(a, b, true));
    test_false(test, string_match(a, c, true));
    test_true(test, string_match(a, c, false));
  }

  // === string_from_format ===
  {
    String formatted = string_from_format(arena, "%s %d", "Age", 30);
    test_true(test, string_match(formatted, S("Age 30"), true));
  }

  // === string_split / string_list_join ===
  {
    String text = S("one,two,three");
    String_List list = string_split(arena, text, S(","));
    test_equal(test, list.node_count, 3);
    test_equal(test, list.total_size, 11);
    test_true(test, string_match(list.first->value, S("one"), true));
    test_true(test, string_match(list.first->next->value, S("two"), true));
    test_true(test, string_match(list.last->value, S("three"), true));

    String joined = string_list_join(arena, &list);
    test_true(test, string_match(joined, S("onetwothree"), true));
  }

  // === string_list_new / string_list_push / string_list_pop ===
  {
    String_List list = string_list_new(arena);
    test_equal(test, list.node_count, 0);

    string_list_push(arena, &list, S("first"));
    test_equal(test, list.node_count, 1);
    test_true(test, string_match(list.first->value, S("first"), true));

    string_list_push(arena, &list, S("second"));
    test_equal(test, list.node_count, 2);
    test_true(test, string_match(list.last->value, S("second"), true));

    string_list_push(arena, &list, S("third"));
    test_equal(test, list.node_count, 3);
    test_true(test, string_match(list.last->value, S("third"), true));

    String popped = string_list_remove_first(&list);
    test_true(test, string_match(popped, S("first"), true));
    test_equal(test, list.node_count, 2);
    test_true(test, string_match(list.first->value, S("second"), true));

    popped = string_list_remove_last(&list);
    test_true(test, string_match(popped, S("third"), true));
    test_equal(test, list.node_count, 1);
    test_true(test, string_match(list.first->value, S("second"), true));
  }

  // === string_from_cstring / cstring_from_string / cstring_length ===
  {
    u8 cstr[] = "Hello";
    String s = string_from_cstring(cstr);
    test_equal(test, s.size, 5);
    test_true(test, memory_match(s.str, "Hello", 5));

    u8* roundtrip = cstring_from_string(arena, s);
    test_equal(test, cstring_length(roundtrip), 5);
    test_true(test, strcmp((char*)roundtrip, "Hello") == 0);
  }

  test_summary(test);
  arena_free(arena);
}