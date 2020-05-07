#ifndef NODE_H_
#define NODE_H_

#include <iostream>
#include <cassert>

template<typename element>
class Node {
private:
	static Node<element> *node_pool;

public:
	element value{};
	Node *next{nullptr};

	explicit Node(const element &val, Node *nextval = nullptr) :
			value(val), next(nextval) {
	}

	Node(Node *nextval = nullptr) :
			next(nextval) {
	}

	~Node() {
	}

	void* operator new(size_t t) {
		if (node_pool == nullptr) {
			return ::new Node{};
		}
		Node<element> *temp = node_pool;
		node_pool = node_pool->next;
		return temp;
	}

	void operator delete(void *ptr) {
		static_cast<Node<element>*>(ptr)->next = node_pool;
		node_pool = (Node<element>*) ptr;
	}

	Node(const Node &rhs) :
			value(rhs.value), next(rhs.next) {
	}

	Node& operator =(const Node &rhs) {
		value = rhs.value;
		next = rhs.next;
		return *this;
	}
	Node(Node &&rhs) noexcept :
			value(std::move(rhs.value), next(std::move(rhs.next))) {
	}

	Node& operator=(Node &&rhs) noexcept {
		value = std::move(rhs.value);
		next = std::move(rhs.next);
		return *this;
	}

	bool operator==(const Node &rhs) {
		if (*this != rhs) {
			if (value == rhs.value && next == rhs.next) {
				return true;
			}
		}
		return false;
	}

	friend std::ostream& operator<<(std::ostream &out, const Node<element> *node) {
		if (node == nullptr) {
			out << "Node is null...\n";
		} else {
			out << node->value;
		}
		return out;
	}
};

template<typename element>
Node<element>* Node<element>::node_pool = nullptr;


#endif /* NODE_H_ */
