#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#define LOG
//#undef LOG
#ifdef LOG
#include <iostream>
#endif
template<typename T>
struct Deleter {
	void operator()(T* ptr) {
#ifdef LOG
		std::cout << "default deleter called" << std::endl;
#endif
		delete ptr;
	}
};
template<typename T, typename DD = Deleter<T>>
class UP{
	public:
	UP(T* ptr = nullptr) : ptr_(ptr) {
#ifdef LOG
		std::cout << "UP c-tor" << std::endl;
#endif
	}
	UP(const UP&) = delete;
	UP& operator=(const UP&&) = delete;
	UP(UP&& that) : ptr_(that.ptr_) {
#ifdef LOG
		std::cout << "UP move c-tor" << std::endl;
#endif
		that.ptr_ = nullptr;
	}
	UP& operator=(UP&& that) {
#ifdef LOG
		std::cout << "UP move assign" << std::endl;
#endif
		if(that == *this) { return this; }
		defaultDeleter(ptr_);
		ptr_ = that.ptr_;
		that.ptr_ = nullptr;
	}
	~UP() {
#ifdef LOG
		std::cout << "UP d-tor" << std::endl;
#endif
		defaultDeleter(ptr_);
	}
	T& operator*() {
		return *ptr_;
	}
	T* operator->() {
		return ptr_;
	}
	T* get() {
		return ptr_;
	}
	private:
	T* ptr_;
	DD defaultDeleter;
};

template <typename T, typename ...ArgsPack>
UP<T> MakeUnique(ArgsPack&& ...args) {
	return UP<T>(new T(std::forward<ArgsPack>(args)...));
}

#endif