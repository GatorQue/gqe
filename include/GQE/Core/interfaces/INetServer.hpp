/**
 * Provides the INetServer interface that defines the default UDP/TCP server
 * interface and implementation. Classes can be derived to provide a custom
 * protocol that is independent of either UDP or TCP Ethernet protocols.
 *
 * @file include/GQE/Core/interfaces/INetServer.hpp
 * @author Ryan Lindeman
 * @date 20130104 - Initial Release
 * @date 20130111 - Generalized VerifyInternal, ProcessInternal, and better sequence number support
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
      /// How many clients are allowed to connect
      static const Uint16 MAX_CLIENTS = 64;
      /// How long to keep trying to resend ack required messages before failing
      static const float MAX_RESEND_TIMEOUT_S;
      /// How long to wait for incoming messages before calling ProcessSend
      static const float RECEIVE_TIMEOUT_S;

      /**
       * INetServer default constructor
       * @param[in] theServerID (title) to use for this server
       * @param[in] theServerVersion to use for this server
       * @param[in] theNetPool derived class to use for getting INetPackets
       * @param[in] theProtocol to use for this NetServer class
       * @param[in] theServerPort to listen on for incoming UDP clients
       * @param[in] theResendTimeout is how long to wait before resending messages
       * @param[in] theMaxResendTimeout is how long to keep resending before giving up
       * @param[in] theReceiveTimeout is how long to wait for incoming message
       * @param[in] theAliveMax is the maximum missed time sync messages
       */
      INetServer(const typeServerID theServerID,
                 const typeVersionInfo theServerVersion,
                 INetPool& theNetPool,
                 const NetProtocol theProtocol = NetUdp,
                 const Uint16 theServerPort = 10101,
                 const Uint16 theMaxClients = MAX_CLIENTS,
                 const float theTimeSyncTimeout = TIME_SYNC_TIMEOUT_S,
                 const Int32 theResendTimeout = RESEND_TIMEOUT_MS,
                 const float theMaxResendTimeout = MAX_RESEND_TIMEOUT_S,
                 const float theReceiveTimeout = RECEIVE_TIMEOUT_S,
                 const Int8 theAliveMax = ALIVE_MAX);

      /**
       * INetServer deconstructor
       */
      virtual ~INetServer();

      /**
       * GetServerInfo is responsible for providing the server information
       * structure for this server. This includes the server ID (title),
       * address, port, and number of active clients and maximum clients.
       * @return theServerInfo structure with the server details
       */
      typeServerInfo GetServerInfo(void) const;

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
       * GetServerID is responsible for returning theServerID to use for this
       * server.
       * @return theServerID for this server
       */
      typeServerID GetServerID(void) const;

      /**
       * SetServerID is responsible for setting theServerID to use for this
       * server. This can only be done if the server is not running (see
       * IsRunning).
       * @param[in] theServerID to use for the server
       */
      void SetServerID(const typeServerID theServerID);

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
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The ServerID (title) to use for this server
      typeServerID mServerID;
      /// Server Version information
      typeVersionInfo mServerVersion;
      /// Network pool address to retrieve and return INetPacket derived classes from/to
      INetPool& mNetPool;

      /**
       * GetHostID is responsible for returning the HostID that will be used
       * to identify each client. This is called by either the
       * ReceivePacketTcp or the ReceivePacketUdp methods. If the maximum
       * possible HostID has been reached it will look through mClients to
       * find the first disabled client and use its HostID instead.
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
       * ProcessIncoming is responsible for processing all incoming network
       * packet messages from each UDP client and providing an optional
       * immediate network packet message response (theTransaction.outgoing is
       * no longer null).
       * @param[in] theIncoming INetPacket to be processed
       * @return pointer to outgoing INetPacket response, NULL otherwise
       */
      virtual INetPacket* ProcessIncoming(INetPacket* theIncoming);

      /**
       * ProcessOutgoing is responsible for creating any outgoing network
       * packet messages from this server to the clients. A custom protocol who
       * wishes to generate outgoing network packets should redefine this
       * method and provide that functionality.
       */
      virtual void ProcessOutgoing(void);

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
       * GetAcknowledgementSize is responsible for returning the size of the
       * Acknowledgement message. This way someone can modify the
       * CreateAcknowledgement method in INetClient and still have the
       * INetServer base class validate each Acknowledgement message size
       * correctly.
       * @return the Acknowledgement message size
       */
      virtual std::size_t GetAcknowledgementSize(void) const;

      /**
       * ProcessAcknowledgement is responsible for processing each
       * Acknowledgement message received.
       * @param[in] thePacket containing the acknowledgement message
       */
      void ProcessAcknowledgement(INetPacket* thePacket);

      /**
       * CreateBroadcast is responsible for creating the Broadcast message
       * reply to a broadcast message request from a local client. The reply
       * includes the ServerID specified.
       * @return pointer to outgoing INetPacket response, NULL otherwise
       */
      virtual INetPacket* CreateBroadcast(void);

      /**
       * GetBroadcastSize is responsible for returning the size of the
       * Broadcast message. This way someone can modify the CreateBroadcast
       * method in INetClient and still have the INetServer base class
       * validate each Broadcast message size correctly.
       * @return the Broadcast message size
       */
      virtual std::size_t GetBroadcastSize(void) const;

      /**
       * GetConnectSize is responsible for returning the size of the Connect
       * message. This way someone can modify the CreateConnect method in the
       * INetClient class and still have the INetServer base class validate
       * each Connect message size.
       * @return the Connect message size (defaults to HEADER_SIZE_B)
       */
      virtual std::size_t GetConnectSize(void) const;

      /**
       * ProcessConnect is responsible for processing each connect message
       * received. Typically this just checks to see if the client already
       * exists and is not banned before calling CreateIdentity to send the
       * Identity message reply.
       * @param[in] thePacket containing the connect message
       * @param[in] theHostID assigned when client was accepted (TCP only)
       * @param[in] theAddress as was received (UDP only)
       * @param[in] thePort as was received (UDP only)
       */
      bool ProcessConnect(INetPacket* thePacket,
                          Uint32 theHostID = 0,
#if (SFML_VERSION_MAJOR < 2)
                          sf::IPAddress theAddress = sf::IPAddress::LocalHost,
#else
                          sf::IpAddress theAddress = sf::IpAddress::LocalHost,
#endif
                          Uint16 thePort = 0);

      /**
       * CreateDisconnect is responsible for providing a custom Disconnect
       * message that will be sent to client when the UdpServer is about to
       * shutdown. The caller must return true and provide
       * theTransaction.outgoing INetPacket.
       * @return pointer to INetPacket with Disconnect message, NULL otherwise
       */
      virtual INetPacket* CreateDisconnect(void);

      /**
       * GetDisconnectSize is responsible for returning the size of the
       * Disconnect message. This way someone can modify the CreateDisconnect
       * method in the INetClient class and still have the INetServer base
       * class validate each Disconnect message size.
       * @return the Disconnect message size (defaults to HEADER_SIZE_B)
       */
      virtual std::size_t GetDisconnectSize(void) const;

      /**
       * ProcessDisconnect is responsible for processing each Disconnect
       * message received. By default it calls DisconnectClient.
       * @param[in] thePacket containing theDisconnect message
       */
      void ProcessDisconnect(INetPacket* thePacket);

      /**
       * CreateIdentity is responsible for creating a client message that will
       * let the client know the HostID it should use for further messages to
       * the server.
       * @param[in] theHostID that the client is being told to use
       * @return pointer to INetPacket with Identity message, NULL otherwise
       */
      virtual INetPacket* CreateIdentity(const Uint32 theHostID);

      /**
       * CreateTimeSync1 is responsible for creating the Sync 1 message that
       * is used to compute the round trip time between the server and each
       * client.
       * @return pointer to INetPacket with TimeSync message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync1(void);

      /**
       * GetTimeSync1Size is responsible for returning the size of the
       * Time Sync 1 message. This way someone can modify the CreateTimeSync1
       * method in the INetClient class and still have the INetServer base
       * class validate each Time Sync 1 message size.
       * @return the Time Sync 1 message size
       */
      virtual std::size_t GetTimeSync1Size(void) const;

      /**
       * ProcessTimeSync1 is responsible for processing each Time Sync 1
       * message received. By default it calls CreateTimeSync2 to create the
       * Time Sync 2 message response.
       * @param[in] thePacket containing theTimeSync1 client response message
       */
      void ProcessTimeSync1(INetPacket* thePacket);

      /**
       * CreateTimeSync2 is responsible for creating the Sync 2 message that
       * is used to compute the round trip time between the server and each
       * client.
       * @param[in] theSourceTime from the Sync 1 message sent earlier
       * @param[in] theDestTime from the Sync 1 message reply received
       * @return pointer to INetPacket with TimeSync message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync2(Int64 theSourceTime, Int64 theDestTime);

      /**
       * GetTimeSync2Size is responsible for returning the size of the
       * Time Sync 2 message. This way someone can modify the CreateTimeSync2
       * method in the INetClient class and still have the INetServer base
       * class validate each Time Sync 2 message size.
       * @return the Time Sync 2 message size
       */
      virtual std::size_t GetTimeSync2Size(void) const;

      /**
       * ProcessTimeSync2 is responsible for processing each Time Sync 2
       * message received. By default it calculates the delay between the
       * server and the client and computes the timestamp offset to use for
       * this client to provide a network wide timestamp that both sides can
       * use to organize game time sensitive events.
       * @param[in] thePacket containing theTimeSync2 client response message
       */
      void ProcessTimeSync2(INetPacket* thePacket);

    private:
      // Structures
      ///////////////////////////////////////////////////////////////////////////
      /// ClientInfo structure holds the data needed for each client
      struct ClientInfo
      {
        /// Client is currently enabled
        bool                   enabled;
        /// Client has connected
        bool                   connected;
        /// Next sequence number to use for next message to client
        Uint32                 sequence;
        /// Last sequence number processed from the client
        Uint32                 lastSN;
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
        /// Client ID to identify this client
        typeClientID           clientID;
        /// Client version info
        typeVersionInfo        version;
        /// Port number to UDP client
        Uint16                 port;
        /// Alive confidence counter
        Int8                   alive;
        /// The delay or ping time to this client from the server in microseconds
        Int64                  delay;
        /// The clock offset to use to when computing time values for this client
        Int64                  offset;
        /// A timer to determine when to send the next time sync to this client
        sf::Clock              timesync;
        /// The resend queue of NetPackets to be resent to client
        std::queue<INetPacket*> resend;
        ClientInfo(const Int8 theAliveMax = ALIVE_MAX) :
          enabled(true),
          connected(false),
          sequence(0),
          lastSN(0),
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

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Protocol flag for this NetServer
      NetProtocol mProtocol;
      /// Server port to listen for incoming UDP clients
      Uint16 mServerPort;
#if (SFML_VERSION_MAJOR < 2)
      /// Server public address for use with GetServerInfo
      sf::IPAddress mServerAddress;
      /// UDP Listener server socket for incoming/outgoing Server UDP packets
      sf::SocketUDP mListenerUdp;
      /// TCP Listener server socket for incoming TCP clients
      sf::SocketTCP mListenerTcp;
      /// TCP Socket selector for monitoring all client connections
      sf::SelectorTCP mSelectorTcp;
      /// UDP Socket selector for monitoring all client connections
      sf::SelectorUDP mSelectorUdp;
#else
      /// Server public address for use with GetServerInfo
      sf::IpAddress mServerAddress;
      /// UDP Listener server socket for incoming/outgoing Server UDP packets
      sf::UdpSocket mListenerUdp;
      /// TCP Listener server socket for incoming TCP clients
      sf::TcpListener mListenerTcp;
      /// Socket selector for monitoring all client connections
      sf::SocketSelector mSelector;
#endif
      /// Map of clients using the Host ID returned
      std::map<const Uint32, ClientInfo> mClients;
      /// Maximum number of active clients allowed
      Uint16 mMaxClients;
      /// Number of active clients currently connected
      Uint16 mActiveClients;
      /// Clock for the source of all time synchronization messages
      sf::Clock mTimeSync;
      /// Time sync send timeout (seconds) determines how frequent to send time sync messages
      float mTimeSyncTimeout;
      /// Resend timeout (milliseconds) is used to determine when to resend messages that require acknowledgements
      Int32 mResendTimeout;
      /// Maximum resend timeout (seconds) before stopping the resending of ack required messages
      float mMaxResendTimeout;
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

      /**
       * Our copy constructor is private because we do not allow copies of
       * our INetServer derived classes
       */
      INetServer(const INetServer&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our INetServer derived classes
       */
      INetServer& operator=(const INetServer&); // Intentionally undefined
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
