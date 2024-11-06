// Generic tools
#define UNUSED(arg) ((void)&(arg))
# define EMPTY(...)
# define DEFER(...) __VA_ARGS__ EMPTY()
# define OBSTRUCT(...) __VA_ARGS__ DEFER(EMPTY)()
# define EXPAND(...) __VA_ARGS__
#define ADD_BRACKETS(...) <JOIN_ARGS(SEP_COMMA, __VA_ARGS__)>
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
#define ADD_PARENS_DEFERED() ADD_PARENS
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
#define CALL_DO_NOTHING(what, args, x)

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
#define CALL_DO_NOTHING_DEFER(what, args, x)
  
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
// #define VERBOSE_VALUE_SET_DEBUG 1
#define COMPARE_PRECISION 15
#define COMPARE_RELATIVE_EPSILON			\
  std::numeric_limits<double>::epsilon()
#define COMPARE_ABSOLUTE_EPSILON 1e-9
#define CHECK_NEW_MATCHES_ORIG 1
#define MAKE_EQUIVALENT_TO_MATLAB 1

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
  using __VA_ARGS__::fromGlymaid;					\
  INHERIT_METHOD_ENUM_MAP_NAMES(names, Names, Name, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_MAP(aliases, Aliases, Alias, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(constant, Constant, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(calculated, Calculated, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(nonvector, Nonvector, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR_EDIT(skipped, Skipped, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(resetone, Resetone, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(initonce, Initonce, __VA_ARGS__)

#define INHERIT_METHODS_VALUE_SET_TYPES(...)	\
  typedef __VA_ARGS__ ParentClass;		\
  friend __VA_ARGS__;				\
  using typename __VA_ARGS__::BaseClass;	\
  using typename __VA_ARGS__::EnumClass;	\
  using typename __VA_ARGS__::EnumType;		\
  using typename __VA_ARGS__::ValueType;	\
  using typename __VA_ARGS__::iterator;		\
  using typename __VA_ARGS__::const_iterator;	\
  using typename __VA_ARGS__::VS_FLAGS;		\
  INHERIT_METHOD_ENUM(__VA_ARGS__);

#define INHERIT_METHODS_VALUE_SET_BASE(...)	\
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__)	\
  using __VA_ARGS__::static_flags;		\
  using __VA_ARGS__::defaults;			\
  using __VA_ARGS__::static_values;		\
  using __VA_ARGS__::child_classes;		\
  using __VA_ARGS__::getValueSetClass;		\
  using __VA_ARGS__::inArrays;			\
  using __VA_ARGS__::remove_skipped;		\
  using __VA_ARGS__::get_pointer_map;		\
  using __VA_ARGS__::max_field_width;		\
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
  using __VA_ARGS__::reset_value_map;		\
  using __VA_ARGS__::value_map_toArray;		\
  using __VA_ARGS__::value_map_fromArray;	\
  using __VA_ARGS__::fromNameWithAliases;	\
  using __VA_ARGS__::setDefault;		\
  using __VA_ARGS__::getDefault;
#define INHERIT_METHOD_VALUE_SET(name, ...) using ParentClass::name;
#define INHERIT_METHODS_VALUE_SET(...)		\
  INHERIT_METHODS_VALUE_SET_TYPES(__VA_ARGS__)	\
  using typename __VA_ARGS__::ValueSetClass;	\
  ADD_METHODS_VALUE_SET_T_BOTH_NODUP(INHERIT_METHOD_VALUE_SET);		\
  ADD_METHODS_VALUE_SET_T_STATIC_NODUP(INHERIT_METHOD_VALUE_SET);	\
  using __VA_ARGS__::begin;			\
  using __VA_ARGS__::end;			\
  using __VA_ARGS__::getValueSetClass;		\
  using __VA_ARGS__::checkDefaults;		\
  using __VA_ARGS__::check;			\
  using __VA_ARGS__::update_values;		\
  using __VA_ARGS__::hasGlymaID;		\
  using __VA_ARGS__::_initMembers;		\
  using __VA_ARGS__::_initMembersState;		\
  using __VA_ARGS__::_initValues;		\
  using __VA_ARGS__::_print;			\
  using __VA_ARGS__::_initMemberPointers;	\
  using __VA_ARGS__::_sizeArray;
#define DECLARE_VALUE_SET_BASE(cls, ...)		\
  typedef cls ThisClass;				\
  friend class ValueSetClassType<cls>;			\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)		\
  using __VA_ARGS__::_copyMembers;			\
  using __VA_ARGS__::_resetValues;			\
  friend ValueSetClass;
#define DECLARE_VALUE_SET_STATIC_BASE(cls, ...)		\
  typedef cls ThisClass;				\
  friend class ValueSetClassType<cls>;			\
  INHERIT_METHODS_VALUE_SET(__VA_ARGS__)

#define DEFAULT_VALUE(cls, name)		\
  cls EnumBaseClass::getDefault(cls EnumClass::name, 0.0)
#define DECLARE_VALUE_SET_STATIC_MEMBER(name)	\
  static double name
#define DECLARE_VALUE_SET_STATIC_MEMBER_CONST(name)	\
  static const double name

#define DECLARE_VALUE_SET_MEMBER(name)		\
  double name = DEFAULT_VALUE(, name)
#define DEFINE_VALUE_SET_MEMBER(name)					\
  name = DEFAULT_VALUE(, name);						\
  ValueSetClass::insertOrig(EnumClass::name, &name, "DEFINE_VALUE_SET_MEMBER: ")
#define DEFINE_VALUE_SET_STATIC_MEMBER(cls, name)			\
  double cls::name = DEFAULT_VALUE(cls::, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER_CONST(cls, name)			\
  const double cls::name = DEFAULT_VALUE(cls::, name)

#define GET_ARGS_AFTER_1(_0, ...) __VA_ARGS__
#define GET_ARGS_AFTER_10(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) __VA_ARGS__
#define GET_ARGS_AFTER_20(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_10(__VA_ARGS__)
#define GET_ARGS_AFTER_30(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_20(__VA_ARGS__)
#define GET_ARGS_AFTER_40(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_30(__VA_ARGS__)
#define GET_ARGS_AFTER_50(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_40(__VA_ARGS__)
#define GET_ARGS_AFTER_60(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_50(__VA_ARGS__)
#define GET_ARGS_AFTER_70(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_60(__VA_ARGS__)
#define GET_ARGS_AFTER_80(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_70(__VA_ARGS__)
#define GET_ARGS_AFTER_90(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_80(__VA_ARGS__)
#define GET_ARGS_AFTER_100(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_90(__VA_ARGS__)
#define GET_ARGS_AFTER_110(...) GET_ARGS_AFTER_10(GET_ARGS_AFTER_100(__VA_ARGS__)
#define GET_ARG_N_AFTER_M(M, N, ...) GET_ARG_N(N, CONCATENATE(GET_ARGS_AFTER_, M)(__VA_ARGS__))
#define GET_ARG_N_AFTER_10(N, ...) GET_ARG_N_AFTER_M(10, N, __VA_ARGS__)
#define GET_ARG_N_AFTER_100(N, ...) GET_ARG_N_AFTER_M(100, N, __VA_ARGS__)
#define GET_ARG_N_AFTER_110(N, ...) GET_ARG_N_AFTER_M(110, N, __VA_ARGS__)

#define GET_ARG_N(N, ...) CONCATENATE(GET_ARG_, N)(__VA_ARGS__)
#define GET_ARG_0(_0, ...) _0
#define GET_ARG_1(_0, _1, ...) _1
#define GET_ARG_2(_0, _1, _2, ...) _2
#define GET_ARG_3(_0, _1, _2, _3, ...) _3
#define GET_ARG_4(_0, _1, _2, _3, _4, ...) _4
#define GET_ARG_5(_0, _1, _2, _3, _4, _5, ...) _5
#define GET_ARG_6(_0, _1, _2, _3, _4, _5, _6, ...) _6
#define GET_ARG_7(_0, _1, _2, _3, _4, _5, _6, _7, ...) _7
#define GET_ARG_8(_0, _1, _2, _3, _4, _5, _6, _7, _8,...) _8
#define GET_ARG_9(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9, ...) _9
#define GET_ARG_10(...) GET_ARG_N_AFTER_10(0, __VA_ARGS__)
#define GET_ARG_11(...) GET_ARG_N_AFTER_10(1, __VA_ARGS__)
#define GET_ARG_12(...) GET_ARG_N_AFTER_10(2, __VA_ARGS__)
#define GET_ARG_13(...) GET_ARG_N_AFTER_10(3, __VA_ARGS__)
#define GET_ARG_14(...) GET_ARG_N_AFTER_10(4, __VA_ARGS__)
#define GET_ARG_15(...) GET_ARG_N_AFTER_10(5, __VA_ARGS__)
#define GET_ARG_16(...) GET_ARG_N_AFTER_10(6, __VA_ARGS__)
#define GET_ARG_17(...) GET_ARG_N_AFTER_10(7, __VA_ARGS__)
#define GET_ARG_18(...) GET_ARG_N_AFTER_10(8, __VA_ARGS__)
#define GET_ARG_19(...) GET_ARG_N_AFTER_10(9, __VA_ARGS__)
#define GET_ARG_20(...) GET_ARG_N_AFTER_10(10, __VA_ARGS__)
#define GET_ARG_21(...) GET_ARG_N_AFTER_10(11, __VA_ARGS__)
#define GET_ARG_22(...) GET_ARG_N_AFTER_10(12, __VA_ARGS__)
#define GET_ARG_23(...) GET_ARG_N_AFTER_10(13, __VA_ARGS__)
#define GET_ARG_24(...) GET_ARG_N_AFTER_10(14, __VA_ARGS__)
#define GET_ARG_25(...) GET_ARG_N_AFTER_10(15, __VA_ARGS__)
#define GET_ARG_26(...) GET_ARG_N_AFTER_10(16, __VA_ARGS__)
#define GET_ARG_27(...) GET_ARG_N_AFTER_10(17, __VA_ARGS__)
#define GET_ARG_28(...) GET_ARG_N_AFTER_10(18, __VA_ARGS__)
#define GET_ARG_29(...) GET_ARG_N_AFTER_10(19, __VA_ARGS__)
#define GET_ARG_30(...) GET_ARG_N_AFTER_10(20, __VA_ARGS__)
#define GET_ARG_31(...) GET_ARG_N_AFTER_10(21, __VA_ARGS__)
#define GET_ARG_32(...) GET_ARG_N_AFTER_10(22, __VA_ARGS__)
#define GET_ARG_33(...) GET_ARG_N_AFTER_10(23, __VA_ARGS__)
#define GET_ARG_34(...) GET_ARG_N_AFTER_10(24, __VA_ARGS__)
#define GET_ARG_35(...) GET_ARG_N_AFTER_10(25, __VA_ARGS__)
#define GET_ARG_36(...) GET_ARG_N_AFTER_10(26, __VA_ARGS__)
#define GET_ARG_37(...) GET_ARG_N_AFTER_10(27, __VA_ARGS__)
#define GET_ARG_38(...) GET_ARG_N_AFTER_10(28, __VA_ARGS__)
#define GET_ARG_39(...) GET_ARG_N_AFTER_10(29, __VA_ARGS__)
#define GET_ARG_40(...) GET_ARG_N_AFTER_10(30, __VA_ARGS__)
#define GET_ARG_41(...) GET_ARG_N_AFTER_10(31, __VA_ARGS__)
#define GET_ARG_42(...) GET_ARG_N_AFTER_10(32, __VA_ARGS__)
#define GET_ARG_43(...) GET_ARG_N_AFTER_10(33, __VA_ARGS__)
#define GET_ARG_44(...) GET_ARG_N_AFTER_10(34, __VA_ARGS__)
#define GET_ARG_45(...) GET_ARG_N_AFTER_10(35, __VA_ARGS__)
#define GET_ARG_46(...) GET_ARG_N_AFTER_10(36, __VA_ARGS__)
#define GET_ARG_47(...) GET_ARG_N_AFTER_10(37, __VA_ARGS__)
#define GET_ARG_48(...) GET_ARG_N_AFTER_10(38, __VA_ARGS__)
#define GET_ARG_49(...) GET_ARG_N_AFTER_10(39, __VA_ARGS__)
#define GET_ARG_50(...) GET_ARG_N_AFTER_10(40, __VA_ARGS__)
#define GET_ARG_51(...) GET_ARG_N_AFTER_10(41, __VA_ARGS__)
#define GET_ARG_52(...) GET_ARG_N_AFTER_10(42, __VA_ARGS__)
#define GET_ARG_53(...) GET_ARG_N_AFTER_10(43, __VA_ARGS__)
#define GET_ARG_54(...) GET_ARG_N_AFTER_10(44, __VA_ARGS__)
#define GET_ARG_55(...) GET_ARG_N_AFTER_10(45, __VA_ARGS__)
#define GET_ARG_56(...) GET_ARG_N_AFTER_10(46, __VA_ARGS__)
#define GET_ARG_57(...) GET_ARG_N_AFTER_10(47, __VA_ARGS__)
#define GET_ARG_58(...) GET_ARG_N_AFTER_10(48, __VA_ARGS__)
#define GET_ARG_59(...) GET_ARG_N_AFTER_10(49, __VA_ARGS__)
#define GET_ARG_60(...) GET_ARG_N_AFTER_10(50, __VA_ARGS__)
#define GET_ARG_61(...) GET_ARG_N_AFTER_10(51, __VA_ARGS__)
#define GET_ARG_62(...) GET_ARG_N_AFTER_10(52, __VA_ARGS__)
#define GET_ARG_63(...) GET_ARG_N_AFTER_10(53, __VA_ARGS__)
#define GET_ARG_64(...) GET_ARG_N_AFTER_10(54, __VA_ARGS__)
#define GET_ARG_65(...) GET_ARG_N_AFTER_10(55, __VA_ARGS__)
#define GET_ARG_66(...) GET_ARG_N_AFTER_10(56, __VA_ARGS__)
#define GET_ARG_67(...) GET_ARG_N_AFTER_10(57, __VA_ARGS__)
#define GET_ARG_68(...) GET_ARG_N_AFTER_10(58, __VA_ARGS__)
#define GET_ARG_69(...) GET_ARG_N_AFTER_10(59, __VA_ARGS__)
#define GET_ARG_70(...) GET_ARG_N_AFTER_10(60, __VA_ARGS__)
#define GET_ARG_71(...) GET_ARG_N_AFTER_10(61, __VA_ARGS__)
#define GET_ARG_72(...) GET_ARG_N_AFTER_10(62, __VA_ARGS__)
#define GET_ARG_73(...) GET_ARG_N_AFTER_10(63, __VA_ARGS__)
#define GET_ARG_74(...) GET_ARG_N_AFTER_10(64, __VA_ARGS__)
#define GET_ARG_75(...) GET_ARG_N_AFTER_10(65, __VA_ARGS__)
#define GET_ARG_76(...) GET_ARG_N_AFTER_10(66, __VA_ARGS__)
#define GET_ARG_77(...) GET_ARG_N_AFTER_10(67, __VA_ARGS__)
#define GET_ARG_78(...) GET_ARG_N_AFTER_10(68, __VA_ARGS__)
#define GET_ARG_79(...) GET_ARG_N_AFTER_10(69, __VA_ARGS__)
#define GET_ARG_80(...) GET_ARG_N_AFTER_10(70, __VA_ARGS__)
#define GET_ARG_81(...) GET_ARG_N_AFTER_10(71, __VA_ARGS__)
#define GET_ARG_82(...) GET_ARG_N_AFTER_10(72, __VA_ARGS__)
#define GET_ARG_83(...) GET_ARG_N_AFTER_10(73, __VA_ARGS__)
#define GET_ARG_84(...) GET_ARG_N_AFTER_10(74, __VA_ARGS__)
#define GET_ARG_85(...) GET_ARG_N_AFTER_10(75, __VA_ARGS__)
#define GET_ARG_86(...) GET_ARG_N_AFTER_10(76, __VA_ARGS__)
#define GET_ARG_87(...) GET_ARG_N_AFTER_10(77, __VA_ARGS__)
#define GET_ARG_88(...) GET_ARG_N_AFTER_10(78, __VA_ARGS__)
#define GET_ARG_89(...) GET_ARG_N_AFTER_10(79, __VA_ARGS__)
#define GET_ARG_90(...) GET_ARG_N_AFTER_10(80, __VA_ARGS__)
#define GET_ARG_91(...) GET_ARG_N_AFTER_10(81, __VA_ARGS__)
#define GET_ARG_92(...) GET_ARG_N_AFTER_10(82, __VA_ARGS__)
#define GET_ARG_93(...) GET_ARG_N_AFTER_10(83, __VA_ARGS__)
#define GET_ARG_94(...) GET_ARG_N_AFTER_10(84, __VA_ARGS__)
#define GET_ARG_95(...) GET_ARG_N_AFTER_10(85, __VA_ARGS__)
#define GET_ARG_96(...) GET_ARG_N_AFTER_10(86, __VA_ARGS__)
#define GET_ARG_97(...) GET_ARG_N_AFTER_10(87, __VA_ARGS__)
#define GET_ARG_98(...) GET_ARG_N_AFTER_10(88, __VA_ARGS__)
#define GET_ARG_99(...) GET_ARG_N_AFTER_10(89, __VA_ARGS__)
#define GET_ARG_100(...) GET_ARG_N_AFTER_10(90, __VA_ARGS__)
#define GET_ARG_101(...) GET_ARG_N_AFTER_100(1, __VA_ARGS__)
#define GET_ARG_102(...) GET_ARG_N_AFTER_100(2, __VA_ARGS__)
#define GET_ARG_103(...) GET_ARG_N_AFTER_100(3, __VA_ARGS__)
#define GET_ARG_104(...) GET_ARG_N_AFTER_100(4, __VA_ARGS__)
#define GET_ARG_105(...) GET_ARG_N_AFTER_100(5, __VA_ARGS__)
#define GET_ARG_106(...) GET_ARG_N_AFTER_100(6, __VA_ARGS__)
#define GET_ARG_107(...) GET_ARG_N_AFTER_100(7, __VA_ARGS__)
#define GET_ARG_108(...) GET_ARG_N_AFTER_100(8, __VA_ARGS__)
#define GET_ARG_109(...) GET_ARG_N_AFTER_100(9, __VA_ARGS__)
#define GET_ARG_110(...) GET_ARG_N_AFTER_110(0, __VA_ARGS__)
#define GET_ARG_111(...) GET_ARG_N_AFTER_110(1, __VA_ARGS__)
#define GET_ARG_112(...) GET_ARG_N_AFTER_110(2, __VA_ARGS__)
#define GET_ARG_113(...) GET_ARG_N_AFTER_110(3, __VA_ARGS__)
#define GET_ARG_114(...) GET_ARG_N_AFTER_110(4, __VA_ARGS__)
#define GET_ARG_115(...) GET_ARG_N_AFTER_110(5, __VA_ARGS__)
#define GET_ARG_116(...) GET_ARG_N_AFTER_110(6, __VA_ARGS__)
#define GET_ARG_117(...) GET_ARG_N_AFTER_110(7, __VA_ARGS__)
#define GET_ARG_118(...) GET_ARG_N_AFTER_110(8, __VA_ARGS__)
#define GET_ARG_119(...) GET_ARG_N_AFTER_110(9, __VA_ARGS__)
#define GET_ARG_120(...) GET_ARG_N_AFTER_110(10, __VA_ARGS__)
#define GET_ARG_121(...) GET_ARG_N_AFTER_110(11, __VA_ARGS__)
#define GET_ARG_122(...) GET_ARG_N_AFTER_110(12, __VA_ARGS__)
#define GET_ARG_123(...) GET_ARG_N_AFTER_110(13, __VA_ARGS__)
#define GET_ARG_124(...) GET_ARG_N_AFTER_110(14, __VA_ARGS__)
#define GET_ARG_125(...) GET_ARG_N_AFTER_110(15, __VA_ARGS__)
#define GET_ARG_126(...) GET_ARG_N_AFTER_110(16, __VA_ARGS__)
#define GET_ARG_127(...) GET_ARG_N_AFTER_110(17, __VA_ARGS__)
#define GET_ARG_128(...) GET_ARG_N_AFTER_110(18, __VA_ARGS__)
#define GET_ARG_129(...) GET_ARG_N_AFTER_110(19, __VA_ARGS__)
#define GET_ARG_130(...) GET_ARG_N_AFTER_110(20, __VA_ARGS__)
#define GET_ARG_131(...) GET_ARG_N_AFTER_110(21, __VA_ARGS__)
#define GET_ARG_132(...) GET_ARG_N_AFTER_110(22, __VA_ARGS__)
#define GET_ARG_133(...) GET_ARG_N_AFTER_110(23, __VA_ARGS__)
#define GET_ARG_134(...) GET_ARG_N_AFTER_110(24, __VA_ARGS__)
#define GET_ARG_135(...) GET_ARG_N_AFTER_110(25, __VA_ARGS__)
#define GET_ARG_136(...) GET_ARG_N_AFTER_110(26, __VA_ARGS__)
#define GET_ARG_137(...) GET_ARG_N_AFTER_110(27, __VA_ARGS__)
#define GET_ARG_138(...) GET_ARG_N_AFTER_110(28, __VA_ARGS__)
#define GET_ARG_139(...) GET_ARG_N_AFTER_110(29, __VA_ARGS__)
#define GET_ARG_140(...) GET_ARG_N_AFTER_110(30, __VA_ARGS__)
#define GET_ARG_141(...) GET_ARG_N_AFTER_110(31, __VA_ARGS__)
#define GET_ARG_142(...) GET_ARG_N_AFTER_110(32, __VA_ARGS__)
#define GET_ARG_143(...) GET_ARG_N_AFTER_110(33, __VA_ARGS__)
#define GET_ARG_144(...) GET_ARG_N_AFTER_110(34, __VA_ARGS__)
#define GET_ARG_145(...) GET_ARG_N_AFTER_110(35, __VA_ARGS__)
#define GET_ARG_146(...) GET_ARG_N_AFTER_110(36, __VA_ARGS__)
#define GET_ARG_147(...) GET_ARG_N_AFTER_110(37, __VA_ARGS__)
#define GET_ARG_148(...) GET_ARG_N_AFTER_110(38, __VA_ARGS__)
#define GET_ARG_149(...) GET_ARG_N_AFTER_110(39, __VA_ARGS__)
#define GET_ARG_150(...) GET_ARG_N_AFTER_110(40, __VA_ARGS__)
#define GET_ARG_151(...) GET_ARG_N_AFTER_110(41, __VA_ARGS__)
#define GET_ARG_152(...) GET_ARG_N_AFTER_110(42, __VA_ARGS__)
#define GET_ARG_153(...) GET_ARG_N_AFTER_110(43, __VA_ARGS__)
#define GET_ARG_154(...) GET_ARG_N_AFTER_110(44, __VA_ARGS__)
#define GET_ARG_155(...) GET_ARG_N_AFTER_110(45, __VA_ARGS__)
#define GET_ARG_156(...) GET_ARG_N_AFTER_110(46, __VA_ARGS__)
#define GET_ARG_157(...) GET_ARG_N_AFTER_110(47, __VA_ARGS__)
#define GET_ARG_158(...) GET_ARG_N_AFTER_110(48, __VA_ARGS__)
#define GET_ARG_159(...) GET_ARG_N_AFTER_110(49, __VA_ARGS__)
#define GET_ARG_160(...) GET_ARG_N_AFTER_110(50, __VA_ARGS__)
#define GET_ARG_161(...) GET_ARG_N_AFTER_110(51, __VA_ARGS__)
#define GET_ARG_162(...) GET_ARG_N_AFTER_110(52, __VA_ARGS__)
#define GET_ARG_163(...) GET_ARG_N_AFTER_110(53, __VA_ARGS__)
#define GET_ARG_164(...) GET_ARG_N_AFTER_110(54, __VA_ARGS__)
#define GET_ARG_165(...) GET_ARG_N_AFTER_110(55, __VA_ARGS__)
#define GET_ARG_166(...) GET_ARG_N_AFTER_110(56, __VA_ARGS__)
#define GET_ARG_167(...) GET_ARG_N_AFTER_110(57, __VA_ARGS__)
#define GET_ARG_168(...) GET_ARG_N_AFTER_110(58, __VA_ARGS__)
#define GET_ARG_169(...) GET_ARG_N_AFTER_110(59, __VA_ARGS__)
#define GET_ARG_170(...) GET_ARG_N_AFTER_110(60, __VA_ARGS__)
#define GET_ARG_171(...) GET_ARG_N_AFTER_110(61, __VA_ARGS__)
#define GET_ARG_172(...) GET_ARG_N_AFTER_110(62, __VA_ARGS__)
#define GET_ARG_173(...) GET_ARG_N_AFTER_110(63, __VA_ARGS__)
#define GET_ARG_174(...) GET_ARG_N_AFTER_110(64, __VA_ARGS__)
#define GET_ARG_175(...) GET_ARG_N_AFTER_110(65, __VA_ARGS__)
#define GET_ARG_176(...) GET_ARG_N_AFTER_110(66, __VA_ARGS__)
#define GET_ARG_177(...) GET_ARG_N_AFTER_110(67, __VA_ARGS__)
#define GET_ARG_178(...) GET_ARG_N_AFTER_110(68, __VA_ARGS__)
#define GET_ARG_179(...) GET_ARG_N_AFTER_110(69, __VA_ARGS__)
#define GET_ARG_180(...) GET_ARG_N_AFTER_110(70, __VA_ARGS__)
#define GET_ARG_181(...) GET_ARG_N_AFTER_110(71, __VA_ARGS__)
#define GET_ARG_182(...) GET_ARG_N_AFTER_110(72, __VA_ARGS__)
#define GET_ARG_183(...) GET_ARG_N_AFTER_110(73, __VA_ARGS__)
#define GET_ARG_184(...) GET_ARG_N_AFTER_110(74, __VA_ARGS__)
#define GET_ARG_185(...) GET_ARG_N_AFTER_110(75, __VA_ARGS__)
#define GET_ARG_186(...) GET_ARG_N_AFTER_110(76, __VA_ARGS__)
#define GET_ARG_187(...) GET_ARG_N_AFTER_110(77, __VA_ARGS__)
#define GET_ARG_188(...) GET_ARG_N_AFTER_110(78, __VA_ARGS__)
#define GET_ARG_189(...) GET_ARG_N_AFTER_110(79, __VA_ARGS__)
#define GET_ARG_190(...) GET_ARG_N_AFTER_110(80, __VA_ARGS__)
#define GET_ARG_191(...) GET_ARG_N_AFTER_110(81, __VA_ARGS__)
#define GET_ARG_192(...) GET_ARG_N_AFTER_110(82, __VA_ARGS__)
#define GET_ARG_193(...) GET_ARG_N_AFTER_110(83, __VA_ARGS__)
#define GET_ARG_194(...) GET_ARG_N_AFTER_110(84, __VA_ARGS__)
#define GET_ARG_195(...) GET_ARG_N_AFTER_110(85, __VA_ARGS__)
#define GET_ARG_196(...) GET_ARG_N_AFTER_110(86, __VA_ARGS__)
#define GET_ARG_197(...) GET_ARG_N_AFTER_110(87, __VA_ARGS__)
#define GET_ARG_198(...) GET_ARG_N_AFTER_110(88, __VA_ARGS__)
#define GET_ARG_199(...) GET_ARG_N_AFTER_110(89, __VA_ARGS__)
#define GET_ARG_200(...) GET_ARG_N_AFTER_110(90, __VA_ARGS__)

#define FOR_EACH_WITH_ARGS_0(...)
#define FOR_EACH_WITH_ARGS_1(what, caller, caller0, sep, args, x)	\
  caller0(what, args, x)
#define FOR_EACH_WITH_ARGS_2(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()	\
  FOR_EACH_WITH_ARGS_1(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_3(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_2(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_4(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_3(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_5(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_4(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_6(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_5(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_7(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_6(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_8(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_7(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_9(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_8(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_10(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_9(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_11(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_10(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_12(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_11(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_13(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_12(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_14(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_13(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_15(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_14(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_16(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_15(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_17(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_16(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_18(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_17(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_19(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_18(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_20(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_19(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_21(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_20(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_22(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_21(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_23(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_22(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_24(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_23(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_25(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_24(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_26(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_25(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_27(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_26(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_28(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_27(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_29(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_28(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_30(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_29(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_31(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_30(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_32(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_31(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_33(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_32(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_34(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_33(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_35(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_34(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_36(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_35(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_37(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_36(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_38(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_37(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_39(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_38(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_40(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_39(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_41(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_40(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_42(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_41(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_43(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_42(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_44(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_43(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_45(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_44(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_46(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_45(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_47(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_46(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_48(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_47(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_49(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_48(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_50(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_49(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_51(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_50(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_52(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_51(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_53(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_52(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_54(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_53(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_55(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_54(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_56(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_55(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_57(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_56(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_58(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_57(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_59(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_58(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_60(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_59(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_61(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_60(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_62(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_61(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_63(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_62(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_64(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_63(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_65(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_64(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_66(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_65(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_67(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_66(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_68(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_67(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_69(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_68(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_70(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_69(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_71(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_70(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_72(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_71(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_73(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_72(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_74(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_73(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_75(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_74(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_76(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_75(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_77(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_76(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_78(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_77(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_79(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_78(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_80(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_79(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_81(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_80(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_82(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_81(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_83(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_82(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_84(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_83(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_85(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_84(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_86(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_85(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_87(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_86(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_88(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_87(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_89(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_88(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_90(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_89(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_91(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_90(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_92(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_91(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_93(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_92(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_94(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_93(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_95(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_94(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_96(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_95(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_97(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_96(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_98(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_97(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_99(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_98(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_100(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_99(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_101(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_100(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_102(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()	\
  FOR_EACH_WITH_ARGS_101(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_103(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_102(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_104(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_103(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_105(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_104(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_106(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_105(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_107(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_106(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_108(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_107(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_109(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_108(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_110(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_109(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_111(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_110(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_112(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_111(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_113(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_112(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_114(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_113(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_115(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_114(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_116(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_115(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_117(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_116(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_118(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_117(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_119(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_118(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_120(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_119(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_121(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_120(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_122(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_121(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_123(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_122(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_124(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_123(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_125(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_124(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_126(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_125(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_127(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_126(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_128(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_127(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_129(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_128(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_130(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_129(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_131(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_130(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_132(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_131(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_133(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_132(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_134(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_133(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_135(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_134(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_136(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_135(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_137(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_136(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_138(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_137(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_139(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_138(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_140(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_139(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_141(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_140(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_142(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_141(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_143(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_142(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_144(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_143(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_145(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_144(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_146(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_145(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_147(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_146(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_148(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_147(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_149(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_148(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_150(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_149(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_151(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_150(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_152(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_151(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_153(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_152(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_154(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_153(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_155(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_154(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_156(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_155(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_157(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_156(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_158(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_157(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_159(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_158(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_160(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_159(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_161(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_160(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_162(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_161(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_163(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_162(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_164(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_163(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_165(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_164(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_166(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_165(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_167(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_166(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_168(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_167(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_169(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_168(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_170(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_169(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_171(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_170(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_172(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_171(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_173(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_172(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_174(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_173(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_175(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_174(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_176(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_175(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_177(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_176(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_178(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_177(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_179(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_178(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_180(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_179(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_181(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_180(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_182(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_181(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_183(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_182(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_184(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_183(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_185(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_184(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_186(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_185(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_187(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_186(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_188(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_187(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_189(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_188(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_190(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_189(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_191(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_190(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_192(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_191(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_193(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_192(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_194(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_193(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_195(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_194(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_196(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_195(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_197(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_196(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_198(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_197(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_199(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_198(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_200(what, caller, caller0, sep, args, x, ...)\
  caller(what, args, x)sep()\
  FOR_EACH_WITH_ARGS_199(what, caller, caller0, sep, args, __VA_ARGS__)

#define NARGS(...)					  \
  FOR_EACH_NARG_(dummy FOR_EACH_NARG_HELPER(__VA_ARGS__), \
		 FOR_EACH_RSEQ_N(), )
#define FOR_EACH_NARG_HELPER(...) , ## __VA_ARGS__
#define FOR_EACH_NARG(...) NARGS(__VA_ARGS__)
#define FOR_EACH_NARG_(...) FOR_EACH_NARG__(__VA_ARGS__)
#define FOR_EACH_NARG__(dummy, ...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, N, ...) N
#define FOR_EACH_RSEQ_N() 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 140, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 40, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

/*
#define LAST_ARG(...)					\
  GET_ARG_N(FOR_EACH_NARG(__VA_ARGS__), _, __VA_ARGS__, FOR_EACH_RSEQ_N())
*/
#define FIRST_ARG(a, ...) a
#define FIRST_ARG_DEFERED() FIRST_ARG
#define LAST_ARG(...)							\
  FOR_EACH_DIFF_CALLERS_(FOR_EACH_NARG(__VA_ARGS__),			\
			 PASS_THROUGH, CALL_DO_NOTHING,			\
			 CALL_WITH_EMPTY_ARGS, SEP_EMPTY, (), __VA_ARGS__)
#define FOR_EACH_DIFF_CALLERS_(N, what, caller, caller0, sep, args, ...)		\
  CONCATENATE(FOR_EACH_WITH_ARGS_, N)(what, caller, caller0, sep, args, __VA_ARGS__)
#define FOR_EACH_(N, what, caller, sep, args, ...)			\
  CONCATENATE(FOR_EACH_WITH_ARGS_, N)(what, caller, caller, sep, args, __VA_ARGS__)
#define FOR_EACH_GENERIC(what, caller, sep, args, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, caller, sep, args, __VA_ARGS__)
#define FOR_EACH(what, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_EMPTY_ARGS, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_DEFERED(what, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_EMPTY_ARGS_DEFER, SEP_SEMICOLON, (), __VA_ARGS__)
#define FOR_EACH_COMMA(what, ...)			\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_EMPTY_ARGS, SEP_COMMA, (), __VA_ARGS__)
#define FOR_EACH_JOIN(what, sep, ...)					\
  FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), what, CALL_WITH_EMPTY_ARGS, sep, (), __VA_ARGS__)
#define JOIN_ARGS(sep, ...)						\
  FOR_EACH_JOIN(PASS_THROUGH, sep, __VA_ARGS__)
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
#define PREFIX_EACH(prefix, ...)					\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(CONCATENATE, (prefix), __VA_ARGS__)
#define SUFFIX_EACH(suffix, ...)					\
  FOR_EACH_WITH_SUFFIX_ARGS_COMMA(CONCATENATE, (suffix), __VA_ARGS__)

#define EQUALS(a, b) (a == b)
#define VAR_IN_LIST(var, ...)						\
  (FOR_EACH_(FOR_EACH_NARG(__VA_ARGS__), EQUALS, CALL_WITH_PREFIX_ARGS,	\
	     SEP_OR, (var), __VA_ARGS__))
#define VAR_IN_PREFIXED_LIST(var, prefix, ...)			\
  VAR_IN_LIST(var, PREFIX_EACH(prefix, __VA_ARGS__))
#define IF_NOT_EMPTY(cond, body)				\
  FOR_EACH_WITH_PREFIX_ARGS_COMMA(FIRST_ARG, (body), cond)
