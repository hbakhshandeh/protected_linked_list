#ifndef PROTECTED_LINKED_LIST_H_
#define PROTECTED_LINKED_LIST_H_

#include "node.h"
#include "guard.h"
#include <cassert>
#include <mutex>

template<typename element>
class ProtectLinkList {
private:
	using node_pointer = Node<element>*;

	node_pointer head;
	node_pointer tail;
	node_pointer fence;
	int leftcnt;
	int rightcnt;

	std::mutex mtx;

	void init() {
		fence = tail = head = nullptr;
		leftcnt = rightcnt = 0;
	}

	void remove_all() {
		while (head != nullptr) {
			fence = head;
			head = head->next;
			delete fence;
		}
	}

	void prev() {
		if (fence == head) {
			return;
		}
		node_pointer temp = head;
		while (temp->next != fence) {
			temp = temp->next;
		}
		fence = temp;
		leftcnt--;
		rightcnt++;
	}

	void next() {
		if (fence != tail) {
			fence = fence->next;
			rightcnt--;
			leftcnt++;
		}
	}

	void move_start() {
		fence = head;
		rightcnt += leftcnt;
		leftcnt = 0;
	}

	void move_end() {
		fence = tail;
		leftcnt += rightcnt;
		rightcnt = 0;
	}

	inline int left_length() const {
		return leftcnt;
	}

	inline int right_length() const {
		return rightcnt;
	}

	void move(const int position) {
		assert((position >= 0) && (position <= rightcnt + leftcnt));

		rightcnt = rightcnt + leftcnt - position; // Set counts
		leftcnt = position;
		fence = head;
		for (int i = 1; i < position; i++) {
			fence = fence->next;
		}
	}

	void insert_head(const element &value) {
		head = new Node<element>(value, head->next);
		++rightcnt;
	}

	void remove_head() {
		assert(!empty() && "List is empty...");

		node_pointer tmp = head->next;
		delete head;
		head = tmp;
		--leftcnt;
	}

public:
	ProtectLinkList() {
		init();
	}

	~ProtectLinkList() {
		remove_all();
	}

	ProtectLinkList(const ProtectLinkList&) = delete;
	ProtectLinkList operator=(const ProtectLinkList&) = delete;
	ProtectLinkList(ProtectLinkList&&) = delete;
	ProtectLinkList& operator=(ProtectLinkList &&) = delete;

	void clear() {
		Guard<std::mutex> {mtx};

		remove_all();
		init();
	}

	bool insert(int position, const element &value) {
		Guard<std::mutex> {mtx};

		assert(right_length() > 0 && "Position is wrong...");

		move(position);

		if (leftcnt == 0 && rightcnt == 0) {
			head = tail = fence = new Node<element>(value, nullptr);
		} else {
			fence->next = new Node<element>(value, fence->next);
			if (tail == fence) {
				tail = fence->next;
			}
		}

		rightcnt++;
		return true;
	}

	bool append(const element &value) {
		Guard<std::mutex> {mtx};

		node_pointer nod = new Node<element>(value, nullptr);
		if (head == nullptr && tail == nullptr) {
			head = tail = fence = nod;
		} else {
			tail = tail->next = nod;
		}
		++rightcnt;

		return true;
	}

	element remove(const int position) {
		Guard<std::mutex> {mtx};

		assert(right_length() > 0 && "Position is wrong...");

		move(position);

		element val = fence->value;
		node_pointer ltemp = fence->next;
		fence->next = ltemp->next;
		if (tail == ltemp) {
			tail = fence;
		}
		delete ltemp;
		--rightcnt;

		return val;
	}

	element get_value(const int position) {
		Guard<std::mutex> {mtx};

		assert(right_length() > 0 && "Position is wrong...");

		move(position);

		return fence->value;
	}

	int size() const {
		return rightcnt + leftcnt;
	}

	bool empty() const {
		return size() < 0;
	}

	node_pointer find(const element &value) {
		Guard<std::mutex> {mtx};

		if (empty()) {
			return nullptr;
		}

		move_start();
		while (fence != nullptr) {
			if (fence->value == value) {
				return fence;
			}
			next();
		}

		return nullptr;
	}

	friend std::ostream& operator<<(std::ostream &out,
			const ProtectLinkList<element> &pllist) {

		if (pllist.empty()) {
			out << "List is empty...\n";
			return out;
		}
		std::cout << "List size: " << pllist.size() << std::endl;

		node_pointer node = pllist.head;
		while (node != nullptr) {
			out << node << " ";
			node = node->next;
		}

		return out;
	}
};

#endif /* PROTECTED_LINKED_LIST_H_ */
