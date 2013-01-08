/**
 * Provides the INetClient interface that defines the default UDP/TCP client
 * interface and implementation. Classes can be derived to provide a custom
 * protocol that is independent of either UDP or TCP Ethernet protocols.
 *
 * @file src/GQE/Core/interfaces/INetClient.cpp
 * @author Ryan Lindeman
 * @date 20121206 - Initial Release
 */
#include <SFML/System.hpp>
#include <GQE/Core/interfaces/INetClient.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <assert.h>

namespace GQE
{
  // Special constants that must be defined here in the CPP file
  const float INetClient::RECEIVE_TIMEOUT_S = 0.5f;
  const float INetClient::CONNECT_RETRY_TIMEOUT_S = 1.0f;
  const float INetClient::CONNECT_TIMEOUT_S = 30.0f;

  INetClient::INetClient(const NetProtocol theProtocol,
#if (SFML_VERSION_MAJOR < 2)
                         const sf::IPAddress theServerAddress,
#else
                         const sf::IpAddress theServerAddress,
#endif
                         const Uint16 theServerPort,
                         const Uint16 theClientPort,
                         const Int32 theResendTimeout,
                         const float theReceiveTimeout,
                         const float theRetryTimeout,
                         const float theConnectTimeout) :
    IProcess(),
    mProtocol(theProtocol),
    mServerAddress(theServerAddress),
    mServerPort(theServerPort),
    mClientPort(theClientPort),
    mContact(NetTcp != theProtocol ? true : false), // Only used for TCP, true for UDP
    mConnected(false),
    mConnectTimeout(theConnectTimeout),
    mRetryTimeout(theRetryTimeout),
    mHostID(0),
    mResendTimeout(theResendTimeout),
    mReceiveTimeout(theReceiveTimeout),
    mDelay(0LL),
    mOffset(0LL)
  {
    ILOG() << "INetClient(" << (theProtocol == NetTcp ? "TCP" : "UDP") << ","
#if (SFML_VERSION_MAJOR < 2)
           << theServerAddress.ToString() << "," << theServerPort << ","
#else
           << theServerAddress.toString() << "," << theServerPort << ","
#endif
           << theClientPort << "," << theResendTimeout << ","
           << theReceiveTimeout << "," << theRetryTimeout << ","
           << theConnectTimeout << ")" << std::endl;
  }

  INetClient::~INetClient()
  {
  }

  Uint32 INetClient::GetHostID(void) const
  {
    return mHostID;
  }

  bool INetClient::IsConnected(void) const
  {
    return mConnected;
  }

  INetPacket* INetClient::GetPacket(std::size_t theCapacity)
  {
    INetPacket* anResult = new (std::nothrow) INetPacket(theCapacity);
    assert(anResult);

    return anResult;
  }

  void INetClient::ReturnPacket(INetPacket* thePacket)
  {
    delete thePacket;
  }

  Int64 INetClient::GetTimestamp(void) const
  {
#if (SFML_VERSION_MAJOR < 2)
    return (Int64)(mTimeSync.GetElapsedTime() * 1000000.0) + mOffset;
#else
    return mTimeSync.getElapsedTime().asMicroseconds() + mOffset;
#endif
  }

  void INetClient::SendPacket(INetPacket* thePacket)
  {
    // Sequence number for each packet that requires an ACK response
    static Uint32 sSequenceNumber = 0;

    // Validate that the packet provided is correct
    if(NULL != thePacket)
    {
      // Only send the packet if connection has been established
      if(mContact)
      {
        // Assign a sequence number to each packet sent
        thePacket->SetSequenceNumber(++sSequenceNumber);

        if(NetTcp == mProtocol)
        {
          // Send the packet to the server
#if (SFML_VERSION_MAJOR < 2)
          mClientTcp.Send((char*)thePacket->GetData(), thePacket->GetSize());
#else
          mClientTcp.send(thePacket->GetData(), thePacket->GetSize());
#endif
        }
        else
        {
          // Send the packet to the server
#if (SFML_VERSION_MAJOR < 2)
          mClientUdp.Send((char*)thePacket->GetData(), thePacket->GetSize(),
                          mServerAddress, mServerPort);
#else
          mClientUdp.send(thePacket->GetData(), thePacket->GetSize(),
                          mServerAddress, mServerPort);
#endif
        }

        // Reset our last sent timer right after sending it
        thePacket->SetLastSent();

        // Check to see if an Ack message is required for this message
        if(thePacket->GetFlag(INetPacket::FlagAckRequired))
        {
          // Put this INetPacket on the resend list
          mResend.push(thePacket);
        }
        else
        {
          // Return the INetPacket since it is no longer needed
          ReturnPacket(thePacket);
        }
      }
      else
      {
        // Return the INetPacket until since we haven't established contact
        ReturnPacket(thePacket);
      }
    } // if(NULL != thePacket)
  }

  void INetClient::DisconnectClient(void)
  {
    // Only send disconnect message if connection was previously established
    if(mConnected)
    {
      // Send Disconnect message to server
      SendPacket(CreateDisconnect());

      // Set our mConnected flag to false
      mConnected = false;
    }
    else
    {
      ILOG() << "INetClient::DisconnectClient() client not connected" << std::endl;
    }

    // TCP protocol specific handling of DisconnectClient
    if(NetTcp == mProtocol)
    {
#if (SFML_VERSION_MAJOR < 2)
      // Remove this socket from our selector
      mSelectorTcp.Remove(mClientTcp);

      // Make sure our client is disconnected if it wasn't previously disconnected
      mClientTcp.Close();
#else
      // Remove this socket from our selector
      mSelector.remove(mClientTcp);

      // Make sure our client is disconnected if it wasn't previously disconnected
      mClientTcp.disconnect();
#endif

      // Reset our contact boolean value too
      mContact = false;
    }
  }

  bool INetClient::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = true;

    // Return the result determined above
    return anResult;
  }

  INetPacket* INetClient::ProcessIncoming(INetPacket& thePacket)
  {
    // Default to no response necessary
    INetPacket* anResult = NULL;

    // Return the response result decided above
    return anResult;
  }

  INetPacket* INetClient::CreateAcknowledgement(const Uint16 theType, const Uint32 theSequenceNumber, bool theYesFlag)
  {
    // Get a packet for our Acknowledgement message
    INetPacket* anResult = GetPacket();

    // Assign the message type and add the assigned HostID to the message
    anResult->SetType(INetPacket::NET_ACKNOWLEDGE);
    anResult->SetFlag(INetPacket::FlagYesResponse, theYesFlag);
    anResult->SetHostID(mHostID);

    // Add the message type being acknowledged
    *anResult << theType;

    // Add the sequence number being acknowledged
    *anResult << theSequenceNumber;

    // Return the response result created above
    return anResult;
  }

  INetPacket* INetClient::CreateConnect(void)
  {
    INetPacket* anResult = NULL;

    // UDP protocol uses a Connect message, TCP uses the typical socket.connect() call
    if(NetUdp == mProtocol)
    {
      // Get a packet for our Connect message
      anResult = GetPacket();

      // Assign the message type and add the assigned HostID to the message
      anResult->SetType(INetPacket::NET_CONNECT);

      // For UDP the client doesn't have a HostID yet so default to 0
      anResult->SetHostID(0);
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* INetClient::CreateDisconnect(void)
  {
    // Get a packet for our Disconnect message
    INetPacket* anResult = GetPacket();

    // Assign the message type and add the assigned HostID as the payload
    anResult->SetType(INetPacket::NET_DISCONNECT);

    // Ack required if using anything except TCP protocol
    anResult->SetFlag(INetPacket::FlagAckRequired, NetTcp != mProtocol);

    // Use the assigned HostID from the server (0 if we don't have one)
    anResult->SetHostID(mHostID);

    // Return the response result created above
    return anResult;
  }

  INetPacket* INetClient::CreateTimeSync1(Int64 theSourceTime)
  {
    // Get destination time in microseconds
#if (SFML_VERSION_MAJOR < 2)
    Int64 anDestTime = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0);
#else
    Int64 anDestTime = mTimeSync.getElapsedTime().asMicroseconds();
#endif

    // Retrieve a INetPacket to create a Time Sync 1 response message to the server
    INetPacket* anResult = GetPacket();

    // Assign the time synchronization message type and the time sync payload
    anResult->SetType(INetPacket::NET_SYNC_1);

    // Use the assigned HostID from the server (0 if we don't have one)
    anResult->SetHostID(mHostID);

    // Start with a time stamp received from the server in microseconds
    *anResult << theSourceTime;

    // Add the 1st time stamp from us in microseconds
    *anResult << anDestTime;

    // Return the INetPacket created above
    return anResult;
  }

  INetPacket* INetClient::CreateTimeSync2(Int64 theSourceTime1, Int64 theDestTime1, Int64 theSourceTime2)
  {
    // Get 2nd destination time in microseconds
#if (SFML_VERSION_MAJOR < 2)
    Int64 anDestTime2 = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0);
#else
    Int64 anDestTime2 = mTimeSync.getElapsedTime().asMicroseconds();
#endif

    // Retrieve a INetPacket to create a Time Sync 2 response message to the server
    INetPacket* anResult = GetPacket();

    // Assign the time synchronization message type and the time sync payload
    anResult->SetType(INetPacket::NET_SYNC_2);

    // Use the assigned HostID from the server (0 if we don't have one)
    anResult->SetHostID(mHostID);

    // Start with a 1st time stamp received from the server in microseconds
    *anResult << theSourceTime1;

    // Return the original time stamp from us in microseconds
    *anResult << theDestTime1;

    // Start with a 2nd time stamp received from the server in microseconds
    *anResult << theSourceTime2;

    // Add the 2nd time stamp from us in microseconds
    *anResult << anDestTime2;

    // Now compute the delay or ping time to the server
    mDelay = ((theSourceTime2 - theSourceTime1) + (anDestTime2 - theDestTime1)) / 2;

    // Next compute the clock offset to the local clock
    mOffset = ((theSourceTime1 - theDestTime1) + (theSourceTime2 - anDestTime2)) / 2;

    // Return the INetPacket created above
    return anResult;
  }

  void INetClient::Process(void)
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

  void INetClient::ProcessTcp(void)
  {
    // Main process loop
    while(mRunning)
    {
      // Stay in this loop while process is running and contact has not been made with server
      while(mRunning && !mContact)
      {
        // Wait for connection to be ready within specified connect timeout.
#if (SFML_VERSION_MAJOR < 2)
        if(mClientTcp.Connect(mServerPort,mServerAddress,mConnectTimeout) == sf::Socket::Done)
#else
        if(mClientTcp.connect(mServerAddress,mServerPort,sf::seconds(mConnectTimeout)) == sf::Socket::Done)
#endif
        {
#if (SFML_VERSION_MAJOR < 2)
          // Add our client connection to the selector after establishing contact with server
          mSelectorTcp.Add(mClientTcp);
#else
          // Add our client connection to the selector after establishing contact with server
          mSelector.add(mClientTcp);
#endif

          // We have made contact with server, next try and establish a connection
          mContact = true;

          // Send Connect message as needed
          SendPacket(CreateConnect());
        }
        else
        {
          ELOG() << "INetClient::ProcessTcp() connection to ("
#if (SFML_VERSION_MAJOR < 2)
                 << mServerAddress.ToString() << ":"
#else
                 << mServerAddress.toString() << ":"
#endif
                 << mServerPort << ") failed, trying again." << std::endl;

          // Sleep for a little while before trying again
#if (SFML_VERSION_MAJOR < 2)
          sf::Sleep(mRetryTimeout);
#else
          sf::sleep(sf::seconds(mRetryTimeout));
#endif
        }
      }

      // Stay in this loop while process is running and contact has been made
      while(mRunning && mContact)
      {
        // Wait the specified receive timeout so we can resend messages and exit thread gracefully.
#if (SFML_VERSION_MAJOR < 2)
        if(mSelectorTcp.Wait(mReceiveTimeout))
#else
        if(mSelector.wait(sf::seconds(mReceiveTimeout)) && mSelector.isReady(mClientTcp))
#endif
        {
          // Receive the INetPacket and preprocess it for validation and Ack messages
          INetPacket* anIncoming = ReceivePacket();

          // Is there a packet that needs to be processed now?
          if(NULL != anIncoming)
          {
            // Process incoming message and immediate send any responses returned
            SendPacket(ProcessIncoming(*anIncoming));

            // Return the incoming message packet
            ReturnPacket(anIncoming);

            // Don't keep pointers around we aren't using anymore
            anIncoming = NULL;
          }
        } // if(mSelector.wait(sf::seconds(0.5f)) && mSelector.isReady(mClient))
        // Do some resends now
        else
        {
          // Process messages to send/resend
          ProcessSend();
        }
      } // while(mRunning && anContact)
    } // while(mRunning)

    // If we made contact with the server, be sure to close the connection
    if(mContact)
    {
      // Disconnect the client from the server
      DisconnectClient();
    }
  }

  void INetClient::ProcessUdp(void)
  {
    // Clock to keep track of connection attempts
    sf::Clock anConnect;

    // Bind it (listen) to the client port provided
#if (SFML_VERSION_MAJOR < 2)
    if (true == mClientUdp.Bind(mClientPort))
#else
    if (mClientUdp.bind(mClientPort) == sf::Socket::Done)
#endif
    {
      // Make a note of the port that was actually bound for this client
#if (SFML_VERSION_MAJOR < 2)
      mClientPort = mClientUdp.GetPort();
#else
      mClientPort = mClientUdp.getLocalPort();
#endif

      // Log the results
      ILOG() << "INetClient::ProcessUdp() client bound to port("
             << mClientPort << ")" << std::endl;

#if (SFML_VERSION_MAJOR < 2)
      // Add our client connection to the selector
      mSelectorUdp.Add(mClientUdp);
#else
      // Add our client connection to the selector
      mSelector.add(mClientUdp);
#endif
    }
    else
    {
      // Unable to listen on the port specified, perhaps it is already open.
      ELOG() << "INetClient::ProcessUdp() error listening on port("
             << mClientPort << ")" << std::endl;

      // Client thread is no longer running
      mRunning = false;
    }

    // Main process loop
    while(mRunning)
    {
      // Send Connection message to server every 1 second until connection is made
#if (SFML_VERSION_MAJOR < 2)
      if(!mConnected && anConnect.GetElapsedTime() > mRetryTimeout)
#else
      if(!mConnected && anConnect.getElapsedTime().asSeconds() > mRetryTimeout)
#endif
      {
        // Send Connect message to the server
        SendPacket(CreateConnect());

        // Reset our connect timer
#if (SFML_VERSION_MAJOR < 2)
        anConnect.Reset();
#else
        anConnect.restart();
#endif
      }

      // Wait the specified receive timeout so we can resend messages and exit thread gracefully.
#if (SFML_VERSION_MAJOR < 2)
      if(mSelectorUdp.Wait(mReceiveTimeout))
#else
      if(mSelector.wait(sf::seconds(mReceiveTimeout)) && mSelector.isReady(mClientUdp))
#endif
      {
        // Receive the INetPacket and preprocess it for validation and Ack messages
        INetPacket* anIncoming = ReceivePacket();

        // Is there a packet that needs to be processed now?
        if(NULL != anIncoming)
        {
          // Process incoming message and immediate send any responses returned
          SendPacket(ProcessIncoming(*anIncoming));

          // Return the incoming message packet
          ReturnPacket(anIncoming);

          // Don't keep pointers around we aren't using anymore
          anIncoming = NULL;
        }
      } // if(mSelector.wait(sf::seconds(mReceiveTimeout)) && mSelector.isReady(mClientUdp))
      else
      {
        // Process messages to send/resend
        ProcessSend();
      }
    } // while(mRunning)

    // Be sure to close the connection on exit of process server
    DisconnectClient();

#if (SFML_VERSION_MAJOR < 2)
    // Remove our client socket from the selector
    mSelectorUdp.Remove(mClientUdp);

    // Unbind our client socket now that we are done
    mClientUdp.Close();
#else
    // Remove our client socket from the selector
    mSelector.remove(mClientUdp);

    // Unbind our client socket now that we are done
    mClientUdp.unbind();
#endif
  }

  void INetClient::ProcessSend(void)
  {
    // Do we need to resend items in our resend queue?
#if (SFML_VERSION_MAJOR < 2)
    while(!mResend.empty() && mResend.front() && (mResend.front()->GetLastSent() * 1000.0f) > mResendTimeout)
#else
    while(!mResend.empty() && mResend.front() && mResend.front()->GetLastSent().asMilliseconds() > mResendTimeout)
#endif
    {
      // Retrieve the outgoing INetPacket from the resend queue
      INetPacket* anOutgoing = mResend.front();

      // Pop it off the resend queue
      mResend.pop();

      // Send the outgoing packet (which will return it to the resend queue
      SendPacket(anOutgoing);
    }
  }

  bool INetClient::VerifyInternal(INetPacket& thePacket, std::size_t theSize)
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
      case INetPacket::NET_DISCONNECT:
        if(INetPacket::HEADER_SIZE_B == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetClient::VerifyInternal() invalid connect/disconnect message size" << std::endl;
        }
        break;

      // Header + 4 bytes only message types
      case INetPacket::NET_IDENTITY:
        if((INetPacket::HEADER_SIZE_B + sizeof(Uint32)) == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetClient::VerifyInternal() invalid identity message size" << std::endl;
        }
        break;

      // Header + 6 bytes only message types
      case INetPacket::NET_ACKNOWLEDGE:
        if((INetPacket::HEADER_SIZE_B + sizeof(Uint16) + sizeof(Uint32)) == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetClient::VerifyInternal() invalid acknowledge message size" << std::endl;
        }
        break;

      // Header + 8 bytes only message types
      case INetPacket::NET_SYNC_1:
        if((INetPacket::HEADER_SIZE_B + sizeof(Int64)) == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetClient::VerifyInternal() invalid TimeSync pass 1 message size" << std::endl;
        }
        break;

      // Header + 24 bytes only message types
      case INetPacket::NET_SYNC_2:
        if((INetPacket::HEADER_SIZE_B + sizeof(Int64)*3) == theSize)
        {
          // Message type is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "INetClient::VerifyInternal() invalid TimeSync pass 2 message size" << std::endl;
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

  INetPacket* INetClient::ProcessInternal(INetPacket* thePacket)
  {
    // Default to returning thePacket address as our return result
    INetPacket* anResult = thePacket;

    // Get the INetPacket message type
    Uint16 anSourceType = thePacket->GetType();

    // Source HostID to use for various messages below
    Uint32 anSourceID = thePacket->GetHostID();

    // Source SequenceNumber to use for Acknowledgement message
    Uint32 anSourceSequenceNumber = thePacket->GetSequenceNumber();

    // Does thePacket require an acknowledgement?
    if(true == thePacket->GetFlag(INetPacket::FlagAckRequired) && mHostID != 0)
    {
      // Send the Acknowledgement message now
      SendPacket(CreateAcknowledgement(anSourceType, anSourceSequenceNumber));
    }

    // Destination message type
    Uint16 anDestType = 0;

    // Destination message sequence number;
    Uint32 anDestSequenceNumber = 0;

    // Destination HostID to use for various messages below
    Uint32 anDestID = 0;

    // Source time from when time synchronization pass 1 was sent
    Int64 anSourceTime1 = 0;

    // Source time from when time synchronization pass 2 was sent
    Int64 anDestTime1 = 0;

    // Source time from when time synchronization pass 3 was sent
    Int64 anSourceTime2 = 0;

    // Found flag for processing resend queues
    bool anFound = false;

    // Switch on the known message types
    switch(anSourceType)
    {
      case INetPacket::NET_IDENTITY:
        // Retrieve the assigned Host ID
        *thePacket >> anDestID;

        // Set our HostID from Identity message
        SetHostID(anDestID);

        // Log identity event
        ILOG() << "INetClient::ProcessInternal() Identity message (" << anDestID << ")" << std::endl;

        // Indicate that our connection to the server has been established
        mConnected = true;

        // Return NULL for our result since we have already processed thePacket
        anResult = NULL;
        break;
      case INetPacket::NET_DISCONNECT:
        // Return NULL for our result since we have already processed thePacket
        anResult = NULL;

        // If we were previously connected, just try and connect again
        if(mConnected)
        {
          // Log disconnect event
          ILOG() << "INetClient::ProcessInternal() Disconnect message" << std::endl;
        }
        else
        {
          // Log disconnect event
          ILOG() << "INetClient::ProcessInternal() Disconnect/Banned message" << std::endl;

          // We must be banned from this server, stop running the client
          mRunning = false;
        }

        // Disconnect this client gracefully and attempt to reconnect again
        DisconnectClient();
        break;
      case INetPacket::NET_ACKNOWLEDGE:
        // Retrieve the source message type
        *thePacket >> anDestType;

        // Retrieve the source sequence number
        *thePacket >> anDestSequenceNumber;

        // Loop through each item in the resend queue to find the message being acknowledged
        for(std::size_t iloop=0;iloop<mResend.size();iloop++)
        {
          // Get the INetPacket from the front of the resend queue
          INetPacket* anPacket = mResend.front();
          assert(anPacket);

          // Erase this message from our resend queue
          mResend.pop();

          // Get the message type and sequence number from the packet
          Uint16 anType = anPacket->GetType();
          Uint32 anSequenceNumber = anPacket->GetSequenceNumber();

          // Does this message match the one we are looking for?
          if(!anFound && anType == anDestType && anSequenceNumber == anDestSequenceNumber)
          {
            // Return the packet now that it has been processed
            ReturnPacket(anPacket);

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
            mResend.push(anPacket);
          }
        } //for(std::size_t iloop=0;iloop<mResend.size();iloop++)

        // Return NULL for our result since we have already processed thePacket
        anResult = NULL;
        break;
      case INetPacket::NET_SYNC_1:
        // Retrieve the source time from the server
        *thePacket >> anSourceTime1;

        // Send the time synchronization pass 2 reply to the server
        SendPacket(CreateTimeSync1(anSourceTime1));

        // Return NULL for our result since we have already processed thePacket
        anResult = NULL;
        break;
      case INetPacket::NET_SYNC_2:
        // Retrieve the source time 1 from the server
        *thePacket >> anSourceTime1;

        // Retrieve the destination time from us earlier
        *thePacket >> anDestTime1;

        // Retrieve the source time 2 from the server
        *thePacket >> anSourceTime2;

        // Send the time synchronization pass 3 reply to the server
        SendPacket(CreateTimeSync2(anSourceTime1, anDestTime1, anSourceTime2));

        // Log time synchronization 2 event
        ILOG() << "INetClient::ProcessInternal() Sync 2 from server (" << anSourceID
               << ") delay=" << mDelay << " offset=" << mOffset << std::endl;

        // Return NULL for our result since we have already processed thePacket
        anResult = NULL;
        break;
      // TODO: Add detection of other internal messages
      default: // All other message types will be processed by ProcessIncoming
        /* do nothing */
        break;
    }

    // Return the result specified above
    return anResult;
  }

  INetPacket* INetClient::ReceivePacket(void)
  {
    // Structure for storing the address and port information for a UDP client
#if (SFML_VERSION_MAJOR < 2)
    sf::IPAddress anAddress;
#else
    sf::IpAddress anAddress;
#endif
    Uint16 anPort;

    // INetPacket return result to store response to be received
    INetPacket* anResult = GetPacket(520);

    // Size of the message received in bytes
    std::size_t anReceived = 0;

    // Socket status result variable
    sf::Socket::Status anStatus = sf::Socket::Error;

    // Boolean indicating server address and port matches (needed for UDP)
    bool anAddressCheck = true;

    // Timestamp obtained right after the incoming message is received
    Int64 anTimestamp = 0LL;

    // The protocol we are using will determine which socket we use
    if(NetTcp == mProtocol)
    {
#if (SFML_VERSION_MAJOR < 2)
      // Attempt to receive data from our TCP client socket
      anStatus = mClientTcp.Receive((char*)anResult->GetData(), anResult->GetCapacity(), anReceived);

      // Get the client timestamp for this message
      anTimestamp = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);
#else
      // Attempt to receive data from our TCP client socket
      anStatus = mClientTcp.receive(anResult->GetData(), anResult->GetCapacity(), anReceived);

      // Get the client timestamp for this message
      anTimestamp = mTimeSync.getElapsedTime().asMicroseconds();
#endif
    }
    else
    {
#if (SFML_VERSION_MAJOR < 2)
      // Attempt to receive data from our UDP client socket
      anStatus = mClientUdp.Receive((char*)anResult->GetData(), anResult->GetCapacity(),
                        anReceived, anAddress, anPort);

      // Get the client timestamp for this message
      anTimestamp = (Int64)(mTimeSync.GetElapsedTime() * 1000000.0f);

      // Perform a quick address check for UDP protocols
      anAddressCheck = (anAddress.ToInteger() == mServerAddress.ToInteger() &&
                        anPort == mServerPort);
#else
      // Attempt to receive data from our UDP client socket
      anStatus = mClientUdp.receive(anResult->GetData(), anResult->GetCapacity(), anReceived,
                        anAddress, anPort);

      // Get the client timestamp for this message
      anTimestamp = mTimeSync.getElapsedTime().asMicroseconds();

      // Perform a quick address check for UDP protocols
      anAddressCheck = (anAddress.toInteger() == mServerAddress.toInteger() &&
                        anPort == mServerPort);
#endif
    }

    // What was the socket status result obtained above?
    if(anStatus == sf::Socket::Done)
    {
      // Set the offset adjusted timestamp for this incoming message
      anResult->SetTimestamp(anTimestamp + mOffset);

      // Too small or failed some verification test? then return NULL to the caller
      if(INetPacket::HEADER_SIZE_B > anReceived ||
         !anAddressCheck ||
         !anResult->HasSync() ||
         !anResult->IsVersionValid() ||
         !VerifyInternal(*anResult, anReceived) ||
         !VerifyIncoming(*anResult, anReceived))
      {
        // Return the INetPacket since the message is invalid
        ReturnPacket(anResult);

        // Return NULL as the result
        anResult = NULL;
      }
      // Valid message
      else
      {
        // See what ProcessInternal returns
        INetPacket* anInternal = ProcessInternal(anResult);

        // If NULL then return the incoming message
        if(NULL == anInternal)
        {
          // Return the incoming message it has now been processed
          ReturnPacket(anResult);
        }

        // Return the result of ProcessInternal
        anResult = anInternal;
      }
    }
    else
    {
      // Unable to receive incoming packet, perhaps socket was reset mid message
      ELOG() << "INetClient::ReceivePacket() error receiving incoming packet" << std::endl;

      // Return the INetPacket since the message is invalid
      ReturnPacket(anResult);

      // Return NULL as the result
      anResult = NULL;

      // Disconnect this client
      DisconnectClient();
    }

    // Return the INetPacket if it is still valid and hasn't been processed yet
    return anResult;
  }

  void INetClient::SetHostID(const Uint32 theHostID)
  {
    mHostID = theHostID;
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
