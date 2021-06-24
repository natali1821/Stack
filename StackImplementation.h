#pragma once

// интерфейс для конкретных реализаций контейнера для стека
template<class T>
class StackImplementation {
public:
	// добавление в хвост
	virtual void push(const T& value) = 0;
	// удаление с хвоста
	virtual void pop() = 0;
	// посмотреть элемент в хвосте
	virtual const T& top() const = 0;
	// проверка на пустоту
	virtual bool isEmpty() const = 0;
	// размер
	virtual size_t size() const = 0;
	// виртуальный деструктор
	virtual ~StackImplementation() {};
};
