/**
 * Provides the NetPool class in the GQE namespace which is responsible for
 * providing a preallocated pool of network packets to be used by the
 * INetEngine and associated classes.
 *
 * @file include/GQE/Core/interfaces/INetPool.hpp
 * @author Ryan Lindeman
 * @date 20130111 - Initial Release
 */
#ifndef   INET_POOL_HPP_INCLUDED
#define   INET_POOL_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/INetPacket.hpp>
#include <list>
#include <queue>

namespace GQE
{
  /// Provides the INetPool class for managing empty network packets
  class GQE_API INetPool
  {
    public:
      // Constants
      ///////////////////////////////////////////////////////////////////////
      static const std::size_t INCOMING_MINIMUM = 10;
      static const std::size_t OUTGOING_MINIMUM = 100;
      static const std::size_t INCOMING_CAPACITY = 512;
      static const std::size_t OUTGOING_CAPACITY = 64;

      /**
       * INetPool default constructor
       * @param[in] theIncomingMinimum to keep available
       * @param[in] theOutgoingMinimum to keep available
       * @param[in] theIncomingCapacity to use when creating incoming INetPackets
       * @param[in] theOutgoingCapacity to use when creating outgoing INetPackets
       */
      INetPool(const std::size_t theIncomingMinimum = INCOMING_MINIMUM,
               const std::size_t theOutgoingMinimum = OUTGOING_MINIMUM,
               const std::size_t theIncomingCapacity = INCOMING_CAPACITY,
               const std::size_t theOutgoingCapacity = OUTGOING_CAPACITY);

      /**
       * INetPool deconstructor
       */
      virtual ~INetPool();

      /**
       * GetIncoming is responsible for returning an INetPacket derived class
       * that will be used to receive incoming network data. This means the
       * INetPacket class should be constructed with the mIncomingSize capacity
       * size to hold the largest packet size necessary.
       * @return pointer to an INetPacket class for incoming messages
       */
      INetPacket* GetIncoming(void);

      /**
       * GetIncomingSize will return the number of incoming messages available
       * for GetIncoming to return.
       * @return the number of incoming INetPackets available
       */
      std::size_t GetIncomingSize(void) const;

      /**
       * ReturnIncoming is responsible for returning the INetPacket derived
       * class provided back to the incoming pool of packets available. The
       * caller is responsible for balancing every GetIncoming call with a
       * corresponding ReturnIncoming call.
       * @param[in] thePacket to be returned to the incoming queue
       */
      void ReturnIncoming(INetPacket* thePacket);

      /**
       * SetIncomingMinimum is responsible for setting the minimum number of
       * incoming INetPacket derived class to keep available before calling the
       * CreatePacket method to produce more incoming INetPacket classes.
       * @param[in] theMinimum to use for incoming INetPacket derived classes
       */
      void SetIncomingMinimum(const std::size_t theMinimum);

      /**
       * SetIncomingCapacity is responsible for setting the incoming INetPacket
       * derived class capacity size that will be used by CreatePacket to
       * produce incoming INetPacket classes.
       * @param[in] theCapacity to use for incoming INetPacket derived classes
       */
      void SetIncomingCapacity(const std::size_t theCapacity);

      /**
       * GetOutgoing is responsible for returning an INetPacket derived class
       * that will be used for outgoing network data. This means the INetPacket
       * class should be constructed with the mOutgoingSize capacity to hold
       * the average outgoing packet size necessary.
       * @return pointer to an INetPacket class for outgoing messages
       */
      INetPacket* GetOutgoing(void);

      /**
       * GetOutgoingSize will return the number of outgoing messages available
       * for GetOutgoing to return.
       * @return the number of outgoing INetPackets available
       */
      std::size_t GetOutgoingSize(void) const;

      /**
       * ReturnOutgoing is responsible for returning the INetPacket derived
       * class provided back to the outgoing pool of packets available. The
       * caller is responsible for balancing every GetOutgoing call with a
       * corresponding ReturnOutgoing call.
       * @param[in] thePacket to be returned to the incoming queue
       */
      void ReturnOutgoing(INetPacket* thePacket);

      /**
       * SetOutgoingMinimum is responsible for setting the minimum number of
       * outgoing INetPacket derived class to keep available before calling the
       * CreatePacket method to produce more outgoing INetPacket classes.
       * @param[in] theMinimum to use for outgoing INetPacket derived classes
       */
      void SetOutgoingMinimum(const std::size_t theMinimum);

      /**
       * SetOutgoingCapacity is responsible for setting the outgoing INetPacket
       * derived class capacity size that will be used by CreatePacket to
       * produce outgoing INetPacket classes.
       * @param[in] theCapacity to use for outgoing INetPacket derived classes
       */
      void SetOutgoingCapacity(const std::size_t theCapacity);

    protected:
      /**
       * CreatePacket is responsible for creating both incoming and outgoing
       * INetPacket derived classes using theCapacity provided. Typically the
       * mIncomingCapacity and mOutgoingCapacity variables are passed to this
       * method to obtain incoming and outgoing INetPacket derived classes.
       * @param[in] theCapacity to use when creating this INetPacket class
       * @param[in] theMinimum size to allocate for this INetPacket class
       * @return pointer to an INetPacket class for use with methods above.
       */
      virtual INetPacket* CreatePacket(const std::size_t theCapacity = INetPacket::HEADER_SIZE_B,
                                       const std::size_t theMinimum = INetPacket::HEADER_SIZE_B);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////
      /// The minimum number of incoming INetPacket classes to keep available
      std::size_t mIncomingMinimum;
      /// The minimum number of outgoing INetPacket classes to keep available
      std::size_t mOutgoingMinimum;
      /// The maximum size capacity to use for creating incoming INetPacket classes
      std::size_t mIncomingCapacity;
      /// The average size capacity to use for creating outgoing INetPacket classes
      std::size_t mOutgoingCapacity;
      /// Mutex to protect our queues
      sf::Mutex mQueueMutex;
      /// Queue of available incoming packet size messages
      std::queue<INetPacket*> mIncoming;
      /// Queue of available outgoing packet size messages
      std::queue<INetPacket*> mOutgoing;
      /// List of all INetPacket derived classes created for cleanup purposes
      std::list<INetPacket*> mPackets;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our INetPool derived classes
       */
      INetPool(const INetPool&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our INetPool derived classes
       */
      INetPool& operator=(const INetPool&); // Intentionally undefined
  }; // class INetPool
} // namespace GQE

#endif // INET_POOL_HPP_INCLUDED

/**
 * @class GQE::INetPool
 * @ingroup Core
 * The INetPool class is responsible for defining the interface of a class that
 * will provide a pool of preallocated network packets to be used by the
 * INetEngine and associated network classes. All network packets will be
 * returned to the INetPool derived class after they have been processed which
 * reduces allocation time and improves network performance. The default
 * implementation for the INetEngine interface is the NetPool class.
 *
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

