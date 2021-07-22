#pragma once

#include "containers/ContainerBase.hpp"

/** ************************************************************************************************
 * This file provides the template for adding new Container methods.
 * Replace ContainerTemplate with the name of the new container and save this file as <new_container_name>.hpp
 * in the include/containers directory.
 * The methods described below are required, but others can be added, as well as any internal
 * class variables.
 * Parent_container is the Container in the next level up which holds this object. If this
 * is the top most object then replace Parent_Container with the name of this ojbect.
 ************************************************************************************************* */

class ContainerTemplate : public ConBase<ContainerTemplate, Parent_Container> {
public:
    ContainerTemplate(Parent_Container* par = nullptr) {
        setParent(par);
    }
    /**
      Copy constructor that makes a deep copy of the given object

      @param other The BFCon object to copy
      */
    ContainerTemplate(const ContainerTemplate* other);

    /**
      Constructor to create an object from the input vector, starting at the given index

      @param vec Vector to create the object from
      @param offset The index in vec to start creating the object from
      */
    ContainerTemplate(const arr &vec, const size_t offset = 0);

    // Data members go here

    std::ostream& _print(std::ostream &out, const uint tab = 0) const;
private:
    friend ContainerBase;
    /**
      Copy items from the given vector to the data members

      @param vec The Vector to copy from
      @param offset The indec in vec to start the copying from
      */
    void _fromArray(const arr &vec, const size_t offset = 0);
    /**
      Convert the object into a vector of doubles

      @return A vector containing the data values from the class
      */
    arr _toArray();

    /**
      Get the size of the data vector
      */
    static size_t _size() {
        return count;
    }

    void _clear() {}
    // the number of elements in this container when it is turned into a vector.
    static const size_t count;
};
