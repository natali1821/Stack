#pragma once
#include "StackImplementation.h"
#include "MyVector.h"

// вариант с использованием ранее написанного вектора и множественного наследования
// если бы вектор не был реализован, то было бы наследование только от интерфейса
// множественное наследование можно заменить на композицию

template<class T>
class VectorStack : public StackImplementation<T>, public MyVector<T> {
public:
	VectorStack();

	VectorStack(const VectorStack<T>& copy);
	VectorStack<T>& operator=(const VectorStack<T>& copy);

	VectorStack(VectorStack<T>&& other) noexcept;
	VectorStack<T>& operator=(VectorStack<T>&& other) noexcept;

	~VectorStack() = default;

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
	MyVector<T> _vectorStack;
};


template<class T>
VectorStack<T>::VectorStack() {
	_vectorStack = MyVector<T>();
}

template<class T>
VectorStack<T>::VectorStack(const VectorStack<T>& copy) {
	_vectorStack = copy._vectorStack;
}

template<class T>
VectorStack<T>& VectorStack<T>::operator=(const VectorStack<T>& copy) {
	_vectorStack = copy._vectorStack;
	return *this;
}

template<class T>
VectorStack<T>::VectorStack(VectorStack<T>&& other) noexcept {
	_vectorStack = std::move(other._vectorStack);
}

template<class T>
VectorStack<T>& VectorStack<T>::operator=(VectorStack<T>&& other) noexcept {
	_vectorStack = std::move(other._vectorStack);
	return *this;
}

template<class T>
void VectorStack<T>::push(const T& value) {
	_vectorStack.pushBack(value);
}

template<class T>
void VectorStack<T>::pop() {
	_vectorStack.popBack();
}

template<class T>
const T& VectorStack<T>::top() const {
	return _vectorStack.at(size() - 1);
}

template<class T>
bool VectorStack<T>::isEmpty() const {
	return _vectorStack.isEmpty();
}

template<class T>
size_t VectorStack<T>::size() const {
	return _vectorStack.size();
}
