/**
 * Provides the INetServer interface that defines the default UDP/TCP server
 * interface and implementation. Classes can be derived to provide a custom
 * protocol that is independent of either UDP or TCP Ethernet protocols.
 *
 * @file include/GQE/Core/interfaces/INetServer.hpp
 * @author Ryan Lindeman
 * @date 20130104 - Initial Release
 */
#ifndef INET_SERVER_HPP_INCLUDED
#define INET_SERVER_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/INetPacket.hpp>
#include <GQE/Core/interfaces/IProcess.hpp>
#include <map>
#include <queue>

namespace GQE
{
  /// Provides the INetServer class for providing a network server services
  class GQE_API INetServer : public IProcess
  {
    public:
      // Constants
      ///////////////////////////////////////////////////////////////////////////
      /// How many Time Sync 2 messages can we miss before client is disconnected?
      static const Int8 ALIVE_MAX = 3;
      /// How long to wait before sending time sync messages
      static const float TIME_SYNC_TIMEOUT_S;
      /// How long to wait for acknowledgement messages before resending messages
      static const Int32 RESEND_TIMEOUT_MS = 33;  // about 30 Hz
      /// How long to wait for incoming messages before calling ProcessSend
      static const float RECEIVE_TIMEOUT_S;

      /**
       * INetServer default constructor
       * @param[in] theProtocol to use for this NetServer class
       * @param[in] theServerPort to listen on for incoming UDP clients
       * @param[in] theResendTimeout is how long to wait before resending messages
       * @param[in] theReceiveTimeout is how long to wait for incoming message
       * @param[in] theAliveMax is the maximum missed time sync messages
       */
      INetServer(const NetProtocol theProtocol = NetUdp,
                 const Uint16 theServerPort = 10101,
                 const float theTimeSyncTimeout = TIME_SYNC_TIMEOUT_S,
                 const Int32 theResendTimeout = RESEND_TIMEOUT_MS,
                 const float theReceiveTimeout = RECEIVE_TIMEOUT_S,
                 const Int8 theAliveMax = ALIVE_MAX);

      /**
       * INetServer deconstructor
       */
      virtual ~INetServer();

      /**
       * GetPacket is responsible for returning a INetPacket derived class
       * to be used for either an Incoming message. ProcessTransaction will
       * then be called to process the incoming message received and provide an
       * optional immediate response to be sent for the incoming message.
       * @param[in] theCapacity to use for the INetPacket provided
       * @return a pointer to the packet to use for incoming/outgoing messages
       */
      virtual INetPacket* GetPacket(std::size_t theCapacity = INetPacket::HEADER_SIZE_B);

      /**
       * ReturnPacket will be called for both the incoming and outgoing
       * packets provided by GetPacket and ProcessTransaction after it has
       * completed the task of sending the immediate response.
       * @param[in] thePacket to be returned
       */
      virtual void ReturnPacket(INetPacket* thePacket);

      /**
       * GetTimestamp will return the offset adjusted time in microsecocnds
       * of the specified client using theHostID that can be used for comparing
       * to the timestamp received from the INetPacket class (see
       * INetPacket::GetTimestamp()). If theHostID is 1 then the server
       * timestamp value will be immediately returned instead.
       * @param[in] theHostID of the client to retrieve timestamp for
       * @return the client offset adjusted timestamp
       */
      Int64 GetTimestamp(Uint32 theHostID) const;

      /**
       * SendPacket is the central method used to send all NetPackets. The
       * reason for centralizing this is so that NetPackets that require
       * acknowledgements can be resent when the ACK messages fail to come in.
       * @param[in] thePacket to be sent
       * @param[in] theHostID of the client to send the message to
       */
      void SendPacket(INetPacket* thePacket, const Uint32 theHostID);

      /**
       * DisconnectClient is responsible for disconnecting client identified by
       * theHostID provided.
       * @param[in] theHostID to be disconnected
       */
      void DisconnectClient(Uint32 theHostID);

    protected:
      // Structures
      ///////////////////////////////////////////////////////////////////////////
      /// ClientInfo structure holds the data needed for each client
      struct ClientInfo
      {
        /// Client is currently enabled
        bool                   enabled;
        /// Next sequence number to use for next message to client
        Uint32                 sequence;
#if (SFML_VERSION_MAJOR < 2)
        /// Socket to use for TCP clients
        sf::SocketTCP*         socket;
        /// IPv4 address to UDP or TCP client
        sf::IPAddress          address;
#else
        /// Socket to use for TCP clients
        sf::TcpSocket*         socket;
        /// IPv4 address to UDP client
        sf::IpAddress          address;
#endif
        /// Port number to UDP client
        Uint16                 port;
        /// Alive confidence counter
        Int8                   alive;
        /// The delay or ping time to this client from the server in microseconds
        Int64                  delay;
        /// The clock offset to use to when computing time values for this client
        Int64                  offset;
        /// A timer to determine when to send the next time sync to this client
        sf::Clock              sync;
        /// The resend queue of NetPackets to be resent to client
        std::queue<INetPacket*> resend;
        ClientInfo(const Int8 theAliveMax = ALIVE_MAX) :
          enabled(true),
          sequence(0),
          socket(NULL),
#if (SFML_VERSION_MAJOR < 2)
          address(sf::IPAddress::LocalHost),
#else
          address(sf::IpAddress::LocalHost),
#endif
          port(0),
          alive(theAliveMax),
          delay(0LL),
          offset(0LL)
        {
        }
      };

      /**
       * GetHostID is responsible for returning the HostID that will be used
       * to identify each client. For UDP connections this will occur when the
       * ProcessTransaction method returns a ServerNewClient event. If at a
       * later time the derived class determines that the client should be
       * forced to disconnect then the ProcessTransaction can return a
       * DisconnectClient event code to force the client to be disconnected
       * which will free up the HostID for another client to use at some
       * later time if desired. The default implementation is to just return
       * the next HostID and increment the HostID value for the next caller.
       * @param[in] theReset flag to reset the HostID counter back to 1 again
       * @return theHostID to use for the current client (2 .. 2^32)
       */
      virtual GQE::Uint32 GetHostID(bool theReset = false);

      /**
       * VerifyIncoming is responsible for verifying the incoming INetPacket
       * message for all user defined message types. The internally processed
       * INetPacket messages will be verified through the VerifyInternal method.
       * @param[in] thePacket to be verified
       * @param[in] theSize received in bytes
       * @return true if thePacket is valid, false otherwise
       */
      virtual bool VerifyIncoming(INetPacket& thePacket, std::size_t theSize);

      /**
       * ProcessTransaction is responsible for processing all incoming network
       * packet messages from each UDP client and providing an optional
       * immediate network packet message response (theTransaction.outgoing is
       * no longer null).
       * @param[in] theIncoming INetPacket to be processed
       * @return pointer to outgoing INetPacket response, NULL otherwise
       */
      virtual INetPacket* ProcessIncoming(INetPacket& theIncoming);

      /**
       * CreateAcknowledgement is responsible for creating the acknowledgement
       * message for those messages received with the FlagAckRequired set. The
       * message includes the message type and sequence number as well as a
       * boolean yes/no flag indicating a positive or negative acknowledgement
       * response as needed.
       * @param[in] theType of the message being acknowledged
       * @param[in] theSequenceNumber of the message being acknowledged
       * @param[in] theYesFlag to use in the acknowledgement message
       * @return pointer to outgoing INetPacket response, NULL otherwise
       */
      virtual INetPacket* CreateAcknowledgement(const Uint16 theType,
                                               const Uint32 theSequenceNumber,
                                               bool theYesFlag = true);

      /**
       * CreateDisconnect is responsible for providing a custom Disconnect
       * message that will be sent to client when the UdpServer is about to
       * shutdown. The caller must return true and provide
       * theTransaction.outgoing INetPacket.
       * @return pointer to INetPacket with Disconnect message, NULL otherwise
       */
      virtual INetPacket* CreateDisconnect(void);

      /**
       * CreateIdentity is responsible for creating a client message that will
       * let the client know the HostID it should use for further messages to
       * the server.
       * @param[in] theHostID that the client is being told to use
       * @return pointer to INetPacket with Identity message, NULL otherwise
       */
      virtual INetPacket* CreateIdentity(Uint32 theHostID);

      /**
       * CreateTimeSync1 is responsible for creating the Sync 1 message that
       * is used to compute the round trip time between the server and each
       * client.
       * @return pointer to INetPacket with TimeSync message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync1(void);

      /**
       * CreateTimeSync2 is responsible for creating the Sync 2 message that
       * is used to compute the round trip time between the server and each
       * client.
       * @param[in] theSourceTime from the Sync 1 message sent earlier
       * @param[in] theDestTime from the Sync 1 message reply received
       * @return pointer to INetPacket with TimeSync message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync2(Int64 theSourceTime, Int64 theDestTime);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Protocol flag for this NetServer
      NetProtocol mProtocol;
      /// Server port to listen for incoming UDP clients
      Uint16 mServerPort;
#if (SFML_VERSION_MAJOR < 2)
      /// UDP Listener server socket for incoming/outgoing Server UDP packets
      sf::SocketUDP mListenerUdp;
      /// TCP Listener server socket for incoming TCP clients
      sf::SocketTCP mListenerTcp;
      /// TCP Socket selector for monitoring all client connections
      sf::SelectorTCP mSelectorTcp;
      /// UDP Socket selector for monitoring all client connections
      sf::SelectorUDP mSelectorUdp;
#else
      /// UDP Listener server socket for incoming/outgoing Server UDP packets
      sf::UdpSocket mListenerUdp;
      /// TCP Listener server socket for incoming TCP clients
      sf::TcpListener mListenerTcp;
      /// Socket selector for monitoring all client connections
      sf::SocketSelector mSelector;
#endif
      /// Map of clients using the Host ID returned
      std::map<const Uint32, ClientInfo> mClients;
      /// Clock for the source of all time synchronization messages
      sf::Clock mTimeSync;
      /// Time sync send timeout (seconds) determines how frequent to send time sync messages
      float mTimeSyncTimeout;
      /// Resend timeout (milliseconds) is used to determine when to resend messages that require acknowledgements
      Int32 mResendTimeout;
      /// Receive timeout (seconds) is used to determine how long to wait for incoming messages
      float mReceiveTimeout;
      /// Alive counter maximum before client will be terminated
      Int8 mAliveMax;

      /**
       * Process is the process thread responsible for calling either
       * ProcessTcp or ProcessUdp depending on the protocol specified at
       * construction time. This method overrides the Process thread in the
       * IProcess base class.
       */
      virtual void Process(void);

      /**
       * ProcessTcp is the process thread responsible for providing the TCP
       * Server thread implementation.
       */
      void ProcessTcp(void);

      /**
       * ProcessUdp is the process thread responsible for providing the UDP
       * Server thread implementation.
       */
      void ProcessUdp(void);

      /**
       * ProcessSend is responsible for processing all messages that need to
       * be sent or resent to the clients. This logic is the same for both
       * UDP and TCP Ethernet protocols.
       */
      void ProcessSend(void);

      /**
       * VerifyInternal is responsible for verifying the internally processed
       * INetPacket messages. It also verified the Sync and Version bytes as
       * part of the process. All user defined messages will be verified by the
       * VerifyIncoming method instead.
       * @param[in] thePacket to be verified
       * @param[in] theSize received in bytes
       * @return true if thePacket is valid, false otherwise
       */
      bool VerifyInternal(INetPacket& thePacket, std::size_t theSize);

      /**
       * ProcessInternal is responsible for processing internally recognized
       * messages such as Acknowledgements, Connect, and Disconnect messages
       * so derived classes can keep their ProcessIncoming messages simple.
       * @param[in] thePacket to be processed
       * @return pointer to thePacket if no internally message was processed
       */
      INetPacket* ProcessInternal(INetPacket* thePacket);

      /**
       * ReceivePacketTcp is the central method used to receive all NetPackets
       * using the TCP server protocol. The reason for centralizing this is so
       * that Acknowledgements can be processed without cluttering up the
       * ProcessTcp thread.
       * @param[in] theHostID assigned to this client (for disconnect purposes)
       * @param[in] theSocket to use to receive the incoming INetPacket from
       * @return pointer to INetPacket to process, NULL otherwise
       */
#if (SFML_VERSION_MAJOR < 2)
      INetPacket* ReceivePacketTcp(const Uint32 theHostID, sf::SocketTCP& theSocket);
#else
      INetPacket* ReceivePacketTcp(const Uint32 theHostID, sf::TcpSocket& theSocket);
#endif

      /**
       * ReceivePacketUdp is the central method used to receive all NetPackets
       * using the UDP server protocol. The reason for centralizing this is so
       * that Acknowledgements can be processed without cluttering up the
       * ProcessUdp thread.
       * @return pointer to INetPacket to process, NULL otherwise
       */
      INetPacket* ReceivePacketUdp(void);

      /**
       * SetTimestamp is responsible for setting the offset adjusted timestamp
       * for each incoming INetPacket.
       * @param[inout] thePacket to set the timestamp for
       * @param[in] theTimestamp in microseconds when the packet was received
       */
      void SetTimestamp(INetPacket* thePacket, const Int64 theTimestamp);
  }; // INetServer class
} // namespace GQE
#endif // INET_SERVER_HPP_INCLUDED

/**
 * @class GQE::INetServer
 * @ingroup Core
 * The INetServer class is responsible for providing both TCP and UDP server
 * services for the NetManager class. The INetServer class implements only the
 * essential messages necessary to allow for an INetClient class to connect,
 * disconnect, acknowledge messages, and time synchronize itself with each
 * client. It is expected that the programmer will inherit from the INetServer
 * class and provide their own message types specific to their application.
 *
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
