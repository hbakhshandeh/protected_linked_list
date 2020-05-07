
#include <thread>
#include <chrono>
#include "protected_linked_list.h"

enum class Numbers {
	threads = 3,
	fills = 3,
	remove =3
};

void fill_elements(ProtectLinkList<int> &plist) {
	for (int i = 0; i < static_cast<int>(Numbers::fills); ++i) {
		plist.append(i + i + 1);
	}
	plist.insert(2, 4);
}
void remove_elements(ProtectLinkList<int> &plist) {
	for (int i = 0; i < static_cast<int>(Numbers::remove); ++i) {
		plist.remove(i);
	}
}

int main() {
//	auto now = std::chrono::system_clock::now();

	ProtectLinkList<int> plist{};

	std::thread fill_threads[static_cast<int>(Numbers::threads)];
	for (int i = 0; i < static_cast<int>(Numbers::threads); ++i) {
		fill_threads[i] = std::thread(fill_elements, std::ref(plist));
	}
	std::thread del_thread(remove_elements, std::ref(plist));

	for (int i = 0; i < static_cast<int>(Numbers::threads); ++i) {
		fill_threads[i].join();
	}
	del_thread.join();

	std::cout << plist <<std::endl;

//	std::chrono::system_clock::duration end = std::chrono::system_clock::now() - now;
//	std::cout << std::chrono::duration<double>(end).count() << std::endl;

	return 0;
}
