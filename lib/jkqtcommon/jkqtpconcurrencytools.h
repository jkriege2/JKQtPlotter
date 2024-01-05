/*
    Copyright (c) 2008-2023 Jan W. Krieger (<jan@jkrieger.de>)

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





#ifndef JKQTPCONCURRENCYTOOLS_H
#define JKQTPCONCURRENCYTOOLS_H

#include "jkqtcommon/jkqtcommon_imexport.h"
#include <QReadWriteLock>
#include <QReadLocker>
#include <QWriteLocker>
#include <mutex>

/** \brief template class that wraps any datatype and combines it with a mutex, exposes the lock()/unlock()
 *         interface, so access to the contained data can be synchronized
 *  \ingroup jkqtptools_concurrency
 *
 *
 */
template <class T>
class JKQTPSynchronized {
public:
    /** \brief Mutex used by this temmplate */
    typedef QReadWriteLock MutexType;

    /** \brief type of AdoptLock tag, which is used in ReadLocker and WriteLocker to adopt a pre-locked JKQTPSynchronized<T> */
    struct AdoptLockType { explicit AdoptLockType() = default; };
    /** \brief tag, which is used in ReadLocker and WriteLocker to adopt a pre-locked JKQTPSynchronized<T> */
    static constexpr AdoptLockType	AdoptLock { };

    /** \brief type of a lock_guard for a JKQTPSynchronized<T> for reading */
    class ReadLocker
    {
    public:
        inline ReadLocker(const JKQTPSynchronized<T> &sync) noexcept: m_sync(sync) { m_sync.lockForRead(); };
        inline ReadLocker(const JKQTPSynchronized<T> &sync, AdoptLockType) noexcept : m_sync(sync) { };
        inline ~ReadLocker() { m_sync.unlock(); }
    private:
        Q_DISABLE_COPY(ReadLocker)
        const JKQTPSynchronized<T> &m_sync;
    };

    /** \brief type of a lock_guard for a JKQTPSynchronized<T> for writing */
    class WriteLocker
    {
    public:
        inline WriteLocker(JKQTPSynchronized<T> &sync) noexcept: m_sync(sync) { m_sync.lockForWrite(); };
        inline WriteLocker(JKQTPSynchronized<T> &sync, AdoptLockType) noexcept : m_sync(sync) { };
        inline ~WriteLocker() { m_sync.unlock(); }
    private:
        Q_DISABLE_COPY(WriteLocker)
        JKQTPSynchronized<T> &m_sync;
    };

    /** \brief type of a lock_guard for a JKQTPSynchronized<T> for writing */
    typedef JKQTPSynchronized<T>::WriteLocker Locker;
    /** \brief contained data type T */
    typedef T data_type;
    /** \brief default constructor, the internal data is default-initialized */
    inline JKQTPSynchronized(): m_data() {}
    /** \brief initializing constructor, the internal data is initialized with \a d */
    inline JKQTPSynchronized(const T& d): m_data(d) {}
    /** \brief initializing constructor, the internal data is initialized with \a d */
    inline JKQTPSynchronized(T&& d): m_data(std::forward<T>(d)) {}
    /** \brief locks \a other and copies its contents to this new class */
    inline JKQTPSynchronized(const JKQTPSynchronized<T>& other): m_data() {
        Locker lock(other);
        m_data=other.m_data;
    }
    /** \brief locks \a other and moves its contents to this new class. The mutex in \a other is NOT moved! */
    inline JKQTPSynchronized(JKQTPSynchronized<T>&& other): m_data() {
        Locker lock(other);
        m_data=std::move(other.m_data);
    }

    /** \brief locks the internal mutex for writing, until unlock() is called
     *
     *  \note Use WriteLocker or Locker instances to actually lock, using a RAII-idiom, as this is safer than doing this by hand!
     */
    inline void lock() const {
        lockForWrite();
    }
    /** \brief locks the internal mutex for writing, until unlock() is called
     *
     *  \note Use WriteLocker or Locker instances to actually lock, using a RAII-idiom, as this is safer than doing this by hand!
     */
    inline void lockForWrite() const {
        m_mutex.lockForWrite();
    }
    /** \brief locks the internal mutex for writing, until unlock() is called
     *
     *  \note Use WriteLocker or Locker instances to actually lock, using a RAII-idiom, as this is safer than doing this by hand!
     */
    inline void lockForRead() const {
        m_mutex.lockForRead();
    }
    /** \brief unlocks the internal mutex from a previous lock(), lockForWrite() or lockForRead() call
     *
     *  \note Use Locker instances to actually lock, using a RAII-idiom, as this is safer than doing this by hand!
     */
    inline void unlock() const {
        m_mutex.unlock();
    }
    /** \brief assign a value to the internal data storage, <b>not thread-safe.</b>
     *
     *  \note You need to lock this object before calling this function.
     */
    inline void set(const T& d) {
        m_data=d;
    }
    /** \brief assign a value to the internal data storage, <b>not thread-safe</b>.
     *
     *  \note You need to lock this object before calling this function.
     */
    inline void set(T&& d) {
        m_data=std::forward<T>(d);
    }
    /** \brief assign a value to the internal data storage, <b>thread-safe</b>.
     */
    inline void set_safe(const T& d) {
        Locker lck(m_mutex);
        m_data=d;
    }
    /** \brief assign a value to the internal data storage, \b thread-safe.
     */
    inline void set_safe(T&& d) {
        Locker lck(m_mutex);
        m_data=std::forward<T>(d);
    }
    /** \brief returns the internal data as a mutable reference, <b>not thread-safe</b>.
     *
     *  \note You need to lock this object before calling this function.
     */
    inline T& get() {
        return m_data;
    }
    /** \brief returns the internal data as a mutable reference, <b>not thread-safe</b>.
     *
     *  \note You need to lock this object before calling this function.
     */
    inline const T& get() const {
        return m_data;
    }
    /** \brief gives access to the internal data, <b>not thread-safe</b>.
     *
     *  \note You need to lock this object before calling this function.
     */
    inline const T* operator->() const {
        return &m_data;
    }
    /** \brief gives access to the internal data, <b>not thread-safe</b>.
     *
     *  \note You need to lock this object before calling this function.
     */
    inline T* operator->() {
        return &m_data;
    }
    /** \brief returns the value in the internal data storage, <b>thread-safe</b>.
     */
    inline T get_safe() const {
        ReadLocker lck(this);
        return m_data;
    }

private:
    T m_data;
    mutable MutexType m_mutex;

};

#endif // JKQTPCONCURRENCYTOOLS_H
