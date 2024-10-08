// Generic tools
#define UNUSED(arg) ((void)&(arg))
#define _Args(...) __VA_ARGS__
#define PASS_THROUGH(X) X
#define STRIP_PARENS(X) X
#define PACK_MACRO_(X) STRIP_PARENS( _Args X )
#define PACK_MACRO(...) PACK_MACRO_((__VA_ARGS__))
#define UNPACK_MACRO(...) __VA_ARGS__
#define UNPACK_PARENS(X) STRIP_PARENS( _Args X )
#define CALL_WITH_PREFIX_ARGS(what, args, x)	\
  what(UNPACK_PARENS(args), x)
  
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
  using __VA_ARGS__::state_updated;					\
  using __VA_ARGS__::error_prefix;					\
  using __VA_ARGS__::print_map;						\
  using __VA_ARGS__::print_vector;					\
  using __VA_ARGS__::string_map;					\
  using __VA_ARGS__::string_vector;					\
  INHERIT_METHOD_ENUM_MAP_NAMES(names, Names, Name, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_MAP(defaults, Defaults, Default, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_MAP(defaults_C3, Defaults_C3, Default_C3, __VA_ARGS__); \
  INHERIT_METHOD_ENUM_VECTOR(constant, Constant, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(calculated, Calculated, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(nonvector, Nonvector, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR_EDIT(skipped, Skipped, __VA_ARGS__);	\
  INHERIT_METHOD_ENUM_VECTOR(resetone, Resetone, __VA_ARGS__);		\
  INHERIT_METHOD_ENUM_VECTOR(initonce, Initonce, __VA_ARGS__)


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
  using __VA_ARGS__::print;			\
  using __VA_ARGS__::check;			\
  using __VA_ARGS__::update_values;		\
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
  cls::getDefault(cls::EnumClass::name, 0.0)
#define DECLARE_VALUE_SET_MEMBER(name)		\
  double name = 0.0
#define DECLARE_VALUE_SET_STATIC_MEMBER(name)	\
  static double name
#define DECLARE_VALUE_SET_STATIC_MEMBER_CONST(name)	\
  static const double name
#define DEFINE_VALUE_SET_MEMBER(name)					\
  name = 0.0;								\
  ValueSetClass::insertOrig(EnumClass::name, &name, "DEFINE_VALUE_SET_MEMBER: ")
#define DEFINE_VALUE_SET_STATIC_MEMBER(cls, name)			\
  double cls::name = DEFAULT_VALUE(cls, name)
#define DEFINE_VALUE_SET_STATIC_MEMBER_CONST(cls, name)			\
  const double cls::name = DEFAULT_VALUE(cls, name)

#define FOR_EACH_1(what, x, ...) what(x)
#define FOR_EACH_2(what, x, ...)\
  what(x);\
  FOR_EACH_1(what,  __VA_ARGS__);
#define FOR_EACH_3(what, x, ...)\
  what(x);\
  FOR_EACH_2(what, __VA_ARGS__);
#define FOR_EACH_4(what, x, ...)\
  what(x);\
  FOR_EACH_3(what,  __VA_ARGS__);
#define FOR_EACH_5(what, x, ...)\
  what(x);\
  FOR_EACH_4(what,  __VA_ARGS__);
#define FOR_EACH_6(what, x, ...)\
  what(x);\
  FOR_EACH_5(what,  __VA_ARGS__);
#define FOR_EACH_7(what, x, ...)\
  what(x);\
  FOR_EACH_6(what,  __VA_ARGS__);
#define FOR_EACH_8(what, x, ...)\
  what(x);\
  FOR_EACH_7(what,  __VA_ARGS__);
#define FOR_EACH_9(what, x, ...)\
  what(x);\
  FOR_EACH_8(what,  __VA_ARGS__);
#define FOR_EACH_10(what, x, ...)\
  what(x);\
  FOR_EACH_9(what,  __VA_ARGS__);
#define FOR_EACH_11(what, x, ...)\
  what(x);\
  FOR_EACH_10(what,  __VA_ARGS__);
#define FOR_EACH_12(what, x, ...)\
  what(x);\
  FOR_EACH_11(what,  __VA_ARGS__);
#define FOR_EACH_13(what, x, ...)\
  what(x);\
  FOR_EACH_12(what,  __VA_ARGS__);
#define FOR_EACH_14(what, x, ...)\
  what(x);\
  FOR_EACH_13(what,  __VA_ARGS__);
#define FOR_EACH_15(what, x, ...)\
  what(x);\
  FOR_EACH_14(what,  __VA_ARGS__);
#define FOR_EACH_16(what, x, ...)\
  what(x);\
  FOR_EACH_15(what,  __VA_ARGS__);
#define FOR_EACH_17(what, x, ...)\
  what(x);\
  FOR_EACH_16(what,  __VA_ARGS__);
#define FOR_EACH_18(what, x, ...)\
  what(x);\
  FOR_EACH_17(what,  __VA_ARGS__);
#define FOR_EACH_19(what, x, ...)\
  what(x);\
  FOR_EACH_18(what,  __VA_ARGS__);
#define FOR_EACH_20(what, x, ...)\
  what(x);\
  FOR_EACH_19(what,  __VA_ARGS__);
#define FOR_EACH_21(what, x, ...)\
  what(x);\
  FOR_EACH_20(what,  __VA_ARGS__);
#define FOR_EACH_22(what, x, ...)\
  what(x);\
  FOR_EACH_21(what,  __VA_ARGS__);
#define FOR_EACH_23(what, x, ...)\
  what(x);\
  FOR_EACH_22(what,  __VA_ARGS__);
#define FOR_EACH_24(what, x, ...)\
  what(x);\
  FOR_EACH_23(what,  __VA_ARGS__);
#define FOR_EACH_25(what, x, ...)\
  what(x);\
  FOR_EACH_24(what,  __VA_ARGS__);
#define FOR_EACH_26(what, x, ...)\
  what(x);\
  FOR_EACH_25(what,  __VA_ARGS__);
#define FOR_EACH_27(what, x, ...)\
  what(x);\
  FOR_EACH_26(what,  __VA_ARGS__);
#define FOR_EACH_28(what, x, ...)\
  what(x);\
  FOR_EACH_27(what,  __VA_ARGS__);
#define FOR_EACH_29(what, x, ...)\
  what(x);\
  FOR_EACH_28(what,  __VA_ARGS__);
#define FOR_EACH_30(what, x, ...)\
  what(x);\
  FOR_EACH_29(what,  __VA_ARGS__);
#define FOR_EACH_31(what, x, ...)\
  what(x);\
  FOR_EACH_30(what,  __VA_ARGS__);
#define FOR_EACH_32(what, x, ...)\
  what(x);\
  FOR_EACH_31(what,  __VA_ARGS__);
#define FOR_EACH_33(what, x, ...)\
  what(x);\
  FOR_EACH_32(what,  __VA_ARGS__);
#define FOR_EACH_34(what, x, ...)\
  what(x);\
  FOR_EACH_33(what,  __VA_ARGS__);
#define FOR_EACH_35(what, x, ...)\
  what(x);\
  FOR_EACH_34(what,  __VA_ARGS__);
#define FOR_EACH_36(what, x, ...)\
  what(x);\
  FOR_EACH_35(what,  __VA_ARGS__);
#define FOR_EACH_37(what, x, ...)\
  what(x);\
  FOR_EACH_36(what,  __VA_ARGS__);
#define FOR_EACH_38(what, x, ...)\
  what(x);\
  FOR_EACH_37(what,  __VA_ARGS__);
#define FOR_EACH_39(what, x, ...)\
  what(x);\
  FOR_EACH_38(what,  __VA_ARGS__);
#define FOR_EACH_40(what, x, ...)\
  what(x);\
  FOR_EACH_39(what,  __VA_ARGS__);
#define FOR_EACH_41(what, x, ...)\
  what(x);\
  FOR_EACH_40(what,  __VA_ARGS__);
#define FOR_EACH_42(what, x, ...)\
  what(x);\
  FOR_EACH_41(what,  __VA_ARGS__);
#define FOR_EACH_43(what, x, ...)\
  what(x);\
  FOR_EACH_42(what,  __VA_ARGS__);
#define FOR_EACH_44(what, x, ...)\
  what(x);\
  FOR_EACH_43(what,  __VA_ARGS__);
#define FOR_EACH_45(what, x, ...)\
  what(x);\
  FOR_EACH_44(what,  __VA_ARGS__);
#define FOR_EACH_46(what, x, ...)\
  what(x);\
  FOR_EACH_45(what,  __VA_ARGS__);
#define FOR_EACH_47(what, x, ...)\
  what(x);\
  FOR_EACH_46(what,  __VA_ARGS__);
#define FOR_EACH_48(what, x, ...)\
  what(x);\
  FOR_EACH_47(what,  __VA_ARGS__);
#define FOR_EACH_49(what, x, ...)\
  what(x);\
  FOR_EACH_48(what,  __VA_ARGS__);
#define FOR_EACH_50(what, x, ...)\
  what(x);\
  FOR_EACH_49(what,  __VA_ARGS__);
#define FOR_EACH_51(what, x, ...)\
  what(x);\
  FOR_EACH_50(what,  __VA_ARGS__);
#define FOR_EACH_52(what, x, ...)\
  what(x);\
  FOR_EACH_51(what,  __VA_ARGS__);
#define FOR_EACH_53(what, x, ...)\
  what(x);\
  FOR_EACH_52(what,  __VA_ARGS__);
#define FOR_EACH_54(what, x, ...)\
  what(x);\
  FOR_EACH_53(what,  __VA_ARGS__);
#define FOR_EACH_55(what, x, ...)\
  what(x);\
  FOR_EACH_54(what,  __VA_ARGS__);
#define FOR_EACH_56(what, x, ...)\
  what(x);\
  FOR_EACH_55(what,  __VA_ARGS__);
#define FOR_EACH_57(what, x, ...)\
  what(x);\
  FOR_EACH_56(what,  __VA_ARGS__);
#define FOR_EACH_58(what, x, ...)\
  what(x);\
  FOR_EACH_57(what,  __VA_ARGS__);
#define FOR_EACH_59(what, x, ...)\
  what(x);\
  FOR_EACH_58(what,  __VA_ARGS__);
#define FOR_EACH_60(what, x, ...)\
  what(x);\
  FOR_EACH_59(what,  __VA_ARGS__);
#define FOR_EACH_61(what, x, ...)\
  what(x);\
  FOR_EACH_60(what,  __VA_ARGS__);
#define FOR_EACH_62(what, x, ...)\
  what(x);\
  FOR_EACH_61(what,  __VA_ARGS__);
#define FOR_EACH_63(what, x, ...)\
  what(x);\
  FOR_EACH_62(what,  __VA_ARGS__);
#define FOR_EACH_64(what, x, ...)\
  what(x);\
  FOR_EACH_63(what,  __VA_ARGS__);
#define FOR_EACH_65(what, x, ...)\
  what(x);\
  FOR_EACH_64(what,  __VA_ARGS__);
#define FOR_EACH_66(what, x, ...)\
  what(x);\
  FOR_EACH_65(what,  __VA_ARGS__);
#define FOR_EACH_67(what, x, ...)\
  what(x);\
  FOR_EACH_66(what,  __VA_ARGS__);
#define FOR_EACH_68(what, x, ...)\
  what(x);\
  FOR_EACH_67(what,  __VA_ARGS__);
#define FOR_EACH_69(what, x, ...)\
  what(x);\
  FOR_EACH_68(what,  __VA_ARGS__);
#define FOR_EACH_70(what, x, ...)\
  what(x);\
  FOR_EACH_69(what,  __VA_ARGS__);
#define FOR_EACH_71(what, x, ...)\
  what(x);\
  FOR_EACH_70(what,  __VA_ARGS__);
#define FOR_EACH_72(what, x, ...)\
  what(x);\
  FOR_EACH_71(what,  __VA_ARGS__);
#define FOR_EACH_73(what, x, ...)\
  what(x);\
  FOR_EACH_72(what,  __VA_ARGS__);
#define FOR_EACH_74(what, x, ...)\
  what(x);\
  FOR_EACH_73(what,  __VA_ARGS__);
#define FOR_EACH_75(what, x, ...)\
  what(x);\
  FOR_EACH_74(what,  __VA_ARGS__);
#define FOR_EACH_76(what, x, ...)\
  what(x);\
  FOR_EACH_75(what,  __VA_ARGS__);
#define FOR_EACH_77(what, x, ...)\
  what(x);\
  FOR_EACH_76(what,  __VA_ARGS__);
#define FOR_EACH_78(what, x, ...)\
  what(x);\
  FOR_EACH_77(what,  __VA_ARGS__);
#define FOR_EACH_79(what, x, ...)\
  what(x);\
  FOR_EACH_78(what,  __VA_ARGS__);
#define FOR_EACH_80(what, x, ...)\
  what(x);\
  FOR_EACH_79(what,  __VA_ARGS__);
#define FOR_EACH_81(what, x, ...)\
  what(x);\
  FOR_EACH_80(what,  __VA_ARGS__);
#define FOR_EACH_82(what, x, ...)\
  what(x);\
  FOR_EACH_81(what,  __VA_ARGS__);
#define FOR_EACH_83(what, x, ...)\
  what(x);\
  FOR_EACH_82(what,  __VA_ARGS__);
#define FOR_EACH_84(what, x, ...)\
  what(x);\
  FOR_EACH_83(what,  __VA_ARGS__);
#define FOR_EACH_85(what, x, ...)\
  what(x);\
  FOR_EACH_84(what,  __VA_ARGS__);
#define FOR_EACH_86(what, x, ...)\
  what(x);\
  FOR_EACH_85(what,  __VA_ARGS__);
#define FOR_EACH_87(what, x, ...)\
  what(x);\
  FOR_EACH_86(what,  __VA_ARGS__);
#define FOR_EACH_88(what, x, ...)\
  what(x);\
  FOR_EACH_87(what,  __VA_ARGS__);
#define FOR_EACH_89(what, x, ...)\
  what(x);\
  FOR_EACH_88(what,  __VA_ARGS__);
#define FOR_EACH_90(what, x, ...)\
  what(x);\
  FOR_EACH_89(what,  __VA_ARGS__);
#define FOR_EACH_91(what, x, ...)\
  what(x);\
  FOR_EACH_90(what,  __VA_ARGS__);
#define FOR_EACH_92(what, x, ...)\
  what(x);\
  FOR_EACH_91(what,  __VA_ARGS__);
#define FOR_EACH_93(what, x, ...)\
  what(x);\
  FOR_EACH_92(what,  __VA_ARGS__);
#define FOR_EACH_94(what, x, ...)\
  what(x);\
  FOR_EACH_93(what,  __VA_ARGS__);
#define FOR_EACH_95(what, x, ...)\
  what(x);\
  FOR_EACH_94(what,  __VA_ARGS__);
#define FOR_EACH_96(what, x, ...)\
  what(x);\
  FOR_EACH_95(what,  __VA_ARGS__);
#define FOR_EACH_97(what, x, ...)\
  what(x);\
  FOR_EACH_96(what,  __VA_ARGS__);
#define FOR_EACH_98(what, x, ...)\
  what(x);\
  FOR_EACH_97(what,  __VA_ARGS__);
#define FOR_EACH_99(what, x, ...)\
  what(x);\
  FOR_EACH_98(what,  __VA_ARGS__);
#define FOR_EACH_100(what, x, ...)\
  what(x);\
  FOR_EACH_99(what,  __VA_ARGS__);
#define FOR_EACH_101(what, x, ...)\
  what(x);\
  FOR_EACH_100(what,  __VA_ARGS__);
#define FOR_EACH_102(what, x, ...)\
  what(x);\
  FOR_EACH_101(what,  __VA_ARGS__);
#define FOR_EACH_103(what, x, ...)\
  what(x);\
  FOR_EACH_102(what, __VA_ARGS__);
#define FOR_EACH_104(what, x, ...)\
  what(x);\
  FOR_EACH_103(what,  __VA_ARGS__);
#define FOR_EACH_105(what, x, ...)\
  what(x);\
  FOR_EACH_104(what,  __VA_ARGS__);
#define FOR_EACH_106(what, x, ...)\
  what(x);\
  FOR_EACH_105(what,  __VA_ARGS__);
#define FOR_EACH_107(what, x, ...)\
  what(x);\
  FOR_EACH_106(what,  __VA_ARGS__);
#define FOR_EACH_108(what, x, ...)\
  what(x);\
  FOR_EACH_107(what,  __VA_ARGS__);
#define FOR_EACH_109(what, x, ...)\
  what(x);\
  FOR_EACH_108(what,  __VA_ARGS__);
#define FOR_EACH_110(what, x, ...)\
  what(x);\
  FOR_EACH_109(what,  __VA_ARGS__);
#define FOR_EACH_111(what, x, ...)\
  what(x);\
  FOR_EACH_110(what,  __VA_ARGS__);
#define FOR_EACH_112(what, x, ...)\
  what(x);\
  FOR_EACH_111(what,  __VA_ARGS__);
#define FOR_EACH_113(what, x, ...)\
  what(x);\
  FOR_EACH_112(what,  __VA_ARGS__);
#define FOR_EACH_114(what, x, ...)\
  what(x);\
  FOR_EACH_113(what,  __VA_ARGS__);
#define FOR_EACH_115(what, x, ...)\
  what(x);\
  FOR_EACH_114(what,  __VA_ARGS__);
#define FOR_EACH_116(what, x, ...)\
  what(x);\
  FOR_EACH_115(what,  __VA_ARGS__);
#define FOR_EACH_117(what, x, ...)\
  what(x);\
  FOR_EACH_116(what,  __VA_ARGS__);
#define FOR_EACH_118(what, x, ...)\
  what(x);\
  FOR_EACH_117(what,  __VA_ARGS__);
#define FOR_EACH_119(what, x, ...)\
  what(x);\
  FOR_EACH_118(what,  __VA_ARGS__);
#define FOR_EACH_120(what, x, ...)\
  what(x);\
  FOR_EACH_119(what,  __VA_ARGS__);
#define FOR_EACH_121(what, x, ...)\
  what(x);\
  FOR_EACH_120(what,  __VA_ARGS__);
#define FOR_EACH_122(what, x, ...)\
  what(x);\
  FOR_EACH_121(what,  __VA_ARGS__);
#define FOR_EACH_123(what, x, ...)\
  what(x);\
  FOR_EACH_122(what,  __VA_ARGS__);
#define FOR_EACH_124(what, x, ...)\
  what(x);\
  FOR_EACH_123(what,  __VA_ARGS__);
#define FOR_EACH_125(what, x, ...)\
  what(x);\
  FOR_EACH_124(what,  __VA_ARGS__);
#define FOR_EACH_126(what, x, ...)\
  what(x);\
  FOR_EACH_125(what,  __VA_ARGS__);
#define FOR_EACH_127(what, x, ...)\
  what(x);\
  FOR_EACH_126(what,  __VA_ARGS__);
#define FOR_EACH_128(what, x, ...)\
  what(x);\
  FOR_EACH_127(what,  __VA_ARGS__);
#define FOR_EACH_129(what, x, ...)\
  what(x);\
  FOR_EACH_128(what,  __VA_ARGS__);
#define FOR_EACH_130(what, x, ...)\
  what(x);\
  FOR_EACH_129(what,  __VA_ARGS__);
#define FOR_EACH_131(what, x, ...)\
  what(x);\
  FOR_EACH_130(what,  __VA_ARGS__);
#define FOR_EACH_132(what, x, ...)\
  what(x);\
  FOR_EACH_131(what,  __VA_ARGS__);
#define FOR_EACH_133(what, x, ...)\
  what(x);\
  FOR_EACH_132(what,  __VA_ARGS__);
#define FOR_EACH_134(what, x, ...)\
  what(x);\
  FOR_EACH_133(what,  __VA_ARGS__);
#define FOR_EACH_135(what, x, ...)\
  what(x);\
  FOR_EACH_134(what,  __VA_ARGS__);
#define FOR_EACH_136(what, x, ...)\
  what(x);\
  FOR_EACH_135(what,  __VA_ARGS__);
#define FOR_EACH_137(what, x, ...)\
  what(x);\
  FOR_EACH_136(what,  __VA_ARGS__);
#define FOR_EACH_138(what, x, ...)\
  what(x);\
  FOR_EACH_137(what,  __VA_ARGS__);
#define FOR_EACH_139(what, x, ...)\
  what(x);\
  FOR_EACH_138(what,  __VA_ARGS__);
#define FOR_EACH_140(what, x, ...)\
  what(x);\
  FOR_EACH_139(what,  __VA_ARGS__);
#define FOR_EACH_141(what, x, ...)\
  what(x);\
  FOR_EACH_140(what,  __VA_ARGS__);
#define FOR_EACH_142(what, x, ...)\
  what(x);\
  FOR_EACH_141(what,  __VA_ARGS__);
#define FOR_EACH_143(what, x, ...)\
  what(x);\
  FOR_EACH_142(what,  __VA_ARGS__);
#define FOR_EACH_144(what, x, ...)\
  what(x);\
  FOR_EACH_143(what,  __VA_ARGS__);
#define FOR_EACH_145(what, x, ...)\
  what(x);\
  FOR_EACH_144(what,  __VA_ARGS__);
#define FOR_EACH_146(what, x, ...)\
  what(x);\
  FOR_EACH_145(what,  __VA_ARGS__);
#define FOR_EACH_147(what, x, ...)\
  what(x);\
  FOR_EACH_146(what,  __VA_ARGS__);
#define FOR_EACH_148(what, x, ...)\
  what(x);\
  FOR_EACH_147(what,  __VA_ARGS__);
#define FOR_EACH_149(what, x, ...)\
  what(x);\
  FOR_EACH_148(what,  __VA_ARGS__);
#define FOR_EACH_150(what, x, ...)\
  what(x);\
  FOR_EACH_149(what,  __VA_ARGS__);
#define FOR_EACH_151(what, x, ...)\
  what(x);\
  FOR_EACH_150(what,  __VA_ARGS__);
#define FOR_EACH_152(what, x, ...)\
  what(x);\
  FOR_EACH_151(what,  __VA_ARGS__);
#define FOR_EACH_153(what, x, ...)\
  what(x);\
  FOR_EACH_152(what,  __VA_ARGS__);
#define FOR_EACH_154(what, x, ...)\
  what(x);\
  FOR_EACH_153(what,  __VA_ARGS__);
#define FOR_EACH_155(what, x, ...)\
  what(x);\
  FOR_EACH_154(what,  __VA_ARGS__);
#define FOR_EACH_156(what, x, ...)\
  what(x);\
  FOR_EACH_155(what,  __VA_ARGS__);
#define FOR_EACH_157(what, x, ...)\
  what(x);\
  FOR_EACH_156(what,  __VA_ARGS__);
#define FOR_EACH_158(what, x, ...)\
  what(x);\
  FOR_EACH_157(what,  __VA_ARGS__);
#define FOR_EACH_159(what, x, ...)\
  what(x);\
  FOR_EACH_158(what,  __VA_ARGS__);
#define FOR_EACH_160(what, x, ...)\
  what(x);\
  FOR_EACH_159(what,  __VA_ARGS__);
#define FOR_EACH_161(what, x, ...)\
  what(x);\
  FOR_EACH_160(what,  __VA_ARGS__);
#define FOR_EACH_162(what, x, ...)\
  what(x);\
  FOR_EACH_161(what,  __VA_ARGS__);
#define FOR_EACH_163(what, x, ...)\
  what(x);\
  FOR_EACH_162(what,  __VA_ARGS__);
#define FOR_EACH_164(what, x, ...)\
  what(x);\
  FOR_EACH_163(what,  __VA_ARGS__);
#define FOR_EACH_165(what, x, ...)\
  what(x);\
  FOR_EACH_164(what,  __VA_ARGS__);
#define FOR_EACH_166(what, x, ...)\
  what(x);\
  FOR_EACH_165(what,  __VA_ARGS__);
#define FOR_EACH_167(what, x, ...)\
  what(x);\
  FOR_EACH_166(what,  __VA_ARGS__);
#define FOR_EACH_168(what, x, ...)\
  what(x);\
  FOR_EACH_167(what,  __VA_ARGS__);
#define FOR_EACH_169(what, x, ...)\
  what(x);\
  FOR_EACH_168(what,  __VA_ARGS__);
#define FOR_EACH_170(what, x, ...)\
  what(x);\
  FOR_EACH_169(what,  __VA_ARGS__);
#define FOR_EACH_171(what, x, ...)\
  what(x);\
  FOR_EACH_170(what,  __VA_ARGS__);
#define FOR_EACH_172(what, x, ...)\
  what(x);\
  FOR_EACH_171(what,  __VA_ARGS__);
#define FOR_EACH_173(what, x, ...)\
  what(x);\
  FOR_EACH_172(what,  __VA_ARGS__);
#define FOR_EACH_174(what, x, ...)\
  what(x);\
  FOR_EACH_173(what,  __VA_ARGS__);
#define FOR_EACH_175(what, x, ...)\
  what(x);\
  FOR_EACH_174(what,  __VA_ARGS__);
#define FOR_EACH_176(what, x, ...)\
  what(x);\
  FOR_EACH_175(what,  __VA_ARGS__);
#define FOR_EACH_177(what, x, ...)\
  what(x);\
  FOR_EACH_176(what,  __VA_ARGS__);
#define FOR_EACH_178(what, x, ...)\
  what(x);\
  FOR_EACH_177(what,  __VA_ARGS__);
#define FOR_EACH_179(what, x, ...)\
  what(x);\
  FOR_EACH_178(what,  __VA_ARGS__);
#define FOR_EACH_180(what, x, ...)\
  what(x);\
  FOR_EACH_179(what,  __VA_ARGS__);
#define FOR_EACH_181(what, x, ...)\
  what(x);\
  FOR_EACH_180(what,  __VA_ARGS__);
#define FOR_EACH_182(what, x, ...)\
  what(x);\
  FOR_EACH_181(what,  __VA_ARGS__);
#define FOR_EACH_183(what, x, ...)\
  what(x);\
  FOR_EACH_182(what,  __VA_ARGS__);
#define FOR_EACH_184(what, x, ...)\
  what(x);\
  FOR_EACH_183(what,  __VA_ARGS__);
#define FOR_EACH_185(what, x, ...)\
  what(x);\
  FOR_EACH_184(what,  __VA_ARGS__);
#define FOR_EACH_186(what, x, ...)\
  what(x);\
  FOR_EACH_185(what,  __VA_ARGS__);
#define FOR_EACH_187(what, x, ...)\
  what(x);\
  FOR_EACH_186(what,  __VA_ARGS__);
#define FOR_EACH_188(what, x, ...)\
  what(x);\
  FOR_EACH_187(what,  __VA_ARGS__);
#define FOR_EACH_189(what, x, ...)\
  what(x);\
  FOR_EACH_188(what,  __VA_ARGS__);
#define FOR_EACH_190(what, x, ...)\
  what(x);\
  FOR_EACH_189(what,  __VA_ARGS__);
#define FOR_EACH_191(what, x, ...)\
  what(x);\
  FOR_EACH_190(what,  __VA_ARGS__);
#define FOR_EACH_192(what, x, ...)\
  what(x);\
  FOR_EACH_191(what,  __VA_ARGS__);
#define FOR_EACH_193(what, x, ...)\
  what(x);\
  FOR_EACH_192(what,  __VA_ARGS__);
#define FOR_EACH_194(what, x, ...)\
  what(x);\
  FOR_EACH_193(what,  __VA_ARGS__);
#define FOR_EACH_195(what, x, ...)\
  what(x);\
  FOR_EACH_194(what,  __VA_ARGS__);
#define FOR_EACH_196(what, x, ...)\
  what(x);\
  FOR_EACH_195(what,  __VA_ARGS__);
#define FOR_EACH_197(what, x, ...)\
  what(x);\
  FOR_EACH_196(what,  __VA_ARGS__);
#define FOR_EACH_198(what, x, ...)\
  what(x);\
  FOR_EACH_197(what,  __VA_ARGS__);
#define FOR_EACH_199(what, x, ...)\
  what(x);\
  FOR_EACH_198(what,  __VA_ARGS__);
#define FOR_EACH_200(what, x, ...)\
  what(x);\
  FOR_EACH_199(what,  __VA_ARGS__);

#define FOR_EACH_WITH_ARGS_1(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x)
#define FOR_EACH_WITH_ARGS_2(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),	\
  FOR_EACH_WITH_ARGS_1(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_3(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_2(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_4(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_3(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_5(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_4(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_6(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_5(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_7(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_6(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_8(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_7(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_9(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_8(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_10(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_9(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_11(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_10(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_12(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_11(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_13(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_12(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_14(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_13(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_15(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_14(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_16(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_15(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_17(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_16(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_18(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_17(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_19(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_18(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_20(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_19(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_21(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_20(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_22(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_21(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_23(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_22(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_24(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_23(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_25(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_24(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_26(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_25(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_27(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_26(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_28(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_27(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_29(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_28(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_30(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_29(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_31(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_30(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_32(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_31(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_33(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_32(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_34(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_33(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_35(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_34(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_36(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_35(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_37(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_36(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_38(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_37(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_39(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_38(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_40(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_39(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_41(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_40(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_42(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_41(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_43(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_42(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_44(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_43(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_45(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_44(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_46(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_45(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_47(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_46(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_48(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_47(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_49(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_48(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_50(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_49(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_51(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_50(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_52(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_51(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_53(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_52(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_54(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_53(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_55(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_54(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_56(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_55(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_57(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_56(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_58(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_57(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_59(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_58(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_60(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_59(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_61(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_60(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_62(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_61(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_63(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_62(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_64(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_63(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_65(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_64(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_66(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_65(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_67(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_66(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_68(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_67(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_69(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_68(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_70(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_69(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_71(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_70(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_72(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_71(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_73(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_72(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_74(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_73(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_75(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_74(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_76(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_75(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_77(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_76(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_78(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_77(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_79(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_78(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_80(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_79(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_81(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_80(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_82(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_81(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_83(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_82(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_84(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_83(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_85(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_84(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_86(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_85(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_87(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_86(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_88(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_87(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_89(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_88(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_90(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_89(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_91(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_90(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_92(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_91(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_93(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_92(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_94(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_93(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_95(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_94(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_96(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_95(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_97(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_96(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_98(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_97(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_99(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_98(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_100(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_99(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_101(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_100(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_102(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),	\
  FOR_EACH_WITH_ARGS_101(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_103(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_102(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_104(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_103(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_105(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_104(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_106(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_105(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_107(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_106(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_108(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_107(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_109(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_108(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_110(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_109(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_111(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_110(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_112(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_111(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_113(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_112(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_114(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_113(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_115(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_114(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_116(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_115(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_117(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_116(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_118(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_117(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_119(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_118(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_120(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_119(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_121(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_120(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_122(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_121(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_123(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_122(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_124(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_123(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_125(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_124(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_126(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_125(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_127(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_126(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_128(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_127(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_129(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_128(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_130(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_129(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_131(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_130(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_132(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_131(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_133(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_132(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_134(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_133(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_135(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_134(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_136(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_135(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_137(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_136(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_138(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_137(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_139(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_138(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_140(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_139(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_141(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_140(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_142(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_141(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_143(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_142(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_144(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_143(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_145(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_144(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_146(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_145(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_147(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_146(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_148(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_147(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_149(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_148(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_150(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_149(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_151(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_150(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_152(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_151(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_153(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_152(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_154(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_153(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_155(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_154(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_156(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_155(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_157(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_156(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_158(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_157(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_159(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_158(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_160(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_159(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_161(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_160(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_162(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_161(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_163(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_162(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_164(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_163(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_165(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_164(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_166(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_165(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_167(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_166(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_168(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_167(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_169(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_168(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_170(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_169(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_171(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_170(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_172(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_171(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_173(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_172(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_174(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_173(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_175(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_174(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_176(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_175(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_177(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_176(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_178(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_177(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_179(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_178(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_180(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_179(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_181(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_180(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_182(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_181(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_183(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_182(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_184(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_183(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_185(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_184(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_186(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_185(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_187(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_186(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_188(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_187(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_189(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_188(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_190(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_189(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_191(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_190(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_192(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_191(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_193(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_192(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_194(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_193(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_195(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_194(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_196(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_195(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_197(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_196(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_198(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_197(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_199(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_198(what, args, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_200(what, args, x, ...)\
  CALL_WITH_PREFIX_ARGS(what, args, x),\
  FOR_EACH_WITH_ARGS_199(what, args, __VA_ARGS__)

#define FOR_EACH_NARG(...) FOR_EACH_NARG_(__VA_ARGS__, FOR_EACH_RSEQ_N())
#define FOR_EACH_NARG_(...) FOR_EACH_ARG_N(__VA_ARGS__)
#define FOR_EACH_ARG_N(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, _65, _66, _67, _68, _69, _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _80, _81, _82, _83, _84, _85, _86, _87, _88, _89, _90, _91, _92, _93, _94, _95, _96, _97, _98, _99, _100, _101, _102, _103, _104, _105, _106, _107, _108, _109, _110, _111, _112, _113, _114, _115, _116, _117, _118, _119, _120, _121, _122, _123, _124, _125, _126, _127, _128, _129, _130, _131, _132, _133, _134, _135, _136, _137, _138, _139, _140, _141, _142, _143, _144, _145, _146, _147, _148, _149, _150, _151, _152, _153, _154, _155, _156, _157, _158, _159, _160, _161, _162, _163, _164, _165, _166, _167, _168, _169, _170, _171, _172, _173, _174, _175, _176, _177, _178, _179, _180, _181, _182, _183, _184, _185, _186, _187, _188, _189, _190, _191, _192, _193, _194, _195, _196, _197, _198, _199, _200, N, ...) N
#define FOR_EACH_RSEQ_N() 200, 199, 198, 197, 196, 195, 194, 193, 192, 191, 190, 189, 188, 187, 186, 185, 184, 183, 182, 181, 180, 179, 178, 177, 176, 175, 174, 173, 172, 171, 170, 169, 168, 167, 166, 165, 164, 163, 162, 161, 160, 159, 158, 157, 156, 155, 154, 153, 152, 151, 140, 149, 148, 147, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 120, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 108, 107, 106, 105, 104, 103, 102, 101, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 40, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0

#define FOR_EACH_(N, what, x, ...) CONCATENATE(FOR_EACH_, N)(what, x, __VA_ARGS__)
#define FOR_EACH(what, x, ...) FOR_EACH_(FOR_EACH_NARG(x, __VA_ARGS__), what, x, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_COMMA_(N, what, args, x, ...) CONCATENATE(FOR_EACH_WITH_ARGS_, N)(what, args, x, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_COMMA(what, args, x, ...) FOR_EACH_WITH_ARGS_COMMA_(FOR_EACH_NARG(x, __VA_ARGS__), what, args, x, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS_(...)	\
  FOR_EACH(STRIP_PARENS, __VA_ARGS__)
#define FOR_EACH_WITH_ARGS(what, args, x, ...)	\
  FOR_EACH_WITH_ARGS_(FOR_EACH_WITH_ARGS_COMMA(what, args, x, __VA_ARGS__))
#define PREFIX_EACH(prefix, ...)		\
  FOR_EACH_WITH_ARGS_COMMA(CONCATENATE, (prefix), __VA_ARGS__)
