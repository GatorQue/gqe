/**
 * Provides the Smart Pointer Template class for handling smart pointers.
 *
 * @file include/GQE/Core/interfaces/TSmartPointer.hpp
 * @author Jacob Dix
 * @date 20120423 - Initial Release
 */
#ifndef TSMART_POINTER_HPP_INCLUDED
#define TSMART_POINTER_HPP_INCLUDED
#include <GQE/Core/classes/ReferenceCount.hpp>
template < typename T > class TSmartPointer
{
private:
    T*    mData;       // pointer
    ReferenceCount* mReference; // Reference count

public:
    TSmartPointer() : mData(0), mReference(0) 
    {
        // Create a new reference 
        mReference = new RC();
        // Increment the reference count
        mReference->AddRef();
    }

    TSmartPointer(T* theValue) : mData(theValue), mReference(0)
    {
        // Create a new reference 
        reference = new RC();
        // Increment the reference count
        reference->AddRef();
    }

    TSmartPointer(const TSmartPointer<T>& theSmartPointer) : mData(theSmartPointer.mData), mReference(theSmartPointer.mReference)
    {
        // Copy constructor
        // Copy the data and reference pointer
        // and increment the reference count
        mReference->AddRef();
    }

    ~TSmartPointer()
    {
        // Destructor
        // Decrement the reference count
        // if reference become zero delete the data
        if(mReference->Release() == 0)
        {
            delete mData;
            delete mReference;
        }
    }

    T& operator* ()
    {
        return *mData;
    }

    T* operator-> ()
    {
        return mData;
    }
    
    TSmartPointer<T>& operator = (const TSmartPointer<T>& theSmartPointer)
    {
        // Assignment operator
        if (this != &theSmartPointer) // Avoid self assignment
        {
            // Decrement the old reference count
            // if reference become zero delete the old data
            if(mReference->Release() == 0)
            {
                delete mData;
                delete mReference;
            }

            // Copy the data and reference pointer
            // and increment the reference count
            mData = theSmartPointer.pData;
            mReference = theSmartPointer.mReference;
            mReference->AddRef();
        }
        return *this;
    }
};

#endif