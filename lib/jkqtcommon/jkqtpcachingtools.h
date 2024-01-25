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
#include "jkqtcommon/jkqtpmathtools.h"
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <functional>
#include <chrono>
#include <atomic>
#include <algorithm>
#include <memory>
#include <unordered_map>

/** \brief tag type to configure JKQTPDataCache for thread-safety
 *  \ingroup jkqtptools_concurrency
 */
struct JKQTPDataCacheThreadSafe {};
/** \brief tag type to configure JKQTPDataCache for non thread-safety
 *  \ingroup jkqtptools_concurrency
 */
struct JKQTPDataCacheNotThreadSafe {};

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
 *
 *  The cache has a maximmum size m_maxEntries.
 *  When you try to add a new object, after which the size would grow beyond this, a fraction 1-m_retainFraction of elements are
 *  deleted from the cache. The delete strategy is least-recently used (LRU). In order to immplement this, the cache keeps track of
 *  the last use timestamp of each entry.
 *
 *  You can deactivate the cleaning by setting m_maxEntries<0, but the the cache may grow indefinitely and there is possibly undefined behaviour
 *  when add one too many items!
 */
template <class TData, class TKey, typename ThreadSafe=JKQTPDataCacheThreadSafe, class TKeyInSignature=TKey>
struct JKQTPDataCache {
    template <typename FF>
    inline JKQTPDataCache(FF generateData, int maxEntries=10000, double retainFraction=0.8):
        m_maxEntries(maxEntries), m_retainFraction(retainFraction), m_generateData(std::forward<FF>(generateData))
    {

    }
    JKQTPDataCache()=delete;
    JKQTPDataCache(const JKQTPDataCache&)=delete;
    JKQTPDataCache& operator=(const JKQTPDataCache&)=delete;
    JKQTPDataCache(JKQTPDataCache&&)=default;
    JKQTPDataCache& operator=(JKQTPDataCache&&)=default;

    template<class... Args>
    inline TData get_inline(Args... args) {
        return get(TKeyInSignature(args...));
    }

    template <typename TSS=ThreadSafe>
    inline TData get(const typename std::enable_if<std::is_same<JKQTPDataCacheThreadSafe, TSS>::value, TKeyInSignature>::type& key) {
        const TKey cacheKey=key;

        QReadLocker lockR(&m_mutex);
        auto it=m_cache.find(cacheKey);
        if (m_cache.end()!=it) {
            m_cacheLastUseTimestamps[cacheKey]->exchange(currenTimestamp());
            return it->second;
        }
        lockR.unlock();

        QWriteLocker lockW(&m_mutex);
        it=m_cache.find(cacheKey);
        if (m_cache.end()!=it) {
            m_cacheLastUseTimestamps.at(cacheKey)->exchange(currenTimestamp());
            return it->second;
        }
        if (m_maxEntries>0 && m_cache.size()>=static_cast<size_t>(m_maxEntries)) cleanCache_notThreadSafe();
        m_cacheLastUseTimestamps.emplace(cacheKey, std::make_shared<std::atomic<int64_t> >(currenTimestamp()));
        const auto newData=m_generateData(key);
        m_cache.emplace(cacheKey,newData);
        return newData;
    }

    template <typename TSS=ThreadSafe>
    inline TData get(const typename std::enable_if<std::is_same<JKQTPDataCacheNotThreadSafe, TSS>::value, TKeyInSignature>::type& key) {
        const TKey cacheKey=key;

        auto it=m_cache.find(cacheKey);
        if (m_cache.end()!=it) {
            m_cacheLastUseTimestamps[cacheKey]->exchange(currenTimestamp());
            return it->second;
        }
        if (m_maxEntries>0 && m_cache.size()>=static_cast<size_t>(m_maxEntries)) cleanCache_notThreadSafe();
        const auto newData=m_generateData(key);
        m_cache.emplace(cacheKey,newData);
        m_cacheLastUseTimestamps.emplace(cacheKey, std::make_shared<std::atomic<int64_t> >(currenTimestamp()));
        return newData;
    }

    template <typename TSS=ThreadSafe>
    inline bool contains(const typename std::enable_if<std::is_same<JKQTPDataCacheThreadSafe, TSS>::value, TKeyInSignature>::type& key) const {
        const TKey cacheKey=key;

        QReadLocker lockR(&m_mutex);
        return m_cache.find(cacheKey)!=m_cache.end();
    }

    template <typename TSS=ThreadSafe>
    inline bool contains(const typename std::enable_if<std::is_same<JKQTPDataCacheNotThreadSafe, TSS>::value, TKeyInSignature>::type& key) const {
        const TKey cacheKey=key;
        return m_cache.find(cacheKey)!=m_cache.end();
    }


    inline int size() const {
        return size_impl<ThreadSafe>();
    }

private:
    template <typename TSS>
    inline typename std::enable_if<std::is_same<JKQTPDataCacheThreadSafe, TSS>::value, int>::type size_impl() const {
        QReadLocker lockR(&m_mutex);
        return m_cache.size();
    }

    template <typename TSS>
    inline typename std::enable_if<std::is_same<JKQTPDataCacheNotThreadSafe, TSS>::value, int>::type size_impl() const {
        return m_cache.size();
    }

    /** \brief generate a timestamp */
    static inline int64_t currenTimestamp() {
        static auto firstTime=std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-firstTime).count();
    }
    /** \brief clean the cache, so at m_retainFraction*m_maxEntries entries remain. */
    inline void cleanCache_notThreadSafe() {
        if (m_maxEntries<0 || m_cache.size()<static_cast<size_t>(m_maxEntries)) return;
        const int deleteItems=jkqtp_boundedRoundTo<int>(1, (1.0-m_retainFraction)*static_cast<double>(m_cache.size()), m_cache.size());
        QList<QPair<TKey,int64_t> > allItems;
        allItems.reserve(m_cacheLastUseTimestamps.size());
        for (auto it=m_cacheLastUseTimestamps.begin(); it!=m_cacheLastUseTimestamps.end(); ++it) {
            allItems.emplaceBack(it->first, it->second->load());
        }
        std::sort(allItems.begin(), allItems.end(), [](const QPair<TKey,int64_t>&a, const QPair<TKey,int64_t>&b) {return a.second>b.second;});
        for (int i=0; i<deleteItems; i++) {
            m_cache.erase(allItems[i].first);
            m_cacheLastUseTimestamps.erase(allItems[i].first);
        }
    }
    const int m_maxEntries;
    const double m_retainFraction;
    std::unordered_map<TKey, TData> m_cache;
    std::unordered_map<TKey, std::shared_ptr<std::atomic<int64_t> > > m_cacheLastUseTimestamps;
    mutable QReadWriteLock m_mutex;
    const std::function<TData(TKeyInSignature)> m_generateData;
};

#endif // JKQTPCACHINGTOOLS_H
