#pragma once
#include "StackImplementation.h"
#include "SinglyLinkedList.h"

template<class T>
class ListStack : public StackImplementation<T>, public SLL<T> {
public:
	ListStack();

	ListStack(const ListStack<T>& copy);
	ListStack<T>& operator=(const ListStack<T>& copy);

	ListStack(ListStack<T>&& other) noexcept;
	ListStack<T>& operator=(ListStack<T>&& other) noexcept;

	~ListStack() = default;

	// добавление в конец
	void push(const T& value) override;
	// удаление с хвоста
	void pop() override;
	// посмотреть элемент в хвосте
	const T& top() const override;
	// проверка на пустоту
	bool isEmpty() const override;
	// размер
	size_t size() const override;
private:
	SLL<T> _listStack;
};


template<class T>
ListStack<T>::ListStack() {
	_listStack = SLL<T>();
}

template<class T>
ListStack<T>::ListStack(const ListStack<T>& copy) {
	_listStack = copy._listStack;
}

template<class T>
ListStack<T>& ListStack<T>::operator=(const ListStack<T>& copy) {
	_listStack = copy._listStack;
	return *this;
}

template<class T>
ListStack<T>::ListStack(ListStack<T>&& other) noexcept {
	_listStack = std::move(other._listStack);
}

template<class T>
ListStack<T>& ListStack<T>::operator=(ListStack<T>&& other) noexcept {
	_listStack = std::move(other._listStack);
	return *this;
}

template<class T>
void ListStack<T>::push(const T& value) {
	_listStack.pushBack(value);
}

template<class T>
void ListStack<T>::pop() {
	_listStack.popBack();
}

template<class T>
const T& ListStack<T>::top() const {
	return _listStack.at(size() - 1);
}

template<class T>
bool ListStack<T>::isEmpty() const {
	return _listStack.isEmpty();
}

template<class T>
size_t ListStack<T>::size() const {
	return _listStack.size();
}
