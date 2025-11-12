#include "../fzac.h"

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

  arena_free(arena);
}

function void
v2f32_test(Test_Result* test)
{
  Arena* arena = arena_alloc();
  test_section("V2f32 Math");

  // === v2f32_add ===
  {
    V2f32 a = {1.0f, 2.0f};
    V2f32 b = {3.0f, 4.0f};
    V2f32 r = v2f32_add(a, b);
    test_typed_equal_float(test, f32, r.x, 4.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 6.0f, Epsilon32);
  }

  // === v2f32_sub ===
  {
    V2f32 a = {5.0f, 4.0f};
    V2f32 b = {2.0f, 1.0f};
    V2f32 r = v2f32_sub(a, b);
    test_typed_equal_float(test, f32, r.x, 3.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 3.0f, Epsilon32);
  }

  // === v2f32_mul / v2f32_div ===
  {
    V2f32 a = {2.0f, 4.0f};
    V2f32 b = {3.0f, 2.0f};
    V2f32 m = v2f32_mul(a, b);
    V2f32 d = v2f32_div(a, b);
    test_typed_equal_float(test, f32, m.x, 6.0f, Epsilon32);
    test_typed_equal_float(test, f32, m.y, 8.0f, Epsilon32);
    test_typed_equal_float(test, f32, d.x, 0.6667f, 0.001f);
    test_typed_equal_float(test, f32, d.y, 2.0f, 0.001f);
  }

  // === v2f32_dot ===
  {
    V2f32 a = {1.0f, 2.0f};
    V2f32 b = {3.0f, 4.0f};
    f32 dot = v2f32_dot(a, b);
    test_typed_equal_float(test, f32, dot, 11.0f, Epsilon32);
  }

  // === v2f32_len / v2f32_len_sqr ===
  {
    V2f32 v = {3.0f, 4.0f};
    test_typed_equal_float(test, f32, v2f32_len(v), 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, v2f32_len_sqr(v), 25.0f, Epsilon32);
  }

  // === v2f32_normalize ===
  {
    V2f32 v = {3.0f, 4.0f};
    V2f32 n = v2f32_normalize(v);
    test_typed_equal_float(test, f32, n.x, 0.6f, Epsilon32);
    test_typed_equal_float(test, f32, n.y, 0.8f, Epsilon32);
    test_typed_equal_float(test, f32, v2f32_len(n), 1.0f, Epsilon32);
  }

  // === v2f32_cross ===
  {
    V2f32 a = {1.0f, 0.0f};
    V2f32 b = {0.0f, 1.0f};
    f32 c = v2f32_cross(a, b);
    test_typed_equal_float(test, f32, c, 1.0f, Epsilon32);
  }

  // === v2f32_scale ===
  {
    V2f32 v = {1.0f, 2.0f};
    V2f32 s = v2f32_scale(v, 3.0f);
    test_typed_equal_float(test, f32, s.x, 3.0f, Epsilon32);
    test_typed_equal_float(test, f32, s.y, 6.0f, Epsilon32);
  }

  // === v2f32_lerp ===
  {
    V2f32 a = {0.0f, 0.0f};
    V2f32 b = {10.0f, 10.0f};
    V2f32 mid = v2f32_lerp(a, b, 0.5f);
    test_typed_equal_float(test, f32, mid.x, 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, mid.y, 5.0f, Epsilon32);
  }

  // === v2f32_dist / v2f32_dist_sqr ===
  {
    V2f32 a = {1.0f, 1.0f};
    V2f32 b = {4.0f, 5.0f};
    test_typed_equal_float(test, f32, v2f32_dist(a, b), 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, v2f32_dist_sqr(a, b), 25.0f, Epsilon32);
  }

  // === v2f32_angle ===
  {
    V2f32 a = {1.0f, 0.0f};
    V2f32 b = {0.0f, 1.0f};
    f32 angle = v2f32_angle(a, b);
    test_typed_equal_float(test, f32, angle, Pi32 * 0.5f, 0.0001f);
  }

  // === v2f32_reflect ===
  {
    V2f32 v = {1.0f, -1.0f};
    V2f32 n = {0.0f, 1.0f};
    V2f32 r = v2f32_reflect(v, n);
    test_typed_equal_float(test, f32, r.x, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 1.0f, Epsilon32);
  }

  // === v2f32_rotate ===
  {
    V2f32 v = {1.0f, 0.0f};
    V2f32 r = v2f32_rotate(v, Pi32 * 0.5f); // 90 degrees
    test_typed_equal_float(test, f32, r.x, 0.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 1.0f, Epsilon32);
  }

  // === v2f32_equals ===
  {
    V2f32 a = {1.0f, 2.0f};
    V2f32 b = {1.0f, 2.0f};
    V2f32 c = {1.0f, 2.1f};
    test_true(test, v2f32_equals(a, b));
    test_false(test, v2f32_equals(a, c));
  }

  {
    M4f32 m;
    m.m0 = 2; m.m4 = 0; m.m8  = 0; m.m12 = 1;
    m.m1 = 0; m.m5 = 3; m.m9  = 0; m.m13 = 2;
    m.m2 = 0; m.m6 = 0; m.m10 = 1; m.m14 = 0;
    m.m3 = 0; m.m7 = 0; m.m11 = 0; m.m15 = 1;  

    V2f32 v = {1, 1};
    V2f32 t = v2f32_transform(v, m);

    test_typed_equal_float(test, f32, t.x, 3.0f, Epsilon32);
    test_typed_equal_float(test, f32, t.y, 5.0f, Epsilon32);
  }

  // === v2f32_refract ===
  {
    V2f32 incident = {0.0f, -1.0f};
    V2f32 normal   = {0.0f, 1.0f};
    V2f32 r = v2f32_refract(incident, normal, 1.0f);
    test_typed_equal_float(test, f32, r.x, 0.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, -1.0f, Epsilon32);
  }

  arena_free(arena);
}

function void
v3f32_test(Test_Result* test)
{
  Arena* arena = arena_alloc();
  test_section("V3f32 Math");

  // === v3f32_add ===
  {
    V3f32 a = {1.0f, 2.0f, 3.0f};
    V3f32 b = {4.0f, 5.0f, 6.0f};
    V3f32 r = v3f32_add(a, b);
    test_typed_equal_float(test, f32, r.x, 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 7.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.z, 9.0f, Epsilon32);
  }

  // === v3f32_sub ===
  {
    V3f32 a = {5.0f, 4.0f, 3.0f};
    V3f32 b = {1.0f, 1.0f, 1.0f};
    V3f32 r = v3f32_sub(a, b);
    test_typed_equal_float(test, f32, r.x, 4.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 3.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.z, 2.0f, Epsilon32);
  }

  // === v3f32_mul / v3f32_divide ===
  {
    V3f32 a = {2.0f, 4.0f, 8.0f};
    V3f32 b = {2.0f, 2.0f, 4.0f};
    V3f32 m = v3f32_mul(a, b);
    V3f32 d = v3f32_div(a, b);
    test_typed_equal_float(test, f32, m.x, 4.0f, Epsilon32);
    test_typed_equal_float(test, f32, m.y, 8.0f, Epsilon32);
    test_typed_equal_float(test, f32, m.z, 32.0f, Epsilon32);
    test_typed_equal_float(test, f32, d.x, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, d.y, 2.0f, Epsilon32);
    test_typed_equal_float(test, f32, d.z, 2.0f, Epsilon32);
  }

  // === v3f32_scale ===
  {
    V3f32 v = {1.0f, 2.0f, 3.0f};
    V3f32 s = v3f32_scale(v, 2.0f);
    test_typed_equal_float(test, f32, s.x, 2.0f, Epsilon32);
    test_typed_equal_float(test, f32, s.y, 4.0f, Epsilon32);
    test_typed_equal_float(test, f32, s.z, 6.0f, Epsilon32);
  }

  // === v3f32_dot ===
  {
    V3f32 a = {1.0f, 2.0f, 3.0f};
    V3f32 b = {4.0f, 5.0f, 6.0f};
    f32 dot = v3f32_dot(a, b);
    test_typed_equal_float(test, f32, dot, 32.0f, Epsilon32);
  }

  // === v3f32_cross ===
  {
    V3f32 a = {1.0f, 0.0f, 0.0f};
    V3f32 b = {0.0f, 1.0f, 0.0f};
    V3f32 c = v3f32_cross(a, b);
    test_typed_equal_float(test, f32, c.x, 0.0f, Epsilon32);
    test_typed_equal_float(test, f32, c.y, 0.0f, Epsilon32);
    test_typed_equal_float(test, f32, c.z, 1.0f, Epsilon32);
  }

  // === v3f32_perpendicular ===
  {
    V3f32 v = {1.0f, 0.0f, 0.0f};
    V3f32 p = v3f32_perpendicular(v);
    f32 dot = v3f32_dot(v, p);
    test_typed_equal_float(test, f32, dot, 0.0f, Epsilon32);
  }

  // === v3f32_length / v3f32_length_sqr ===
  {
    V3f32 v = {2.0f, 3.0f, 6.0f};
    test_typed_equal_float(test, f32, v3f32_length(v), 7.0f, Epsilon32);
    test_typed_equal_float(test, f32, v3f32_length_sqr(v), 49.0f, Epsilon32);
  }

  // === v3f32_normalize ===
  {
    V3f32 v = {3.0f, 0.0f, 4.0f};
    V3f32 n = v3f32_normalize(v);
    test_typed_equal_float(test, f32, v3f32_length(n), 1.0f, Epsilon32);
  }

  // === v3f32_ortho_normalize ===
  {
    V3f32 a = {1.0f, 1.0f, 0.0f};
    V3f32 b = {0.0f, 1.0f, 1.0f};
    v3f32_ortho_normalize(&a, &b);
    f32 len_a = v3f32_length(a);
    f32 len_b = v3f32_length(b);
    f32 dot   = v3f32_dot(a, b);
    test_typed_equal_float(test, f32, len_a, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, len_b, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, dot, 0.0f, 0.0001f);
  }

  // === v3f32_transform ===
  {
    M4f32 m;
    m.m0 = 2; m.m4 = 0; m.m8  = 0; m.m12 = 1;
    m.m1 = 0; m.m5 = 3; m.m9  = 0; m.m13 = 2;
    m.m2 = 0; m.m6 = 0; m.m10 = 4; m.m14 = 3;
    m.m3 = 0; m.m7 = 0; m.m11 = 0; m.m15 = 1;
    V3f32 v = {1, 1, 1};
    V3f32 t = v3f32_transform(v, m);
    test_typed_equal_float(test, f32, t.x, 3.0f, Epsilon32);
    test_typed_equal_float(test, f32, t.y, 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, t.z, 7.0f, Epsilon32);
  }

  // === v3f32_rotate_by_quaternion ===
  {
    V3f32 v = {1.0f, 0.0f, 0.0f};
    Qf32 q;
    f32 angle = Pi32 * 0.5f; // 90 degrees around Z
    f32 s = sinf(angle * 0.5f);
    f32 c = cosf(angle * 0.5f);
    q.x = 0.0f; q.y = 0.0f; q.z = s; q.w = c;
    V3f32 r = v3f32_rotate_by_quaternion(v, q);
    test_typed_equal_float(test, f32, r.x, 0.0f, 0.001f);
    test_typed_equal_float(test, f32, r.y, 1.0f, 0.001f);
    test_typed_equal_float(test, f32, r.z, 0.0f, 0.001f);
  }

  // === v3f32_rotate_by_axis_angle ===
  {
    V3f32 v = {1.0f, 0.0f, 0.0f};
    V3f32 axis = {0.0f, 0.0f, 1.0f};
    f32 angle = Pi32 * 0.5f;
    V3f32 r = v3f32_rotate_by_axis_angle(v, axis, angle);
    test_typed_equal_float(test, f32, r.x, 0.0f, 0.001f);
    test_typed_equal_float(test, f32, r.y, 1.0f, 0.001f);
    test_typed_equal_float(test, f32, r.z, 0.0f, 0.001f);
  }

  // === v3f32_distance / v3f32_distanceSqr ===
  {
    V3f32 a = {1.0f, 2.0f, 3.0f};
    V3f32 b = {4.0f, 6.0f, 3.0f};
    test_typed_equal_float(test, f32, v3f32_distance(a, b), 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, v3f32_distanceSqr(a, b), 25.0f, Epsilon32);
  }

  // === v3f32_angle ===
  {
    V3f32 a = {1.0f, 0.0f, 0.0f};
    V3f32 b = {0.0f, 1.0f, 0.0f};
    f32 angle = v3f32_angle(a, b);
    test_typed_equal_float(test, f32, angle, Pi32 * 0.5f, 0.0001f);
  }

  // === v3f32_reflect ===
  {
    V3f32 v = {1.0f, -1.0f, 0.0f};
    V3f32 n = {0.0f, 1.0f, 0.0f};
    V3f32 r = v3f32_reflect(v, n);
    test_typed_equal_float(test, f32, r.x, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, 1.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.z, 0.0f, Epsilon32);
  }

  // === v3f32_lerp ===
  {
    V3f32 a = {0.0f, 0.0f, 0.0f};
    V3f32 b = {10.0f, 10.0f, 10.0f};
    V3f32 mid = v3f32_lerp(a, b, 0.5f);
    test_typed_equal_float(test, f32, mid.x, 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, mid.y, 5.0f, Epsilon32);
    test_typed_equal_float(test, f32, mid.z, 5.0f, Epsilon32);
  }

  // === v3f32_barycenter ===
  {
    V3f32 a = {0.0f, 0.0f, 0.0f};
    V3f32 b = {1.0f, 0.0f, 0.0f};
    V3f32 c = {0.0f, 1.0f, 0.0f};
    V3f32 p = {0.25f, 0.25f, 0.0f};
    V3f32 bary = v3f32_barycenter(p, a, b, c);
    test_typed_equal_float(test, f32, bary.x + bary.y + bary.z, 1.0f, Epsilon32);
    test_true(test, bary.x > 0.0f && bary.y > 0.0f && bary.z > 0.0f);
  }

  // === v3f32_unproject ===
  #if 0
  {
    M4f32 proj = m4f32_perspective(Pi32 / 2.0f, 1.0f, 0.1f, 100.0f);
    M4f32 view = m4f32_look_at((V3f32){0, 0, 5}, (V3f32){0, 0, 0}, (V3f32){0, 1, 0});
    V3f32 source = {0.0f, 0.0f, 0.0f};
    V3f32 world = v3f32_unproject(source, proj, view);
    test_true(test, v3f32_length(world) > 0.0f);
  }
  #endif

  // === v3f32_invert ===
  {
    V3f32 v = {1.0f, -2.0f, 3.0f};
    V3f32 i = v3f32_invert(v);
    test_typed_equal_float(test, f32, i.x, -1.0f, Epsilon32);
    test_typed_equal_float(test, f32, i.y, 2.0f, Epsilon32);
    test_typed_equal_float(test, f32, i.z, -3.0f, Epsilon32);
  }

  // === v3f32_equals ===
  {
    V3f32 a = {1.0f, 2.0f, 3.0f};
    V3f32 b = {1.0f, 2.0f, 3.0f};
    V3f32 c = {1.0f, 2.1f, 3.0f};
    test_true(test, v3f32_equals(a, b));
    test_false(test, v3f32_equals(a, c));
  }

  // === v3f32_refract ===
  {
    V3f32 v = {0.0f, -1.0f, 0.0f};
    V3f32 n = {0.0f, 1.0f, 0.0f};
    V3f32 r = v3f32_refract(v, n, 1.0f);
    test_typed_equal_float(test, f32, r.x, 0.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.y, -1.0f, Epsilon32);
    test_typed_equal_float(test, f32, r.z, 0.0f, Epsilon32);
  }

  arena_free(arena);
}

int main(void)
{
  Test_Result* test = &test_init();

  string_test(test);
  v2f32_test(test);
  v3f32_test(test);

  test_summary(test);

  return 0;
}
