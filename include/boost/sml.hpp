//
// Copyright (c) 2016 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once
#if (__cplusplus < 201305L && _MSC_VER < 1900)
#error "Boost.SML requires C++14 support (Clang-3.4+, GCC-5.1+, MSVC-2015+)"
#else
#define BOOST_SML_VERSION 1'1'0
#define BOOST_SML_NAMESPACE_BEGIN \
  namespace boost {               \
  namespace sml {                 \
  inline namespace v1_1_0 {
#define BOOST_SML_NAMESPACE_END \
  }                             \
  }                             \
  }
#if defined(__clang__)
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...)
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-string-literal-operator-template"
#pragma clang diagnostic ignored "-Wzero-length-array"
#elif defined(__GNUC__)
#define __has_builtin(...) 0
#define __BOOST_SML_UNUSED __attribute__((unused))
#define __BOOST_SML_VT_INIT \
  {}
#define __BOOST_SML_ZERO_SIZE_ARRAY(...) __VA_ARGS__ _[0]
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#define __BOOST_SML_TEMPLATE_KEYWORD template
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#elif defined(_MSC_VER)
#define __has_builtin(...) 0
#define __BOOST_SML_UNUSED
#define __BOOST_SML_VT_INIT
#define __BOOST_SML_ZERO_SIZE_ARRAY(...)
#define __BOOST_SML_ZERO_SIZE_ARRAY_CREATE(...) __VA_ARGS__ ? __VA_ARGS__ : 1
#define __BOOST_SML_TEMPLATE_KEYWORD
#pragma warning(disable : 4503)
#pragma warning(disable : 4200)
#endif
BOOST_SML_NAMESPACE_BEGIN
#define __BOOST_SML_REQUIRES(...) typename aux::enable_if<__VA_ARGS__, int>::type = 0
namespace aux {
using byte = unsigned char;
struct none_type {};
template <class...>
struct type {};
template <class T, T>
struct non_type {};
template <class, class>
struct pair {};
template <class...>
struct type_list {
  using type = type_list;
};
template <bool...>
struct bool_list {
  using type = bool_list;
};
template <class... Ts>
struct inherit : Ts... {
  using type = inherit;
};
template <class T>
T &&declval();
template <class T, T V>
struct integral_constant {
  using type = integral_constant;
  static constexpr T value = V;
};
using true_type = integral_constant<bool, true>;
using false_type = integral_constant<bool, false>;
template <class...>
using void_t = void;
template <class...>
struct always : aux::true_type {};
template <class...>
struct never : aux::false_type {};
template <bool B, class T, class F>
struct conditional {
  using type = T;
};
template <class T, class F>
struct conditional<false, T, F> {
  using type = F;
};
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
template <bool B, class T = void>
struct enable_if {};
template <class T>
struct enable_if<true, T> {
  using type = T;
};
template <bool B, class T = void>
using enable_if_t = typename enable_if<B, T>::type;
template <class, class>
struct is_same : false_type {};
template <class T>
struct is_same<T, T> : true_type {};
template <class T, class U>
#if defined(_MSC_VER)
struct is_base_of : integral_constant<bool, __is_base_of(T, U)> {
};
#else
using is_base_of = integral_constant<bool, __is_base_of(T, U)>;
#endif
template <class T>
struct remove_reference {
  using type = T;
};
template <class T>
struct remove_reference<T &> {
  using type = T;
};
template <class T>
struct remove_reference<const T &> {
  using type = T;
};
template <class T>
struct remove_reference<T &&> {
  using type = T;
};
template <class T>
using remove_reference_t = typename remove_reference<T>::type;
template <class>
struct function_traits;
template <class R, class... TArgs>
struct function_traits<R (*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class... TArgs>
struct function_traits<R(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...)> {
  using args = type_list<TArgs...>;
};
template <class R, class T, class... TArgs>
struct function_traits<R (T::*)(TArgs...) const> {
  using args = type_list<TArgs...>;
};
template <class T>
using function_traits_t = typename function_traits<T>::args;
}
namespace aux {
using swallow = int[];
template <int...>
struct index_sequence {
  using type = index_sequence;
};
#if __has_builtin(__make_integer_seq)
template <class T, T...>
struct integer_sequence;
template <int... Ns>
struct integer_sequence<int, Ns...> {
  using type = index_sequence<Ns...>;
};
template <int N>
struct make_index_sequence_impl {
  using type = typename __make_integer_seq<integer_sequence, int, N>::type;
};
#else
template <class, class>
struct concat;
template <int... I1, int... I2>
struct concat<index_sequence<I1...>, index_sequence<I2...>> : index_sequence<I1..., (sizeof...(I1) + I2)...> {};
template <int N>
struct make_index_sequence_impl
    : concat<typename make_index_sequence_impl<N / 2>::type, typename make_index_sequence_impl<N - N / 2>::type>::type {};
template <>
struct make_index_sequence_impl<0> : index_sequence<> {};
template <>
struct make_index_sequence_impl<1> : index_sequence<0> {};
#endif
template <int N>
using make_index_sequence = typename make_index_sequence_impl<N>::type;
template <class...>
struct join {
  using type = type_list<>;
};
template <class T>
struct join<T> {
  using type = T;
};
template <class... Ts>
struct join<type_list<Ts...>> : type_list<Ts...> {};
template <class... T1s, class... T2s>
struct join<type_list<T1s...>, type_list<T2s...>> : type_list<T1s..., T2s...> {};
template <class... T1s, class... T2s, class... T3s>
struct join<type_list<T1s...>, type_list<T2s...>, type_list<T3s...>> : type_list<T1s..., T2s..., T3s...> {};
template <class... T1s, class... T2s, class... Ts>
struct join<type_list<T1s...>, type_list<T2s...>, Ts...> : join<type_list<T1s..., T2s...>, Ts...> {};
template <class... Ts, class... T1s, class... T2s, class... T3s, class... T4s, class... T5s, class... T6s, class... T7s,
          class... T8s, class... T9s, class... T10s, class... T11s, class... T12s, class... T13s, class... T14s, class... T15s,
          class... T16s, class... Us>
struct join<type_list<Ts...>, type_list<T1s...>, type_list<T2s...>, type_list<T3s...>, type_list<T4s...>, type_list<T5s...>,
            type_list<T6s...>, type_list<T7s...>, type_list<T8s...>, type_list<T9s...>, type_list<T10s...>, type_list<T11s...>,
            type_list<T12s...>, type_list<T13s...>, type_list<T14s...>, type_list<T15s...>, type_list<T16s...>, Us...>
    : join<type_list<Ts..., T1s..., T2s..., T3s..., T4s..., T5s..., T6s..., T7s..., T8s..., T9s..., T10s..., T11s..., T12s...,
                     T13s..., T14s..., T15s..., T16s...>,
           Us...> {};
template <class... TArgs>
using join_t = typename join<TArgs...>::type;
template <class, class...>
struct unique_impl;
template <class T1, class T2, class... Rs, class... Ts>
struct unique_impl<type<T1, Rs...>, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, unique_impl<type<inherit<T1>, Rs...>, Ts...>,
                    unique_impl<type<inherit<T1, type<T2>>, Rs..., T2>, Ts...>> {};
template <class T1, class... Rs>
struct unique_impl<type<T1, Rs...>> : type_list<Rs...> {};
template <class... Ts>
struct unique : unique_impl<type<none_type>, Ts...> {};
template <class T>
struct unique<T> : type_list<T> {};
template <class... Ts>
using unique_t = typename unique<Ts...>::type;
template <class, class...>
struct is_unique;
template <class T>
struct is_unique<T> : true_type {};
template <class T1, class T2, class... Ts>
struct is_unique<T1, T2, Ts...>
    : conditional_t<is_base_of<type<T2>, T1>::value, false_type, is_unique<inherit<T1, type<T2>>, Ts...>> {};
template <class... Ts>
using is_unique_t = is_unique<none_type, Ts...>;
template <template <class...> class, class>
struct apply;
template <template <class...> class T, template <class...> class U, class... Ts>
struct apply<T, U<Ts...>> {
  using type = T<Ts...>;
};
template <template <class...> class T, class D>
using apply_t = typename apply<T, D>::type;
template <int, class T>
struct tuple_type {
  explicit tuple_type(const T &object) : value(object) {}
  T value;
};
template <class, class...>
struct tuple_impl;
template <int... Ns, class... Ts>
struct tuple_impl<index_sequence<Ns...>, Ts...> : tuple_type<Ns, Ts>... {
  explicit tuple_impl(Ts... ts) : tuple_type<Ns, Ts>(ts)... {}
};
template <>
struct tuple_impl<index_sequence<0>> {
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class... Ts>
using tuple = tuple_impl<make_index_sequence<sizeof...(Ts)>, Ts...>;
template <int N, class T>
auto &get_by_id(tuple_type<N, T> *object) {
  return static_cast<tuple_type<N, T> &>(*object).value;
}
struct init {};
template <class T>
struct pool_type {
  explicit pool_type(const T &object) : value(object) {}
  template <class TObject>
  pool_type(const init &i, const TObject &object) : value(i, object) {}
  T value;
};
template <class T>
T &try_get(...) {
  static_assert(never<T>::value, "Type T has to be passed via constructor!");
}
template <class T>
static T &try_get(pool_type<T> *object) {
  return static_cast<pool_type<T> &>(*object).value;
}
template <class T, class TPool>
T &get(TPool &p) {
  return static_cast<pool_type<T> &>(p).value;
}
template <class T, class TPool>
const T &cget(TPool &p) {
  return static_cast<const pool_type<T> &>(p).value;
}
template <class... Ts>
struct pool : pool_type<Ts>... {
  using boost_di_inject__ = type_list<Ts...>;
  explicit pool(Ts... ts) : pool_type<Ts>(ts)... {}
  template <class... TArgs>
  pool(init &&, pool<TArgs...> &&p) : pool_type<Ts>(try_get<Ts>(&p))... {}
  template <class... TArgs>
  pool(const pool<TArgs...> &p) : pool_type<Ts>(init{}, &p)... {}
};
template <>
struct pool<> {
  using boost_di_inject__ = type_list<>;
  explicit pool(...) {}
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <int, class>
struct type_id_type {};
template <class, class...>
struct type_id_impl;
template <int... Ns, class... Ts>
struct type_id_impl<index_sequence<Ns...>, Ts...> : type_id_type<Ns, Ts>... {};
template <class... Ts>
struct type_id : type_id_impl<make_index_sequence<sizeof...(Ts)>, Ts...> {};
template <class T, int, int N>
constexpr auto get_id_impl(type_id_type<N, T> *) {
  return N;
}
template <class T, int D>
constexpr auto get_id_impl(...) {
  return D;
}
template <class TIds, int D, class T>
constexpr auto get_id() {
  return get_id_impl<T, D>((TIds *)0);
}
template <template <class...> class, class T>
struct is : false_type {};
template <template <class...> class T, class... Ts>
struct is<T, T<Ts...>> : true_type {};
template <class>
struct size;
template <template <class...> class T, class... Ts>
struct size<T<Ts...>> {
  static constexpr auto value = sizeof...(Ts);
};
#if defined(_MSC_VER)
constexpr auto max_impl() { return 0; }
constexpr auto max_impl(int r) { return r; }
constexpr auto max_impl(int r, int i) { return r > i ? r : i; }
constexpr auto max_impl(int r, int i, int ints...) { return i > r ? max_impl(i, ints) : max_impl(r, ints); }
template <int... Ts>
constexpr auto max() {
  return max_impl(Ts...);
}
#else
template <int... Ts>
constexpr auto max() {
  auto max = 0;
  int _[]{0, (Ts > max ? max = Ts : max)...};
  (void)_;
  return max;
}
#endif
template <class TExpr, class = void>
struct zero_wrapper : TExpr {
  explicit zero_wrapper(const TExpr &expr) : TExpr(expr) {}
};
template <class, class>
struct zero_wrapper_impl;
template <class TExpr, class... TArgs>
struct zero_wrapper_impl<TExpr, type_list<TArgs...>> {
  auto operator()(TArgs... args) const { return reinterpret_cast<const TExpr &>(*this)(args...); }
  __BOOST_SML_ZERO_SIZE_ARRAY(byte);
};
template <class TExpr>
struct zero_wrapper<TExpr, void_t<decltype(+declval<TExpr>())>>
    : zero_wrapper_impl<TExpr, function_traits_t<decltype(&TExpr::operator())>> {
  zero_wrapper(...) {}
};
template <class T, T...>
struct string;
template <char... Chrs>
struct string<char, Chrs...> {
  using type = string;
  static auto c_str() {
    static constexpr char str[] = {Chrs..., 0};
    return str;
  }
};
template <class T>
struct string<T> {
  using type = T;
  static auto c_str() { return c_str_impl((T *)0); }
  template <class U>
  static decltype(U::c_str()) c_str_impl(U *) {
    return U::c_str();
  }
  static auto c_str_impl(...) { return __PRETTY_FUNCTION__; }
};
}
namespace back {
struct no_policy {
  using type = no_policy;
  template <class>
  using rebind = no_policy;
  template <class...>
  using defer = no_policy;
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
struct thread_safety_policy__ {};
struct defer_queue_policy__ {};
struct logger_policy__ {};
struct testing_policy__ {};
template <class, class TDeps, class TEvent>
void log_process_event(const aux::type<no_policy> &, TDeps &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TEvent>
void log_process_event(const aux::type<TLogger> &, TDeps &deps, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_process_event<SM>(event);
}
template <class, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<no_policy> &, TDeps &, const TSrcState &, const TDstState &) {}
template <class SM, class TLogger, class TDeps, class TSrcState, class TDstState>
void log_state_change(const aux::type<TLogger> &, TDeps &deps, const TSrcState &src, const TDstState &dst) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_state_change<SM>(src, dst);
}
template <class, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<no_policy> &, TDeps &, const TAction &, const TEvent &) {}
template <class SM, class TLogger, class TDeps, class TAction, class TEvent>
void log_action(const aux::type<TLogger> &, TDeps &deps, const TAction &action, const TEvent &event) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_action<SM>(action, event);
}
template <class, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<no_policy> &, TDeps &, const TGuard &, const TEvent &, bool) {}
template <class SM, class TLogger, class TDeps, class TGuard, class TEvent>
void log_guard(const aux::type<TLogger> &, TDeps &deps, const TGuard &guard, const TEvent &event, bool result) {
  return static_cast<aux::pool_type<TLogger &> &>(deps).value.template log_guard<SM>(guard, event, result);
}
}
namespace back {
struct _ {};
struct initial {};
struct unexpected {};
struct entry_exit {};
struct internal_event {
  static auto c_str() { return "internal_event"; }
};
struct anonymous : internal_event {
  static auto c_str() { return "anonymous"; }
};
template <class T, class TEvent = T>
struct on_entry : internal_event, entry_exit {
  static auto c_str() { return "on_entry"; }
  explicit on_entry(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class T, class TEvent = T>
struct on_exit : internal_event, entry_exit {
  static auto c_str() { return "on_exit"; }
  explicit on_exit(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class TException>
struct exception : internal_event {
  using type = TException;
  explicit exception(const TException &exception = {}) : exception_(exception) {}
  const TException &exception_;
};
template <class T, class TEvent = T>
struct unexpected_event : internal_event, unexpected {
  explicit unexpected_event(const TEvent &event = {}) : event_(event) {}
  const TEvent &event_;
};
template <class TEvent>
struct event_type {
  using event_t = TEvent;
  using generic_t = TEvent;
  using mapped_t = void;
};
template <class TEvent>
struct event_type<exception<TEvent>> {
  using event_t = TEvent;
  using generic_t = exception<TEvent>;
  using mapped_t = void;
};
template <class TEvent, class T>
struct event_type<unexpected_event<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = unexpected_event<T>;
  using mapped_t = void;
};
template <class TEvent, class T>
struct event_type<on_entry<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_entry<T>;
  using mapped_t = on_entry<T, TEvent>;
};
template <class TEvent, class T>
struct event_type<on_exit<T, TEvent>> {
  using event_t = TEvent;
  using generic_t = on_exit<T>;
  using mapped_t = on_exit<T, TEvent>;
};
template <class TEvent>
using get_event_t = typename event_type<TEvent>::event_t;
template <class TEvent>
using get_generic_t = typename event_type<TEvent>::generic_t;
template <class TEvent>
using get_mapped_t = typename event_type<TEvent>::mapped_t;
}
namespace back {
template <class>
class sm;
template <class>
struct sm_impl;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class T, class... Ts>
struct transitions<T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    if (aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{})) {
      return true;
    }
    return transitions<Ts...>::execute(event, sm, deps, subs, current_state);
  }
};
template <class T>
struct transitions<T> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const on_exit<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                           typename SM::state_t &current_state) {
    aux::get<T>(sm.transitions_).execute(event, sm, deps, subs, current_state, typename SM::has_entry_exits{});
    return false;
  }
};
template <>
struct transitions<aux::true_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    sm.process_internal_event(unexpected_event<TEvent>{event}, deps, subs, current_state);
    return false;
  }
};
template <>
struct transitions<aux::false_type> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &) {
    return false;
  }
};
template <class TSM, class T, class... Ts>
struct transitions_sub<sm<TSM>, T, Ts...> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    return execute_impl(event, sm, deps, subs, current_state);
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state) {
    const auto handled = aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
    return handled ? handled : transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
  }
  template <class _, class TEvent, class SM, class TDeps, class TSubs>
  static bool execute_impl(const back::on_entry<_, TEvent> &event, SM &sm, TDeps &deps, TSubs &subs,
                           typename SM::state_t &current_state) {
    transitions<T, Ts...>::execute(event, sm, deps, subs, current_state);
    aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
    return true;
  }
};
template <class TSM>
struct transitions_sub<sm<TSM>> {
  template <class TEvent, class SM, class TDeps, class TSubs>
  static bool execute(const TEvent &event, SM &, TDeps &deps, TSubs &subs, typename SM::state_t &) {
    return execute_impl(event, deps, subs);
  }
  template <class TEvent, class TDeps, class TSubs>
  static bool execute_impl(const TEvent &event, TDeps &deps, TSubs &subs) {
    aux::get<sm_impl<TSM>>(subs).template process_event<TEvent>(event, deps, subs);
    return true;
  }
  template <class _, class TEvent, class TDeps, class TSubs>
  static bool execute_impl(const on_entry<_, TEvent> &event, TDeps &deps, TSubs &subs) {
    return aux::get<sm_impl<TSM>>(subs).process_event(event, deps, subs);
  }
};
}
namespace back {
template <class>
class sm;
template <class>
struct state;
template <class>
struct event;
template <class...>
struct transitions;
template <class...>
struct transitions_sub;
template <class, class>
struct state_mappings;
template <class S, class... Ts>
struct state_mappings<S, aux::type_list<Ts...>> {
  using element_type = state<S>;
  using types = aux::type_list<Ts...>;
};
template <class, class>
struct event_mappings;
template <class E, class... Ts>
struct event_mappings<E, aux::inherit<Ts...>> {
  using element_type = event<E>;
  using types = aux::type_list<Ts...>;
};
template <class...>
struct unique_mappings;
template <class, class...>
struct unique_mappings_impl;
template <class... Ts>
using unique_mappings_t = typename unique_mappings<Ts...>::type;
template <class, class, class, class R>
struct get_mapping : aux::type_list<R> {};
template <class E, class T, class R>
struct get_mapping<event<E>, event<E>, T, R>
    : aux::type_list<event_mappings<E, aux::apply_t<unique_mappings_t, aux::join_t<typename R::types, typename T::types>>>> {};
template <class S, class T, class R>
struct get_mapping<state<S>, state<S>, T, R>
    : aux::type_list<state_mappings<S, aux::join_t<typename R::types, typename T::types>>> {};
template <class T, class... Ts>
struct extend_mapping : aux::join_t<typename get_mapping<typename T::element_type, typename Ts::element_type, T, Ts>::type...> {
};
template <class T, class... Ts>
using extend_mapping_t = aux::apply_t<aux::inherit, typename extend_mapping<T, Ts...>::type>;
template <bool, class, class...>
struct conditional_mapping;
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<true, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type = unique_mappings_impl<aux::type<aux::inherit<T1>, extend_mapping_t<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct conditional_mapping<false, aux::type<T1, aux::inherit<Rs...>>, T2, Ts...> {
  using type =
      unique_mappings_impl<aux::type<aux::inherit<T1, aux::type<typename T2::element_type>>, aux::inherit<T2, Rs...>>, Ts...>;
};
template <class T1, class T2, class... Rs, class... Ts>
struct unique_mappings_impl<aux::type<T1, aux::inherit<Rs...>>, T2, Ts...>
    : conditional_mapping<aux::is_base_of<aux::type<typename T2::element_type>, T1>::value, aux::type<T1, aux::inherit<Rs...>>,
                          T2, Ts...>::type {};
template <class T1, class Rs>
struct unique_mappings_impl<aux::type<T1, Rs>> : aux::apply_t<aux::inherit, Rs> {};
template <class... Ts>
struct unique_mappings : unique_mappings_impl<aux::type<aux::none_type, aux::inherit<>>, Ts...> {};
template <class T>
struct unique_mappings<T> : aux::inherit<T> {};
template <class, class...>
struct mappings;
template <class... Ts>
struct mappings<aux::pool<Ts...>>
    : unique_mappings_t<
          event_mappings<typename Ts::event, aux::inherit<state_mappings<typename Ts::src_state, aux::type_list<Ts>>>>...> {};
template <class T>
using mappings_t = typename mappings<T>::type;
template <class, class TUnexpected>
transitions<TUnexpected> get_state_mapping_impl(...);
template <class T, class, class... Ts>
transitions<Ts...> get_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping {
  using type = decltype(get_state_mapping_impl<T, TUnexpected>((TMappings *)0));
};
template <class S>
transitions_sub<S> get_sub_state_mapping_impl(...);
template <class T, class... Ts>
transitions_sub<T, Ts...> get_sub_state_mapping_impl(state_mappings<T, aux::type_list<Ts...>> *);
template <class T, class TMappings, class TUnexpected>
struct get_state_mapping<sm<T>, TMappings, TUnexpected> {
  using type = decltype(get_sub_state_mapping_impl<sm<T>>((TMappings *)0));
};
template <class T, class TMappings, class TUnexpected>
using get_state_mapping_t = typename get_state_mapping<T, TMappings, TUnexpected>::type;
template <class>
transitions<aux::true_type> get_event_mapping_impl(...);
template <class T, class TMappings>
TMappings get_event_mapping_impl(event_mappings<T, TMappings> *);
template <class T, class TMappings>
using get_event_mapping_t = decltype(get_event_mapping_impl<T>((TMappings *)0));
}
namespace concepts {
struct callable_fallback {
  void operator()();
};
template <class T>
aux::false_type test_callable(aux::non_type<void (callable_fallback::*)(), &T::operator()> *);
template <class>
aux::true_type test_callable(...);
template <class, class T>
struct callable : decltype(test_callable<aux::inherit<T, callable_fallback>>(0)) {};
}
namespace back {
template <class TEvent>
using get_event = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<>, aux::type_list<TEvent>>;
template <class, class, class TEvent>
struct get_all_events_impl {
  using type = get_event<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, TDst, unexpected_event<TEvent>> {
  using type = aux::type_list<TEvent>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, TDst, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<TSrc, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TDst>::events>;
};
template <class TSrc, class TDst, class TEvent>
struct get_all_events_impl<sm<TSrc>, sm<TDst>, TEvent> {
  using type = aux::join_t<get_event<TEvent>, typename sm<TSrc>::events, typename sm<TDst>::events>;
};
template <class, class TEvent>
struct get_sub_internal_events_impl {
  using type = aux::conditional_t<aux::is_base_of<internal_event, TEvent>::value, aux::type_list<TEvent>, aux::type_list<>>;
};
template <class T, class TEvent>
struct get_sub_internal_events_impl<sm<T>, TEvent> {
  using type = aux::join_t<aux::type_list<TEvent>, typename sm_impl<T>::sub_internal_events_t>;
};
template <class... Ts>
using get_all_events =
    aux::join_t<typename get_all_events_impl<typename Ts::src_state, typename Ts::dst_state, typename Ts::event>::type...>;
template <class... Ts>
using get_sub_internal_events =
    aux::join_t<typename get_sub_internal_events_impl<typename Ts::src_state, typename Ts::event>::type...>;
template <class... Ts>
using get_events = aux::type_list<typename Ts::event...>;
template <class T>
struct get_exception : aux::type_list<> {};
template <class T>
struct get_exception<exception<T>> : aux::type_list<exception<T>> {};
template <class... Ts>
using get_exceptions = aux::join_t<typename get_exception<Ts>::type...>;
template <class... Ts>
using get_states = aux::join_t<aux::type_list<typename Ts::src_state, typename Ts::dst_state>...>;
template <class... Ts>
using get_initial_states =
    aux::join_t<typename aux::conditional<Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class... Ts>
using get_history_states = aux::join_t<
    typename aux::conditional<!Ts::history && Ts::initial, aux::type_list<typename Ts::src_state>, aux::type_list<>>::type...>;
template <class>
no_policy get_policy(...);
template <class T, class TPolicy>
TPolicy get_policy(aux::pair<T, TPolicy> *);
template <class SM, class... TPolicies>
struct sm_policy {
  using sm = SM;
  using thread_safety_policy = decltype(get_policy<thread_safety_policy__>((aux::inherit<TPolicies...> *)0));
  using defer_queue_policy = decltype(get_policy<defer_queue_policy__>((aux::inherit<TPolicies...> *)0));
  using logger_policy = decltype(get_policy<logger_policy__>((aux::inherit<TPolicies...> *)0));
  template <class T>
  using rebind = sm_policy<T, TPolicies...>;
};
template <class>
struct get_sub_sm : aux::type_list<> {};
template <class T>
struct get_sub_sm<sm<T>> : aux::join_t<aux::type_list<T>, typename sm<T>::state_machines> {};
template <class... Ts>
using get_sub_sms = aux::join_t<typename get_sub_sm<Ts>::type...>;
template <class... Ts>
using get_sm_t = aux::type_list<typename Ts::sm...>;
template <class... Ts>
using merge_deps = aux::join_t<typename Ts::deps...>;
template <class>
struct convert_to_sm;
template <class... Ts>
struct convert_to_sm<aux::type_list<Ts...>> {
  using type = aux::type_list<sm_impl<Ts>...>;
};
template <class TSM>
struct sm_impl {
  template <class T>
  using rebind = sm<sm_policy<T>>;
  using sm_t = typename TSM::sm;
  using thread_safety_t = typename TSM::thread_safety_policy::type;
  using defer_policy_t = typename TSM::defer_queue_policy;
  template <class T>
  using defer_queue_t = typename defer_policy_t::template rebind<T>;
  template <class... Ts>
  using defer_event_t = typename defer_policy_t::template defer<Ts...>;
  using logger_t = typename TSM::logger_policy::type;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());
  using states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using states_ids_t = aux::apply_t<aux::type_id, states_t>;
  using initial_states_t = aux::apply_t<aux::unique_t, aux::apply_t<get_initial_states, transitions_t>>;
  using initial_states_ids_t = aux::apply_t<aux::type_id, initial_states_t>;
  using history_states_t = aux::apply_t<get_history_states, transitions_t>;
  using has_history_states =
      aux::integral_constant<bool, aux::size<initial_states_t>::value != aux::size<history_states_t>::value>;
  using sub_internal_events_t = aux::apply_t<get_sub_internal_events, transitions_t>;
  using events_t = aux::apply_t<aux::unique_t, aux::join_t<sub_internal_events_t, aux::apply_t<get_events, transitions_t>>>;
  using events_ids_t = aux::apply_t<aux::inherit, events_t>;
  using has_unexpected_events = typename aux::is_base_of<unexpected, aux::apply_t<aux::inherit, events_t>>::type;
  using has_entry_exits = typename aux::is_base_of<entry_exit, aux::apply_t<aux::inherit, events_t>>::type;
  using defer_t = defer_queue_t<aux::apply_t<defer_event_t, events_t>>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using state_t = aux::conditional_t<(aux::size<states_t>::value > 0xFF), unsigned short, aux::byte>;
  static constexpr auto regions = aux::size<initial_states_t>::value;
  static_assert(regions > 0, "At least one initial state is required");
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  using exceptions = aux::apply_t<aux::unique_t, aux::apply_t<get_exceptions, events_t>>;
  using has_exceptions = aux::integral_constant<bool, (aux::size<exceptions>::value > 0)>;
#endif
  struct mappings : mappings_t<transitions_t> {};
  sm_impl(const aux::init &, const aux::pool_type<sm_t &> *t) : transitions_((t->value)()) {
    initialize(typename sm_impl<TSM>::initial_states_t{});
  }
  sm_impl(const aux::init &, ...) : transitions_(sm_t{}()) { initialize(typename sm_impl<TSM>::initial_states_t{}); }
  sm_impl(sm_impl &&) = default;
  sm_impl(const sm_impl &) = delete;
  sm_impl &operator=(const sm_impl &) = delete;
  template <class TEvent, class TDeps, class TSubs>
  bool process_event(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled =
        process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(
        event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
#endif
    process_internal_events(anonymous{}, deps, subs);
    process_defer_events(deps, subs, handled, aux::type<defer_queue_t<TEvent>>{}, events_t{});
    return handled;
  }
  void initialize(const aux::type_list<> &) {}
  template <class... TStates>
  void initialize(const aux::type_list<TStates...> &) {
    auto region = 0, i = region;
    (void)aux::swallow{0, (region = i, current_state_[region] = aux::get_id<states_ids_t, 0, TStates>(), ++i, 0)...};
  }
  template <class TDeps, class TSubs>
  void start(TDeps &deps, TSubs &subs) {
    process_internal_events(on_entry<_, initial>{}, deps, subs);
    process_internal_events(anonymous{}, deps, subs);
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &, TDeps &, TSubs &, ...) {
    return false;
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    aux::make_index_sequence<regions>{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_events(const TEvent &event, TDeps &deps, TSubs &subs) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(!aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &, TDeps &, TSubs &, ...) {
    return false;
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_generic_t<TEvent>, events_ids_t>::value &&
                                 !aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                        has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<get_generic_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                    current_state);
#endif
  }
  template <class TEvent, class TDeps, class TSubs,
            __BOOST_SML_REQUIRES(aux::is_base_of<get_mapped_t<TEvent>, events_ids_t>::value)>
  bool process_internal_event(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state) {
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    return process_event_noexcept<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, current_state,
                                                                                       has_exceptions{});
#else
    return process_event_impl<get_event_mapping_t<get_mapped_t<TEvent>, mappings>>(event, deps, subs, states_t{},
                                                                                   current_state);
#endif
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          const aux::index_sequence<0> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state_[0]](event, *this, deps, subs, current_state_[0]);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates, int... Ns>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          const aux::index_sequence<Ns...> &) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    auto handled = false;
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    (void)aux::swallow{0, (handled |= dispatch_table[current_state_[Ns]](event, *this, deps, subs, current_state_[Ns]), 0)...};
    return handled;
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs, class... TStates>
  bool process_event_impl(const TEvent &event, TDeps &deps, TSubs &subs, const aux::type_list<TStates...> &,
                          state_t &current_state) {
    using dispatch_table_t = bool (*)(const TEvent &, sm_impl &, TDeps &, TSubs &, state_t &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &get_state_mapping_t<TStates, TMappings, has_unexpected_events>::template execute<TEvent, sm_impl, TDeps, TSubs>...};
    const auto lock = create_lock(aux::type<thread_safety_t>{});
    (void)lock;
    return dispatch_table[current_state](event, *this, deps, subs, current_state);
  }
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::false_type) noexcept {
    return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, state_t &current_state, aux::true_type) noexcept {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, current_state);
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TMappings, class TEvent, class TDeps, class TSubs>
  bool process_event_noexcept(const TEvent &event, TDeps &deps, TSubs &subs, aux::true_type) {
    try {
      return process_event_impl<TMappings>(event, deps, subs, states_t{}, aux::make_index_sequence<regions>{});
    } catch (...) {
      return process_exception(deps, subs, exceptions{});
    }
  }
  template <class TDeps, class TSubs>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<> &) {
    return process_internal_events(exception<_>{}, deps, subs);
  }
  template <class TDeps, class TSubs, class E, class... Es>
  bool process_exception(TDeps &deps, TSubs &subs, const aux::type_list<E, Es...> &) {
    try {
      throw;
    } catch (const typename E::type &e) {
      return process_internal_events(E{e}, deps, subs);
    } catch (...) {
      return process_exception(deps, subs, aux::type_list<Es...>{});
    }
  }
#endif
  template <class TDeps, class TSubs, class... TEvents>
  void process_defer_events(TDeps &, TSubs &, const bool, const aux::type<no_policy> &, const aux::type_list<TEvents...> &) {}
  template <class TDeps, class TSubs, class TEvent>
  bool process_event_no_deffer(TDeps &deps, TSubs &subs, const void *data) {
    const auto &event = *static_cast<const TEvent *>(data);
    log_process_event<sm_t>(aux::type<logger_t>{}, deps, event);
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS)
    const auto handled = process_event_noexcept<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, has_exceptions{});
#else
    const auto handled = process_event_impl<get_event_mapping_t<TEvent, mappings>>(event, deps, subs, states_t{},
                                                                                   aux::make_index_sequence<regions>{});
#endif
    if (handled) {
      defer_.pop();
    }
    return handled;
  }
  template <class TDeps, class TSubs, class TDeferQueue, class... TEvents>
  void process_defer_events(TDeps &deps, TSubs &subs, const bool handled, const aux::type<TDeferQueue> &,
                            const aux::type_list<TEvents...> &) {
    if (handled) {
      auto size = defer_.size();
      using dispatch_table_t = bool (sm_impl::*)(TDeps &, TSubs &, const void *);
      const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TEvents))] = {
          &sm_impl::process_event_no_deffer<TDeps, TSubs, TEvents>...};
      while (size-- && (this->*dispatch_table[defer_.front().id])(deps, subs, defer_.front().data))
        ;
    }
  }
  template <class TVisitor, class... TStates>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &, const aux::index_sequence<0> &) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    dispatch_table[current_state_[0]](visitor);
  }
  template <class TVisitor, class... TStates, int... Ns>
  void visit_current_states(const TVisitor &visitor, const aux::type_list<TStates...> &,
                            const aux::index_sequence<Ns...> &) const {
    using dispatch_table_t = void (*)(const TVisitor &);
    const static dispatch_table_t dispatch_table[__BOOST_SML_ZERO_SIZE_ARRAY_CREATE(sizeof...(TStates))] = {
        &sm_impl::visit_state<TVisitor, TStates>...};
    int _[]{0, (dispatch_table[current_state_[Ns]](visitor), 0)...};
    (void)_;
  }
  template <class TVisitor, class TState>
  static void visit_state(const TVisitor &visitor) {
    visitor(aux::string<TState>{});
  }
  no_policy create_lock(const aux::type<no_policy> &) { return {}; }
  template <class TLockPolicy>
  auto create_lock(const aux::type<TLockPolicy> &) {
    struct lock_guard {
      explicit lock_guard(thread_safety_t &synch) : thread_safety_(synch) { thread_safety_.lock(); }
      ~lock_guard() { thread_safety_.unlock(); }
      thread_safety_t &thread_safety_;
    };
    return lock_guard{thread_safety_};
  }
  transitions_t transitions_;
  state_t current_state_[regions];
  thread_safety_t thread_safety_;
  defer_t defer_;
};
template <class TSM>
class sm {
  using sm_t = typename TSM::sm;
  using logger_t = typename TSM::logger_policy::type;
  using logger_dep_t =
      aux::conditional_t<aux::is_same<no_policy, logger_t>::value, aux::type_list<>, aux::type_list<logger_t &>>;
  using transitions_t = decltype(aux::declval<sm_t>().operator()());

 public:
  using states = aux::apply_t<aux::unique_t, aux::apply_t<get_states, transitions_t>>;
  using state_machines = aux::apply_t<get_sub_sms, states>;
  using events = aux::apply_t<aux::unique_t, aux::apply_t<get_all_events, transitions_t>>;
  using transitions = aux::apply_t<aux::type_list, transitions_t>;

 private:
  using sm_all_t = aux::apply_t<aux::inherit, aux::join_t<aux::type_list<sm_t>, aux::apply_t<get_sm_t, state_machines>>>;
  using sub_sms_t =
      aux::apply_t<aux::pool,
                   typename convert_to_sm<aux::join_t<aux::type_list<TSM>, aux::apply_t<get_sub_sms, states>>>::type>;
  using deps = aux::apply_t<merge_deps, transitions_t>;
  using deps_t =
      aux::apply_t<aux::pool,
                   aux::apply_t<aux::unique_t, aux::join_t<deps, logger_dep_t, aux::apply_t<merge_deps, sub_sms_t>>>>;
  struct events_ids : aux::apply_t<aux::inherit, events> {};

 public:
  sm(sm &&) = default;
  sm(const sm &) = delete;
  sm &operator=(const sm &) = delete;
  sm(aux::init, deps_t &deps) : deps_(deps), sub_sms_{deps} { aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_); }
  template <class... TDeps, __BOOST_SML_REQUIRES(aux::is_unique_t<TDeps...>::value)>
  explicit sm(TDeps &&... deps) : deps_{aux::init{}, aux::pool<TDeps...>{deps...}}, sub_sms_{aux::pool<TDeps...>{deps...}} {
    aux::get<sm_impl<TSM>>(sub_sms_).start(deps_, sub_sms_);
  }
  template <class TEvent, __BOOST_SML_REQUIRES(aux::is_base_of<TEvent, events_ids>::value)>
  void process_event(const TEvent &event) {
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(event, deps_, sub_sms_);
  }
  template <class TEvent, __BOOST_SML_REQUIRES(!aux::is_base_of<TEvent, events_ids>::value)>
  void process_event(const TEvent &event) {
    aux::get<sm_impl<TSM>>(sub_sms_).process_event(unexpected_event<_, TEvent>{event}, deps_, sub_sms_);
  }
  template <class T = sm_t, class TVisitor, __BOOST_SML_REQUIRES(concepts::callable<void, TVisitor>::value)>
  void visit_current_states(const TVisitor &visitor) const {
    using type = sm_impl<typename TSM::template rebind<T>>;
    using states_t = typename type::states_t;
    constexpr auto regions = type::regions;
    aux::cget<type>(sub_sms_).visit_current_states(visitor, states_t{}, aux::make_index_sequence<regions>{});
  }
  template <class T = sm_t, class TState>
  bool is(const TState &) const {
    auto result = false;
    visit_current_states<T>(
        [&](auto state) { result |= aux::is_same<typename TState::type, typename decltype(state)::type>::value; });
    return result;
  }
  template <class T = sm_t, class... TStates,
            __BOOST_SML_REQUIRES(sizeof...(TStates) == sm_impl<typename TSM::template rebind<T>>::regions)>
  bool is(const TStates &...) const {
    auto result = true;
    auto i = 0;
    using type = sm_impl<typename TSM::template rebind<T>>;
    using states_ids_t = typename type::states_ids_t;
    int state_ids[] = {aux::get_id<states_ids_t, 0, typename TStates::type>()...};
    visit_current_states<T>(
        [&](auto state) { result &= (aux::get_id<states_ids_t, 0, typename decltype(state)::type>() == state_ids[i++]); });
    return result;
  }
  template <class T = sm_t, class... TStates>
  void __set_current_states(const TStates &...) {
    using type = sm_impl<typename TSM::template rebind<T>>;
    using states_ids_t = typename type::states_ids_t;
    auto &sm = aux::get<sm_impl<TSM>>(sub_sms_);
    auto region = 0;
    int _[]{0, (sm.current_state_[region++] = aux::get_id<states_ids_t, 0, typename TStates::type>(), 0)...};
    (void)_;
  }

 private:
  deps_t deps_;
  sub_sms_t sub_sms_;
};
}
namespace concepts {
template <class T>
decltype(aux::declval<T>().operator()()) composable_impl(int);
template <class>
void composable_impl(...);
template <class T>
struct composable : aux::is<aux::pool, decltype(composable_impl<T>(0))> {};
}
namespace front {
struct operator_base {};
struct action_base {};
template <class, class>
aux::type_list<action_base> args1__(...);
template <class T, class E>
auto args1__(int) -> aux::function_traits_t<decltype(&T::operator())>;
template <class T, class E>
auto args__(int) -> aux::function_traits_t<decltype(&T::__BOOST_SML_TEMPLATE_KEYWORD operator() < back::get_event_t<E>>)>;
template <class T, class E>
auto args__(...) -> decltype(args1__<T, E>(0));
template <class T, class E>
using args_t = decltype(args__<T, E>(0));
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const TEvent &, TDeps &deps) {
  return aux::get<T>(deps);
}
template <class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<TEvent> &, const TEvent &event, TDeps &) {
  return event;
}
template <class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const TEvent &event, TDeps &) {
  return event;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::unexpected_event<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_entry<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<const TEvent &> &, const back::on_exit<T, TEvent> &event, TDeps &) {
  return event.event_;
}
template <class T, class TEvent, class TDeps>
decltype(auto) get_arg(const aux::type<T> &, const back::exception<TEvent> &event, TDeps &) {
  return event.exception_;
}
template <class, class, class>
struct call;
template <class TEvent>
struct call<TEvent, aux::type_list<>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &, TSM &, TDeps &, TSubs &) {
    return object();
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object());
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object();
    back::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object();
  }
};
template <class TEvent>
struct call<TEvent, aux::type_list<TEvent>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &, TSubs &) {
    return object(event);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<TEvent>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    using result_type = decltype(object(event));
    return execute_impl<typename TSM::sm_t>(aux::type<result_type>{}, object, event, deps);
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<bool> &, T object, const TEvent &event, TDeps &deps) {
    const auto result = object(event);
    back::log_guard<TSM>(aux::type<TLogger>{}, deps, object, event, result);
    return result;
  }
  template <class TSM, class T, class TDeps>
  static auto execute_impl(const aux::type<void> &, T object, const TEvent &event, TDeps &deps) {
    back::log_action<TSM>(aux::type<TLogger>{}, deps, object, event);
    object();
  }
};
template <class TEvent>
struct call<TEvent, aux::type_list<action_base>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};
template <class TEvent, class TLogger>
struct call<TEvent, aux::type_list<action_base>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return object(event, sm, deps, subs);
  }
};
template <class TEvent, class... Ts>
struct call<TEvent, aux::type_list<Ts...>, back::no_policy> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    return object(get_arg(aux::type<Ts>{}, event, deps)...);
  }
};
template <class TEvent, class... Ts, class TLogger>
struct call<TEvent, aux::type_list<Ts...>, TLogger> {
  template <class T, class TSM, class TDeps, class TSubs>
  static auto execute(T object, const TEvent &event, TSM &, TDeps &deps, TSubs &) {
    return object(get_arg(aux::type<Ts>{}, event, deps)...);
  }
};
template <class... Ts>
class seq_ : operator_base {
 public:
  explicit seq_(Ts... ts) : a(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  void for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    (void)aux::swallow{
        0, (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&a), event, sm, deps, subs),
            0)...};
  }
  aux::tuple<Ts...> a;
};
template <class... Ts>
class and_ : operator_base {
 public:
  explicit and_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    auto result = true;
    (void)aux::swallow{
        0, (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs)
                ? result
                : result = false)...};
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class... Ts>
class or_ : operator_base {
 public:
  explicit or_(Ts... ts) : g(ts...) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return for_all(aux::make_index_sequence<sizeof...(Ts)>{}, event, sm, deps, subs);
  }

 private:
  template <int... Ns, class TEvent, class TSM, class TDeps, class TSubs>
  auto for_all(const aux::index_sequence<Ns...> &, const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    auto result = false;
    (void)aux::swallow{
        0, (call<TEvent, args_t<Ts, TEvent>, typename TSM::logger_t>::execute(aux::get_by_id<Ns>(&g), event, sm, deps, subs)
                ? result = true
                : result)...};
    return result;
  }
  aux::tuple<Ts...> g;
};
template <class T>
class not_ : operator_base {
 public:
  explicit not_(T t) : g(t) {}
  template <class TEvent, class TSM, class TDeps, class TSubs>
  auto operator()(const TEvent &event, TSM &sm, TDeps &deps, TSubs &subs) {
    return !call<TEvent, args_t<T, TEvent>, typename TSM::logger_t>::execute(g, event, sm, deps, subs);
  }

 private:
  T g;
};
}
template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
auto operator!(const T &t) {
  return front::not_<aux::zero_wrapper<T>>(aux::zero_wrapper<T>{t});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator&&(const T1 &t1, const T2 &t2) {
  return front::and_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<bool, T1>::value &&concepts::callable<bool, T2>::value)>
auto operator||(const T1 &t1, const T2 &t2) {
  return front::or_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
template <class T1, class T2, __BOOST_SML_REQUIRES(concepts::callable<void, T1>::value &&concepts::callable<void, T2>::value)>
auto operator,(const T1 &t1, const T2 &t2) {
  return front::seq_<aux::zero_wrapper<T1>, aux::zero_wrapper<T2>>(aux::zero_wrapper<T1>{t1}, aux::zero_wrapper<T2>{t2});
}
namespace front {
namespace actions {
template <class... Ts>
class defer_event {
  using ids_t = aux::type_id<Ts...>;
  static constexpr auto alignment = aux::max<alignof(Ts)...>();
  static constexpr auto size = aux::max<sizeof(Ts)...>();
  template <class T>
  static void dtor_impl(aux::byte *data) {
    reinterpret_cast<T *>(data)->~T();
  }

 public:
  defer_event(defer_event &&) = default;
  defer_event(const defer_event &) = delete;
  defer_event &operator=(const defer_event &) = delete;
  template <class T>
  defer_event(T object) {
    id = aux::get_id<ids_t, -1, T>();
    dtor = &dtor_impl<T>;
    new (&data) T(static_cast<T &&>(object));
  }
  ~defer_event() { dtor(data); }
  alignas(alignment) aux::byte data[size];
  int id = -1;

 private:
  void (*dtor)(aux::byte *);
};
struct defer : action_base {
  template <class TEvent, class TSM, class TDeps, class TSubs>
  void operator()(const TEvent &event, TSM &sm, TDeps &, TSubs &) {
    sm.defer_.push(event);
  }
};
}
}
namespace detail {
template <class T, __BOOST_SML_REQUIRES(concepts::composable<typename T::sm>::value)>
using state_machine = back::sm<T>;
}
template <class T>
struct thread_safe : aux::pair<back::thread_safety_policy__, thread_safe<T>> {
  using type = T;
};
template <template <class...> class T>
struct defer_queue : aux::pair<back::defer_queue_policy__, defer_queue<T>> {
  template <class U>
  using rebind = T<U>;
  template <class... Ts>
  using defer = front::actions::defer_event<Ts...>;
};
template <class T>
struct logger : aux::pair<back::logger_policy__, logger<T>> {
  using type = T;
};
struct testing : aux::pair<back::testing_policy__, testing> {};
template <class T, class... TPolicies>
using sm = detail::state_machine<back::sm_policy<T, TPolicies...>>;
namespace concepts {
aux::false_type transitional_impl(...);
template <class T>
auto transitional_impl(T &&t) -> aux::always<typename T::dst_state, typename T::src_state, typename T::event, typename T::deps,
                                             decltype(T::initial), decltype(T::history)>;
template <class T>
struct transitional : decltype(transitional_impl(aux::declval<T>())) {};
}
namespace front {
namespace actions {
struct process {
  template <class TEvent>
  class process_impl : public action_base {
   public:
    explicit process_impl(const TEvent &event) : event(event) {}
    template <class T, class TSM, class TDeps, class TSubs>
    void operator()(const T &, TSM &sm, TDeps &deps, TSubs &subs) {
      sm.process_event(event, deps, subs);
    }

   private:
    TEvent event;
  };
  template <class TEvent>
  auto operator()(const TEvent &event) {
    return process_impl<TEvent>{event};
  }
};
}
}
namespace front {
template <class, class>
struct transition_eg;
template <class, class>
struct transition_ea;
template <class TEvent>
struct event {
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_eg<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_ea<event, aux::zero_wrapper<T>>{*this, aux::zero_wrapper<T>{t}};
  }
  auto operator()() const { return TEvent{}; }
};
}
namespace front {
struct initial_state {};
struct history_state {};
struct terminate_state {
  static auto c_str() { return "terminate"; }
};
template <class...>
struct transition;
template <class, class>
struct transition_sa;
template <class, class>
struct transition_sg;
template <class, class>
struct transition_eg;
template <class>
struct state;
template <class TState>
struct state_impl {
  template <class T>
  auto operator<=(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T>
  auto operator+(const T &t) const {
    return transition<TState, T>{static_cast<const TState &>(*this), t};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<bool, T>::value)>
  auto operator[](const T &t) const {
    return transition_sg<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
  template <class T, __BOOST_SML_REQUIRES(concepts::callable<void, T>::value)>
  auto operator/(const T &t) const {
    return transition_sa<TState, aux::zero_wrapper<T>>{static_cast<const TState &>(*this), aux::zero_wrapper<T>{t}};
  }
};
template <class TState>
struct state : state_impl<state<TState>> {
  using type = TState;
  static constexpr auto initial = false;
  static constexpr auto history = false;
  auto operator*() const { return state<TState(initial_state)>{}; }
  auto operator()(const initial_state &) const { return state<TState(initial_state)>{}; }
  auto operator()(const history_state &) const { return state<TState(history_state)>{}; }
  template <class... Ts>
  auto operator()(const state<Ts> &...) const {
    return state<TState(Ts...)>{};
  }
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(initial_state)> : state_impl<state<TState(initial_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = false;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class TState>
struct state<TState(history_state)> : state_impl<state<TState(history_state)>> {
  using type = TState;
  static constexpr auto initial = true;
  static constexpr auto history = true;
  template <class T>
  auto operator=(const T &t) const {
    return transition<T, state>{t, *this};
  }
};
template <class T, class = void>
struct state_sm {
  using type = state<T>;
};
template <class T>
struct state_sm<T, aux::enable_if_t<concepts::composable<T>::value>> {
  using type = state<back::sm<back::sm_policy<T>>>;
};
}
namespace front {
struct internal {};
template <class, class>
struct ignore;
template <class E, class... Ts>
struct ignore<E, aux::type_list<Ts...>> {
  template <class T>
  struct non_events {
    using type = aux::conditional_t<aux::is_same<back::get_event_t<E>, aux::remove_reference_t<T>>::value ||
                                        aux::is_same<T, action_base>::value,
                                    aux::type_list<>, aux::type_list<T>>;
  };
  using type = aux::join_t<typename non_events<Ts>::type...>;
};
template <class T, class E, class = void>
struct get_deps {
  using type = typename ignore<E, args_t<T, E>>::type;
};
template <class T, class E>
using get_deps_t = typename get_deps<T, E>::type;
template <template <class...> class T, class... Ts, class E>
struct get_deps<T<Ts...>, E, aux::enable_if_t<aux::is_base_of<operator_base, T<Ts...>>::value>> {
  using type = aux::join_t<get_deps_t<Ts, E>...>;
};
struct always {
  bool operator()() const { return true; }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
struct none {
  void operator()() {}
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class...>
struct transition;
template <class E, class G>
struct transition<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class G, class A>
struct transition<front::event<E>, G, A> {
  front::event<E> e;
  G g;
  A a;
};
template <class S2, class G, class A>
struct transition<state<S2>, G, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::g;
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>::a;
  transition(const G &g, const A &a) : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, A>{g, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, A>{g, a};
  }
};
template <class S1, class S2>
struct transition<state<S1>, state<S2>> : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none> {
  transition(const state<S1> &, const state<S2> &)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, none>{always{}, none{}} {}
};
template <class S2, class G>
struct transition_sg<state<S2>, G> : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>::g;
  transition_sg(const state<S2> &, const G &g)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, G, none>{g, none{}} {}
  template <class T>
  auto operator/(const T &t) const {
    return transition<state<S2>, G, aux::zero_wrapper<T>>{g, aux::zero_wrapper<T>{t}};
  }
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, G, none>{g, none{}};
  }
};
template <class S2, class A>
struct transition_sa<state<S2>, A> : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A> {
  using transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>::a;
  transition_sa(const state<S2> &, const A &a)
      : transition<state<internal>, state<S2>, front::event<back::anonymous>, always, A>{always{}, a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<back::anonymous>, always, A>{always{}, a};
  }
};
template <class S2, class E>
struct transition<state<S2>, front::event<E>> {
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, none>{always{}, none{}};
  }
  const state<S2> &s2;
  front::event<E> e;
};
template <class E, class G>
struct transition_eg<front::event<E>, G> {
  template <class T>
  auto operator/(const T &t) const {
    return transition<front::event<E>, G, aux::zero_wrapper<T>>{e, g, aux::zero_wrapper<T>{t}};
  }
  front::event<E> e;
  G g;
};
template <class E, class A>
struct transition_ea<front::event<E>, A> {
  front::event<E> e;
  A a;
};
template <class S1, class S2, class G, class A>
struct transition<state<S1>, transition<state<S2>, G, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, G, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, A>{t.g, t.a} {}
};
template <class S1, class E, class G, class A>
struct transition<state<S1>, transition<front::event<E>, G, A>>
    : transition<state<internal>, state<S1>, front::event<E>, G, A> {
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::g;
  using transition<state<internal>, state<S1>, front::event<E>, G, A>::a;
  transition(const state<S1> &, const transition<front::event<E>, G, A> &t)
      : transition<state<internal>, state<S1>, front::event<E>, G, A>{t.g, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S1>, front::event<E>, G, A>{g, a};
  }
};
template <class S1, class S2, class E>
struct transition<state<S1>, transition<state<S2>, front::event<E>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, none> {
  transition(const state<S1> &, const transition<state<S2>, front::event<E>> &)
      : transition<state<S1>, state<S2>, front::event<E>, always, none>{always{}, none{}} {}
};
template <class S1, class S2, class G>
struct transition<state<S1>, transition_sg<state<S2>, G>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none> {
  transition(const state<S1> &, const transition_sg<state<S2>, G> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, G, none>{t.g, none{}} {}
};
template <class S1, class S2, class A>
struct transition<state<S1>, transition_sa<state<S2>, A>>
    : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A> {
  transition(const state<S1> &, const transition_sa<state<S2>, A> &t)
      : transition<state<S1>, state<S2>, front::event<back::anonymous>, always, A>{always{}, t.a} {}
};
template <class S2, class E, class G>
struct transition<state<S2>, transition_eg<front::event<E>, G>>
    : transition<state<internal>, state<S2>, front::event<E>, G, none> {
  using transition<state<internal>, state<S2>, front::event<E>, G, none>::g;
  transition(const state<S2> &, const transition_eg<front::event<E>, G> &t)
      : transition<state<internal>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, G, none>{g, none{}};
  }
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, transition<state<S2>, transition_eg<front::event<E>, G>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, none> {
  transition(const state<S1> &, const transition<state<S2>, transition_eg<front::event<E>, G>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, none>{t.g, none{}} {}
};
template <class S2, class E, class A>
struct transition<state<S2>, transition_ea<front::event<E>, A>>
    : transition<state<internal>, state<S2>, front::event<E>, always, A> {
  using transition<state<internal>, state<S2>, front::event<E>, always, A>::a;
  transition(const state<S2> &, const transition_ea<front::event<E>, A> &t)
      : transition<state<internal>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
  template <class T>
  auto operator=(const T &) const {
    return transition<T, state<S2>, front::event<E>, always, A>{always{}, a};
  }
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, transition<state<S2>, transition_ea<front::event<E>, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, always, A> {
  transition(const state<S1> &, const transition<state<S2>, transition_ea<front::event<E>, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, always, A>{always{}, t.a} {}
};
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, transition<state<S2>, transition<front::event<E>, G, A>>>
    : transition<state<S1>, state<S2>, front::event<E>, G, A> {
  transition(const state<S1> &, const transition<state<S2>, transition<front::event<E>, G, A>> &t)
      : transition<state<S1>, state<S2>, front::event<E>, G, A>{t.g, t.a} {}
};
template <class T, class TSubs, class... Ts, class... THs>
void update_composite_states(TSubs &subs, aux::true_type, const aux::type_list<THs...> &) {
  auto &sm = aux::get<T>(subs);
  (void)aux::swallow{0, (sm.current_state_[aux::get_id<typename T::initial_states_ids_t, -1, THs>()] =
                             aux::get_id<typename T::states_ids_t, -1, THs>(),
                         0)...};
}
template <class T, class TSubs>
void update_composite_states(TSubs &subs, aux::false_type, ...) {
  aux::get<T>(subs).initialize(typename T::initial_states_t{});
}
template <class SM, class TDeps, class TSubs, class TSrcState, class TDstState>
void update_current_state(SM &, TDeps &deps, TSubs &, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const TDstState &) {
  back::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps, aux::string<typename TSrcState::type>{},
                                            aux::string<typename TDstState::type>{});
  current_state = new_state;
}
template <class SM, class TDeps, class TSubs, class TSrcState, class T>
void update_current_state(SM &, TDeps &deps, TSubs &subs, typename SM::state_t &current_state,
                          const typename SM::state_t &new_state, const TSrcState &, const state<back::sm<T>> &) {
  back::log_state_change<typename SM::sm_t>(aux::type<typename SM::logger_t>{}, deps, aux::string<typename TSrcState::type>{},
                                            aux::string<T>{});
  current_state = new_state;
  update_composite_states<back::sm_impl<T>>(subs, typename back::sm_impl<T>::has_history_states{},
                                            typename back::sm_impl<T>::history_states_t{});
}
template <class S1, class S2, class E, class G, class A>
struct transition<state<S1>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                           state<src_state>{}, state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                           state<src_state>{}, state<dst_state>{});
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S2, class E, class G, class A>
struct transition<state<internal>, state<S2>, front::event<E>, G, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, aux::join_t<get_deps_t<G, E>, get_deps_t<A, E>>>;
  transition(const G &g, const A &a) : g(g), a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, ...) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
      return true;
    }
    return false;
  }
  G g;
  A a;
};
template <class S1, class S2, class E, class A>
struct transition<state<S1>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                         state<src_state>{}, state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                         state<src_state>{}, state<dst_state>{});
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S2, class E, class A>
struct transition<state<internal>, state<S2>, front::event<E>, always, A> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = A;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<A, E>>;
  transition(const always &, const A &a) : a(a) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, ...) {
    call<TEvent, args_t<A, TEvent>, typename SM::logger_t>::execute(a, event, sm, deps, subs);
    return true;
  }
  A a;
};
template <class S1, class S2, class E, class G>
struct transition<state<S1>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
      update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                           state<src_state>{}, state<dst_state>{});
      sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
      return true;
    }
    return false;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    if (call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs)) {
      update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                           state<src_state>{}, state<dst_state>{});
      return true;
    }
    return false;
  }
  G g;
};
template <class S2, class E, class G>
struct transition<state<internal>, state<S2>, front::event<E>, G, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = G;
  using action = none;
  using deps = aux::apply_t<aux::unique_t, get_deps_t<G, E>>;
  transition(const G &g, const none &) : g(g) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &, ...) {
    return call<TEvent, args_t<G, TEvent>, typename SM::logger_t>::execute(g, event, sm, deps, subs);
  }
  G g;
};
template <class S1, class S2, class E>
struct transition<state<S1>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = typename state<S1>::type;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &event, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::true_type) {
    sm.process_internal_event(back::on_exit<back::_, TEvent>{event}, deps, subs, current_state);
    update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                         state<src_state>{}, state<dst_state>{});
    sm.process_internal_event(back::on_entry<back::_, TEvent>{event}, deps, subs, current_state);
    return true;
  }
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &, SM &sm, TDeps &deps, TSubs &subs, typename SM::state_t &current_state, aux::false_type) {
    update_current_state(sm, deps, subs, current_state, aux::get_id<typename SM::states_ids_t, -1, dst_state>(),
                         state<src_state>{}, state<dst_state>{});
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
template <class S2, class E>
struct transition<state<internal>, state<S2>, front::event<E>, always, none> {
  static constexpr auto initial = state<S2>::initial;
  static constexpr auto history = state<S2>::history;
  using src_state = typename state<S2>::type;
  using dst_state = internal;
  using event = E;
  using guard = always;
  using action = none;
  using deps = aux::type_list<>;
  transition(const always &, const none &) {}
  template <class TEvent, class SM, class TDeps, class TSubs>
  bool execute(const TEvent &, SM &, TDeps &, TSubs &, typename SM::state_t &, ...) {
    return true;
  }
  __BOOST_SML_ZERO_SIZE_ARRAY(aux::byte);
};
}
using _ = back::_;
template <class TEvent>
front::event<TEvent> event __BOOST_SML_VT_INIT;
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_entry<_, TEvent>> on_entry __BOOST_SML_VT_INIT;
template <class TEvent>
__BOOST_SML_UNUSED front::event<back::on_exit<_, TEvent>> on_exit __BOOST_SML_VT_INIT;
template <class TEvent>
front::event<back::unexpected_event<TEvent>> unexpected_event __BOOST_SML_VT_INIT;
template <class T>
front::event<back::exception<T>> exception __BOOST_SML_VT_INIT;
using anonymous = back::anonymous;
using initial = back::initial;
template <class T>
typename front::state_sm<T>::type state __BOOST_SML_VT_INIT;
#if !defined(_MSC_VER)
template <class T, T... Chrs>
auto operator""_s() {
  return front::state<aux::string<T, Chrs...>>{};
}
template <class T, T... Chrs>
auto operator""_e() {
  return event<aux::string<T, Chrs...>>;
}
#endif
__BOOST_SML_UNUSED static front::state<front::terminate_state> X;
__BOOST_SML_UNUSED static front::history_state H;
__BOOST_SML_UNUSED static front::actions::defer defer;
__BOOST_SML_UNUSED static front::actions::process process;
template <class... Ts, __BOOST_SML_REQUIRES(aux::is_same<aux::bool_list<aux::always<Ts>::value...>,
                                                         aux::bool_list<concepts::transitional<Ts>::value...>>::value)>
auto make_transition_table(Ts... ts) {
  return aux::pool<Ts...>{ts...};
}
BOOST_SML_NAMESPACE_END
#undef __BOOST_SML_UNUSED
#undef __BOOST_SML_VT_INIT
#undef __BOOST_SML_ZERO_SIZE_ARRAY
#undef __BOOST_SML_ZERO_SIZE_ARRAY_CREATE
#undef __BOOST_SML_TEMPLATE_KEYWORD
#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#undef __has_builtin
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#undef __has_builtin
#endif
#endif