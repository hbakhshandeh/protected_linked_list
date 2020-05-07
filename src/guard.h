#ifndef GUARD_H_
#define GUARD_H_

#include <mutex>

template <typename mutex_type>
class Guard {
private:
	std::mutex &mtx;
	std::unique_lock<std::mutex> ul_guard;
public:
	explicit Guard(mutex_type &m) : mtx(m), ul_guard(mtx, std::defer_lock) {
		ul_guard.lock();
	}
	~Guard() {
		ul_guard.unlock();
	}
};



#endif /* GUARD_H_ */
