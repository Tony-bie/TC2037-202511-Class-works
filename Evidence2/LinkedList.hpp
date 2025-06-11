//
//  LinkedList.hpp
//  LinkedList
//
//  Created by Vicente Cubells on 25/09/20.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp

#include <iostream>
#include "Node.hpp"

template <class T>
class LinkedList {
protected:
    Node<T> * _first = nullptr;
    int _size = 0;
    
    /* Clase Iterator */
    class Iterator {
        const LinkedList<T> * _data;
        int _position;
        
    public:
        Iterator( const LinkedList<T> * _adata, int _aposition)
        : _data(_adata), _position(_aposition) {}
        
        Node<T> operator *() const { return *(_data->at(_position)); }
        const  Iterator & operator ++() { ++_position; return *this; }
        bool operator != (const Iterator & it) const { return _position != it._position; }
    };
    
public:
    /* Constructor */
    LinkedList() { };
    
    /* Destructor */
    virtual ~LinkedList();
    
    /* Obtener el tamaño de la lista */
    int size() const;
    
    /* Obtener el primer elemento */
    Node<T> * first() const;
    
    /* Determinar si la lista está vacía */
    bool empty() const;
    
    /* Insertar un elemento en una posición dada */
    void insert(const T &, int);
    void insert(Node<T> *, int);
    
    /* Insertar un elemento al inicio */
    void insert_front(const T &);
    void insert_front(Node<T> *);

    /* Insertar un elemento al final */
    void insert_back(const T &);
    void insert_back(Node<T> *);
    
    /* Eliminar un elemento y regresar un apuntador al mismo.
     * Nota: No liberan la memoria ocupada por el nodo eliminado
     */
    /* Eliminar el elemento en la posición dada */
    Node<T> * remove(int);
    
    /* Eliminar el primer elemento */
    Node<T> * remove_front();
    
    /* Eliminar el último elemento */
    Node<T> * remove_back();
    
    /* Eliminar un elemento dado */
    Node<T> * remove(Node<T> *);
    
    /* Eliminar todos los elementos de la lista y liberar la memoria ocupada */
    virtual void clear();
    
    /* Obtener el nodo que se encuentra en una posición */
    Node<T> * at(int) const;
    
    /* Obtener la posición de un nodo */
    virtual int index(Node<T> *) const;
    
    /* Obtener la posición de un valor */
    virtual int index(const T &) const;

    /* Mostrar el contenido de la lista */
    template <typename Tn>
    friend std::ostream & operator <<(std::ostream &, const LinkedList<Tn> &);
    
    /* Obtener la cantidad de ocurrencias de un elemento */
    virtual int count(const T &) const;
    
    /* Invertir una lista */
    virtual void reverse();
    
    /* Funciones que utiliza el foreach */
    Iterator begin() const { return { this, 0}; }
    Iterator end() const { return {this, _size }; }
    
    /* Sobrecarga del operador índice */
    Node<T> * operator [](const int);
    
    /* Clonar una lista */
    LinkedList<T> * clone();
    
    /* Eliminar un rango de elementos */
    void deleteRange(int, int);
    
    /* Obtener un subconjunto de elementos de la lista a partir de un rango */
    LinkedList<T> * subList(int, int);
    
    /* Obtener la unión de dos listas */
    LinkedList<T> * Union(LinkedList<T> * );
    
    /* Obtener la intersección de dos listas */
    LinkedList<T> * Intersection(LinkedList<T> * );
    
    /* Obtener la diferencia de dos listas */
    LinkedList<T> * Except(LinkedList<T> * );
    
};

template <class T>
LinkedList<T>::~LinkedList()
{
    this->clear();
}

/* Obtener el tamaño de la lista
 * Complejidad: O(1)
 */
template <class T>
int LinkedList<T>::size() const
{
    return this->_size;
}

/* Obtener el primer elemento
 * Complejidad: O(1)
 */
template <class T>
Node<T> * LinkedList<T>::first() const
{
    return this->_first;
}

/* Determinar si la lista está vacía
 * Complejidad: O(1)
 */
template <class T>
bool LinkedList<T>::empty() const
{
    return this->_first == nullptr;
}

/* Insertar un nodo en una posición dada
 * Si position < 0 se inserta al inicio
 * Si position > _size se inserta al final
 * en cualquier otro caso, se inserta en la posición dada
 * Complejidad: O(1) si es al inicio, O(n) cualquier otro caso
 */
template <class T>
void LinkedList<T>::insert(const T & value, int position)
{
    /* Crear el nuevo nodo a insertar */
    Node<T> * newnode = new Node<T>(value);
    
    this->insert(newnode, position);
}

template <class T>
void LinkedList<T>::insert(Node<T> * node, int position)
{
    /* Cuando la lista está vacía o position < 0 se inserta al inicio */
    if (this->empty() || position <= 0) {
        node->setNext(this->_first);
        this->_first = node;
    }
    /* Cuando se inserta en cualquier posición diferente del inicio */
    else {
        
        /* Si position > size se inserta al final */
        if (position > this->_size) { position = this->_size; }
        
        /* Obtener el nodo que está en la posición anterior */
        Node<T> * previous = this->at(position-1);
        
        /* Actualizar apuntadores */
        node->setNext(previous->getNext());
        previous->setNext(node);
    }
    
    /* Incrementar el tamaño de la lista */
    ++this->_size;
}

/* Insertar un elemento al inicio
 * Complejidad: O(1)
 */
template <class T>
void LinkedList<T>::insert_front(const T & value)
{
    this->insert(value, 0);
}

template <class T>
void LinkedList<T>::insert_front(Node<T> * node)
{
    this->insert(node, 0);
}

/* Insertar un elemento al final
 * Complejidad: O(n)
 */
template <class T>
void LinkedList<T>::insert_back(const T & value)
{
    this->insert(value, this->_size);
}

template <class T>
void LinkedList<T>::insert_back(Node<T> * node)
{
    this->insert(node, this->_size);
}

/* Eliminar el elemento en la posición dada
 * Complejidad: O(1) si es al inicio, O(n) cualquier otro caso
 */
/* Eliminar un elemento dado */
template <class T>
Node<T> * LinkedList<T>::remove(Node<T> * node)
{
    return this->remove( this->index(node) );
}

template <class T>
Node<T> * LinkedList<T>::remove(int position)
{
    /* Cuando la lista está vacía o position es inválida */
    if (this->empty() || (position < 0 || position >= this->_size )) {
        return nullptr;
    }

    /* Declarar variable temporal para guardar el nodo a eliminar */
    Node<T> * removenode;
    
    /* Eliminar el primer nodo de la lista */
    if (position == 0) {
        /* Se guarda el apuntador a first */
        removenode = this->_first;
        
        /* Se actualiza first al siguiente nodo */
        this->_first = this->_first->getNext();
        
    }
    /* Eliminar cualquier otro nodo */
    else {
        /* Se obtiene el apuntador al anterior */
        Node<T> * previous = this->at(position-1);
        
        /* Se guarda el apuntador al nodo a eliminar */
        removenode = previous->getNext();
        
        /* Se actualiza el anterior para que apunte al siguiente nodo */
        previous->setNext(removenode->getNext());
    }

    /* Establecer el siguiente en nullptr */
    removenode->setNext(nullptr);
    
    /* Decrementar el tamaño de la lista */
    --this->_size;
    
    return removenode;
}

/* Eliminar el primer elemento
 * Complejidad: O(1)
 */
template <class T>
Node<T> * LinkedList<T>::remove_front()
{
    return this->remove(0);
}

/* Eliminar el último elemento
 * Complejidad: O(n)
 */
template <class T>
Node<T> * LinkedList<T>::remove_back()
{
    return this->remove(this->_size - 1);
}

/* Eliminar todos los elementos de la lista y liberar la memoria ocupada
 * Complejidad: O(n)
 */
template <class T>
void LinkedList<T>::clear()
{
    /* Cuando la lista está vacía */
    if ( this->empty() ) { return; }
    
    /* Obtener una referencia al primer nodo */
    Node<T> * tmp = this->_first;
    
    /* Desplazarse por la lista e ir eliminando los nodos */
    while (tmp != nullptr) {
        /* Mover first al siguiente elemento */
        this->_first = tmp->getNext();
        
        /* Liberar la memoria del nodo */
        delete tmp;
        
        /* Apuntar al primero */
        tmp = this->_first;
    }
    
    /* Establecer el size en 0 */
    this->_size = 0;
    
    /* Establecer first en nullptr */
    this->_first = nullptr;
}

/* Obtener el nodo que se encuentra en una posición
 * Complejidad: O(n)
 */
template  <class T>
Node<T> * LinkedList<T>::at(int position) const
{
    /* Cuando la lista está vacía o position es inválida */
    if (this->empty() || position < 0 || position >= this->_size) {
        return nullptr;
    }
    
    /*  Buscar el nodo que se encuentra en position */
    int pos = 0;
    
    /* Obtener una referencia al primer nodo */
    Node<T> * tmp = this->_first;
    
    /* Desplazarse por la lista hasta encontrar el nodo */
    while (tmp != nullptr && pos++ < position)
    {
        tmp = tmp->getNext();
    }
    
    return tmp;
}

/* Obtener la posición de un nodo
 * Complejidad: O(n)
 */
template  <class T>
int LinkedList<T>::index(Node<T> * node) const
{
    /* Cuando la lista está vacía o node es nullptr */
    if (this->empty() || node == nullptr) {
        return -1;
    }
    
    /* Buscar el valor del nodo y regresar su posición */
    return this->index( node->getInfo() );
}

/* Obtener la posición de un valor
 * Complejidad: O(n)
 */
template  <class T>
int LinkedList<T>::index(const T & value) const
{
    /* Cuando la lista está vacía */
    if ( this->empty() ) {
        return -1;
    }
    
    /* Buscar value y regresar su posición */
    int pos = 0;
    
    /* Obtener una referencia al primer nodo */
    Node<T> * tmp = this->_first;
    
    /* Desplazarse por la lista hasta encontrar el value */
    while (tmp != nullptr && tmp->getInfo() != value)
    {
        tmp = tmp->getNext();
        ++pos;
    }
    
    /* Si el value no se encuentra en la lista */
    if (pos == this->_size) { return -1; }
    
    return pos;
}

/* Obtener la cantidad de ocurrencias de un elemento
 * Complejidad: O(n)
 */
template  <class T>
int LinkedList<T>::count(const T & value) const
{
    /* Obtener una referencia al primer elemento */
    Node<T> * tmp = this->_first;
    
    /* Contador de ocurrencias */
    int ocurr = 0;
    
    /* Recorrer la lista */
    while (tmp != nullptr) {
        
        /* Comparar con el valor buscado */
        if (tmp->getInfo() == value) {
            ++ocurr;
        }
        
        /* Desplazarse al siguiente elemento */
        tmp = tmp->getNext();
    }
    
    return ocurr;
}

/* Invertir una lista
 * Complejidad: O(n)
 */
template  <class T>
void LinkedList<T>::reverse()
{
    /* Obtener una referencia al segundo elemento */
    Node<T> * next = this->_first->getNext();
    
    Node<T> * previous = nullptr;
    
    /* Recorrer la lista */
    while (next != nullptr) {
        
        /* Invertir el apuntador next del nodo _first para que apunte al nodo anterior */
        this->_first->setNext(previous);
        
        /* Mover previous a la posición de _first */
        previous = this->_first;
        
        /* Mover _first al siguiente nodo */
        this->_first = next;
        
        /* Mover next al nodo despúes de él */
        next = next->getNext();
    }
    
    /* Invertir el apuntador next del último nodo para que apunte al nodo anterior */
    this->_first->setNext(previous);
}

/* Obtener el elemento de una posición
 * Complejidad: O(n)
 */
template  <class T>
Node<T> * LinkedList<T>::operator [](const int position)
{
    return this->at(position);
}

/* Mostrar el contenido de la lista
 * Complejidad: O(n)
 */
template <class T>
std::ostream & operator <<(std::ostream & os, const LinkedList<T> & list)
{
    /* Recorrer los elementos con un iterador */
    for (const Node<T> & node : list) {
        os << node << " ";
    }
    
    return os;
}

/* Clonar una lista
 * Complejidad: O(n)
 */
template <class T>
LinkedList<T> * LinkedList<T>::clone()
{
    /* Crear una lista vacía */
    LinkedList<T> * list = new LinkedList<T>();
    
    /* Obtener una referencia al primer elemento */
    Node<T> * tmp = this->_first;
    
    /* Recorrer la lista */
    while (tmp != nullptr) {
        /* Insertar un elemento en la lista nueva */
        list->insert_back( tmp->getInfo() );
        
        /* Desplazarse al siguiente elemento */
        tmp = tmp->getNext();
    }
    
    return list;
}

/* Eliminar un rango de elementos */
template <class T>
void LinkedList<T>::deleteRange(int from, int to)
{

   if (from < 0 || to >= _size || from > to) {
        return; // Índices inválidos
    }

    Node<T>* current = _first;
    Node<T>* prev = nullptr;
    int index = 0;

    // Avanza hasta la posición 'from'
    while (index < from) {
        prev = current;
        current = current->getNext();  // Avanzar
        index++;
    }

    // Eliminar los nodos entre 'from' y 'to'
    while (index <= to && current != nullptr) {
        Node<T>* temp = current;
        current = current->getNext();  // Avanzar
        delete temp;  // Eliminar el nodo de la lista
        _size--;
        index++;
    }

    // Reconectar la lista después de la eliminación
    if (prev != nullptr) {
        prev->setNext(current);  // Reconectar desde el nodo anterior
    } else {
        _first = current;  // Ajustar el primer nodo si 'from' era el primer nodo
    }
}
/*Tipo de complejidad deleteRange: O(n). Los valores dentro del rango son eleminados.
*/

/* Obtener un subconjunto de elementos de la lista a partir de un rango */
template <class T>
LinkedList<T> * LinkedList<T>::subList(int from, int to)
{
    if (from < 0 || to >= _size || from > to) {
        return nullptr;  // Índices inválidos
    }

    LinkedList<T>* newList = new LinkedList<T>();  // Crear una nueva sublista

    Node<T>* current = _first;
    Node<T>* previous = nullptr;
    int index = 0;

    // Avanzar hasta la posición 'from'
    while (index < from) {
        previous = current;
        current = current->getNext();  // Avanzar
        index++;
    }

    // Mover los elementos seleccionados a la sublista y eliminarlos de la lista principal
    while (index <= to && current != nullptr) {
        Node<T>* temp = current;

        // Agregar el elemento actual a la sublista
        newList->insert_back(current->getInfo());

        // Eliminar el nodo de la lista principal
        current = current->getNext();
        if (previous != nullptr) {
            previous->setNext(current);  // Reconectar la lista si hay un nodo previo
        } else {
            _first = current;  // Si 'from' era el primer nodo, ajustar el primer nodo
        }

        delete temp;  // Liberar el nodo eliminado
        _size--;  // Reducir el tamaño de la lista principal
        index++;
    }

    return newList;  // Devolver la sublista con los elementos movidos
}
/*Tipo de complejidad Subnlist: O(n). Ya que se crea una nueva sublista con un cierto rango
*/
/* Obtener la unión de dos listas */
template <class T>
LinkedList<T> * LinkedList<T>::Union(LinkedList<T> * listB)
{
LinkedList<T> * newList = new LinkedList<T>();

    Node<T>* current1 = this->_first;
    Node<T>* current2 = listB->_first;

    // Añadir todos los elementos de la primera lista usando insert_back
    while (current1 != nullptr) {
        newList->insert_back(current1->getInfo()); // Usar insert_back para añadir
        current1 = current1->getNext();
    }

    // Añadir elementos de la segunda lista, evitando duplicados
    while (current2 != nullptr) {
        bool exists = false;
        Node<T>* NewCurrent = newList->_first;
        while (NewCurrent != nullptr) {
            if (NewCurrent->getInfo() == current2->getInfo()) {
                exists = true;
                break;
            }
            NewCurrent = NewCurrent->getNext();
        }

        if (!exists) {
            newList->insert_back(current2->getInfo()); // Usar insert_back
        }

        current2 = current2->getNext();
    }

    return newList;
}
/*Tipo de complejidad Union: O(n + m). Se crea una nueva lista con la unión
de dos listas*/
/* Obtener la intersección de dos listas */
template <class T>
LinkedList<T> * LinkedList<T>::Intersection(LinkedList<T> * listB)
{
  LinkedList<T>* intersection = new LinkedList<T>();

    Node<T>* current1 = this->_first; // Recorrer la lista Union

    // Recorre la lista Union (lista actual)
    while (current1 != nullptr) {
        Node<T>* current2 = listB->_first;

        // Comparar el elemento de Union con los elementos de la Sublista
        while (current2 != nullptr) {
            if (current1->getInfo() == current2->getInfo()) {
                // Solo añadir los elementos que están en ambas listas
                intersection->insert_back(current1->getInfo());
                break;
            }
            current2 = current2->getNext();
        }

        current1 = current1->getNext();
    }

    return intersection;  // Devolver la lista Intersection con los elementos comunes
}
/*Tipo de complejidad en intersection: O(n x m). Se crea una nueva lista intersection
para almacera elementos similares entre Union y listB*/
/* Obtener la diferencia de dos listas */
template <class T>
LinkedList<T> * LinkedList<T>::Except(LinkedList<T> * listB)
{
 LinkedList<T> * newList = new LinkedList<T>();  // Crear una nueva lista para almacenar la diferencia

    Node<T>* current1 = this->_first;  // Recorrer la lista Union (la lista actual)

    // Recorrer la lista Union (lista actual)
    while (current1 != nullptr) {
        bool exists = false;
        Node<T>* current2 = listB->_first;  // Recorrer la sublista B para verificar si el elemento está presente

        // Verificar si el elemento de Union está en la sublista
        while (current2 != nullptr) {
            if (current1->getInfo() == current2->getInfo()) {
                exists = true;  // Marcar que el elemento está presente en ambas listas
                break;  // Salir del bucle si encontramos el elemento en la sublista
            }
            current2 = current2->getNext();  // Avanzar al siguiente nodo en la sublista
        }

        // Si el elemento no está en la sublista, lo agregamos a la nueva lista
        if (!exists) {
            newList->insert_back(current1->getInfo());
        }

        current1 = current1->getNext();  // Avanzar al siguiente nodo en la lista Union
    }

    return newList;  // Devolver la nueva lista con los elementos que no están en la sublista
}
 /* Tipo de complejidad Except: O(n x m). Ya que recorre cada elemento de la lista Union y por cada elemeneto
 realiza una búsqueda en listB*/   
#endif /* LinkedList_hpp */
