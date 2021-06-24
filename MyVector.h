#pragma once
#include <iostream>
#include <exception>
#include <math.h>
#include <utility>

// стратегия изменения capacity
enum class ResizeStrategy {
	Additive,
	Multiplicative
};

template<class T>
class MyVector
{
public:
	// реализовать итераторы
	class VectorIterator{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type      = T;
		using pointer         = T*;
		using reference        = T&;

		VectorIterator(T* ptr);
		VectorIterator(const VectorIterator& copy);
		VectorIterator operator=(const VectorIterator& copy);

		reference operator*();
		pointer operator->();

		VectorIterator& operator++();
		VectorIterator& operator--();
		VectorIterator operator++(int);
		VectorIterator operator--(int);

		bool operator!=(const VectorIterator& other);
		bool operator==(const VectorIterator& other);

		difference_type operator-(const VectorIterator& other);
	private:
		T* _ptr;
	};
	class ConstVectorIterator{
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = const T;
		using pointer            = const T*;
		using reference          = const T&;

		ConstVectorIterator(T* ptr);
		ConstVectorIterator(const ConstVectorIterator& copy);
		ConstVectorIterator operator=(const ConstVectorIterator& copy);

		reference operator*() const;
		pointer operator->() const;

		ConstVectorIterator& operator++();
		ConstVectorIterator& operator--();
		ConstVectorIterator operator++(int);
		ConstVectorIterator operator--(int);

		bool operator!=(const ConstVectorIterator& other) const;
		bool operator==(const ConstVectorIterator& other) const;

		difference_type operator-(const ConstVectorIterator& other);
	private:
		T* _ptr;
	};

	// заполнить вектор значениями T()
	MyVector(size_t size = 0,
			 ResizeStrategy = ResizeStrategy::Multiplicative,
			 float coef = 1.5f);
	// заполнить вектор значениями value
	MyVector(size_t size,
			 const T& value,
			 ResizeStrategy = ResizeStrategy::Multiplicative,
			 float coef = 1.5f);

	MyVector(const MyVector<T>& copy);
	MyVector& operator=(const MyVector<T>& copy);

	MyVector(MyVector<T>&& other) noexcept;
	MyVector& operator=(MyVector<T>&& other) noexcept;

	~MyVector();

	size_t capacity() const;
	size_t size() const;
	float loadFactor() const;

	VectorIterator begin();
	ConstVectorIterator cbegin() const;
	VectorIterator end();
	ConstVectorIterator cend() const;

	// доступ к элементу,
	// должен работать за O(1)
	T& at(const size_t idx);
	const T& at(const size_t idx) const;
	T& operator[](const size_t idx);
	const T& operator[](const size_t idx) const;

	// добавить в конец,
	// должен работать за amort(O(1))
	void pushBack(const T& value);
	// вставить,
	// должен работать за O(n)
	void pushFront(const T& value);
	void insert(const size_t idx, const T& value);     // версия для одного значения
	void insert(const size_t idx, const MyVector<T>& value);      // версия для вектора
	void insert(ConstVectorIterator it, const T& value);  // версия для одного значения
	void insert(ConstVectorIterator it, const MyVector<T>& value);   // версия для вектора

	// удалить с конца,
	// должен работать за amort(O(1))
	void popBack();
	// удалить
	// должен работать за O(n)
	void popFront();
	void erase(const size_t pos);
	void erase(const size_t pos, size_t len);            // удалить len элементов начиная с i

	// найти элемент,
	// должен работать за O(n)
	// если isBegin == true, найти индекс первого элемента, равного value, иначе последнего
	// если искомого элемента нет, вернуть end
	ConstVectorIterator find(const T& value, bool isBegin = true);

	// зарезервировать память (принудительно задать capacity)
	void reserve(const size_t newCapacity);

	// изменить размер
	// если новый размер больше текущего, то новые элементы забиваются value
	// если меньше - обрезаем вектор
	void resize(const size_t newSize, const T& value = T());

	// очистка вектора, без изменения capacity
	void clear();

	void reallocVector(const size_t newSize = size());
	bool isLoaded() const;
private:
	T* _data;
	size_t _size;
	size_t _capacity;
	ResizeStrategy _resizeStrategy;
	float _coef;
};

//VectorIterator
template<class T>
MyVector<T>::VectorIterator::VectorIterator(T* ptr) {
	_ptr = ptr;
}

template<class T>
MyVector<T>::VectorIterator::VectorIterator(const MyVector::VectorIterator& copy) {
	_ptr = copy._ptr;
}

template<class T>
class MyVector<T>::VectorIterator MyVector<T>::VectorIterator::operator=(const MyVector<T>::VectorIterator& copy) {
	if (this != &copy) {
		_ptr = copy._ptr;
	}
	return *this;
}

template<class T>
T& MyVector<T>::VectorIterator::operator*() {
	return *_ptr;
}

template<class T>
T* MyVector<T>::VectorIterator::operator->() {
	return _ptr;
}

template<class T>
class MyVector<T>::VectorIterator& MyVector<T>::VectorIterator::operator++() {
	++_ptr;
	return *this;
}

template<class T>
class MyVector<T>::VectorIterator& MyVector<T>::VectorIterator::operator--() {
	--_ptr;
	return *this;
}

template<class T>
class MyVector<T>::VectorIterator MyVector<T>::VectorIterator::operator++(int) {
	VectorIterator tmp = *this;
	++(*this);
	return tmp;
}

template<class T>
class MyVector<T>::VectorIterator MyVector<T>::VectorIterator::operator--(int) {
	VectorIterator tmp = *this;
	--(*this);
	return tmp;
}

template<class T>
bool MyVector<T>::VectorIterator::operator!=(const MyVector::VectorIterator& other) {
	return _ptr != other._ptr;
}

template<class T>
bool MyVector<T>::VectorIterator::operator==(const MyVector::VectorIterator& other) {
	return _ptr == other._ptr;
}

template<class T>
std::ptrdiff_t MyVector<T>::VectorIterator::operator-(const MyVector::VectorIterator& other) {
	return _ptr - other._ptr;
}

//ConstVectorIterator
template<class T>
MyVector<T>::ConstVectorIterator::ConstVectorIterator(T* ptr) {
	_ptr = ptr;
}

template<class T>
MyVector<T>::ConstVectorIterator::ConstVectorIterator(const MyVector<T>::ConstVectorIterator& copy) {
	_ptr = copy._ptr;
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::ConstVectorIterator::operator=(const MyVector<T>::ConstVectorIterator& copy) {
	if (this != &copy) {
		_ptr = copy._ptr;
	}
	return *this;
};

template<class T>
const T& MyVector<T>::ConstVectorIterator::operator*() const{
	return *_ptr;
}

template<class T>
const T* MyVector<T>::ConstVectorIterator::operator->() const{
	return _ptr;
}

template<class T>
class MyVector<T>::ConstVectorIterator& MyVector<T>::ConstVectorIterator::operator++() {
	++_ptr;
	return *this;
}

template<class T>
class MyVector<T>::ConstVectorIterator& MyVector<T>::ConstVectorIterator::operator--() {
	--_ptr;
	return *this;
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::ConstVectorIterator::operator++(int) {
	ConstVectorIterator tmp = *this;
	++(*this);
	return tmp;
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::ConstVectorIterator::operator--(int) {
	ConstVectorIterator tmp = *this;
	--(*this);
	return tmp;
}

template<class T>
bool MyVector<T>::ConstVectorIterator::operator!=(const MyVector::ConstVectorIterator& other) const{
	return _ptr != other._ptr;
}

template<class T>
bool MyVector<T>::ConstVectorIterator::operator==(const MyVector::ConstVectorIterator& other) const{
	return _ptr == other._ptr;
}

template<class T>
std::ptrdiff_t MyVector<T>::ConstVectorIterator::operator-(const MyVector::ConstVectorIterator& other) {
	return _ptr - other._ptr;
}

//Vector
template<class T>
MyVector<T>::MyVector(size_t size, ResizeStrategy strategy, float coef) {
	_size = size;
	_resizeStrategy = strategy;
	_coef = coef;
	if (!_size) {
		_capacity = 1;
		_data = new T[_capacity];
		return;
	}
	switch(_resizeStrategy) {
	case(ResizeStrategy::Additive):
		_capacity = ceil(_size + _coef);
		break;
	case(ResizeStrategy::Multiplicative):
		_capacity = ceil(_size * _coef);
		break;
	}
	_data = new T[_capacity];
	for(size_t i = 0; i < _size; ++i) {
		_data[i] = T();
	}
}

template<class T>
MyVector<T>::MyVector(size_t size, const T& value, ResizeStrategy strategy, float coef) {
	_size = size;
	_resizeStrategy = strategy;
	_coef = coef;
	if (!_size) {
		_capacity = 1;
		_data = new T[_capacity];
		return;
	}
	switch(_resizeStrategy) {
	case(ResizeStrategy::Additive):
		_capacity = ceil(_size + _coef);
		break;
	case(ResizeStrategy::Multiplicative):
		_capacity = ceil(_size * _coef);
		break;
	}
	_data = new T[_capacity];
	for(size_t i = 0; i < _size; ++i) {
		_data[i] = value;
	}
}

template<class T>
MyVector<T>::MyVector(const MyVector<T>& copy) {
	_size = copy.size();
	_capacity = copy.capacity();
	_resizeStrategy = copy._resizeStrategy;
	_coef = copy._coef;
	_data = nullptr;
	if (size()) {
		_data = new T[capacity()];
		for (size_t i = 0; i < size(); ++i) {
			_data[i] = copy.at(i);
		}
	}
}

template<class T>
MyVector<T>::MyVector(MyVector<T>&& other) noexcept {
	_data = std::exchange(other._data, nullptr);
	_size = std::exchange(other._size, 0);
	_capacity = std::exchange(other._capacity, 0);
	_coef = std::exchange(other._coef, 0);
	_resizeStrategy = other._resizeStrategy;
}

template<class T>
MyVector<T>& MyVector<T>::operator=(const MyVector<T>& copy){
	if (this != &copy) {
		_size = copy.size();
		_capacity = copy.capacity();
		_coef = copy._coef;
		_resizeStrategy = copy._resizeStrategy;
		delete[] _data;
		_data = new T[capacity()];
		for (size_t i = 0; i < size(); ++i) {
			_data[i] = copy.at(i);
		}
	}
	return *this;
}

template<class T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>&& other) noexcept {
	if (this != &other) {
		delete[] _data;
		_data = std::exchange(other._data, nullptr);
		_size = std::exchange(other._size, 0);
		_capacity = std::exchange(other._capacity, 0);
		_coef = std::exchange(other._coef, 0);
		_resizeStrategy = other._resizeStrategy;
	}
	return *this;
}

template<class T>
MyVector<T>::~MyVector() {
	if (_data) {
		delete[] _data;
		_data = nullptr;
	}
	_size = 0;
	_capacity = 0;
	_coef = 0;
}

template<class T>
size_t MyVector<T>::capacity() const {
	return _capacity;
}

template<class T>
size_t MyVector<T>::size() const {
	return _size;
}

template<class T>
float MyVector<T>::loadFactor() const {
	return (float)_size / _capacity;
}

template<class T>
class MyVector<T>::VectorIterator MyVector<T>::begin() {
	return MyVector<T>::VectorIterator(&_data[0]);
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::cbegin() const {
	return MyVector<T>::ConstVectorIterator(&_data[0]);
}

template<class T>
class MyVector<T>::VectorIterator MyVector<T>::end(){
	return MyVector<T>::VectorIterator(&_data[size()]);
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::cend() const{
	return MyVector<T>::ConstVectorIterator(&_data[size()]);
}

template<class T>
T& MyVector<T>::at(const size_t idx) {
	if (idx >= size()) {
		throw std::out_of_range("Called at(idx) : idx >= size of vector ");
	}
	return _data[idx];
}

template<class T>
const T& MyVector<T>::at(const size_t idx) const {
	if (idx >= size()) {
		throw std::out_of_range("Called at(idx) : idx >= size of vector ");
	}
	return _data[idx];
}

template<class T>
T& MyVector<T>::operator[](const size_t idx) {
	return at(idx);
}

template<class T>
const T& MyVector<T>::operator[](const size_t idx) const {
	return at(idx);
}

template<class T>
void MyVector<T>::reserve(const size_t capacity) {
	if (capacity > _capacity) {
		T* tmp = new T[capacity];
		for (size_t i = 0; i < size(); ++i) {
			tmp[i] = _data[i];
		}
		delete[] _data;
		_data = tmp;
		_capacity = capacity;
	}
}

template<class T>
void MyVector<T>::pushBack(const T& value) {
	//insert(size(), value)
	if (isLoaded()) {
		reallocVector(size());
	}
	_data[size()] = value;
	++_size;
}

template<class T>
void MyVector<T>::pushFront(const T& value) {
	insert(0, value);
}

template<class T>
void MyVector<T>::insert(const size_t idx, const T& value) {
	if (idx > size()) {
		throw std::out_of_range("Called insert(idx) : idx > size");
	}
	if (isLoaded()) {
		reallocVector(size());
	}
	T* tmp = new T[capacity()];
	for (size_t i = 0; i < idx; ++i) {
		tmp[i] = _data[i];
	}
	tmp[idx] = value;
	for (size_t i = idx; i < size(); ++i) {
		tmp[i + 1] = _data[i];
	}
	delete[] _data;
	_data = tmp;
	++_size;
}

template<class T>
void MyVector<T>::insert(const size_t idx, const MyVector<T>& value) {
	if (idx > size()) {
		throw std::out_of_range("Called insert(idx) : idx > size");
	}
	size_t newSize = size() + value.size();
	if (newSize > capacity()) {
		reallocVector(newSize);
	}
	size_t i = 0, j = 0;
	T* tmp = new T[capacity()];
	for (i; i < idx; ++i) {
		tmp[i] = _data[i];
	}
	for (i, j; j < value.size(); ++i, ++j) {
		tmp[i] = value.at(j);
	}
	for (i, j = idx; j < size(); ++i, ++j) {
		tmp[i] = _data[j];
	}
	delete[] _data;
	_data = tmp;
	_size = newSize;
}

template<class T>
void MyVector<T>::insert(MyVector<T>::ConstVectorIterator it, const T& value){
	size_t idx = 0;
	for (MyVector<T>::ConstVectorIterator tmp = cbegin(); tmp != cend(); ++tmp) {
		++idx;
		if (tmp == it) {
			break;
		}
	}
	insert(idx, value);
}

template<class T>
void MyVector<T>::insert(MyVector<T>::ConstVectorIterator it, const MyVector<T>& value){
	size_t idx = 0;
	for (MyVector<T>::ConstVectorIterator tmp = cbegin(); tmp != cend(); ++tmp) {
		++idx;
		if (tmp == it) {
			break;
		}
	}
	insert(idx, value);
}

template<class T>
void MyVector<T>::popBack() {
	//erase(size(), value);
	--_size;
}

template<class T>
void MyVector<T>::popFront() {
	erase(0, 1);
}

template<class T>
void MyVector<T>::erase(const size_t pos) {
	erase(pos, 1);
}

template<class T>
void MyVector<T>::erase(const size_t pos, size_t len) {
	if (pos >= size()) {
		throw std::out_of_range("Called erase(pos) : pos >= size");
	}
	if (!size()) {
		return;
	}
	if (len > size() - pos) {
		len = size() - pos;
	}
	size_t sizeTmp = size() - len;
	T* tmp = new T[capacity()];
	for (size_t i = 0; i < pos; ++i) {
		tmp[i] = _data[i];
	}
	for (size_t i = pos; i < sizeTmp; ++i) {
		tmp[i] = _data[i + len];
	}
	delete[] _data;
	_data = tmp;
	_size = sizeTmp;
}

template<class T>
class MyVector<T>::ConstVectorIterator MyVector<T>::find(const T& value, bool isBegin) {
	MyVector<T>::ConstVectorIterator it = cbegin();
	for (MyVector<T>::ConstVectorIterator tmp = cbegin(); tmp != cend(); ++tmp) {
		if (*tmp == value) {
			it = tmp;
			if (isBegin) {
				break;
			}
		}
	}
	if (*it != value) {
		return cend();
	}
	return it;
}

template<class T>
void MyVector<T>::resize(const size_t newSize, const T& value) {
	if (newSize < 0) {
		throw std::invalid_argument("Invalid size");
	}
	if (newSize > size() && newSize <= capacity()) {
		for (size_t i = size(); i < newSize; ++i) {
			_data[i] = value;
		}
	}
	else if (newSize > capacity()) {
		size_t oldSize = size();
		reallocVector(newSize);
		for (size_t i = oldSize; i < size(); i++) {
			_data[i] = value;
		}
	}
	_size = newSize;
}

template<class T>
void MyVector<T>::clear() {
	_size = 0;
}

template<class T>
void MyVector<T>::reallocVector(const size_t newSize) {
	switch(_resizeStrategy) {
	case(ResizeStrategy::Additive):
		_capacity = ceil(newSize + _coef);
		break;
	case(ResizeStrategy::Multiplicative):
		_capacity = ceil(newSize * _coef);
	break;
	}
	T* tmp = new T[capacity()];
	for (size_t i = 0; i < size(); ++i) {
		tmp[i] = _data[i];
	}
	delete[] _data;
	_data = tmp;
	_size = newSize;
}

template<class T>
bool MyVector<T>::isLoaded() const{
	return (loadFactor() == 1);
}
