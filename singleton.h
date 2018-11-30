/************************************************************
 *	Copyright (C), 2018, SUZHOU, WSL
 *	@file:		singleton.h
 *	@author:	ShuangLongWu<340144967@qq.com>
 *	@date:		2018/07/30
 *	@brief:		单例模板
 *
 *	@note:
 ************************************************************/
#ifndef COMMON_SINGLETON_H_
#define COMMON_SINGLETON_H_
#include <memory>

template <typename T>
class Singleton {
    using SingletonOPtr = std::unique_ptr<T>;

public:
    template <typename... Args>
    static T& get(Args&&... args)
    {
        static SingletonOPtr instance_(new T(std::forward<Args>(args)...));
        return *instance_;
    }

    constexpr Singleton(const Singleton&) = delete;
    constexpr Singleton& operator=(const Singleton&) = delete;

protected:
    Singleton()  = default;
    ~Singleton() = default;
};
#endif  // !COMMON_SINGLETON_H_
