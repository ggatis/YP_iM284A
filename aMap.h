/**
 * @file    aMap.h
 *
 * @brief   Declaration of class aMap
 *
 * @note    This example code is free software: you can redistribute it and/or modify it.
 *
 *          This program is provided by EDI on an "AS IS" basis without
 *          any warranties in the hope that it will be useful.
 * 
 * Gatis Gaigals @ EDI, 2024
 */

#ifndef _aMap_H_
#define _aMap_H_

#include <cstdint>
#include <map>
#include <initializer_list> //initializer_list support

template < typename Key, typename T >

class aMap;

//general template definition
template < typename Key, typename T >

class aMap {

private:
    std::map < Key, T > data;

public:
    //constructor to initialize the map with an initializer list
    aMap( std::initializer_list < std::pair< const Key, T > > initList ) : data( initList ) {}

    //insert a key-value pair
    void insert( const Key& key, const T& value ) {
        data[key] = value;
    }

    //check if the map contains a key
    bool contains( const Key& key ) const {
        return data.find( key ) != data.end();
    }

    //get the value associated with a key, or return a default value if the key is not found
    T value( const Key& key, const T& defaultValue = T() ) const {
        typename std::map<Key, T>::const_iterator it = data.find( key );
        if ( data.end() != it ) {
            return it->second;
        } else {
            return defaultValue;
        }
    }


    //size of the map
    //size_t size() const {
    uint16_t size() const {
        return data.size();
    }

    //clear the map
    void clear() {
        data.clear();
    }

    //remove a key from the map
    void remove( const Key& key ) {
        data.erase( key );
    }

    //additional methods to expose std::map functionalities as needed
    typename std::map<Key, T>::iterator begin() {
        return data.begin();
    }

    typename std::map<Key, T>::iterator end() {
        return data.end();
    }

    typename std::map<Key, T>::const_iterator begin() const {
        return data.begin();
    }

    typename std::map<Key, T>::const_iterator end() const {
        return data.end();
    }
};


//Specialization for const char*
template < typename Key >

class aMap < Key, const char* > {

private:
    std::map < Key, const char* > data;

public:
    //constructor to initialize the map with an initializer list
    aMap( std::initializer_list< std::pair< const Key, const char* > > initList ) : data( initList ) {}

    //insert a key-value pair
    void insert( const Key& key, const char* value ) {
        data[key] = value;
    }

    //check if the map contains a key
    bool contains( const Key& key ) const {
        return data.find( key ) != data.end();
    }

    //get the value associated with a key, or return a default value if the key is not found
    const char* value(const Key& key, const char* defaultValue = "" ) const {
        //auto it = data.find(key);
        typename std::map< Key, const char* >::const_iterator it = data.find( key );
        if ( data.end() != it ) {
            return it->second;
        } else {
            return defaultValue;
        }
    }

    //size of the map
    uint16_t size() const {
        return data.size();
    }

    //clear the map
    void clear() {
        data.clear();
    }

    //remove a key from the map
    void remove( const Key& key ) {
        data.erase( key );
    }

    //additional methods to expose std::map functionalities as needed
    typename std::map<Key, const char*>::iterator begin() {
        return data.begin();
    }

    typename std::map<Key, const char*>::iterator end() {
        return data.end();
    }

    typename std::map<Key, const char*>::const_iterator begin() const {
        return data.begin();
    }

    typename std::map<Key, const char*>::const_iterator end() const {
        return data.end();
    }
};

#endif // _aMap_H_
