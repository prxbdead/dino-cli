#ifndef SORTED_LIST_H
#define SORTED_LIST_H

template <typename T>
class SortedList {
	struct elem_t {
		T val;
		elem_t* next;
	};

	elem_t* first = NULL, * last = NULL;
	size_t length = 0;
	
	public:
	    SortedList(); // konstruktor
	    SortedList(const SortedList<T>&); // másoló konstruktor
	    ~SortedList(); // destruktor
	    T operator[](const size_t) const; // indexelés operátor túlterhelése
	    size_t size() const; // lista méretének lekérése
	    int insert(const T); // elem beszúrása
	    void erase(const size_t); // elem eltávolítása
	    void empty(); // lista kiürítése
	    void print() const; // lista kiírása
	    void* ref() const; // a lista referenciája
	    void* ref(const size_t) const; // a lista adott elemének referenciája
	    int contains(const T) const; // tartalmazza-e a lista az adott elemet
};

#include "aat.cpp"

#endif
