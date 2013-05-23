#include <GQE/Core/classes/ReferenceCount.hpp>

namespace GQE
{
  ReferenceCount::ReferenceCount()
  {
    mCount=0;
  }
  void ReferenceCount::AddRef()
  {
    mCount++;
  }
  int ReferenceCount::Release()
  {
    return mCount--;
  }
}