//
// immer - immutable data structures for C++
// Copyright (C) 2016, 2017 Juan Pedro Bolivar Puente
//
// This file is part of immer.
//
// immer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// immer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with immer.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

#include <scm/detail/invoke.hpp>
#include <scm/detail/function_args.hpp>
#include <scm/detail/convert.hpp>

namespace scm {
namespace detail {

// this anonymous namespace should help avoiding registration clashes
// among translation units.
namespace {

template <typename Tag, typename R, typename Fn>
auto subr_wrapper_impl(Fn fn, pack<R>, pack<>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] () -> SCM { return to_scm(invoke(fn_)); };
}
template <typename Tag, typename Fn, typename R, typename T1>
auto subr_wrapper_impl(Fn fn, pack<R>, pack<T1>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1) -> SCM {
        return to_scm(invoke(fn_, to_cpp<T1>(a1)));
    };
}
template <typename Tag, typename Fn, typename R, typename T1, typename T2>
auto subr_wrapper_impl(Fn fn, pack<R>, pack<T1, T2>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1, SCM a2) -> SCM {
        return to_scm(invoke(fn_, to_cpp<T1>(a1), to_cpp<T2>(a2)));
    };
}
template <typename Tag, typename Fn, typename R, typename T1, typename T2,
          typename T3>
auto subr_wrapper_impl(Fn fn, pack<R>, pack<T1, T2, T3>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1, SCM a2, SCM a3) -> SCM {
        return to_scm(invoke(fn_, to_cpp<T1>(a1), to_cpp<T2>(a2),
                             to_cpp<T3>(a3)));
    };
}

template <typename Tag, typename Fn>
auto subr_wrapper_impl(Fn fn, pack<void>, pack<>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] () -> SCM { invoke(fn_); return SCM_UNSPECIFIED; };
}
template <typename Tag, typename Fn, typename T1>
auto subr_wrapper_impl(Fn fn, pack<void>, pack<T1>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1) -> SCM {
        invoke(fn_, to_cpp<T1>(a1)); return SCM_UNSPECIFIED;
    };
}
template <typename Tag, typename Fn, typename T1, typename T2>
auto subr_wrapper_impl(Fn fn, pack<void>, pack<T1, T2>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1, SCM a2) -> SCM {
        invoke(fn_, to_cpp<T1>(a1), to_cpp<T2>(a2));
        return SCM_UNSPECIFIED;
    };
}
template <typename Tag, typename Fn, typename T1, typename T2, typename T3>
auto subr_wrapper_impl(Fn fn, pack<void>, pack<T1, T2, T3>)
{
    check_call_once<Tag, Fn>();
    static const Fn fn_ = fn;
    return [] (SCM a1, SCM a2, SCM a3) -> SCM {
        invoke(fn_, to_cpp<T1>(a1), to_cpp<T2>(a2), to_cpp<T3>(a3));
        return SCM_UNSPECIFIED;
    };
}

template <typename Tag, typename Fn, typename... Args>
auto subr_wrapper_aux(Fn fn, pack<Args...>)
{
    return subr_wrapper_impl<Tag>(
        fn, pack<std::result_of_t<Fn(Args...)>>{}, pack<Args...>{});
}

template <typename Tag, typename Fn>
auto subr_wrapper(Fn fn)
{
    return subr_wrapper_aux<Tag>(fn, function_args_t<Fn>{});
}

} // anonymous namespace
} // namespace detail
} // namespace scm
