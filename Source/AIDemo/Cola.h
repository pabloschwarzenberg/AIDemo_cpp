#ifndef __COLA_H__
#define __COLA_H__

void error_fatal(const char* s);

template <class T> class Elemento
{
public:
	Elemento();

	T valor;
	Elemento<T>* siguiente;
};

template <class T> Elemento<T>::Elemento()
{
	siguiente = nullptr;
}

template <class T> class Cola
{
public:
	Cola();
	virtual ~Cola();

	bool vacia();
	bool contiene(T valor);
	void encolar(T valor);
	T minimo();
	void extraer(T valor);

private:
	Elemento<T>* inicio;
	Elemento<T>* fin;
};

template <class T> Cola<T>::Cola()
{
	inicio = nullptr;
	fin = nullptr;
}

template <class T> Cola<T>::~Cola()
{
	Elemento<T>* cursor = inicio;
	Elemento<T>* actual;
	while (cursor != nullptr)
	{
		actual = cursor;
		cursor = cursor->siguiente;
		delete actual;
	}
}

template <class T> bool Cola<T>::vacia()
{
	if (inicio == nullptr && fin == nullptr)
		return true;
	else
		return false;
}

template <class T> bool Cola<T>::contiene(T valor)
{
	Elemento<T>* cursor;

	cursor = inicio;
	while (cursor != nullptr)
	{
		if (cursor->valor == valor)
			return true;
		cursor = cursor->siguiente;
	}
	return false;
}

template <class T> void Cola<T>::encolar(T valor)
{
	Elemento<T>* nodo;

	nodo = new Elemento<T>();
	if (nodo == nullptr)
		error_fatal("Error encolando elemento");
	nodo->valor = valor;
	if (inicio == nullptr)
	{
		inicio = nodo;
		fin = nodo;
	}
	else
	{
		fin->siguiente = nodo;
		fin = nodo;
	}
}

template <class T> T Cola<T>::minimo()
{
	Elemento<T>* cursor = inicio;
	Elemento<T>* m = inicio;
	while (cursor != nullptr)
	{
		if (cursor->valor < m->valor)
			m = cursor;
		cursor = cursor->siguiente;
	}
	return m->valor;
}

template <class T> void Cola<T>::extraer(T valor)
{
	Elemento<T>* anterior = inicio;
	if (inicio->valor == valor)
	{
		if (fin == inicio)
		{
			delete inicio;
			inicio = nullptr;
			fin = nullptr;
			return;
		}
		inicio = inicio->siguiente;
		delete anterior;
		return;
	}
	Elemento<T>* cursor = inicio->siguiente;
	while (!(cursor->valor == valor))
	{
		anterior = cursor;
		cursor = cursor->siguiente;
	}
	if (cursor->valor == valor)
	{
		anterior->siguiente = cursor->siguiente;
		delete cursor;
	}
}

#endif