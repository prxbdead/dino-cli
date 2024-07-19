template <typename T>
SortedList<T>::SortedList() {
	first = new elem_t;
	first->next = last;
	length = 0;
}

template <typename T>
SortedList<T>::SortedList(const SortedList<T>& a) {
	first = new elem_t;
	first->next = last;

	elem_t* prev = first;
	elem_t* val = static_cast<elem_t*>(a.ref(0));

	while(val != NULL) {
		elem_t* curr = new elem_t({val->val, last});
		prev->next = curr;
		prev = curr;
		val = val->next;
	}

	length = a.size();
}

template <typename T>
SortedList<T>::~SortedList() {
	elem_t* curr = first;
	while(curr != NULL) {
		elem_t* prev = curr;
		curr = curr->next;
		delete prev;
	}
}

template <typename T>
int SortedList<T>::insert(const T val) {
	elem_t* curr = first;
	int i = 0;
	while(curr->next != NULL && curr->next->val < val) {
		curr = curr->next;
		++i;
	}

	elem_t* v = new elem_t({ val, curr->next });
	curr->next = v;
	++length;
	return i;
}

template <typename T>
void SortedList<T>::print() const {
	elem_t* curr = first->next;
	while(curr != NULL) {
		std::cout << curr->val << " ";
		curr = curr->next;
	}
}

template <typename T>
T SortedList<T>::operator[](const size_t index) const {
	if(length <= index || index < 0) {
		throw std::out_of_range("out of range");
	}

	elem_t* curr = first;
	
	for(size_t i = 0; i <= index; ++i) {
		curr = curr->next;
	}

	return curr->val;
}

template <typename T>
void SortedList<T>::erase(const size_t index) {
	if(length <= index || index < 0) {
		throw std::out_of_range("out of range");
	}

	elem_t* curr = first;

	for(int i = 0; i < index; ++i) {
		curr = curr->next;
	}

	elem_t* next = curr->next->next;
	delete curr->next;
	curr->next = next; 
	--length;
}

template <typename T>
void SortedList<T>::empty() {
	elem_t* curr = first->next;
	first->next = last;
	
	while(curr != NULL) {
		elem_t* prev = curr;
		curr = curr->next;
		delete prev;
	}

	length = 0;
}

template <typename T>
size_t SortedList<T>::size() const {
	return length;
}

template <typename T>
void* SortedList<T>::ref() const {
	return first;
}

template <typename T>
void* SortedList<T>::ref(const size_t index) const {
	if(length <= index || index < 0) {
		throw std::out_of_range("out of range");
	}

	elem_t* curr = first->next;

	for(int i = 0; i < index; ++i) {
		curr = curr->next;
	}

	return curr;
}

template <typename T>
int SortedList<T>::contains(const T val) const {
	elem_t* curr = first->next;
	for(int i = 0; i < length; ++i) {
		if(curr->val == val)
			return i;

		curr = curr->next;
	}

	return -1;
}