#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

#include <utility>

template<typename T>
struct Deleter {
	void operator()(T* ptr) {
		delete ptr;
	}
};
template<typename T, typename DD = Deleter<T>>
class UP{
	public:
	UP(T* ptr = nullptr) : ptr_(ptr) {
	}
	UP(const UP&) = delete;
	UP& operator=(const UP&&) = delete;
	UP(UP&& that) : ptr_(that.ptr_) {
		that.ptr_ = nullptr;
	}
	UP& operator=(UP&& that) {
		if(that == *this) { return this; }
		defaultDeleter(ptr_);
		ptr_ = that.ptr_;
		that.ptr_ = nullptr;
	}
	~UP() {
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