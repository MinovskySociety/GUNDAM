//#ifndef _TYPE_CHECK_HPP
//#define _TYPE_CHECK_HPP
//
//#include <assert.h>
//#include <type_traits>
//
////char check_Defined_IDType(...);
////
////template <typename T>
////void* check_Defined_IDType(T const& t, decltype(&(T::DefinedIDType)) p = 0);
////
//////template <typename T>
//////struct has_Defined_IDType
//////    :public std::integral_constant<
//////            bool,
//////           (sizeof(check_Defined_IDType(*static_cast(0))) == sizeof(void*))>{
//////};
////
////template <typename T>
////struct has_Defined_IDType
////{
////    static const bool value =
////        sizeof(check_Defined_IDType(*static_cast(0))) != sizeof(char);
////};
//
////struct Hello
////{
////    int helloworld() { return 0; }
////};
////
////struct Generic {};
////
////// SFINAE test
////template <typename T>
////class has_helloworld
////{
////    typedef char one;
////    struct two { char x[2]; };
////
////    template <typename C> static one test( std::typeof(&C::helloworld) ) ;
////    template <typename C> static two test(...);
////
////public:
////    enum { value = sizeof(test<T>(0)) == sizeof(char) };
////};
//
//template<class T>
//auto serialize_imp(std::ostream& os, T const& obj, int)
//    -> decltype(os << obj, void())
//{
//  os << obj;
//}
//
//template<class T>
//auto serialize_imp(std::ostream& os, T const& obj, long)
//    -> decltype(obj.stream(os), void())
//{
//  obj.stream(os);
//}
//
//template<class T>
//auto serialize(std::ostream& os, T const& obj)
//    -> decltype(serialize_imp(os, obj, 0), void())
//{
//  serialize_imp(os, obj, 0);
//}
//
//#endif // _TYPE_CHECK_HPP
