// Generic tools
#define UNUSED(arg) ((void)&(arg))
# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__
#define EVAL(...)  EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__
#define SEP_COMMA() ,
#define SEP_SEMICOLON() ;
#define SEP_EMPTY()
#define SEP_OR() ||
#define SEP_AND() &&
#define SEP_BITWISE_OR() |
#define SEP_BITWISE_AND() &
#define SEP_ADD() +
#define SEP_MULTIPLY() *

#define _Args(...) __VA_ARGS__
#define PASS_THROUGH(X) X
#define STRIP_PARENS(X) X
#define PACK_MACRO_(X) STRIP_PARENS( _Args X )
#define PACK_MACRO(...) PACK_MACRO_((__VA_ARGS__))
#define UNPACK_MACRO(...) __VA_ARGS__
#define UNPACK_PARENS(X) STRIP_PARENS( _Args X )
#define ADD_PARENS(...) (__VA_ARGS__)
#define STR_MACRO(X) #X
#define CALL_MACRO(what, ...)			\
  what(__VA_ARGS__)
#define CALL_MACRO_PACKED(what, args)		\
  what args
#define CALL_WITH_EMPTY_ARGS(what, args, x)	\
  what(x)
#define CALL_WITH_PREFIX_ARGS(what, args, x)	\
  what(UNPACK_PARENS(args), x)
#define CALL_WITH_SUFFIX_ARGS(what, args, x)	\
  what(x, UNPACK_PARENS(args))
#define CALL_WITH_PREFIX_ARGS_PACKED(what, args, x)	\
  what(UNPACK_PARENS args, x)
#define CALL_WITH_SUFFIX_ARGS_PACKED(what, args, x)	\
  what(x, UNPACK_PARENS args)
  
#define STRINGIZE(arg)  STRINGIZE1(arg)
#define STRINGIZE1(arg) STRINGIZE2(arg)
#define STRINGIZE2(arg) #arg

#define JOIN(arg1, arg2)  JOIN1(arg1, arg2)
#define JOIN1(arg1, arg2) JOIN2(arg1, arg2)
#define JOIN2(arg1, arg2) arg1 arg2

#define CONCATENATE(arg1, arg2)   CONCATENATE1(arg1, arg2)
#define CONCATENATE1(arg1, arg2)  CONCATENATE2(arg1, arg2)
#define CONCATENATE2(arg1, arg2)  arg1##arg2

// Control parameters
// #define CHECK_VALUE_SET_ALTS 1
#define COMPARE_PRECISION 15
#define COMPARE_RELATIVE_EPSILON			\
  std::numeric_limits<double>::epsilon()
#define COMPARE_ABSOLUTE_EPSILON 1e-9

class _EmptyMacroType {
public:
  static std::size_t memberCount() {
    return 0;
  }
  static std::string memberState() {
    return "";
  }
  static void select(bool = true) {}
};

// Inheritance for ValueSet
#define INHERIT_METHOD_ENUM_BASE(name1, name2, ...)	\
  using __VA_ARGS__::name1;					\
  using __VA_ARGS__::print ## name2;				\
  using __VA_ARGS__::string ## name2
#define INHERIT_METHOD_ENUM_MAP(name1, name2, name3, ...)	\
  INHERIT_METHOD_ENUM_BASE(name1, name2, __VA_ARGS__);		\
  using __VA_ARGS__::get ## name3
#define INHERIT_METHOD_ENUM_MAP_NAMES(name1, name2, name3, ...)	\
  using __VA_ARGS__::name1;					\
  using __VA_ARGS__::get ## name3
#define INHERIT_METHOD_ENUM_VECTOR(name1, name2, ...)		\
  INHERIT_METHOD_ENUM_BASE(name1, name2, __VA_ARGS__);		\
  using __VA_ARGS__::is ## name2;				\
  using __VA_ARGS__::check ## name2;				\
  using __VA_ARGS__::checkNot ## name2
#define INHERIT_METHOD_ENUM_VECTOR_EDIT(name1, name2, ...)	\
  INHERIT_METHOD_ENUM_VECTOR(name1, name2, __VA_ARGS__);	\
  using __VA_ARGS__::add ## name2;				\
  using __VA_ARGS__::remove ## name2;				\
  using __VA_ARGS__::addMultiple ## name2;			\
  using __VA_ARGS__::removeMultiple ## name2;			\
  using __VA_ARGS__::clear ## name2
#define INHERIT_METHOD_ENUM(...)					\
  using __VA_ARGS__::module;						\
  using __VA_ARGS__::param_type;					\
  using __VA_ARGS__::error_prefix;					\
  using __VA_ARGS__::print_map;						\
  using __VA_ARGS__::print_vector;					\
  using __VA_ARGS__::string_map;					\
  using __VA_ARGS__::string_vector;					\
  using __VA_ARGS__::fromName;						\
  INHERIT_METHOD_ENUM_MAP_NAMES(names, Names, Name, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(constant, Constant, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(calculated, Calculated, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(nonvector, Nonvector, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR_EDIT(skipped, Skipped, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(resetone, Resetone, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(initonce, Initonce, __VA_ARGS__)

/*
  INHERIT_METHOD_ENUM_MAP(defaults, Defaults, Default, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_MAP(defaults_C3, Defaults_C3, Default_C3, __VA_ARGS__); \
 */

#define INHERIT_METHODS_VALUE_SET_TYPES(...)	\
  typedef __VA_ARGS__ ParentClass;		\
  using typename __VA_ARGS__::BaseClass;	\
  using typename __VA_ARGS__::EnumClass;	\
  using typename __VA_ARGS__::EnumType;		\
  using typename __VA_ARGS__::ValueType;	\
  using typename __VA_ARGS__::iterator;		\
  using typename __VA_ARGS__::const_iterator;	\
  INHERIT_METHOD_ENUM(__VA_ARGS__);

#define INHERIT_METHODS_VALUE_SET_BASE(...)	\
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__)	\
  using __VA_ARGS__::defaults;			\
  using __VA_ARGS__::inArrays;			\
  using __VA_ARGS__::remove_skipped;		\
  using __VA_ARGS__::get_pointer_map;		\
  using __VA_ARGS__::print_value_map;		\
  using __VA_ARGS__::compareValues;		\
  using __VA_ARGS__::check_value_map;		\
  using __VA_ARGS__::check_value_maps;		\
  using __VA_ARGS__::update_value_map;		\
  using __VA_ARGS__::copy_value_map;		\
  using __VA_ARGS__::init_value_map;		\
  using __VA_ARGS__::get_value;			\
  using __VA_ARGS__::get_value_const;		\
  using __VA_ARGS__::set_value;			\
  using __VA_ARGS__::set_value_from_EnzymeAct;	\
  using __VA_ARGS__::get_value_orig;		\
  using __VA_ARGS__::set_value_orig;		\
  using __VA_ARGS__::insert_value_orig;		\
  using __VA_ARGS__::check_value_alt;		\
  using __VA_ARGS__::check_value_alts;		\
  using __VA_ARGS__::reset_value_map;		\
  using __VA_ARGS__::value_map_toArray;		\
  using __VA_ARGS__::value_map_fromArray;
#define INHERIT_METHODS_VALUE_SET(...)		\
  /*INHERIT_METHODS_VALUE_SET_BASE(__VA_ARGS__)*/	\
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__)		\
  using typename __VA_ARGS__::ValueSetClass;	\
  using __VA_ARGS__::begin;			\
  using __VA_ARGS__::end;			\
  using __VA_ARGS__::initDefaults;		\
  using __VA_ARGS__::checkDefaults;		\
  using __VA_ARGS__::print;			\
  using __VA_ARGS__::check;			\
  using __VA_ARGS__::update_values;		\
  using __VA_ARGS__::has;			\
  using __VA_ARGS__::set;			\
  using __VA_ARGS__::get;			\
  using __VA_ARGS__::sizeArray;			\
  using __VA_ARGS__::toArray;			\
  using __VA_ARGS__::fromArray;
#define DECLARE_VALUE_SET_BASE(cls, ...)       \
  typedef cls ThisClass;		       \
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)
#define DECLARE_VALUE_SET_STATIC_BASE(cls, ...)	\
  typedef cls ThisClass;		       \
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)

#define DEFAULT_VALUE(cls, name)		\
  cls EnumBaseClass::getDefault(cls EnumClass::name, 0.0)
#define DECLARE_VALUE_SET_MEMBER(name)		\
  double name = DEFAULT_VALUE(, name)
#define DECLARE_VALUE_SET_STATIC_MEMBER(name)	\
  static double name
#define DECLARE_VALUE_SET_STATIC_MEMBER_CONST(name)	\
  static const double name
#define DEFINE_VALUE_SET_MEMBER(name)					\
  name = DEFAULT_VALUE(, name);						\
  ValueSetClass::insertOrig(EnumClass::name, &name, "DEFINE_VALUE_SET_MEMBER: ")
#define DEFINE_VALUE_SET_STATIC_MEMBER(cls, name)			\
  double cls::name = DEFAULT_VALUE(cls::, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER_CONST(cls, name)			\
  const double cls::name = DEFAULT_VALUE(cls::, name)


#define FOR_EACH_WITH_ARGS_1(what, caller, sep, args, x)	\
  caller(what, args, x)
#define FOR_EACH_WITH_ARGS_2(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()	\
  FOR_EACH_WITH_ARGS_1(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_3(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_2(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_4(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_3(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_5(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_4(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_6(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_5(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_7(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_6(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_8(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_7(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_9(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_8(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_10(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_9(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_11(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_10(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_12(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_11(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_13(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_12(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_14(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_13(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_15(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_14(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_16(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_15(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_17(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_16(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_18(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_17(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_19(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_18(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_20(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_19(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_21(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_20(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_22(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_21(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_23(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_22(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_24(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_23(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_25(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_24(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_26(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_25(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_27(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_26(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_28(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_27(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_29(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_28(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_30(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_29(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_31(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_30(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_32(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_31(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_33(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_32(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_34(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_33(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_35(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_34(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_36(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_35(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_37(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_36(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_38(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_37(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_39(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_38(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_40(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_39(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_41(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_40(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_42(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_41(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_43(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_42(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_44(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_43(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_45(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_44(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_46(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_45(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_47(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_46(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_48(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_47(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_49(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_48(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_50(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_49(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_51(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_50(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_52(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_51(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_53(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_52(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_54(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_53(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_55(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_54(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_56(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_55(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_57(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_56(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_58(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_57(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_59(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_58(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_60(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_59(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_61(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_60(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_62(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_61(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_63(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_62(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_64(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_63(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_65(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_64(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_66(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_65(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_67(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_66(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_68(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_67(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_69(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_68(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_70(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_69(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_71(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_70(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_72(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_71(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_73(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_72(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_74(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_73(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_75(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_74(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_76(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_75(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_77(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_76(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_78(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_77(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_79(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_78(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_80(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_79(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_81(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_80(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_82(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_81(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_83(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_82(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_84(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_83(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_85(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_84(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_86(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_85(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_87(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_86(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_88(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_87(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_89(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_88(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_90(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_89(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_91(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_90(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_92(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_91(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_93(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_92(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_94(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_93(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_95(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_94(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_96(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_95(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_97(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_96(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_98(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_97(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_99(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_98(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_100(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_99(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_101(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_100(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_102(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()	\
  FOR_EACH_WITH_ARGS_101(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_103(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_102(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_104(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_103(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_105(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_104(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_106(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_105(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_107(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_106(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_108(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_107(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_109(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_108(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_110(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_109(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_111(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_110(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_112(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_111(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_113(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_112(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_114(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_113(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_115(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_114(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_116(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_115(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_117(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_116(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_118(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_117(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_119(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_118(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_120(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_119(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_121(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_120(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_122(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_121(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_123(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_122(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_124(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_123(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_125(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_124(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_126(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_125(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_127(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_126(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_128(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_127(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_129(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_128(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_130(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_129(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_131(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_130(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_132(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_131(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_133(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_132(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_134(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_133(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_135(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_134(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_136(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_135(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_137(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_136(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_138(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_137(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_139(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_138(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_140(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_139(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_141(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_140(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_142(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_141(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_143(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_142(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_144(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_143(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_145(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_144(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_146(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_145(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_147(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_146(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_148(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_147(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_149(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_148(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_150(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_149(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_151(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_150(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_152(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_151(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_153(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_152(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_154(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_153(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_155(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_154(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_156(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_155(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_157(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_156(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_158(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_157(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_159(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_158(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_160(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_159(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_161(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_160(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_162(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_161(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_163(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_162(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_164(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_163(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_165(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_164(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_166(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_165(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_167(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_166(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_168(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_167(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_169(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_168(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_170(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_169(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_171(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_170(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_172(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_171(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_173(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_172(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_174(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_173(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_175(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_174(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_176(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_175(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_177(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_176(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_178(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_177(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_179(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_178(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_180(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_179(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_181(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_180(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_182(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_181(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_183(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_182(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_184(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_183(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_185(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_184(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_186(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_185(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_187(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_186(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_188(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_187(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_189(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_188(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_190(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_189(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_191(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_190(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_192(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_191(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_193(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_192(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_194(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_193(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_195(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_194(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_196(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_195(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_197(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_196(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_198(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_197(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_199(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_198(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_200(what, caller, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_199(what, caller, sep, args, __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, N, ...) N
#define FOR_EACH_RSEQ_N() 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 140, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 40, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, caller, sep, args, ...)			\
  CONCATENATE(FOR_EACH_WITH_ARGS_, N)(what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH(what, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_PREFIX_ARGS(what, sep, args, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_PREFIX_ARGS, sep, args, __VA_ARGS__)
#define FOR_EACH_SUFFIX_ARGS(what, sep, args, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_SUFFIX_ARGS, sep, args, __VA_ARGS__)
#define FOR_EACH_PREFIX_ARGS_PACKED(what, sep, args, ...)		\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what,				\
	    CALL_WITH_PREFIX_ARGS_PACKED, sep, args, __VA_ARGS__)
#define FOR_EACH_SUFFIX_ARGS_PACKED(what, sep, args, ...)		\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what,				\
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
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what,				\
	    CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (),			\
	    FOR_EACH_WITH_PREFIX_ARGS_COMMA(ADD_PARENS, args,		\
					    __VA_ARGS__))
#define FOR_EACH_WITH_SUFFIX_ARGS_PACKED(what, args, ...)		\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what,				\
	    CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (),			\
	    FOR_EACH_WITH_SUFFIX_ARGS_COMMA(ADD_PARENS, args,		\
					    __VA_ARGS__))
#define FOR_EACH_WITH_ARGS_PACKED(what, args, ...)	\
  FOR_EACH_WITH_PREFIX_ARGS_PACKED(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_PREFIX_ARGS(what, args, ...)	\
  FOR_EACH_WITH_ARGS_(FOR_EACH_WITH_PREFIX_ARGS_COMMA(what, args, __VA_ARGS__))
#define FOR_EACH_WITH_SUFFIX_ARGS(what, args, ...)	\
  FOR_EACH_WITH_ARGS_(FOR_EACH_WITH_SUFFIX_ARGS_COMMA(what, args, __VA_ARGS__))
#define FOR_EACH_WITH_ARGS(what, args, ...)	\
  FOR_EACH_WITH_PREFIX_ARGS(what, args, __VA_ARGS__)
#define PREFIX_EACH(prefix, ...)		\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(CONCATENATE, (prefix), __VA_ARGS__)

#define EQUALS(a, b) (a == b)
#define VAR_IN_LIST(var, ...)						\
  (FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), EQUALS, CALL_WITH_PREFIX_ARGS,	\
	     SEP_OR, (var), __VA_ARGS__))
#define VAR_IN_PREFIXED_LIST(var, prefix, ...)			\
  VAR_IN_LIST(var, PREFIX_EACH(prefix, __VA_ARGS__))
