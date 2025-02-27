#include "macros_iter.hpp"

// Generic tools
#define UNUSED(arg) ((void)&(arg))
# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__
# define OBSTRUCT_INDIRECT() OBSTRUCT

// On Clang/GNU compilers, ", ## __VA_ARGS__" will result in an empty
//   token if __VA_ARGS__ is empty. This is not True for MSVC.
// #define EAT_COMMA(...) , ## __VA_ARGS__
// #define APPEND_VA_ARGS(x, ...) x EAT_COMMA(__VA_ARGS__)

#define APPEND_VA_ARGS_1(x, ...) x
#define APPEND_VA_ARGS_0(x, ...) x, __VA_ARGS__
#define APPEND_VA_ARGS(x, ...)                  \
  IF_FUNC(IS_EMPTY(__VA_ARGS__), APPEND_VA_ARGS_)(x, __VA_ARGS__)


#define NARGS_ARG_N_(...) NARGS_ARG_N(__VA_ARGS__)

#define IS_LESS_THAN_NARGS_N_(N, ...)           \
  CHECK_PARENS_SAFE(IS_FLAG, 0, GET_ARG_N(N, __VA_ARGS__))
#define IS_LESS_THAN_NARGS_N(N, ...)                                    \
  IS_LESS_THAN_NARGS_N_(DEC(N), __VA_ARGS__, EXPAND(NARGS_FLAGS()))
#define IS_MORE_THAN_NARGS_N(N, ...)            \
  COMPL(IS_LESS_THAN_NARGS_N(N, __VA_ARGS__))

#define IS_LESS_THAN_NARGS_MAX(...)                     \
  IS_LESS_THAN_NARGS_N(INC(NARGS_MAX), __VA_ARGS__)
#define IS_MORE_THAN_NARGS_MAX(...)                     \
  IS_MORE_THAN_NARGS_N(INC(NARGS_MAX), __VA_ARGS__)

#define CAT(a, ...) a ## __VA_ARGS__
#define CHECK_EMPTY_N(x, n, ...) n
#define CHECK_EMPTY(...) CHECK_EMPTY_N(__VA_ARGS__, 0,)

#define CHECK_ARG_N_(check, N, ...)                     \
  check(GET_ARG_N_EXPANDED(N, __VA_ARGS__))
#define CHECK_ARG_N(check, N, ...)                      \
  CHECK_ARG_N_(check, N, __VA_ARGS__, NARGS_EMPTY())
#define CHECK_PARENS_SAFE_ARG_N_(check, res, N, ...)                    \
  CHECK_PARENS_SAFE(check, res, GET_ARG_N_EXPANDED(N, __VA_ARGS__))
#define CHECK_PARENS_SAFE_ARG_N(check, res, N, ...)                      \
  CHECK_PARENS_SAFE_ARG_N_(check, res, N, __VA_ARGS__, NARGS_EMPTY())

#define CHECK_PARENS_SAFE_1(check, res, ...) res
#define CHECK_PARENS_SAFE_0(check, res, ...) check(__VA_ARGS__)
#define CHECK_PARENS_SAFE(check, res, ...)    \
  IF_FUNC(HAS_PARENS(__VA_ARGS__), CHECK_PARENS_SAFE_)  \
  (check, res, __VA_ARGS__)

#define HAS_PARENS_EXPAND UNIQUE_FLAG, 
#define HAS_PARENS_(...) CONCATENATE(HAS_PARENS_, EXPAND __VA_ARGS__)
#define HAS_PARENS(...) COMPL(IS_FLAG_ARG_N(0, HAS_PARENS_(__VA_ARGS__)))


#define HAS_PARENS_NESTED_(...)                         \
  CONCATENATE(HAS_PARENS_, EXPAND EXPAND __VA_ARGS__)
#define CASE_PARENS_NESTED_1(...) 1
#define CASE_PARENS_NESTED_0(...) HAS_PARENS(__VA_ARGS__)
#define HAS_PARENS_NESTED(...)                                  \
  IF_FUNC(COMPL(IS_FLAG_ARG_N(0, HAS_PARENS_NESTED_(__VA_ARGS__))),     \
          CASE_PARENS_NESTED_)(__VA_ARGS__)

#define IS_SPACE_ARG_N(N, ...) CHECK_ARG_N(IS_SPACE, N, __VA_ARGS__)
#define IS_EMPTY(...)                                                   \
  AND(CHECK_PARENS_SAFE_ARG_N(IS_SPACE, 0, 0, __VA_ARGS__),             \
      IS_LESS_THAN_NARGS_N(2, __VA_ARGS__))
#define UNIQUE_FLAG 19305820ABC
#define IS_FLAG_19305820ABC ~, 1,
#define IS_FLAG(x) CHECK_EMPTY(CONCATENATE_MAX(IS_FLAG_, x))
#define IS_FLAG_ARG_N(N, ...) CHECK_ARG_N(IS_FLAG, N, __VA_ARGS__)
#define CHECK_FLAG(name, x) IS_FLAG(CONCATENATE_MAX(name, x)())
#define CHECK_FLAG_PARENS_SAFE(name, res, x)    \
  CHECK_PARENS_SAFE(IS_FLAG, res, x)

#define AND_(x, y)                                              \
  CHECK_FLAG(AND_, CONCATENATE_MAX(x, y))
#define AND(x, y) AND_(BOOL(x), BOOL(y))
#define AND_11() UNIQUE_FLAG
#define OR_(x, y)                                               \
  CHECK_FLAG(OR_, CONCATENATE_MAX(x, y))
#define OR(x, y) OR_(BOOL(x), BOOL(y))
#define OR_01() UNIQUE_FLAG
#define OR_10() UNIQUE_FLAG
#define OR_11() UNIQUE_FLAG
#define NOT(x) CHECK_FLAG(NOT_, x)
#define NOT_() UNIQUE_FLAG
#define NOT_0() UNIQUE_FLAG
#define IS_SPACE_() UNIQUE_FLAG
#define IS_SPACE(x) CHECK_FLAG(IS_SPACE_, x)
#define COMPL(b) CONCATENATE_MAX(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0
#define BOOL(x) COMPL(NOT(x))
#define IIF(c) CONCATENATE_MAX(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t
#define IF(c) IIF(BOOL(c))
#define IF_ELSE(c, body) IF(c)(EXPAND, PACK_MACRO(body) EMPTY)
#define IIF_FUNC(b, name) CONCATENATE_MAX(name, b)
#define IF_FUNC(c, name) IIF_FUNC(BOOL(c), name)
#define WHEN(c) IF(c)(EXPAND, EMPTY)
#define REPEAT(count, macro, ...)                       \
  WHEN(count)(                                          \
    OBSTRUCT(REPEAT_INDIRECT) () (                      \
      DEC(count), macro, __VA_ARGS__                    \
    )                                                   \
    OBSTRUCT(macro) (                                   \
      DEC(count), __VA_ARGS__                           \
    )                                                   \
  )
#define REPEAT_INDIRECT() REPEAT
#define NEST(count, macro, ...)                         \
  WHEN(count) (                                         \
    OBSTRUCT(NEST_INDIRECT) () (                        \
      DEC(count), macro,                                \
      OBSTRUCT(macro) (                                 \
        __VA_ARGS__                                     \
      )                                                 \
    )                                                   \
  )                                                     \
  WHEN(NOT(count)) (                                    \
    __VA_ARGS__                                         \
  )
#define NEST_INDIRECT() NEST

#define ADD_BRACKETS(...) <__VA_ARGS__>
// <JOIN_ARGS(SEP_COMMA, __VA_ARGS__)>
#define ADD_BRACKETS1(...) ADD_BRACKETS(__VA_ARGS__)
#define ADD_BRACKETS2(...) ADD_BRACKETS1(__VA_ARGS__)
#define ADD_BRACKETS3(...) ADD_BRACKETS2(__VA_ARGS__)
#define ADD_BRACKETS_DEFERED() ADD_BRACKETS
#define _OPTION1(X, ...) __VA_ARGS__
#define _OPTION2(X, ...) X
#define OPTIONAL_VA_ARGS(...) OPTIONAL_VA_ARGS1(__VA_ARGS__)
#define OPTIONAL_VA_ARGS1(...) OPTIONAL_VA_ARGS2(__VA_ARGS__)
#define OPTIONAL_VA_ARGS2(D, ...)			\
  _OPTIONAL_VA_ARGS(NARGS_REQ(__VA_ARGS__), D, __VA_ARGS__)
#define _OPTIONAL_VA_ARGS(...) _OPTIONAL_VA_ARGS1(__VA_ARGS__)
#define _OPTIONAL_VA_ARGS1(N, D, ...) _OPTION ## N(D, __VA_ARGS__)
#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) EVAL6(EVAL6(EVAL6(__VA_ARGS__)))
#define EVAL6(...) __VA_ARGS__
#define SEP_COMMA() ,
#define SEP_SEMICOLON() ;
#define SEP_EMPTY()
#define SEP_OR() ||
#define SEP_AND() &&
#define SEP_BITWISE_OR() |
#define SEP_BITWISE_AND() &
#define SEP_ADD() +
#define SEP_MULTIPLY() *
#define MAP_TYPE_MACRO(...)                                     \
  std::map OBSTRUCT(ADD_BRACKETS)( __VA_ARGS__ )
#define VECTOR_TYPE_MACRO(...)                                  \
  std::vector OBSTRUCT(ADD_BRACKETS_DEFERED)()( __VA_ARGS__ )
#define MAP_INIT_MACRO(...)                     \
  MAP_TYPE_MACRO(__VA_ARGS__)()
#define VECTOR_INIT_MACRO(...)                  \
  VECTOR_TYPE_MACRO(__VA_ARGS__)()
#define STRING_INIT_MACRO(...)                  \
  std::string(__VA_ARGS__)

#define _Args(...) __VA_ARGS__
#define PASS_THROUGH(X) X
#define STRIP_PARENS(X) X
#define PACK_MACRO_(X) STRIP_PARENS( _Args X )
#define PACK_MACRO(...) PACK_MACRO_((__VA_ARGS__))
#define PACK_MACRO_DEFERED() PACK_MACRO
#define UNPACK_MACRO(...) __VA_ARGS__
#define UNPACK_PARENS(X) STRIP_PARENS( _Args X )
#define UNPACK_PARENS_COND_0(X) X
#define UNPACK_PARENS_COND_1(X) UNPACK_PARENS(X)
#define UNPACK_PARENS_COND(X)                           \
  IF_FUNC(HAS_PARENS(X), UNPACK_PARENS_COND_)(X)
#define EMPTY_BRACES() {}
#define EMPTY_BRACES_DEFERED() EMPTY_BRACES
#define ADD_BRACES(...) {JOIN_ARGS(SEP_COMMA, __VA_ARGS__)}
#define ADD_BRACES_DEFERED() ADD_BRACES
#define ADD_PARENS(...) (__VA_ARGS__)
#define ADD_PARENS_DEFERED() ADD_PARENS
#define STR_MACRO(X) #X
#define CALL_MACRO(what, ...)			\
  what(__VA_ARGS__)

#define CALL_MACRO_PACKED(what, args)		\
  what args
#define CALL_WITH_EMPTY_ARGS(what, args, x)	\
  what(x)
#define CALL_WITH_EMPTY_ARGS_PACKED(what, args, x)      \
  what x
#define CALL_WITH_PREFIX_ARGS(what, args, x)	\
  what(UNPACK_PARENS(args), x)
#define CALL_WITH_SUFFIX_ARGS(what, args, x)	\
  what(x, UNPACK_PARENS(args))
#define CALL_WITH_PREFIX_ARGS_PACKED(what, args, x)	\
  what(UNPACK_PARENS args, x)
#define CALL_WITH_SUFFIX_ARGS_PACKED(what, args, x)	\
  what(x, UNPACK_PARENS args)
#define CALL_WITH_PREFIX_ARGS_PACKED_ITER(what, args, x)	\
  what(UNPACK_PARENS args, EXPAND x)
#define CALL_WITH_PREFIX_ARGS_PACKED_ITER2(what, args, x)	\
  what(UNPACK_PARENS args, UNPACK_PARENS x)
#define CALL_WITH_SUFFIX_ARGS_PACKED_ITER(what, args, x)	\
  what(EXPAND x, UNPACK_PARENS args)

#define CALL_NESTED_1(what, caller, args, var1) \
  caller(what, args, (PACK_MACRO var1))
#define CALL_NESTED_2(what, caller, args, var1, var2)           \
  caller(what, args, (PACK_MACRO var1,                          \
                      PACK_MACRO var2))
#define CALL_NESTED_3(what, caller, args, var1, var2, var3)          \
  caller(what, args, (PACK_MACRO var1,                               \
                      PACK_MACRO var2,                               \
                      PACK_MACRO var3))
#define CALL_NESTED_(N, ...)                    \
  CONCATENATE(CALL_NESTED_, N)(__VA_ARGS__)
#define CALL_NESTED_N_(what, caller, args, ...)                         \
  caller(what, args, (__VA_ARGS__))
#define CALL_NESTED_N(...)                      \
  CALL_NESTED_N_(__VA_ARGS__)
#define CALL_NESTED(what, args, x)              \
  CALL_NESTED_N(what, EXPAND args, x)

#define CALL_NESTED_PACKED_NO_ARGS_1(what, var1)        \
  what(PACK_MACRO(PACK_MACRO var1))
#define CALL_NESTED_PACKED_NO_ARGS_2(what, var1, var2)                  \
  what(PACK_MACRO(PACK_MACRO var1), PACK_MACRO(PACK_MACRO var2))
#define CALL_NESTED_PACKED_NO_ARGS_3(what, var1, var2, var3)            \
  what(PACK_MACRO(PACK_MACRO var1), PACK_MACRO(PACK_MACRO var2),        \
       PACK_MACRO(PACK_MACRO var3))
#define CALL_NESTED_PACKED_1(what, args, var1)   \
  what(EXPAND args,                              \
       PACK_MACRO(PACK_MACRO var1))
#define CALL_NESTED_PACKED_2(what, args, var1, var2)                    \
  what(EXPAND args,                                                     \
       PACK_MACRO(PACK_MACRO var1),                                     \
       PACK_MACRO(PACK_MACRO var2))
#define CALL_NESTED_PACKED_3(what, args, var1, var2, var3)              \
  what(EXPAND args,                                                     \
       PACK_MACRO(PACK_MACRO var1),                                     \
       PACK_MACRO(PACK_MACRO var2),                                     \
       PACK_MACRO(PACK_MACRO var3))

#define CALL_NESTED_PACKED_(N, what, args, ...)                 \
  CONCATENATE(CALL_NESTED_PACKED_, N)(what, args, __VA_ARGS__)
#define CALL_NESTED_PACKED(what, args, x)                               \
  CALL_NESTED_PACKED_(NARGS x, what, args, EXPAND x)
#define CALL_NESTED_PACKED_NO_ARGS_(N, what, ...)                       \
  CONCATENATE(CALL_NESTED_PACKED_NO_ARGS_, N)(what, __VA_ARGS__)
#define CALL_NESTED_PACKED_NO_ARGS(what, args, x)       \
  CALL_NESTED_PACKED_NO_ARGS_(NARGS x, what, EXPAND x)

#define CALL_DO_NOTHING(what, args, x)
#define CALL_DO_NOTHING_DEFER(what, args, x)
#define CALL_MACRO_PACKED_DEFER(what, args)		\
  DEFER(what)() args
#define CALL_WITH_EMPTY_ARGS_DEFER(what, args, x)	\
  DEFER(what)()(x)
#define CALL_WITH_PREFIX_ARGS_DEFER(what, args, x)	\
  DEFER(what)()(UNPACK_PARENS(args), x)
#define CALL_WITH_SUFFIX_ARGS_DEFER(what, args, x)	\
  DEFER(what)()(x, UNPACK_PARENS(args))
#define CALL_WITH_PREFIX_ARGS_PACKED_DEFER(what, args, x)	\
  DEFER(what)()(UNPACK_PARENS args, x)
#define CALL_WITH_SUFFIX_ARGS_PACKED_DEFER(what, args, x)	\
  DEFER(what)()(x, UNPACK_PARENS args)

#define CALL_WITH_APPEND_ARGS_(what, append, args, x)   \
  CALL_MACRO_PACKED(what, append(args, x))
#define CALL_WITH_APPEND_ARGS(what, args, x)            \
  CALL_WITH_APPEND_ARGS_(what, EXPAND args, x)
  
#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define JOIN(arg1, arg2)  JOIN1(arg1, arg2)
#define JOIN1(arg1, arg2) JOIN2(arg1, arg2)
#define JOIN2(arg1, arg2) arg1 arg2

#define CONCATENATE(arg1, ...)   CONCATENATE1(arg1, __VA_ARGS__)
#define CONCATENATE1(arg1, ...)  CONCATENATE2(arg1, __VA_ARGS__)
#define CONCATENATE2(arg1, ...)  CAT(arg1, __VA_ARGS__)
#define CONCATENATE_MAX(...) CONCATENATE(__VA_ARGS__)

// Control parameters
// #define VERBOSE_VALUE_SET_DEBUG 1
// #define VALUE_SET_VAL_MEMBERS 1
// #define CHECK_NEW_MATCHES_ORIG 1
// #define MAKE_EQUIVALENT_TO_MATLAB 1
#define COMPARE_PRECISION 15
#define COMPARE_RELATIVE_EPSILON			\
  std::numeric_limits<double>::epsilon()
#define COMPARE_ABSOLUTE_EPSILON 1e-9

// Inheritance for ValueSet
#define INHERIT_METHOD_ENUM(...)					\
  using __VA_ARGS__::module;						\
  using __VA_ARGS__::param_type;					\
  using __VA_ARGS__::error_prefix

#define INHERIT_METHODS_VALUE_SET_TYPES(...)	\
  typedef __VA_ARGS__ ParentClass;		\
  friend __VA_ARGS__;				\
  using typename __VA_ARGS__::BaseClass;	\
  using typename __VA_ARGS__::EnumClass;	\
  using typename __VA_ARGS__::EnumBaseClass;	\
  using typename __VA_ARGS__::EnumType;		\
  using typename __VA_ARGS__::ValueType;	\
  using typename __VA_ARGS__::iterator;		\
  using typename __VA_ARGS__::const_iterator;	\
  using typename __VA_ARGS__::VS_FLAGS;		\
  INHERIT_METHOD_ENUM(__VA_ARGS__);

#define INHERIT_METHODS_VALUE_SET_BASE(...)     \
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__) 
#define INHERIT_METHOD_VALUE_SET(name, ...) using ParentClass::name;
#define INHERIT_METHODS_VALUE_SET(...)                                  \
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__)                          \
  using typename __VA_ARGS__::ValueSetClass;                            \
  ADD_METHODS_VALUE_SET_T_BOTH_NODUP(INHERIT_METHOD_VALUE_SET);         \
  ADD_METHODS_VALUE_SET_T_STATIC_NODUP(INHERIT_METHOD_VALUE_SET);
#define DECLARE_VALUE_SET_BASE(cls, ...)		\
  typedef cls ThisClass;				\
  friend class ValueSetClassType<cls>;			\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)		\
  friend BaseClass;                                     \
  friend ValueSetClass;
#define DECLARE_VALUE_SET_STATIC_BASE(cls, ...)		\
  typedef cls ThisClass;				\
  friend class ValueSetClassType<cls>;			\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)

#define DEFAULT_VALUE(cls, name)		\
  cls EnumBaseClass::getDefault(cls EnumClass::name, 0.0)
#ifdef VALUE_SET_VAL_MEMBERS
#define DECLARE_VALUE_SET_STATIC_MEMBER(name)	\
  static Value name
#define DECLARE_VALUE_SET_STATIC_MEMBER_CONST(name)	\
  static const Value name
#define DECLARE_VALUE_SET_MEMBER(name)		\
  Value name = DEFAULT_VALUE(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER(cls, name)			\
  Value cls::name = DEFAULT_VALUE(cls::, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER_CONST(cls, name)			\
  const Value cls::name = DEFAULT_VALUE(cls::, name)
#else // VALUE_SET_VAL_MEMBERS
#define DECLARE_VALUE_SET_STATIC_MEMBER(name)	\
  static double name
#define DECLARE_VALUE_SET_STATIC_MEMBER_CONST(name)	\
  static const double name
#define DECLARE_VALUE_SET_MEMBER(name)		\
  double name = DEFAULT_VALUE(, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER(cls, name)			\
  double cls::name = DEFAULT_VALUE(cls::, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER_CONST(cls, name)			\
  const double cls::name = DEFAULT_VALUE(cls::, name)
#endif // VALUE_SET_VAL_MEMBERS


#define ADD(N, M) EVAL(ADD_(N, M))
#define ADD_(N, M) NEST(M, INC, N)
#define SUBTRACT(N, M) EVAL(SUBTRACT_(N, M))
#define SUBTRACT_(N, M) NEST(M, DEC, N)
#define ADD_DEFERED() ADD
#define SUBTRACT_DEFERED() SUBTRACT

#define GET_ARG_N_BEFORE_END(N, ...)                                    \
  GET_ARG_N(SUBTRACT(NARGS(__VA_ARGS__), INC(N)), __VA_ARGS__)
#define GET_ARG_N_COMMA(N, ...) , GET_ARG_N(N, __VA_ARGS__)
#define FIRST_N_ARGS(N, _0, ...)                    \
  GET_ARGS_BEFORE_N(N, _0, __VA_ARGS__)
/*  
  WHEN(N) (                                         \
    _0 REPEAT(DEC(N), GET_ARG_N_COMMA, __VA_ARGS__) \
  )
*/
#define DROP_N_ARGS(N, ...)                     \
  OBSTRUCT(FIRST_N_ARGS)(SUBTRACT(NARGS(__VA_ARGS__), N), __VA_ARGS__)


// This version alone does not work on windows where macros are limited
//   to 127 arguments or less and there are more parameters in some
//   modules (e.g. PS)
#define NARGS_(...) NARGS(__VA_ARGS__)
#define NARGS_DIRECT_(...) NARGS_DIRECT__(__VA_ARGS__)
#define NARGS_DIRECT__(dummy, ...) NARGS_ARG_N(__VA_ARGS__)
#define NARGS_DIRECT(...)                                 \
  NARGS_DIRECT_(APPEND_VA_ARGS(dummy, __VA_ARGS__),       \
                NARGS_REVERSE_SEQ_N(), )
#define NARGS_UNSAFE_(...) NARGS_ARG_N(__VA_ARGS__)
#define NARGS_UNSAFE(...)                               \
  NARGS_UNSAFE_(__VA_ARGS__, NARGS_REVERSE_SEQ_N())

#define NARGS_ADD(...) ADD(__VA_ARGS__)
#define NARGS_ALT_1(...)                        \
  NARGS_DIRECT(__VA_ARGS__)
#define NARGS_ALT_0(...)                                                \
  ADD(NARGS_MAX,                                                        \
      NARGS_DIRECT(NARGS_REMAINDER(__VA_ARGS__)))
#define NARGS_ALT_(...)                                                 \
  CONCATENATE_MAX(NARGS_ALT_, IS_LESS_THAN_NARGS_MAX(__VA_ARGS__))      \
  (__VA_ARGS__)
#define NARGS_ALT_INDIRECT_() NARGS_ALT_
#define NARGS_ALT(...) NARGS_ALT_(__VA_ARGS__)
#define NARGS(...) NARGS_ALT(__VA_ARGS__)

#define FIRST_ARG(a, ...) a
#define FIRST_ARG_DEFERED() FIRST_ARG
#define FIRST_ARG_PACKED(args) FIRST_ARG args
#define LAST_ARG(...)							\
  FOR_EACH_DIFF_CALLERS(PASS_THROUGH, CALL_DO_NOTHING,                  \
                        CALL_WITH_EMPTY_ARGS, SEP_EMPTY, (),            \
                        __VA_ARGS__)
#define LAST_ARG_PACKED(args) LAST_ARG args
  
#define FOR_EACH(what, ...)			\
  FOR_EACH_GENERIC(what, CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_DEFERED(what, ...)			\
  FOR_EACH_GENERIC(what, CALL_WITH_EMPTY_ARGS_DEFER, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_COMMA(what, ...)			\
  FOR_EACH_GENERIC(what, CALL_WITH_EMPTY_ARGS, SEP_COMMA, (), __VA_ARGS__)
#define FOR_EACH_JOIN(what, sep, ...)					\
  FOR_EACH_GENERIC(what, CALL_WITH_EMPTY_ARGS, sep, (), __VA_ARGS__)
#define JOIN_ARGS(sep, ...)						\
  FOR_EACH_JOIN(PASS_THROUGH, sep, __VA_ARGS__)


#define FOR_EACH_UNSAFE(what, ...)                                      \
  FOR_EACH_GENERIC_UNSAFE(what, CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_COMMA_UNSAFE(what, ...)			\
  FOR_EACH_GENERIC_UNSAFE(what, CALL_WITH_EMPTY_ARGS, SEP_COMMA, (), __VA_ARGS__)
#define FOR_EACH_JOIN_UNSAFE(what, sep, ...)                            \
  FOR_EACH_GENERIC_UNSAFE(what, CALL_WITH_EMPTY_ARGS, sep, (), __VA_ARGS__)
#define JOIN_ARGS_UNSAFE(sep, ...)                      \
  FOR_EACH_JOIN_UNSAFE(PASS_THROUGH, sep, __VA_ARGS__)


#define APPEND_ARG(args, a)                                             \
  (EXPAND args, a)
#define PREPEND_ARG(args, a)                                            \
  (a, EXPAND args)
#define FOR_EACH_PREFIX_ARGS(what, sep, args, ...)			\
  FOR_EACH_GENERIC(what, CALL_WITH_PREFIX_ARGS, sep, args, __VA_ARGS__)
#define FOR_EACH_SUFFIX_ARGS(what, sep, args, ...)			\
  FOR_EACH_GENERIC(what, CALL_WITH_SUFFIX_ARGS, sep, args, __VA_ARGS__)
#define FOR_EACH_PREFIX_ARGS_PACKED(what, sep, args, ...)		\
  FOR_EACH_GENERIC(what,                                                \
                   CALL_WITH_PREFIX_ARGS_PACKED, sep, args, __VA_ARGS__)
#define FOR_EACH_SUFFIX_ARGS_PACKED(what, sep, args, ...)		\
  FOR_EACH_GENERIC(what,                                                \
                   CALL_WITH_SUFFIX_ARGS_PACKED, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_PREFIX_ARGS_COMMA(what, args, ...)	\
  FOR_EACH_PREFIX_ARGS(what, SEP_COMMA, args, __VA_ARGS__)
#define FOR_EACH_WITH_SUFFIX_ARGS_COMMA(what, args, ...)	\
  FOR_EACH_SUFFIX_ARGS(what, SEP_COMMA, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_COMMA(what, args, ...)		\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_(...)	\
  FOR_EACH(STRIP_PARENS, __VA_ARGS__)
#define FOR_EACH_WITH_PREFIX_ARGS_PACKED(what, args, ...)		\
  FOR_EACH_GENERIC(what,                                                \
                   CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (),             \
                   FOR_EACH_WITH_PREFIX_ARGS_COMMA(ADD_PARENS, args,    \
                                                   __VA_ARGS__))
#define FOR_EACH_WITH_SUFFIX_ARGS_PACKED(what, args, ...)               \
  FOR_EACH_GENERIC(what,                                                \
                   CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (),             \
                   FOR_EACH_WITH_SUFFIX_ARGS_COMMA(ADD_PARENS, args,    \
                                                   __VA_ARGS__))
#define FOR_EACH_WITH_ARGS_PACKED(what, args, ...)              \
  FOR_EACH_WITH_PREFIX_ARGS_PACKED(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_PREFIX_ARGS(what, args, ...)                      \
  FOR_EACH_WITH_ARGS_(FOR_EACH_WITH_PREFIX_ARGS_COMMA(what, args, __VA_ARGS__))
#define FOR_EACH_WITH_SUFFIX_ARGS(what, args, ...)                      \
  FOR_EACH_WITH_ARGS_(FOR_EACH_WITH_SUFFIX_ARGS_COMMA(what, args, __VA_ARGS__))
#define FOR_EACH_WITH_ARGS(what, args, ...)             \
  FOR_EACH_WITH_PREFIX_ARGS(what, args, __VA_ARGS__)
#define PREFIX_EACH(prefix, ...)					\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(CONCATENATE, (prefix), __VA_ARGS__)
#define SUFFIX_EACH(suffix, ...)					\
  FOR_EACH_WITH_SUFFIX_ARGS_COMMA(CONCATENATE, (suffix), __VA_ARGS__)

#define EQUALS(a, b) (a == b)
#define VAR_IN_LIST(var, ...)                           \
  (FOR_EACH_GENERIC(EQUALS, CALL_WITH_PREFIX_ARGS,	\
                    SEP_OR, (var), __VA_ARGS__))
#define VAR_IN_PREFIXED_LIST(var, prefix, ...)			\
  VAR_IN_LIST(var, PREFIX_EACH(prefix, __VA_ARGS__))
#define IF_NOT_EMPTY_1(...)
#define IF_NOT_EMPTY_0(...) __VA_ARGS__
#define IF_NOT_EMPTY(cond, ...)                        \
  CONCATENATE_MAX(IF_NOT_EMPTY_, IS_EMPTY(cond))(__VA_ARGS__)
// #define IF_NOT_EMPTY(cond, body)				\
//   FOR_EACH_WITH_PREFIX_ARGS_COMMA(FIRST_ARG, (body), cond)
#define PASS_IF_NOT_EMPTY(x) IF_NOT_EMPTY(x, x)
#define PREFIX_IF_NOT_EMPTY(prefix, x)		\
  IF_NOT_EMPTY(x, PACK_MACRO(prefix()) x)
#define SUFFIX_IF_NOT_EMPTY(x, suffix)	\
  IF_NOT_EMPTY(x, x PACK_MACRO(suffix()))
#define JOIN_MACROS(...)				\
  FOR_EACH_SUFFIX_ARGS(SUFFIX_IF_NOT_EMPTY, SEP_EMPTY,	\
		       (SEP_COMMA), __VA_ARGS__)

#define FOR_EACH_GENERIC_NESTED_(N, var1, sep, args, ...)               \
  CONCATENATE(FOR_EACH_GENERIC_NESTED_, N)(var1, sep, args, __VA_ARGS__)
#define FOR_EACH_GENERIC_NESTED_1(var1, sep, args, vars2, ...)          \
  FOR_EACH_GENERIC(FOR_EACH_GENERIC_NESTED_N_DEFERED,                   \
                   CALL_WITH_SUFFIX_ARGS_DEFER, sep,                    \
                   (sep, APPEND_ARG(args, var1), __VA_ARGS__),          \
                   EXPAND vars2)
#define FOR_EACH_GENERIC_NESTED_1_DEFERED() FOR_EACH_GENERIC_NESTED_1
#define FOR_EACH_GENERIC_NESTED_0_EXPAND_ARGS_(vars2, sep, what, caller, args, ...) \
  FOR_EACH_GENERIC(what, CALL_NESTED, sep,                              \
                   (caller, APPEND_VA_ARGS(args, __VA_ARGS__)),         \
                   EXPAND vars2)
#define FOR_EACH_GENERIC_NESTED_0_EXPAND_ARGS(...)                      \
  FOR_EACH_GENERIC_NESTED_0_EXPAND_ARGS_(__VA_ARGS__)
#define FOR_EACH_GENERIC_NESTED_0(var1, sep, args, vars2)               \
  FOR_EACH_GENERIC_NESTED_0_EXPAND_ARGS(vars2, sep, EXPAND args, var1)
#define FOR_EACH_GENERIC_NESTED_0_DEFERED() FOR_EACH_GENERIC_NESTED_0
#define FOR_EACH_GENERIC_NESTED_N(var1, sep, args, ...)          \
  FOR_EACH_GENERIC_NESTED_(BOOL(DEC(NARGS(__VA_ARGS__))),       \
                           var1, sep, args, __VA_ARGS__)
#define FOR_EACH_GENERIC_NESTED_N_DEFERED() FOR_EACH_GENERIC_NESTED_N
#define FOR_EACH_GENERIC_NESTED(what, caller, sep, args, ...)           \
  EVAL(FOR_EACH_GENERIC_NESTED_N(args, sep, (what, caller), __VA_ARGS__))

#define FOR_EACH_GENERIC_NESTED_PACKED(what, sep, args, ...)    \
  FOR_EACH_GENERIC_NESTED(what, CALL_NESTED_PACKED, sep,        \
                          args, __VA_ARGS__)
#define FOR_EACH_GENERIC_NESTED_PACKED_NO_ARGS(what, sep, ...)          \
  FOR_EACH_GENERIC_NESTED(what, CALL_NESTED_PACKED_NO_ARGS, sep,        \
                          (), __VA_ARGS__)


#define COMBO_ARGS_BASE(METHOD, ...)                           \
  FOR_EACH_GENERIC_NESTED(METHOD, CALL_WITH_EMPTY_ARGS_PACKED, \
                          SEP_COMMA, (), __VA_ARGS__)
#define COMBO_ARGS(...)                         \
  COMBO_ARGS_BASE(ADD_PARENS, __VA_ARGS__)

#define IS_PACKED_TYPE_EXPAND
#define IS_PACKED_TYPE_(...)                            \
  CONCATENATE(IS_PACKED_TYPE_, EXPAND __VA_ARGS__)

#define PACK_TYPE(...) ADD_PARENS(__VA_ARGS__)
#define PACK_RETURN_TYPE(...) EVAL(__VA_ARGS__)
#define UNPACK_TYPE(X) EVAL(UNPACK_PARENS_COND(X))

#define _GET_ARG_TYPE(arg) UNPACK_TYPE(GET_ARG_N(0, EXPAND arg))
#define _GET_ARG_NAME(arg) GET_ARG_N(1, EXPAND arg)
#define _GET_ARG_DEFV(arg)                              \
  __GET_ARG_DEFV(GET_ARG_N(2, EXPAND arg))
#define __GET_ARG_DEFV(defV)                            \
  OBSTRUCT(IF_NOT_EMPTY)(defV, OBSTRUCT(PACK_MACRO)(= UNPACK_TYPE(defV)))
#define MAKE_ARG(arg)                           \
  IF(HAS_PARENS_NESTED(arg))(_GET_ARG_NAME(arg))
#define MAKE_ARG_T(arg)                                         \
  IF(HAS_PARENS_NESTED(arg))                                    \
  (_GET_ARG_TYPE(arg) _GET_ARG_NAME(arg) _GET_ARG_DEFV(arg))
#define MAKE_ARG_T_NODEF(arg)                                   \
  IF(HAS_PARENS_NESTED(arg))                                    \
  (_GET_ARG_TYPE(arg) _GET_ARG_NAME(arg))

#define VS_ARGS(args) VS_ARGS_ args
#define VS_ARGS_(...)                                   \
  EVAL(ADD_PARENS(FOR_EACH_COMMA_UNSAFE(MAKE_ARG, __VA_ARGS__)))
#define VS_ARGS_T(args) VS_ARGS_T_ args
#define VS_ARGS_T_(...)                                                 \
  EVAL(ADD_PARENS(FOR_EACH_COMMA_UNSAFE(MAKE_ARG_T, __VA_ARGS__)))
#define VS_ARGS_T_NODEF(args) VS_ARGS_T_NODEF_ args
#define VS_ARGS_T_NODEF_(...)                                           \
  EVAL(ADD_PARENS(FOR_EACH_COMMA_UNSAFE(MAKE_ARG_T_NODEF, __VA_ARGS__)))

#ifdef DO_TEST_MACROS_PREPROCESS

MACRO_TEST(BOOL, 1, BOOL(1));
MACRO_TEST(BOOL, 0, BOOL(0));
MACRO_TEST(BOOL, 0, BOOL());
// MACRO_TEST(BOOL, 1, BOOL((0)));

MACRO_TEST(AND, 1, AND(1, 1));
MACRO_TEST(AND, 0, AND(1, 0));
MACRO_TEST(AND, 0, AND(1, ));
MACRO_TEST(AND, 0, AND(, 0));

MACRO_TEST(OR, 1, OR(1, 1));
MACRO_TEST(OR, 0, OR(0, 0));
MACRO_TEST(OR, 1, OR(1, ));
MACRO_TEST(OR, 1, OR(, 1));
MACRO_TEST(OR, 0, OR(, ));

MACRO_TEST(HAS_PARENS, 0, HAS_PARENS(A, B, C));
MACRO_TEST(HAS_PARENS, 1, HAS_PARENS((A)));
MACRO_TEST(HAS_PARENS, 1, HAS_PARENS((A, B, C)));
MACRO_TEST(HAS_PARENS, 0, HAS_PARENS());
MACRO_TEST(HAS_PARENS, 0, HAS_PARENS( ));
MACRO_TEST(HAS_PARENS, 0, HAS_PARENS(EMPTY()));
MACRO_TEST(HAS_PARENS, 1, HAS_PARENS(()));

MACRO_TEST(HAS_PARENS_NESTED, 0, HAS_PARENS_NESTED(A, B, C));
MACRO_TEST(HAS_PARENS_NESTED, 1, HAS_PARENS_NESTED((A)));
MACRO_TEST(HAS_PARENS_NESTED, 1, HAS_PARENS_NESTED((A, B, C)));
MACRO_TEST(HAS_PARENS_NESTED, 0, HAS_PARENS_NESTED());
MACRO_TEST(HAS_PARENS_NESTED, 0, HAS_PARENS_NESTED( ));
MACRO_TEST(HAS_PARENS_NESTED, 0, HAS_PARENS_NESTED(EMPTY()));
MACRO_TEST(HAS_PARENS_NESTED, 1, HAS_PARENS_NESTED(()));
MACRO_TEST(HAS_PARENS_NESTED, 1, HAS_PARENS_NESTED(((A))));
MACRO_TEST(HAS_PARENS_NESTED, 1, HAS_PARENS_NESTED((())));

MACRO_TEST(IS_EMPTY, 1, IS_EMPTY());
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(0));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(1));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(0, 0));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(1, 1));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(A, B, C));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(TEST_SEQ_MAX_ARGS));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(TEST_SEQ_MAX_ARGS_PLUS1));
MACRO_TEST(IS_EMPTY, 0, IS_EMPTY(TEST_SEQ_MAX_ITER));

MACRO_TEST(IS_SPACE, 0, IS_SPACE(A));
MACRO_TEST(IS_SPACE, 0, IS_SPACE(1));
MACRO_TEST(IS_SPACE, 0, IS_SPACE(0));
MACRO_TEST(IS_SPACE, 1, IS_SPACE());

MACRO_TEST(NARGS, 0, NARGS());
MACRO_TEST(NARGS, 1, NARGS(A));
MACRO_TEST(NARGS, 3, NARGS(A, B, C));
MACRO_TEST(NARGS, NARGS_MAX, NARGS(TEST_SEQ_MAX_ARGS));
MACRO_TEST(NARGS, INC(NARGS_MAX), NARGS(TEST_SEQ_MAX_ARGS_PLUS1));
MACRO_TEST(NARGS, INC_MAX, NARGS(TEST_SEQ_MAX_ITER));
MACRO_TEST(NARGS, 3, NARGS(A, B, ));

MACRO_TEST(IS_LESS_THAN_NARGS_MAX, 1, IS_LESS_THAN_NARGS_MAX());
MACRO_TEST(IS_LESS_THAN_NARGS_MAX, 1, IS_LESS_THAN_NARGS_MAX(A));
MACRO_TEST(IS_LESS_THAN_NARGS_MAX, 1, IS_LESS_THAN_NARGS_MAX(A, B, C));
MACRO_TEST(IS_LESS_THAN_NARGS_MAX, 1, IS_LESS_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS));
MACRO_TEST(IS_LESS_THAN_NARGS_MAX, 0, IS_LESS_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS_PLUS1));

MACRO_TEST(IS_MORE_THAN_NARGS_MAX, 0, IS_MORE_THAN_NARGS_MAX());
MACRO_TEST(IS_MORE_THAN_NARGS_MAX, 0, IS_MORE_THAN_NARGS_MAX(A));
MACRO_TEST(IS_MORE_THAN_NARGS_MAX, 0, IS_MORE_THAN_NARGS_MAX(A, B, C));
MACRO_TEST(IS_MORE_THAN_NARGS_MAX, 0, IS_MORE_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS));
MACRO_TEST(IS_MORE_THAN_NARGS_MAX, 1, IS_MORE_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS_PLUS1));

MACRO_TEST(IS_LESS_THAN_NARGS_N, 0, IS_LESS_THAN_NARGS_N(1, A, B, C));
MACRO_TEST(IS_LESS_THAN_NARGS_N, 1, IS_LESS_THAN_NARGS_N(4, A, B, C));
MACRO_TEST(IS_LESS_THAN_NARGS_N, 0, IS_LESS_THAN_NARGS_N(3, A, B, C));
MACRO_TEST(IS_LESS_THAN_NARGS_N, 1, IS_LESS_THAN_NARGS_N(2, ));

MACRO_TEST(IS_MORE_THAN_NARGS_N, 1, IS_MORE_THAN_NARGS_N(1, A, B, C));
MACRO_TEST(IS_MORE_THAN_NARGS_N, 0, IS_MORE_THAN_NARGS_N(4, A, B, C));

MACRO_TEST(FOR_EACH, A, FOR_EACH(PASS_THROUGH, A));
MACRO_TEST(FOR_EACH, PACK_MACRO(A; B; C), FOR_EACH(PASS_THROUGH, A, B, C));
MACRO_TEST(FOR_EACH_COMMA, (A), ADD_PARENS(FOR_EACH_COMMA(PASS_THROUGH, A)));
MACRO_TEST(FOR_EACH_COMMA, (A, B, C), ADD_PARENS(FOR_EACH_COMMA(PASS_THROUGH, A, B, C)));

MACRO_TEST(GET_ARG_N_BEFORE_END, A, GET_ARG_N_BEFORE_END(2, A, B, C));
MACRO_TEST(GET_ARG_N_BEFORE_END, B, GET_ARG_N_BEFORE_END(1, A, B, C));
MACRO_TEST(GET_ARG_N_BEFORE_END, C, GET_ARG_N_BEFORE_END(0, A, B, C));
MACRO_TEST(GET_ARG_N_BEFORE_END,  , GET_ARG_N_BEFORE_END(0, A, B, ));

MACRO_TEST(LAST_ARG, C, LAST_ARG(A, B, C));
MACRO_TEST(LAST_ARG,  , LAST_ARG(A, B, ));

MACRO_TEST(IS_SPACE_ARG_N, 0, IS_SPACE_ARG_N(0, A));
MACRO_TEST(IS_SPACE_ARG_N, 1, IS_SPACE_ARG_N(0, ));
MACRO_TEST(IS_SPACE_ARG_N, 1, IS_SPACE_ARG_N(124, B));

MACRO_TEST(GET_ARGS_AFTER, PACK_MACRO(B, C),
           EVAL(GET_ARGS_AFTER_N(1, A, B, C)));
MACRO_TEST(GET_ARGS_AFTER,
           PACK_MACRO(_11, _12, _13),
           EVAL(GET_ARGS_AFTER_N(10, _1, _2, _3, _4, _5, _6, _7, _8, _9,
                                 _10, _11, _12, _13)));
MACRO_TEST(GET_ARGS_AFTER,
           PACK_MACRO(_16, _17, _18, _19, _20),
           EVAL(GET_ARGS_AFTER_N(15, _1, _2, _3, _4, _5, _6, _7, _8, _9,
                                 _10, _11, _12, _13, _14, _15, _16, _17,
                                 _18, _19, _20)));

#define TEST_ARG_NODEF (const double&, val, )
#define TEST_ARG_DEF (bool, includePrefixes, false)
#define TEST_ARG_BRACES                                                 \
  (const std::vector<std::string>, skip_keys,                           \
   std::vector<std::string>())
#define TEST_ARG_COMPLEX                                                \
  (PACK_TYPE(const MAP_TYPE_MACRO(std::string, std::string)&),          \
   key_aliases,                                                         \
   PACK_TYPE(MAP_INIT_MACRO(std::string, std::string)))
#define TEST_ARG_STRING                                                 \
  (std::string, filename, STRING_INIT_MACRO(""))
#define TEST_ARGS                                                       \
  (TEST_ARG_NODEF, TEST_ARG_DEF, TEST_ARG_BRACES, TEST_ARG_COMPLEX,     \
   TEST_ARG_STRING)

MACRO_TEST_EVAL(ARG_TYPE, const double&, _GET_ARG_TYPE(TEST_ARG_NODEF));
MACRO_TEST_EVAL(ARG_TYPE, bool, _GET_ARG_TYPE(TEST_ARG_DEF));
MACRO_TEST_EVAL(ARG_TYPE, const std::vector<std::string>,
                _GET_ARG_TYPE(TEST_ARG_BRACES));
MACRO_TEST_EVAL(ARG_TYPE,
                PACK_MACRO(const std::map<std::string, std::string>&),
                _GET_ARG_TYPE(TEST_ARG_COMPLEX));
MACRO_TEST_EVAL(ARG_TYPE, std::string,
                _GET_ARG_TYPE(TEST_ARG_STRING));

MACRO_TEST_EVAL(ARG_NAME, val, _GET_ARG_NAME(TEST_ARG_NODEF));
MACRO_TEST_EVAL(ARG_NAME, includePrefixes, _GET_ARG_NAME(TEST_ARG_DEF));
MACRO_TEST_EVAL(ARG_NAME, skip_keys, _GET_ARG_NAME(TEST_ARG_BRACES));
MACRO_TEST_EVAL(ARG_NAME, key_aliases, _GET_ARG_NAME(TEST_ARG_COMPLEX));
MACRO_TEST_EVAL(ARG_NAME, filename, _GET_ARG_NAME(TEST_ARG_STRING));

MACRO_TEST_EVAL(ARG_DEFV, , _GET_ARG_DEFV(TEST_ARG_NODEF));
MACRO_TEST_EVAL(ARG_DEFV, = false, _GET_ARG_DEFV(TEST_ARG_DEF));
MACRO_TEST_EVAL(ARG_DEFV, = {}, _GET_ARG_DEFV(TEST_ARG_BRACES));
MACRO_TEST_EVAL(ARG_DEFV, = {}, _GET_ARG_DEFV(TEST_ARG_COMPLEX));
MACRO_TEST_EVAL(ARG_DEFV, STRING_INIT_MACRO(""),
                _GET_ARG_DEFV(TEST_ARG_STRING));

VS_ARGS(TEST_ARGS)
VS_ARGS_T(TEST_ARGS);
VS_ARGS_T_NODEF(TEST_ARGS)
VS_ARGS(())
VS_ARGS_T(())
VS_ARGS_T_NODEF(())

#endif // DO_TEST_MACROS_PREPROCESS
