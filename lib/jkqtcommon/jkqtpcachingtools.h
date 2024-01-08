/*
    Copyright (c) 2008-2024 Jan W. Krieger (<jan@jkrieger.de>)

    last modification: $LastChangedDate$  (revision $Rev$)

    This software is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License (LGPL) as published by
    the Free Software Foundation, either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License (LGPL) for more details.

    You should have received a copy of the GNU Lesser General Public License (LGPL)
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/





#ifndef JKQTPCACHINGTOOLS_H
#define JKQTPCACHINGTOOLS_H

#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <functional>

/** \brief this class can be used to implement a general cache for values
 *  \ingroup jkqtptools_concurrency
 *
 *  It is typically used to generate a static (ThreadSafe=true) of thread_local (ThreadSafe=false) cache inside a function.
 *
 *  The class is parametrized by a key (TKeay) and value (TData) data type and receives (in the constructor) a functor that
 *  calculates the data for a key. An additional template parameter ThreadSafe indicates (true|false) whether the class
 *  is implemented in a thread-safe way (for static instances) or not (then it should be used as thread_local instances in a
 *  multi-threaded environment or in a single-thread environment).
 *
 *  The class member function get(key) returns a value for a given key, which is either taken from the internal cache,
 *  or generated using the functor provided to the constructor. In the latter case the generated value is stored in the internal cache.
 *
 *  Internally the cache maps TKey to TData, but the signature of the get()-function and the generator functor actually uses TKeyInSignature,
 *  which may differ from TKey. The only limitation is that TKeyInSignature can be converted/assigned to a TKey
 */
template <class TData, class TKey, bool ThreadSafe=true, class TKeyInSignature=TKey>
struct JKQTPDataCache {
    template <typename FF>
    inline JKQTPDataCache(FF generateData):
        m_generateData(std::forward<FF>(generateData))
    {

    }

    template<class... Args>
    inline TData get_inline(Args... args) {
        return get(TKeyInSignature(args...));
    }

    template <bool TSS=ThreadSafe>
    inline TData get(const typename std::enable_if<TSS, TKeyInSignature>::type& key) {
        const TKey cacheKey=key;

        QReadLocker lockR(&m_mutex);
        if (m_cache.contains(cacheKey)) return m_cache[cacheKey];
        lockR.unlock();

        QWriteLocker lockW(&m_mutex);
        if (m_cache.contains(cacheKey)) return m_cache[cacheKey];

        const auto newData=m_generateData(key);
        m_cache[cacheKey]=newData;
        return newData;
    }

    template <bool TSS=ThreadSafe>
    inline TData get(const typename std::enable_if<!TSS, TKeyInSignature>::type& key) {
        const TKey cacheKey=key;

        if (m_cache.contains(cacheKey)) return m_cache[cacheKey];
        const auto newData=m_generateData(key);
        m_cache[cacheKey]=newData;
        return newData;
    }
private:
    QHash<TKey, TData> m_cache;
    mutable typename std::enable_if<ThreadSafe, QReadWriteLock>::type m_mutex;
    const std::function<TData(TKeyInSignature)> m_generateData;
    Q_DISABLE_COPY(JKQTPDataCache)
};

#endif // JKQTPCACHINGTOOLS_H
