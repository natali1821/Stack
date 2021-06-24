#pragma once
#include <iostream>
#include <utility>
#include <exception>
#include <cstdlib>

template<class T>
class SLL {
private:
	class Node {
	public:
		Node* _next;
		T _data;
		Node(T data) {
			_data = data;
			_next = nullptr;
		}
	};
	Node* _head;
	size_t _size;
public:
	class Iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = T;
		using pointer           = T*;
		using reference         = T&;
		Iterator(Node* ptr);
		reference operator*();
		pointer operator->();
		Iterator& operator++(); //prefix
		Iterator operator++(int); //postfix
		bool operator!=(const Iterator& other);
		bool operator==(const Iterator& other);
		difference_type operator-(const Iterator& other);
		Node* getPtr() const;
	private:
		Node* _ptr;
	};

	SLL();

	//the rule of five
	SLL(const SLL& other);
	SLL(SLL<T>&& other) noexcept;

	SLL& operator=(const SLL& other);
	SLL& operator=(SLL<T>&& other) noexcept;

	~SLL();

	const T& at(const size_t pos) const;
	T& at(const size_t pos);
	const T& operator[](const size_t pos) const;
	T& operator[](const size_t pos);
	Node* getNode(const size_t pos) const;

	size_t getIndex(Node* node);

	//insert
	void insert(size_t idx, const T& value);
	void insertAfterNode(Node* node, const T& value);
	void pushBack(const T& value);
	void pushFront(const T& value);

	//remove
	void clear();
	void remove(size_t idx);
	void removeNextNode(Node* node);
	void popBack();
	void popFront();

	// search, О(n)
	long long int findIndex(const T& value) const;
	Node* findNode(const T& value) const;

	// разворот списка
	void reverse();						// изменение текущего списка
	SLL<T> reverse() const;			// полчение нового списка (для константных объектов)
	SLL<T> getReverseList() const;	// чтобы неконстантный объект тоже мог возвращать новый развернутый список

	size_t size() const;
	void print();
	bool isEmpty() const;

	void forEach(T (*fn)(T));
	SLL<T> map(T (*fn)(T));
	void filter(bool (*fn)(T));

	Iterator begin() const;
	Iterator end() const;
};

//Iterator
template<class T>
SLL<T>::Iterator::Iterator(Node* ptr) : _ptr(ptr) {}

template<class T>
T& SLL<T>::Iterator::operator*() {
	return _ptr->_data;
}

template<class T>
T* SLL<T>::Iterator::operator->() {
	return &(_ptr->_data);
}

template<class T>
class SLL<T>::Iterator& SLL<T>::Iterator::operator++() {
	_ptr = _ptr->_next;
	return *this;
}

template<class T>
class SLL<T>::Iterator SLL<T>::Iterator::operator++(int) {
	Iterator tmp = *this;
	++(*this);
	return tmp;
}

template<class T>
bool SLL<T>::Iterator::operator!=(const Iterator& other) {
	return _ptr != other._ptr;
}

template<class T>
bool SLL<T>::Iterator::operator==(const Iterator& other) {
	return _ptr == other._ptr;
}

template<class T>
std::ptrdiff_t SLL<T>::Iterator::operator-(const Iterator& other) {
	return _ptr - other._ptr;
}

template<class T>
class SLL<T>::Node* SLL<T>::Iterator::getPtr() const{
	return _ptr;
}

//SinglyLinkedList
template<class T>
SLL<T>::SLL() {
	_head = nullptr;
	_size = 0;
}

template<class T>
SLL<T>::SLL(const SLL& other) {
	if (other._head) {
		Node* tmp = other._head;
		Node* cur = new Node(tmp->_data);
		_head = cur;
		while (tmp->_next) {
			tmp = tmp->_next;
			cur->_next = new Node(tmp->_data);
			cur = cur->_next;
		}
		_size = other.size();
	}
	else {
		_head = nullptr;
		_size = 0;
	}
}

template<class T>
SLL<T>::SLL(SLL<T>&& other) noexcept{
	_size = std::exchange(other._size, 0);
	_head = std::exchange(other._head, nullptr);
}

template<class T>
SLL<T>& SLL<T>::operator=(const SLL& other){
	if (other.isEmpty()) {
		clear();
		_head = nullptr;
		_size = 0;
		return *this;
	}
	if (this != &other) {
		_size = other.size();
		clear();
		Node* tmp = other._head;
		Node* cur = new Node(tmp->_data);
		_head = cur;
		while (tmp->_next) {
			tmp = tmp->_next;
			cur->_next = new Node(tmp->_data);
			cur = cur->_next;
		}
	}
	return *this;
}

template<class T>
SLL<T>& SLL<T>::operator=(SLL<T>&& other) noexcept{
	if (this != &other) {
		clear();
		_size = std::exchange(other._size, 0);
		_head = std::exchange(other._head, nullptr);
	}
	return *this;
}

template<class T>
SLL<T>::~SLL() {
	clear();
}

template<class T>
const T& SLL<T>::at(const size_t pos) const {
	if (pos >= size()) {
		throw std::out_of_range("at at(): position >= size of list");
	}
	Node* cur = _head;
	for (size_t i = 0; i < pos; ++i) {
		cur = cur->_next;
	}
	return cur->_data;
}

template<class T>
T& SLL<T>::at(const size_t pos) {
	if (pos >= size()) {
		throw std::out_of_range("at at(): position >= size of list");
	}
	Node* cur = _head;
	for(size_t i = 0; i < pos; ++i) {
		cur = cur->_next;
	}
	return cur->_data;
}

template<class T>
const T& SLL<T>::operator[](const size_t pos) const{
	return at(pos);
}

template<class T>
T& SLL<T>::operator[](const size_t pos) {
	return at(pos);
}

template<class T>
class SLL<T>::Node* SLL<T>::getNode(const size_t pos) const{
	if (pos >= size()) {
		throw std::out_of_range("at getNode() : position >+ size of list");
	}
	Node* cur = _head;
	for (size_t i = 0; i < pos; ++i) {
		cur = cur->_next;
	}
	return cur;
}

template<class T>
size_t SLL<T>::getIndex(Node* node) {
	Node* cur = _head;
	size_t pos = 0;
	while(cur->_next) {
		if (cur == node) {
			break;
		}
		++pos;
		cur = cur->_next;
	}
	return pos;
}

template<class T>
void SLL<T>::insert(size_t idx, const T& value) {
	if (idx > size()) {
		throw std::out_of_range("at insert(): position > size of list");
	}
	if (isEmpty()) {
		_head = new Node(value);
		++_size;
		return;
	}
	if (!idx) {
		Node* tmp = _head;
		_head = new Node(value);
		_head->_next = tmp;
	}
	else {
		size_t pos;
		Node* cur = _head;
		for (pos = 0; pos < idx - 1; ++pos) {
			cur = cur->_next;
		}
		Node* tmp = new Node(value);
		tmp->_next = cur->_next;
		cur->_next = tmp;
	}
	++_size;
}

template<class T>
void SLL<T>::insertAfterNode(Node* node, const T& value){
	size_t i = getIndex(node);
	insert(i + 1, value);
}

template<class T>
void SLL<T>::pushBack(const T& value) {
	/*if (!_head) {
		_head = new Node(value);
	}
	else {
		Node* cur = _head;
		while(cur->_next) {
			cur = cur->_next;
		}
		cur->_next = new Node(value);
	}*/
	insert(size(), value);
}

template<class T>
void SLL<T>::pushFront(const T& value) {
	insert(0, value);
}

template<class T>
void SLL<T>::clear(){
	while (_size) {
		popBack();
	}
}

template<class T>
void SLL<T>::remove(size_t idx) {
	if (isEmpty()) {
		return;
	}
	if (idx >= size()) {
		throw std::out_of_range("at erase(): position >= size of list");
	}
	if (!idx) {
		Node* tmp = _head;
		_head = _head->_next;
		delete[] tmp;
	}
	else {
		Node* cur = _head;
		for (size_t pos = 0; pos < idx - 1; ++pos) {
			cur = cur->_next;
		}
		Node* tmp = cur->_next;
		cur->_next = tmp->_next;
		delete[] tmp;
	}
	--_size;
}

template<class T>
void SLL<T>::removeNextNode(Node* node) {
	size_t i = getIndex(node);
	remove(i + 1);
}

template<class T>
void SLL<T>::popBack() {
	remove(size() - 1);
}

template<class T>
void SLL<T>::popFront() {
	remove(0);
}

template<class T>
long long int SLL<T>::findIndex(const T& value) const {
	Node* cur = _head;
	long long int i = 0;
	while (cur->_next) {
		if (cur->_data == value) {
			return i;
		}
		cur = cur->_next;
		++i;
	}
	return -1;
}

template<class T>
class SLL<T>::Node* SLL<T>::findNode(const T& value) const {
	auto it = begin();
	while (it != end()) {
		if (*it == value) {
			return it.getPtr();
		}
		++it;
	}
	return nullptr;
}

template<class T>
void SLL<T>::reverse() {
	Node* prev = nullptr;
	Node* cur = _head;
	while (cur) {
		Node* tmp = cur->_next; //save next element on list
		cur->_next = prev; // link current element with previos
		prev = cur;
		cur = tmp; //move to next element on list
	}
	_head = prev;
}

template<class T>
SLL<T> SLL<T>::reverse() const {
	SLL<T> tmp = *this;
	tmp.reverse();
	return tmp;
}

template<class T>
SLL<T> SLL<T>::getReverseList() const {
	SLL<T> tmp = *this;
	tmp.reverse();
	return tmp;
}

template<class T>
size_t SLL<T>::size() const{
	return _size;
}

template<class T>
void SLL<T>::print() {
	if (!_head) {
		std::cout << "nullptr" << std::endl;
	}
	else {
		Node* cur = _head;
		do {
			std::cout << cur->_data << "->";
		} while(cur = cur->_next);
		std::cout << "nullptr" << std::endl;
	}
}

template<class T>
bool SLL<T>::isEmpty() const {
	return !size();
}

template<class T>
void SLL<T>::forEach(T (*fn)(T)) {
	if (isEmpty()) {
		return;
	}
	Node* cur = _head;
	while (cur) {
		cur->_data = fn(cur->_data);
		cur = cur->_next;
	}
}

template<class T>
SLL<T> SLL<T>::map(T (*fn)(T)) {
	SLL<T> tmp(*this);
	tmp.forEach(fn);
	return tmp;
}

template<class T>
void SLL<T>::filter(bool (*fn)(T)) {
	if (isEmpty()) {
		return;
	}
	int pos = 0;
	Node* cur = _head;
	while (cur){
		if (fn(cur->_data)) {
			cur = cur->_next;
		}
		else {
			cur = cur->_next;
			remove(pos);
			--pos;
		}
		++pos;
	}
}

template<class T>
class SLL<T>::Iterator SLL<T>::begin() const{
	return SLL::Iterator(_head);
}

template<class T>
class SLL<T>::Iterator SLL<T>::end() const {
	return SLL::Iterator(nullptr);
}
