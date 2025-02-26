#include "macros_iter.hpp"

// Generic tools
#define UNUSED(arg) ((void)&(arg))
# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__
# define OBSTRUCT_INDIRECT() OBSTRUCT

#define NARGS_ARG_N_(...) NARGS_ARG_N(__VA_ARGS__)
#define IS_LESS_THAN_NARGS_MAX_0(...)        \
  NARGS_ARG_N_(dummy, ## __VA_ARGS__, 0, 0, EXPAND(NARGS_ZEROS()))
#define IS_LESS_THAN_NARGS_MAX_1(...)        \
  NARGS_ARG_N_(dummy, ## __VA_ARGS__, 1, 1, EXPAND(NARGS_ONES()))
#define IS_LESS_THAN_NARGS_MAX(...)                             \
  AND(COMPL(BOOL(IS_LESS_THAN_NARGS_MAX_0(__VA_ARGS__))),       \
      BOOL(IS_LESS_THAN_NARGS_MAX_1(__VA_ARGS__)))
#define IS_MORE_THAN_NARGS_MAX(...)                             \
  NOT(IS_LESS_THAN_NARGS_MAX(__VA_ARGS__))

#define CAT(a, ...) a ## __VA_ARGS__
#define CHECK_EMPTY_N(x, n, ...) n
#define CHECK_EMPTY(...) CHECK_EMPTY_N(__VA_ARGS__, 0,)
#define IS_EMPTY_N(x, ...) CHECK_EMPTY_N(dummy, ## __VA_ARGS__, x, )
#define IS_EMPTY_(...)                                                  \
  AND(BOOL(IS_EMPTY_N(1, ## __VA_ARGS__)),                              \
      COMPL(BOOL(IS_EMPTY_N(0, ## __VA_ARGS__))))

#define IS_EMPTY(...)                                                   \
  BOOL(IS_EMPTY_(__VA_ARGS__))
#define AND(x, y)                                               \
  CHECK_EMPTY(CONCATENATE_MAX(AND_, CONCATENATE_MAX(x, y)))
#define AND_00
#define AND_01
#define AND_10
#define AND_11 ~, 1,
#define OR(x, y)
#define OR_00
#define OR_01 ~, 1,
#define OR_10 ~, 1,
#define OR_11 ~, 1,
#define NOT(x) CHECK_EMPTY(CONCATENATE_MAX(NOT_, x))
#define NOT_0 ~, 1,
#define NOT_1
#define COMPL(b) CONCATENATE_MAX(COMPL_, b)
#define COMPL_0 1
#define COMPL_1 0
#define BOOL(x) COMPL(NOT(x))
#define IIF(c) CONCATENATE_MAX(IIF_, c)
#define IIF_0(t, ...) __VA_ARGS__
#define IIF_1(t, ...) t
#define IF(c) IIF(BOOL(c))
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

#define ADD_BRACKETS(...) <JOIN_ARGS(SEP_COMMA, __VA_ARGS__)>
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
  std::map OBSTRUCT(ADD_BRACKETS_DEFERED) () ( __VA_ARGS__ )

#define _Args(...) __VA_ARGS__
#define PASS_THROUGH(X) X
#define STRIP_PARENS(X) X
#define PACK_MACRO_(X) STRIP_PARENS( _Args X )
#define PACK_MACRO(...) PACK_MACRO_((__VA_ARGS__))
#define PACK_MACRO_DEFERED() PACK_MACRO
#define UNPACK_MACRO(...) __VA_ARGS__
#define UNPACK_PARENS(X) STRIP_PARENS( _Args X )
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
  WHEN(N) (                                         \
    _0 REPEAT(DEC(N), GET_ARG_N_COMMA, __VA_ARGS__) \
  )
#define DROP_N_ARGS(N, ...)                     \
  OBSTRUCT(FIRST_N_ARGS)(SUBTRACT(NARGS(__VA_ARGS__), N), __VA_ARGS__)


// This version alone does not work on windows where macros are limited
//   to 127 arguments or less and there are more parameters in some
//   modules (e.g. PS)
#define NARGS_(...) NARGS(__VA_ARGS__)
#define NARGS_DIRECT_HELPER(...) , ## __VA_ARGS__
#define NARGS_DIRECT_(...) NARGS_DIRECT__(__VA_ARGS__)
#define NARGS_DIRECT__(dummy, ...) NARGS_ARG_N(__VA_ARGS__)
#define NARGS_DIRECT(...)                                 \
  NARGS_DIRECT_(dummy NARGS_DIRECT_HELPER(__VA_ARGS__),   \
                NARGS_REVERSE_SEQ_N(), )

// This version uses ", ## __VA_ARGS__" which may not work with MSVC
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


#define FOR_EACH_NARG_HELPER(...) , ## __VA_ARGS__
#define FOR_EACH_NARG(...) NARGS(__VA_ARGS__)
#define FOR_EACH_NARG_(...) FOR_EACH_NARG__(__VA_ARGS__)
#define FOR_EACH_NARG__(dummy, ...) NARGS_ARG_N(__VA_ARGS__)

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
// #define IF_NOT_EMPTY_1(body)
// #define IF_NOT_EMPTY_0(body) body
// #define IF_NOT_EMPTY(cond, body)
// CONCATENATE_MAX(IF_NOT_EMPTY_, IS_EMPTY(cond))(body)
#define IF_NOT_EMPTY(cond, body)				\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(FIRST_ARG, (body), cond)
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
                   (caller, args, ## __VA_ARGS__), EXPAND vars2)
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


#ifdef DO_TEST_MACROS_PREPROCESS

TEST_IS_EMPTY;
TEST_NARGS;

TEST_IS_LESS_THAN_NARGS_MAX_0 = IS_LESS_THAN_NARGS_MAX();
TEST_IS_LESS_THAN_NARGS_MAX_1 = IS_LESS_THAN_NARGS_MAX(A);
TEST_IS_LESS_THAN_NARGS_MAX_3 = IS_LESS_THAN_NARGS_MAX(3);
TEST_IS_LESS_THAN_NARGS_MAX_MAX = IS_LESS_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS);
TEST_IS_LESS_THAN_NARGS_MAX_MAXP1 = IS_LESS_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS_PLUS1);

TEST_IS_MORE_THAN_NARGS_MAX_0 = IS_MORE_THAN_NARGS_MAX();
TEST_IS_MORE_THAN_NARGS_MAX_1 = IS_MORE_THAN_NARGS_MAX(A);
TEST_IS_MORE_THAN_NARGS_MAX_3 = IS_MORE_THAN_NARGS_MAX(3);
TEST_IS_MORE_THAN_NARGS_MAX_MAX = IS_MORE_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS);
TEST_IS_MORE_THAN_NARGS_MAX_MAXP1 = IS_MORE_THAN_NARGS_MAX(TEST_SEQ_MAX_ARGS_PLUS1);

FOR1 = FOR_EACH(PASS_THROUGH, A);
FOR3 = FOR_EACH(PASS_THROUGH, A, B, C);
FOR_EACH_COMMA1 = ADD_PARENS(FOR_EACH_COMMA(PASS_THROUGH, A));
FOR_EACH_COMMA3 = ADD_PARENS(FOR_EACH_COMMA(PASS_THROUGH, A, B, C));
TEST_GET_A = GET_ARG_N_BEFORE_END(2, A, B, C);
TEST_GET_B = GET_ARG_N_BEFORE_END(1, A, B, C);
TEST_GET_C = GET_ARG_N_BEFORE_END(0, A, B, C);
TEST_GET_ARG_EMPTY = GET_ARG_N_BEFORE_END(0, A, B, );
TEST_NARGS_3 = NARGS(A, B, );
TEST_LAST_ARG_EMPTY = LAST_ARG(A, B, );


#endif // DO_TEST_MACROS_PREPROCESS
