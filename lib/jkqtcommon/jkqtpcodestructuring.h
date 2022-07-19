/*
    Copyright (c) 2008-2022 Jan W. Krieger (<jan@jkrieger.de>)

    

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/



#ifndef JKQTPCODESTRUCTURING_H_INCLUDED
#define JKQTPCODESTRUCTURING_H_INCLUDED
#include <functional>


/** \brief C++11 finally construct (executes a callable-object when the edestructor is executed)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     JKQTPFinalAct __finalpaint([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
 *
 * \see JKQTPFinally()
 */
template <class F>
class JKQTPFinalAct
{
public:
    explicit JKQTPFinalAct(F f) noexcept
      : f_(std::move(f)), invoke_(true) {}

    JKQTPFinalAct(JKQTPFinalAct&& other) noexcept
     : f_(std::move(other.f_)),
       invoke_(other.invoke_)
    {
        other.invoke_ = false;
    }

    JKQTPFinalAct(const JKQTPFinalAct&) = delete;
    JKQTPFinalAct& operator=(const JKQTPFinalAct&) = delete;

    ~JKQTPFinalAct() noexcept
    {
        if (invoke_) f_();
    }

private:
    F f_;
    bool invoke_;
};

/** \brief C++11 finally construct (executes a callable-object at the end of a scope)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     auto __finalpaint=JKQTPFinally([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
 *
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(const F& f) noexcept
{
    return JKQTPFinalAct<F>(f);
}

/** \brief C++11 finally construct (executes a callable-object at the end of a scope)
 * \ingroup jkqtptools_codestructuring
 *
 * Typical usage:
 * \code
 *   {
 *     // the instruction 'painter.restore()' will be executed at the end
 *     // of the block, when __finalpaint is destroyed (see (*) below)
 *     auto __finalpaint=JKQTPFinally([&painter]() { painter.restore(); });
 *
 *     // ...
 *     // do something ...
 *     // ...
 *
 *   } // (*) 'painter.restore()' is executed before the end of this block!
 * \endcode
 *
 * \see JKQTPFinalAct
 */
template <class F>
inline JKQTPFinalAct<F> JKQTPFinally(F&& f) noexcept
{
    return JKQTPFinalAct<F>(std::forward<F>(f));
}


#endif // JKQTPCODESTRUCTURING_H_INCLUDED
