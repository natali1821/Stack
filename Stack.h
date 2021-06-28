#pragma once
#include "MyVectorStack.h"
#include "SinglyLinkedListStack.h"
#include "StackImplementation.h"
#include <stdexcept>
#include <utility>
// уровень абстракции
// клиентский код подключает именно этот хедер

// на основе какого контейнера работает стек
enum class StackContainer {
	Vector = 0,
	List,
	// можно дополнять другими контейнерами
};

// декларация класса с реализацией
template<class T>
class StackImplementation;

template<class T>
class Stack {
public:
	// большая пятерка
	Stack(StackContainer = StackContainer::Vector);
	// элементы массива последовательно подкладываются в стек
	Stack(const T* valueArray, const size_t arraySize,
			StackContainer container = StackContainer::Vector);

	explicit Stack(const Stack& copy);
	Stack& operator=(const Stack& copy);

	Stack(Stack&& moveStack) noexcept;
	Stack& operator=(Stack&& moveStack) noexcept;

	~Stack();

	// добавление в хвост
	void push(const T& value);
	// удаление с хвоста
	void pop();
	// посмотреть элемент в хвосте
	T& top();
	const T& top() const;
	// проверка на пустоту
	bool isEmpty() const;
	// размер
	size_t size() const;
private:
	// указатель на имплементацию (уровень реализации)
	StackImplementation<T>* _pimpl = nullptr;
	// тип контейнера, наверняка понадобится
	StackContainer _containerType;
};


template<class T>
Stack<T>::Stack(StackContainer container)
	: _containerType(container)
{
	switch(container) {
	case(StackContainer::Vector):
		_pimpl = new VectorStack<T>();
		break;
	case(StackContainer::List):
		_pimpl = new ListStack<T>();
		break;
	default:
		throw std::invalid_argument("Invalid type of container");
	}
}

template<class T>
Stack<T>::Stack(const T* valueArray, const size_t arraySize, StackContainer container)
	: _containerType(container)
{
	switch(container) {
	case(StackContainer::Vector):
		_pimpl = new VectorStack<T>();
		break;
	case(StackContainer::List):
		_pimpl = new ListStack<T>();
		break;
	default:
		throw std::invalid_argument("Invalid type of container");
	}
	for (size_t i = 0; i < arraySize; ++i) {
		_pimpl->push(valueArray[i]);
	}
}

template<class T>
Stack<T>::Stack(const Stack& copy)
	: _containerType(copy._containerType)
{
	switch(_containerType) {
	case(StackContainer::Vector):
		_pimpl = new VectorStack<T>((VectorStack<T>)(*copy._pimpl));
		break;
	case(StackContainer::List):
		_pimpl = new ListStack<T>((ListStack<T>)(*copy._pimple));
		break;
	default:
		throw std::invalid_argument("Invalid type of container");
	}
}

template<class T>
Stack<T>& Stack<T>::operator=(const Stack& copy) {
	if (this != &copy) {
		_containerType = copy._containerType;
		switch(_containerType) {
		case(StackContainer::Vector):
			_pimpl = new VectorStack<T>((VectorStack<T>)(*copy._pimpl));
			break;
		case(StackContainer::List):
			_pimpl = new ListStack<T>((ListStack<T>)(*copy._pimple));
			break;
		default:
			throw std::invalid_argument("Invalid type of container");
		}
	}
	return *this;
}

template<class T>
Stack<T>::Stack(Stack&& moveStack) noexcept{
	_pimpl = std::exchange(moveStack._pimpl, nullptr);
	_containerType = moveStack._containerType;
}

template<class T>
Stack<T>& Stack<T>::operator=(Stack&& moveStack) noexcept{
	if (this != &moveStack) {
		_pimpl = std::exchange(moveStack._pimpl, nullptr);
		_containerType = moveStack._containerType;
	}
	return *this;
}

template<class T>
Stack<T>::~Stack() {
	delete[] _pimpl;
}

template<class T>
void Stack<T>::push(const T& value) {
	_pimpl->push(value);
}

template<class T>
void Stack<T>::pop() {
	_pimpl->pop();
}

template<class T>
T& Stack<T>::top() {
	return _pimpl->top();
}

template<class T>
const T& Stack<T>::top() const{
	return _pimpl->top();
}

template<class T>
bool Stack<T>::isEmpty() const {
	return _pimpl->isEmpty();
}

template<class T>
size_t Stack<T>::size() const {
	return _pimpl->size();
}
