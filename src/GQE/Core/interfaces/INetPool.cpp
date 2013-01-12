/**
 * Provides the INetPool class in the GQE namespace which is responsible for
 * providing a preallocated pool of network packets to be used by the
 * INetEngine and associated classes.
 *
 * @file src/GQE/Core/interfaces/INetPool.cpp
 * @author Ryan Lindeman
 * @date 20121116 - Initial Release
 */
#include <GQE/Core/interfaces/INetPool.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  INetPool::INetPool(const std::size_t theIncomingMinimum,
                     const std::size_t theOutgoingMinimum,
                     const std::size_t theIncomingCapacity,
                     const std::size_t theOutgoingCapacity) :
    mIncomingMinimum(theIncomingMinimum),
    mOutgoingMinimum(theOutgoingMinimum),
    mIncomingCapacity(theIncomingCapacity),
    mOutgoingCapacity(theOutgoingCapacity)
  {
  }

  INetPool::~INetPool()
  {
    // First set our minimum values to 0 so no new packets will be created
    mIncomingMinimum = 0;
    mOutgoingMinimum = 0;

    // Clear our incoming queue of packets
    while(!mIncoming.empty())
    {
      mIncoming.pop();
    }

    // Clear our outgoing queue of packets
    while(!mOutgoing.empty())
    {
      mOutgoing.pop();
    }

    // Now delete all packets that were created
    while(!mPackets.empty())
    {
      // Get the packet at the front of our list
      INetPacket* anPacket = mPackets.front();

      // Pop the packet from the front
      mPackets.pop_front();

      // Delete the packet
      delete anPacket;
      anPacket = NULL;
    }

    // It should already be clear, but this will make sure
    mPackets.clear();
  }

  INetPacket* INetPool::GetIncoming(void)
  {
    // Default to no packet returned (rarely should happen)
    INetPacket* anResult = NULL;

    // Is our incoming queue below our minimum threshold, then add some new packets
    if(mIncoming.size() < mIncomingMinimum)
    {
      // Add packets to our incoming queue (up to our minimum number of packets)
      for(std::size_t iloop = 0; iloop < mIncomingMinimum; iloop++)
      {
        // Attempt to create a packet with default minimum the same as capacity
        INetPacket* anPacket = CreatePacket(mIncomingCapacity, mIncomingCapacity);

        // Make sure one was returned
        if(anPacket)
        {
          // Obtain a lock on our queue mutex before modifying our queues
          sf::Lock anLock(mQueueMutex);

          // Add this packet to our list of all packets created
          mPackets.push_back(anPacket);

          // Also add this packet to our list of available outgoing packets
          mIncoming.push(anPacket);
        }
        else
        {
          WLOG() << "INetPool::GetIncoming() null packet returned by CreatePacket" << std::endl;
        }
      }
    }

    // Now attempt to retrieve a packet and return it to the caller
    if(mIncoming.size())
    {
      // Obtain a lock on our queue mutex before modifying our queues
      sf::Lock anLock(mQueueMutex);

      // Grab the packet on the front of the queue
      anResult = mIncoming.front();

      // Pop the packet off the queue
      mIncoming.pop();
    }

    // Return the result provided above
    return anResult;
  }

  std::size_t INetPool::GetIncomingSize(void) const
  {
    return mIncoming.size();
  }

  void INetPool::ReturnIncoming(INetPacket* thePacket)
  {
    // Obtain a lock on our queue mutex before modifying our queues
    sf::Lock anLock(mQueueMutex);

    // Reset the packet during the return of every packet
    thePacket->Clear();

    // Push the newly cleared packet into our incoming queue
    mIncoming.push(thePacket);
  }

  void INetPool::SetIncomingMinimum(const std::size_t theMinimum)
  {
    mIncomingMinimum = theMinimum;
  }

  void INetPool::SetIncomingCapacity(const std::size_t theCapacity)
  {
    // Check to make sure theCapacity specified is larger than the HEADER_SIZE_B minimum
    mIncomingCapacity = theCapacity < INetPacket::HEADER_SIZE_B ? INetPacket::HEADER_SIZE_B : theCapacity;
  }

  INetPacket* INetPool::GetOutgoing(void)
  {
    // Default to no packet returned (rarely should happen)
    INetPacket* anResult = NULL;

    // Is our outgoing queue below our minimum threshold, then add some new packets
    if(mOutgoing.size() < mOutgoingMinimum)
    {
      // Add packets to our outgoing queue (up to our minimum number of packets)
      for(std::size_t iloop = 0; iloop < mOutgoingMinimum; iloop++)
      {
        // Attempt to create a packet (with default minimum of HEADER_SIZE_B)
        INetPacket* anPacket = CreatePacket(mOutgoingCapacity);

        // Make sure one was returned
        if(anPacket)
        {
          // Obtain a lock on our queue mutex before modifying our queues
          sf::Lock anLock(mQueueMutex);

          // Add this packet to our list of all packets created
          mPackets.push_back(anPacket);

          // Also add this packet to our list of available outgoing packets
          mOutgoing.push(anPacket);
        }
        else
        {
          WLOG() << "INetPool::GetOutgoing() null packet returned by CreatePacket" << std::endl;
        }
      }
    }

    // Now attempt to retrieve a packet and return it to the caller
    if(mOutgoing.size())
    {
      // Obtain a lock on our queue mutex before modifying our queues
      sf::Lock anLock(mQueueMutex);

      // Grab the packet on the front of the queue
      anResult = mOutgoing.front();

      // Pop the packet off the queue
      mOutgoing.pop();
    }

    // Return the result provided above
    return anResult;
  }

  std::size_t INetPool::GetOutgoingSize(void) const
  {
    return mOutgoing.size();
  }

  void INetPool::ReturnOutgoing(INetPacket* thePacket)
  {
    // Obtain a lock on our queue mutex before modifying our queues
    sf::Lock anLock(mQueueMutex);

    // Reset the packet during the return of every packet
    thePacket->Clear();

    // Push the newly cleared packet into our outgoing queue
    mOutgoing.push(thePacket);
  }

  void INetPool::SetOutgoingMinimum(const std::size_t theMinimum)
  {
    mOutgoingMinimum = theMinimum;
  }

  void INetPool::SetOutgoingCapacity(const std::size_t theCapacity)
  {
    mOutgoingCapacity = theCapacity;
  }

  INetPacket* INetPool::CreatePacket(const std::size_t theCapacity, const std::size_t theMinimum)
  {
    INetPacket* anResult = new (std::nothrow) INetPacket(theCapacity, theMinimum);

    // Return the result created above
    return anResult;
  }
} // namespace GQE

/**
 * Copyright (c) 2010-2012 Ryan Lindeman
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

