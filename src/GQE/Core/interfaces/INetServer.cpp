/**
 * Provides the INetServer interface that defines the default UDP/TCP server
 * interface and implementation. Classes can be derived to provide a custom
 * protocol that is independent of either UDP or TCP Ethernet protocols.
 *
 * @file src/GQE/Core/interfaces/INetServer.cpp
 * @author Ryan Lindeman
 * @date 20130104 - Initial Release
 * @date 20130111 - Generalized VerifyInternal, ProcessInternal, and better sequence number support
 */

#include <SFML/System.hpp>
#include <GQE/Core/interfaces/INetServer.hpp>
#include <GQE/Core/interfaces/INetPool.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <assert.h>

namespace GQE
{
  // Special constants that must be defined here in the CPP file
  const float INetServer::TIME_SYNC_TIMEOUT_S = 1.0f;
  const float INetServer::RECEIVE_TIMEOUT_S = 0.5f;
  const float INetServer::MAX_RESEND_TIMEOUT_S = 15.0f;

  INetServer::INetServer(INetPool& theNetPool,
                         const NetProtocol theProtocol,
                         const Uint16 theServerPort,
                         const float theTimeSyncTimeout,
                         const Int32 theResendTimeout,
                         const float theMaxResendTimeout,
                         const float theReceiveTimeout,
                         const Int8 theAliveMax) :
    IProcess(),
    mNetPool(theNetPool),
    mProtocol(theProtocol),
    mServerPort(theServerPort),
    mTimeSyncTimeout(theTimeSyncTimeout),
    mResendTimeout(theResendTimeout),
    mMaxResendTimeout(theMaxResendTimeout),
    mReceiveTimeout(theReceiveTimeout),
    mAliveMax(theAliveMax)
  {
    ILOG() << "INetServer(" << (theProtocol == NetTcp ? "TCP" : "UDP") << ","
           << theServerPort << "," << theTimeSyncTimeout << ","
           << theResendTimeout << "," << theMaxResendTimeout << ","
           << theReceiveTimeout << "," << (Uint32)theAliveMax << ")" << std::endl;
  }

  INetServer::~INetServer()
  {
  }

  Int64 INetServer::GetTimestamp(Uint32 theHostID) const
  {
    // Default to the server timestamp even if theHostID does't exist
#if (SFML_VERSION_MAJOR < 2)
    Int64 anResult = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
    Int64 anResult = mTimeSync.getElapsedTime().asMicroseconds();
#endif

    // Do they want the server timestamp value? then return that
    if(1 != theHostID)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::const_iterator anIterator;
      anIterator = mClients.find(theHostID);

      // Only return the timestamp offset if the client exists and is enabled
      if(anIterator != mClients.end() && anIterator->second.enabled)
      {
        // Add the client offset to our server timestamp value
        anResult += anIterator->second.offset;
      }
      else
      {
        ELOG() << "INetServer::GetTimestamp(" << theHostID << ") client doesn't exist or is disabled" << std::endl;
      }
    }

    // Return the timestamp value obtained above
    return anResult;
  }

  void INetServer::SendPacket(INetPacket* thePacket, const Uint32 theHostID)
  {
    // Ignore NULL NetPackets so SendPacket can be chained with CreateIdentity, CreateDisconnect, etc
    if(NULL != thePacket)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::iterator anIterator;
      anIterator = mClients.find(theHostID);

      // Only send message if client was found
      if(anIterator != mClients.end())
      {
        // Is this client currently enabled?
        if(anIterator->second.enabled)
        {
          // Assign a sequence number to each packet sent if not assigned
          if(thePacket->GetSequenceNumber() == 0)
          {
            // Assign a sequence number to each packet sent
            thePacket->SetSequenceNumber(++anIterator->second.sequence);

            // Set our first sent timestamp for this packet
            thePacket->SetFirstSent();
          }

          // Depending on the protocol will determine how we send our data
          if(mProtocol == NetTcp)
          {
            // Send the INetPacket now to the address specified
#if (SFML_VERSION_MAJOR < 2)
            anIterator->second.socket->Send((char*)thePacket->GetData(), thePacket->GetSize());
#else
            anIterator->second.socket->send(thePacket->GetData(), thePacket->GetSize());
#endif
          }
          else
          {
            // Send the INetPacket now to the address specified
#if (SFML_VERSION_MAJOR < 2)
            mListenerUdp.Send((char*)thePacket->GetData(), thePacket->GetSize(),
                              anIterator->second.address, anIterator->second.port);
#else
            mListenerUdp.send(thePacket->GetData(), thePacket->GetSize(),
                              anIterator->second.address, anIterator->second.port);
#endif
          }

          // Reset our last sent timer right after sending it
          thePacket->SetLastSent();

          // Check to see if an Ack message is required for this message
          if(thePacket->GetFlag(INetPacket::FlagAckRequired))
          {
            // Check our first sent timer against our maximum resend timeout
#if (SFML_VERSION_MAJOR < 2)
            if(thePacket->GetFirstSent() < mMaxResendTimeout)
#else
            if(thePacket->GetFirstSent().asSeconds() < mMaxResendTimeout)
#endif
            {
              // Put this INetPacket on resend list for this client
              anIterator->second.resend.push(thePacket);
            }
            // We exceeded our maximum resend timeout, return the packet now
            else
            {
              // TODO: Add event for packets who were never acknowledged

              // Return the INetPacket since it is no longer needed
              mNetPool.ReturnOutgoing(thePacket);
            }
          }
          else
          {
            // Return the INetPacket since it is no longer needed
            mNetPool.ReturnOutgoing(thePacket);
          }
        }
        else
        {
          ELOG() << "INetServer::SendPacket(" << theHostID << ") client is disabled" << std::endl;

          // Return the INetPacket since it is no longer needed
          mNetPool.ReturnOutgoing(thePacket);
        }
      }
      else
      {
        ELOG() << "INetServer::SendPacket(" << theHostID << ") client doesn't exist" << std::endl;

        // Return the INetPacket since the client doesn't exist
        mNetPool.ReturnOutgoing(thePacket);
      }
    }
  }

  void INetServer::DisconnectClient(Uint32 theHostID)
  {
    // Attempt to find theHostID provided in our client map
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Now find the client in our map and flag him as disconnected
    anIterator = mClients.find(theHostID);

    // If found, disable the client
    if(anIterator != mClients.end())
    {
      // Prepare and send Disconnect message only if client is enabled
      if(anIterator->second.enabled)
      {
        // Create and send the Disconnect message to the client
        SendPacket(CreateDisconnect(), theHostID);

        // Disable this client from processing further messages
        anIterator->second.enabled = false;

        // Is there a TCP socket? then disconnect and remove it now
        if(anIterator->second.socket)
        {
#if (SFML_VERSION_MAJOR < 2)
          // Remove this socket from our socket selector
          mSelectorTcp.Remove(*anIterator->second.socket);

          // Close the TcpSocket and delete it as we go
          anIterator->second.socket->Close();
#else
          // Remove this socket from our socket selector
          mSelector.remove(*anIterator->second.socket);

          // Close the TcpSocket and delete it as we go
          anIterator->second.socket->disconnect();
#endif

          // Delete each TcpSocket as we go
          delete anIterator->second.socket;
          anIterator->second.socket = NULL;
        }
      }
      else
      {
        WLOG() << "INetServer::DisconnectClient(" << theHostID << ") client already disconnecting" << std::endl;
      }
    }
    else
    {
      ELOG() << "INetServer::DisconnectClient(" << theHostID << ") host not found" << std::endl;
    }
  }

  Uint32 INetServer::GetHostID(bool theReset)
  {
    static Uint32 sHostID = 1; // As the server, we are always HostID 1, clients will be 2 .. 2^32

    // Default to unknown HostID value
    Uint32 anResult = 0;

    // Reset our client HostID counter?
    if(theReset)
    {
      sHostID = 1;
    }

    // Have we hit the maximum possible HostID value? then look for a disabled client
    if(sHostID == ~0)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::iterator anIterator;
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        // Look for a disabled client so we can steal its HostID value
        if(false == anIterator->second.enabled)
        {
          // Reuse its HostID value
          anResult = anIterator->first;

          // Now erase the client from our map so a new client can be added in its place
          mClients.erase(anIterator);

          // Exit our for loop now
          break;
        }
      }

      // An available HostID was not found above
      if(anResult == 0)
      {
        ELOG() << "INetServer::GetHostID() unable to find a HostID to assign" << std::endl;
      }
    }
    else
    {
      // Just increment the HostID value and return it
      anResult = ++sHostID;
    }

    // Return the assigned client HostID value obtained above
    return anResult;
  }

  bool INetServer::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = true;

    // Return the result determined above
    return anResult;
  }

  INetPacket* INetServer::ProcessIncoming(INetPacket& thePacket)
  {
    // Default to no response necessary
    INetPacket* anResult = NULL;

    // Return the response result decided above
    return anResult;
  }

  INetPacket* INetServer::CreateAcknowledgement(const Uint16 theType, const Uint32 theSequenceNumber, bool theYesFlag)
  {
    // Get a packet for our Acknowledgement message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message type and Yes/No flag value
      anResult->SetType(INetPacket::NET_ACKNOWLEDGE);
      anResult->SetFlag(INetPacket::FlagYesResponse, theYesFlag);
      anResult->SetHostID(1); // Server HostID is always 1

      // Add the message type being acknowledged
      *anResult << theType;

      // Add the sequence number being acknowledged
      *anResult << theSequenceNumber;
    }

    // Return the response result created above
    return anResult;
  }

  std::size_t INetServer::GetAcknowledgementSize(void) const
  {
    return INetPacket::HEADER_SIZE_B + sizeof(Uint16) + sizeof(Uint32);
  }

  void INetServer::ProcessAcknowledgement(INetPacket* thePacket)
  {
    // An iterator for finding the client message
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Client/Source HostID number
    Uint32 anSourceID = thePacket->GetHostID();

    // Original message type to be acknowledged
    Uint16 anDestType = 0;

    // Original message sequence number to be acknowledged
    Uint32 anDestSequenceNumber = 0;

    // Retrieve the source message type
    *thePacket >> anDestType;

    // Retrieve the source sequence number
    *thePacket >> anDestSequenceNumber;

    // Find the client resend queue to use
    anIterator = mClients.find(anSourceID);
    if(anIterator != mClients.end())
    {
      // Flag indicating message to be acknowledged was found
      bool anFound = false;

      // Loop through each item in the resend queue to find the message being acknowledged
      for(std::size_t iloop=0;iloop<anIterator->second.resend.size();iloop++)
      {
        // Get the INetPacket from the front of the resend queue
        INetPacket* anPacket = anIterator->second.resend.front();
        assert(anPacket);

        // Erase this message from our resend queue
        anIterator->second.resend.pop();

        // Get the message type and sequence number from the packet
        Uint16 anType = anPacket->GetType();
        Uint32 anSequenceNumber = anPacket->GetSequenceNumber();

        // Does this message match the one we are looking for?
        if(!anFound && anType == anDestType && anSequenceNumber == anDestSequenceNumber)
        {
          // Return the packet now that it has been processed
          mNetPool.ReturnOutgoing(anPacket);

          // Indicate we found the value we were looking for
          anFound = true;

          // Was it the first item? then we can be done
          if(iloop == 0)
          {
            // Exit for loop
            break;
          }
          else
          {
            // Not at beginning, subtract one so we end at same starting place
            iloop--;
          }
        }
        else
        {
          // By pushing the element to the end we can look at the next element
          anIterator->second.resend.push(anPacket);
        }
      } //for(std::size_t iloop=0;iloop<anClientIter->second.resend.size();iloop++)
    }
    else
    {
      ELOG() << "INetServer::ProcessAcknowledgement() Client(" << anSourceID << ") not found" << std::endl;
    }
  }

  std::size_t INetServer::GetConnectSize(void) const
  {
    return INetPacket::HEADER_SIZE_B;
  }

  bool INetServer::ProcessConnect(INetPacket* thePacket,
                                  Uint32 theHostID,
#if (SFML_VERSION_MAJOR < 2)
                                  sf::IPAddress theAddress,
#else
                                  sf::IpAddress theAddress,
#endif
                                  Uint16 thePort)
  {
    // Result indicating we accept this client
    bool anResult = false;

    // Are we using the TCP protocol?
    if(mProtocol == NetTcp)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Now find the client in our map and flag him as disconnected
      anIterator = mClients.find(theHostID);

      // Did we find the TCP client that was previously accepted
      if(anIterator != mClients.end())
      {
        // Change this client to connected
        anIterator->second.connected = true;

        // Accept this client
        anResult = true;
      }
      else
      {
        WLOG() << "INetServer::ProcessConnect() TCP Client("
               << theHostID << ") doesn't exist" << std::endl;
      }
    }
    // Protocol is UDP
    else
    {
      // An iterator for finding the client message
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Look through each client and see if we can find this client
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        // Does the client already exist in our list of clients?
        if(anIterator->second.enabled &&
#if (SFML_VERSION_MAJOR < 2)
           anIterator->second.address.ToInteger() == theAddress.ToInteger() &&
#else
           anIterator->second.address.toInteger() == theAddress.toInteger() &&
#endif
           anIterator->second.port == thePort)
        {
          // Exit the for loop
          break;
        }
      }

      // Set our return result based on client not found
      anResult = anIterator == mClients.end();

      // If client was found then check for banned flag
      if(false == anResult)
      {
        WLOG() << "INetServer::ProcessConnect() UDP Client("
#if (SFML_VERSION_MAJOR < 2)
               << theAddress.ToString()
#else
               << theAddress.toString()
#endif
               << ":" << thePort << ") already exists" << std::endl;
      }
    }

    // Return the acceptance result found above
    return anResult;
  }

  INetPacket* INetServer::CreateDisconnect(void)
  {
    // Get a packet for our Disconnect message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message type and add the assigned HostID as the payload
      anResult->SetType(INetPacket::NET_DISCONNECT);

      // Ack required if using anything except TCP protocol
      anResult->SetFlag(INetPacket::FlagAckRequired, NetTcp != mProtocol);

      // Server is always HostID 1
      anResult->SetHostID(1);
    }

    // Return the response result created above
    return anResult;
  }

  std::size_t INetServer::GetDisconnectSize(void) const
  {
    return INetPacket::HEADER_SIZE_B;
  }

  void INetServer::ProcessDisconnect(INetPacket* thePacket)
  {
    Uint32 anSourceID = thePacket->GetHostID();

    // Log disconnect event
    ILOG() << "INetServer::ProcessDisconnect() Client (" << anSourceID << ") being disconnected" << std::endl;

    // Disconnect this client
    DisconnectClient(anSourceID);
  }

  INetPacket* INetServer::CreateIdentity(Uint32 theHostID)
  {
    // Retrieve a INetPacket to create an Identity message to the client
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message type and add the assigned HostID as the payload
      anResult->SetType(INetPacket::NET_IDENTITY);

      // Ack required if using anything except TCP protocol
      anResult->SetFlag(INetPacket::FlagAckRequired, NetTcp != mProtocol);

      // Server is always HostID 1
      anResult->SetHostID(1);

      // Return the HostID assigned to this client
      *anResult << theHostID;
    }

    // Return the INetPacket created above
    return anResult;
  }

  INetPacket* INetServer::CreateTimeSync1(void)
  {
    // Get source time in microseconds
#if (SFML_VERSION_MAJOR < 2)
    Int64 anSourceTime = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
    Int64 anSourceTime = mTimeSync.getElapsedTime().asMicroseconds();
#endif

    // Retrieve a INetPacket to create a Time Sync 1 message to the client
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the time synchronization message type and the time sync payload
      anResult->SetType(INetPacket::NET_SYNC_1);

      // Server is always HostID 1
      anResult->SetHostID(1);

      // Start with a time stamp from us in microseconds
      *anResult << anSourceTime;
    }

    // Return the INetPacket created above
    return anResult;
  }

  std::size_t INetServer::GetTimeSync1Size(void) const
  {
    return INetPacket::HEADER_SIZE_B + sizeof(Int64)*2;
  }

  void INetServer::ProcessTimeSync1(INetPacket* thePacket)
  {
    Uint32 anSourceID = thePacket->GetHostID();
    Int64 anSourceTime1 = 0LL;
    Int64 anDestTime1 = 0LL;

    // Retrieve the source time from the Sync 1 message
    *thePacket >> anSourceTime1;

    // Retrieve the destination time from the Sync 1 message
    *thePacket >> anDestTime1;

    // Create and send the Sync 2 message back to the client
    SendPacket(CreateTimeSync2(anSourceTime1, anDestTime1), anSourceID);
  }

  INetPacket* INetServer::CreateTimeSync2(Int64 theSourceTime, Int64 theDestTime)
  {
    // Get source time in microseconds
#if (SFML_VERSION_MAJOR < 2)
    Int64 anSourceTime2 = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
    Int64 anSourceTime2 = mTimeSync.getElapsedTime().asMicroseconds();
#endif

    // Retrieve a INetPacket to create a Heartbeat message to the client
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the time synchronization message type and the time sync payload
      anResult->SetType(INetPacket::NET_SYNC_2);

      // Server is always HostID 1
      anResult->SetHostID(1);

      // Return the original time stamp from us in microseconds
      *anResult << theSourceTime;

      // Return the received time stamp from the client in microseconds
      *anResult << theDestTime;

      // Add a 2nd time stamp from us in microseconds
      *anResult << anSourceTime2;
    }

    // Return the INetPacket created above
    return anResult;
  }

  std::size_t INetServer::GetTimeSync2Size(void) const
  {
    return INetPacket::HEADER_SIZE_B + sizeof(Int64)*4;
  }

  void INetServer::ProcessTimeSync2(INetPacket* thePacket)
  {
    // An iterator for finding the client message
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Client/Source HostID number
    Uint32 anSourceID = thePacket->GetHostID();

    // Server timestamp from Time Sync 1 message
    Int64 anSourceTime1 = 0LL;

    // Client timestamp from Time Sync 1 client reply
    Int64 anDestTime1 = 0LL;

    // Server timestamp from Time Sync 2 message
    Int64 anSourceTime2 = 0LL;

    // Client timestamp from Time Sync 2 client reply
    Int64 anDestTime2 = 0LL;

    // Retrieve the source time from the Sync 1 message
    *thePacket >> anSourceTime1;

    // Retrieve the destination time from the Sync 1 message
    *thePacket >> anDestTime1;

    // Retrieve the source time from the Sync 2 message
    *thePacket >> anSourceTime2;

    // Retrieve the destination time from the Sync 2 message
    *thePacket >> anDestTime2;

    // Find the client resend queue to use
    anIterator = mClients.find(anSourceID);

    if(anIterator != mClients.end())
    {
      // First increment our alive counter
      anIterator->second.alive = mAliveMax;

      // Next compute the delay or ping time to the client
      anIterator->second.delay = ((anSourceTime2 - anSourceTime1) + (anDestTime2 - anDestTime1)) / 2;

      // Next compute the clock offset to the local clock
      anIterator->second.offset = ((anDestTime1 - anSourceTime1) + (anDestTime2 - anSourceTime2)) / 2;
    }
    else
    {
      ELOG() << "INetServer::ProcessTimeSync2() Client(" << anSourceID << ") not found" << std::endl;
    }
  }

  void INetServer::Process(void)
  {
    // Which process loop we run depends on the Protocol specified
    if(NetTcp == mProtocol)
    {
      ProcessTcp();
    }
    // Always default to UDP if TCP wasn't specified
    else
    {
      ProcessUdp();
    }
  }

  void INetServer::ProcessTcp(void)
  {
    // Attempt to listen on the port specified
#if (SFML_VERSION_MAJOR < 2)
    if(mListenerTcp.Listen(mServerPort))
#else
    if(mListenerTcp.listen(mServerPort) == sf::Socket::Done)
#endif
    {
      // Log the results
      ILOG() << "INetServer::ProcessTcp() server bound to port(" << mServerPort << ")" << std::endl;

#if (SFML_VERSION_MAJOR < 2)
      // Add Listener socket to socket selector
      mSelectorTcp.Add(mListenerTcp);
#else
      // Add Listener socket to socket selector
      mSelector.add(mListenerTcp);
#endif
    }
    else
    {
      // Unable to listen on the port specified, perhaps it is already open.
      ELOG() << "INetServer::ProcessTcp() error listening on port("
             << mServerPort << ")" << std::endl;

      // Thread is no longer running
      mRunning = false;
    }

    // While process thread is running
    while(mRunning)
    {
      // Wait the specified receive timeout so we can resend messages and exit thread gracefully.
#if (SFML_VERSION_MAJOR < 2)
      unsigned int anReady = mSelectorTcp.Wait(mReceiveTimeout);
      for(unsigned int iloop = 0; iloop<anReady; ++iloop)
#else
      if(mSelector.wait(sf::seconds(mReceiveTimeout)))
#endif
      {
#if (SFML_VERSION_MAJOR < 2)
        // Get the first socket that is ready
        sf::SocketTCP anReadySocket = mSelectorTcp.GetSocketReady(iloop);

        // Check our Listener socket first
        if(anReadySocket == mListenerTcp)
#else
        // Check our Listener socket first
        if(mSelector.isReady(mListenerTcp))
#endif
        {
          ClientInfo anClient(mAliveMax);
#if (SFML_VERSION_MAJOR < 2)
          // Create a new TCP socket for this client
          anClient.socket = new (std::nothrow) sf::SocketTCP();

          // Handle accepting new client connection
          if (mListenerTcp.Accept(*anClient.socket, &anClient.address) == sf::Socket::Done)
#else
          // Create a new TCP socket for this client
          anClient.socket = new (std::nothrow) sf::TcpSocket();

          // Handle accepting new client connection
          if (mListenerTcp.accept(*anClient.socket) == sf::Socket::Done)
#endif
          {
            // Get the HostID to use for this client
            Uint32 anHostID = GetHostID();

            ILOG() << "INetSever::ProcessTcp() accepting client(" << anHostID << ") connection" << std::endl;

            // Insert the client into our map
            mClients.insert(std::pair<const Uint32, ClientInfo>(anHostID, anClient));

#if (SFML_VERSION_MAJOR < 2)
            // Add new client to our selector
            mSelectorTcp.Add(*anClient.socket);
#else
            // Add new client to our selector
            mSelector.add(*anClient.socket);
#endif
          }
          else
          {
            ELOG() << "INetServer::ProcessTcp() error accepting client" << std::endl;
          }
        }
        // Check all our client sockets next
        else
        {
          std::map<const Uint32, ClientInfo>::iterator anIterator;
          for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
          {
            ClientInfo anClient = anIterator->second;
#if (SFML_VERSION_MAJOR < 2)
            if(anClient.socket && anReadySocket == *anClient.socket)
#else
            if(mSelector.isReady(*anClient.socket))
#endif
            {
              // Receive the INetPacket and process internally handled messages
              INetPacket* anIncoming = ReceivePacketTcp(anIterator->first, *anClient.socket);

              // Is there a packet that needs to be processed now?
              if(NULL != anIncoming)
              {
                // Retrieve the HostID from the incoming message for events processed below
                Uint32 anHostID = anIncoming->GetHostID();

                // Process incoming message and immediate send any responses returned
                SendPacket(ProcessIncoming(*anIncoming), anHostID);

                // Return the incoming message packet
                mNetPool.ReturnIncoming(anIncoming);

                // Don't keep pointers around we aren't using anymore
                anIncoming = NULL;
              }
            } // if(mSelector.isReady(anClient.socket))
          } // for each client in mClients
        } // if(mSelector.isReady(mListenerTcp))
#if (SFML_VERSION_MAJOR < 2)
      } // for(unsigned int iloop = 0; iloop<anReady; ++iloop)
      // Do some resending now
      if(anReady == 0)
#else
      } // if(mSelector.wait(sf::seconds(mReceiveTimeout)))
      // Do some resending now
      else
#endif
      {
        // Process messages to send/resend
        ProcessSend();
      }
    } // while(mRunning)

    // Close our TCP listener socket at this point
#if (SFML_VERSION_MAJOR < 2)
    mListenerTcp.Close();
#else
    mListenerTcp.close();
#endif

    // Server is shutting down, send disconnect event to each connected client
    if(!mClients.empty())
    {
      // Client list iterator
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Loop through each client and send a disconnect message
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        // Disconnect this client
        DisconnectClient(anIterator->first);
      }

      // Clear the list of clients
      mClients.clear();
    }
  }

  void INetServer::ProcessUdp(void)
  {
    // Bind it (listen) to the port
#if (SFML_VERSION_MAJOR < 2)
    if (true == mListenerUdp.Bind(mServerPort))
#else
    if (mListenerUdp.bind(mServerPort) == sf::Socket::Done)
#endif
    {
      // Log the results
      ILOG() << "INetServer::ProcessUdp() server bound to port(" << mServerPort << ")" << std::endl;

      // Add server socket to socket selector
#if (SFML_VERSION_MAJOR < 2)
      mSelectorUdp.Add(mListenerUdp);
#else
      mSelector.add(mListenerUdp);
#endif
    }
    else
    {
      // Unable to listen on the port specified, perhaps it is already open.
      ELOG() << "INetServer::ProcessUdp() error listening on port(" << mServerPort << ")" << std::endl;

      // Server thread is no longer running
      mRunning = false;
    }

    while(mRunning)
    {
      // Wait the specified receive timeout so we can resend messages and exit thread gracefully.
#if (SFML_VERSION_MAJOR < 2)
      if(mSelectorUdp.Wait(mReceiveTimeout) > 0)
#else
      if(mSelector.wait(sf::seconds(mReceiveTimeout)) && mSelector.isReady(mListenerUdp))
#endif
      {
        // Receive the INetPacket and preprocess it for validation and Ack messages
        INetPacket* anIncoming = ReceivePacketUdp();

        // Is there a packet that needs to be processed now?
        if(NULL != anIncoming)
        {
          // Retrieve the HostID from the incoming message for events processed below
          Uint32 anHostID = anIncoming->GetHostID();

          // Process incoming message and immediate send any responses returned
          SendPacket(ProcessIncoming(*anIncoming), anHostID);

          // Return the incoming message packet
          mNetPool.ReturnIncoming(anIncoming);

          // Don't keep pointers around we aren't using anymore
          anIncoming = NULL;
        }
      } // if(mSelector.wait(sf::seconds(mReceiveTimeout)) && mSelector.isReady(mListenerUdp))
      // Do some resending now
      else
      {
        ProcessSend();
      }
    } // while(mRunning)

    // Server is shutting down, send disconnect event to each connected client
    if(!mClients.empty())
    {
      // Client list iterator
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Loop through each client and send a disconnect message
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        // Disconnect this client
        DisconnectClient(anIterator->first);
      }

      // Clear the list of clients
      mClients.clear();
    }

    // Close our server socket at this point
#if (SFML_VERSION_MAJOR < 2)
    mListenerUdp.Close();
#else
    mListenerUdp.unbind();
#endif
  }

  void INetServer::ProcessSend(void)
  {
    // Client list iterator
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Loop through each client and resend any messages that are waiting to be resent
    for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
    {
      // Is this client enabled?
      if(anIterator->second.enabled)
      {
        // Should we send another time synchronization message sequence?
#if (SFML_VERSION_MAJOR < 2)
        if(anIterator->second.timesync.GetElapsedTime() >= mTimeSyncTimeout)
#else
        if(anIterator->second.timesync.getElapsedTime().asSeconds() >= mTimeSyncTimeout)
#endif
        {
          // Decrement our alive counter every time we send a time sync message
          if(0 < --anIterator->second.alive)
          {
            // Send the heartbeat monitor now
            SendPacket(CreateTimeSync1(), anIterator->first);

            // Reset our sync clock
#if (SFML_VERSION_MAJOR < 2)
            anIterator->second.timesync.Reset();
#else
            anIterator->second.timesync.restart();
#endif
          }
          // Client hasn't responded, disconnect client
          else
          {
            // Disconnect the client
            DisconnectClient(anIterator->first);
          }
        } // if(anIterator->second.sync.getElapsedTime().asSeconds() >= mTimeSyncTimeout)

        // Do we need to resend items in our resend queue?
        while(!anIterator->second.resend.empty() && anIterator->second.resend.front() &&
#if (SFML_VERSION_MAJOR < 2)
              (anIterator->second.resend.front()->GetLastSent() * 1000.0) > mResendTimeout)
#else
              anIterator->second.resend.front()->GetLastSent().asMilliseconds() > mResendTimeout)
#endif
        {
          // Retrieve the outgoing INetPacket from the resend queue
          INetPacket* anOutgoing = anIterator->second.resend.front();
          assert(anOutgoing);

          // Pop it off the resend queue
          anIterator->second.resend.pop();

          // Send the outgoing packet (which will return it to the resend queue
          SendPacket(anOutgoing, anIterator->first);
        }
      } // if(anIterator->second.enabled)
    } // for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
  }

  bool INetServer::VerifyInternal(INetPacket& thePacket, std::size_t theSize)
  {
    // Default to invalid message until proven valid
    bool anResult = false;

    // Get the message type now and verify the size
    Uint16 anSourceType = thePacket.GetType();

    // Validate the known message types according to size
    switch(anSourceType)
    {
      // Header size only message types
      case INetPacket::NET_CONNECT:
        if(GetConnectSize() == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetServer::VerifyInternal() invalid connect message size("
                 << (Uint32)GetConnectSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_DISCONNECT:
        if(GetDisconnectSize() == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetServer::VerifyInternal() invalid disconnect message size("
                 << (Uint32)GetDisconnectSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_ACKNOWLEDGE:
        if(GetAcknowledgementSize() == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetServer::VerifyInternal() invalid acknowledge message size("
                 << (Uint32)GetAcknowledgementSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_SYNC_1:
        if(GetTimeSync1Size() == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetServer::VerifyInternal() invalid TimeSync pass 1 message size("
                 << (Uint32)GetTimeSync1Size() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_SYNC_2:
        if(GetTimeSync2Size() == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetServer::VerifyInternal() invalid TimeSync pass 2 message size("
                 << (Uint32)GetTimeSync2Size() << "!=" << theSize << ")" << std::endl;
        }
        break;

      // All other unknown message types
      default:
        // Default to true for all other unknown message types
        anResult = true;
        break;
    }

    // Return the valid result discovered above
    return anResult;
  }

  INetPacket* INetServer::ProcessInternal(INetPacket* thePacket)
  {
    // Client list iterator
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Default to returning thePacket address as our return result
    INetPacket* anResult = NULL;

    // Get the INetPacket message type
    Uint16 anSourceType = thePacket->GetType();

    // Source HostID to use for various messages below
    Uint32 anSourceID = thePacket->GetHostID();

    // Source SequenceNumber to use for Acknowledgement message
    Uint32 anSourceSequenceNumber = thePacket->GetSequenceNumber();

    // Does thePacket require an acknowledgement?
    if(true == thePacket->GetFlag(INetPacket::FlagAckRequired))
    {
      // Send the Acknowledgement message now
      SendPacket(CreateAcknowledgement(anSourceType, anSourceSequenceNumber), anSourceID);
    }

    // Try to find the source HostID in our client map
    anIterator = mClients.find(anSourceID);

    // Not the last one we processed? then process it now
    if(anIterator == mClients.end() || anSourceSequenceNumber != anIterator->second.lastSN)
    {
      // Update our lastSN value first
      anIterator->second.lastSN = anSourceSequenceNumber;

      // Switch on the known message types
      switch(anSourceType)
      {
        case INetPacket::NET_DISCONNECT:
          // Call the ProcessDisconnect method to handle this message
          ProcessDisconnect(thePacket);
          break;
        case INetPacket::NET_ACKNOWLEDGE:
          // Call the ProcessAcknowledgement method to handle this message
          ProcessAcknowledgement(thePacket);
          break;
        case INetPacket::NET_SYNC_1:
          // Call ProcessTimeSync1 to process the Time Sync 1 message reply
          ProcessTimeSync1(thePacket);
          break;
        case INetPacket::NET_SYNC_2:
          // Call ProcessTimeSync2 to process the Time Sync 2 message reply
          ProcessTimeSync2(thePacket);
          break;
        default: // All other message types will be processed by ProcessIncoming
          anResult = thePacket;
          break;
      }
    }
    else
    {
      // Don't process it again
    }

    // Return the result specified above
    return anResult;
  }

#if (SFML_VERSION_MAJOR < 2)
  INetPacket* INetServer::ReceivePacketTcp(const Uint32 theHostID, sf::SocketTCP& theSocket)
#else
  INetPacket* INetServer::ReceivePacketTcp(const Uint32 theHostID, sf::TcpSocket& theSocket)
#endif
  {
    // INetPacket return result to store response to be received
    INetPacket* anResult = mNetPool.GetIncoming();

    // Size of the message received in bytes
    std::size_t anReceived = 0;

    // Receive the message now from the socket if possible
#if (SFML_VERSION_MAJOR < 2)
    if (theSocket.Receive((char*)anResult->GetData(), anResult->GetCapacity(), anReceived) == sf::Socket::Done)
#else
    if (theSocket.receive(anResult->GetData(), anResult->GetCapacity(), anReceived) == sf::Socket::Done)
#endif
    {
      // Get the server timestamp for this message
#if (SFML_VERSION_MAJOR < 2)
      Int64 anTimestamp = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
      Int64 anTimestamp = mTimeSync.getElapsedTime().asMicroseconds();
#endif

      // Set our incomging timestamp value
      SetTimestamp(anResult, anTimestamp);

      // Too small or failed some verification test? then return NULL to the caller
      if(INetPacket::HEADER_SIZE_B > anReceived ||
         !anResult->HasSync() ||
         !anResult->IsVersionValid() ||
         !VerifyInternal(*anResult, anReceived) ||
         !VerifyIncoming(*anResult, anReceived))
      {
        // Return the INetPacket since the message is invalid
        mNetPool.ReturnIncoming(anResult);

        // Return NULL as the result
        anResult = NULL;
      }
      // Valid message
      else
      {
        // NET_CONNECT message types are handled here for UDP and ProcessTcp for TCP
        if(INetPacket::NET_CONNECT == anResult->GetType())
        {
          // See if ProcessConnect accepts this client
          bool anAccept = ProcessConnect(anResult, theHostID);

          // Was the client accepted? then send client his HostID value now
          if(anAccept)
          {
            // Create and send the Identity message response for this new client
            SendPacket(CreateIdentity(theHostID), theHostID);

            // Log connect event
            ILOG() << "INetServer::ReceivePacketTcp() New client (" << theHostID << ")" << std::endl;
          }

          // Return the incoming message
          mNetPool.ReturnIncoming(anResult);

          // No need for further processing of the incoming message
          anResult = NULL;
        }
        else
        {
          // See what ProcessInternal returns
          INetPacket* anInternal = ProcessInternal(anResult);

          // If NULL then return the incoming message
          if(NULL == anInternal)
          {
            // Return the incoming message it has now been processed
            mNetPool.ReturnIncoming(anResult);
          }

          // Return the result of ProcessInternal
          anResult = anInternal;
        }
      }
    }
    else
    {
      // Unable to receive incoming packet, perhaps socket was reset mid message
      ELOG() << "INetServer::ReceivePacketTcp() error receiving incoming packet" << std::endl;

      // Return the INetPacket since the message is invalid
      mNetPool.ReturnIncoming(anResult);

      // Return NULL as the result
      anResult = NULL;

      // Disconnect the client
      DisconnectClient(theHostID);
    }

    // Return the INetPacket if it is still valid and hasn't been processed yet
    return anResult;
  }

  INetPacket* INetServer::ReceivePacketUdp(void)
  {
    // Structure for storing the address and port information for this client
    ClientInfo anClient(mAliveMax);

    // INetPacket return result to store response to be received
    INetPacket* anResult = mNetPool.GetIncoming();

    // Size of the message received in bytes
    std::size_t anReceived = 0;

    // Receive the message now from the socket if possible
#if (SFML_VERSION_MAJOR < 2)
    if (mListenerUdp.Receive((char*)anResult->GetData(), anResult->GetCapacity(), anReceived,
                             anClient.address, anClient.port) == sf::Socket::Done)
#else
    if (mListenerUdp.receive(anResult->GetData(), anResult->GetCapacity(), anReceived,
                             anClient.address, anClient.port) == sf::Socket::Done)
#endif
    {
      // Get the server timestamp for this message
#if (SFML_VERSION_MAJOR < 2)
      Int64 anTimestamp = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
      Int64 anTimestamp = mTimeSync.getElapsedTime().asMicroseconds();
#endif

      // Set our incomging timestamp value
      SetTimestamp(anResult, anTimestamp);

      // Too small or failed some verification test? then return NULL to the caller
      if(INetPacket::HEADER_SIZE_B > anReceived ||
         !anResult->HasSync() ||
         !anResult->IsVersionValid() ||
         !VerifyInternal(*anResult, anReceived) ||
         !VerifyIncoming(*anResult, anReceived))
      {
        // Return the INetPacket since the message is invalid
        mNetPool.ReturnIncoming(anResult);

        // Return NULL as the result
        anResult = NULL;
      }
      // Valid message
      else
      {
        // NET_CONNECT message types are handled here for UDP and ProcessTcp for TCP
        if(INetPacket::NET_CONNECT == anResult->GetType())
        {
          // See if ProcessConnect accepts this client
          bool anAccept = ProcessConnect(anResult, 0, anClient.address, anClient.port);

          // Was the client accepted? then add the client now
          if(anAccept)
          {
            // Set the connected flag to true
            anClient.connected = true;

            // Assign a HostID for this new client
            Uint32 anSourceID = GetHostID();

            // Insert the client into our map
            mClients.insert(std::pair<const Uint32, ClientInfo>(anSourceID, anClient));

            // Create and send the Identity message response for this new client
            SendPacket(CreateIdentity(anSourceID), anSourceID);

            // Log connect event
            ILOG() << "INetServer::ReceivePacketUdp() New client (" << anSourceID << ")" << std::endl;
          }

          // Return the incoming message
          mNetPool.ReturnIncoming(anResult);

          // No need for further processing of the incoming message
          anResult = NULL;
        }
        // Otherwise call ProcessInternal to handle every other message type
        else
        {
          // See what ProcessInternal returns
          INetPacket* anInternal = ProcessInternal(anResult);

          // If NULL then return the incoming message
          if(NULL == anInternal)
          {
            // Return the incoming message it has now been processed
            mNetPool.ReturnIncoming(anResult);
          }

          // Return the result of ProcessInternal
          anResult = anInternal;
        }
      }
    }
    else
    {
      // Unable to receive incoming packet, perhaps socket was reset mid message
      ELOG() << "INetServer::ReceivePacketUdp() error receiving incoming packet" << std::endl;

      // Return the INetPacket since the message is invalid
      mNetPool.ReturnIncoming(anResult);

      // Return NULL as the result
      anResult = NULL;
    }

    // Return the INetPacket if it is still valid and hasn't been processed yet
    return anResult;
  }

  void INetServer::SetTimestamp(INetPacket* thePacket, const Int64 theTimestamp)
  {
    // An iterator for finding the client message
    std::map<const Uint32, ClientInfo>::iterator anIterator;

    // Was a valid INetPacket provided?
    if(NULL != thePacket)
    {
      // Attempt to find the client using the HostID from thePacket
      anIterator = mClients.find(thePacket->GetHostID());

      // Did we find the client? then get the offset and set the timestamp
      if(anIterator != mClients.end())
      {
        // Set the offset adjusted timestamp for this incoming message
        thePacket->SetTimestamp(theTimestamp + anIterator->second.offset);
      }
    }
    else
    {
      ELOG() << "INetServer::SetTimestamp() null packet pointer provided" << std::endl;
    }
  }

} // namespace GQE

/**
 * Copyright (c) 2010-2013 Ryan Lindeman
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
