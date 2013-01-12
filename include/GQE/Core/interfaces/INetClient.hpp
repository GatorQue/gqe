/**
 * Provides the INetClient interface that defines the default UDP/TCP client
 * interface and implementation. Classes can be derived to provide a custom
 * protocol that is independent of either UDP or TCP Ethernet protocols.
 *
 * @file include/GQE/Core/interfaces/INetClient.hpp
 * @author Ryan Lindeman
 * @date 20121206 - Initial Release
 * @date 20130111 - Generalized VerifyInternal, ProcessInternal, and better sequence number support
 */
#ifndef INET_CLIENT_HPP_INCLUDED
#define INET_CLIENT_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/INetPacket.hpp>
#include <GQE/Core/interfaces/IProcess.hpp>
#include <queue>

namespace GQE
{
  /// Provides the UdpClient class for providing a UDP Client service
  class GQE_API INetClient : public IProcess
  {
    public:
      // Constants
      ///////////////////////////////////////////////////////////////////////////
      /// How many Time Sync 2 messages can we miss before client is disconnected?
      static const Int8 ALIVE_MAX = 3;
      /// How long to wait for acknowledgement messages before resending messages
      static const Int32 RESEND_TIMEOUT_MS = 33;  // about 30 Hz
      /// How long to wait for incoming messages before calling ProcessSend
      static const float RECEIVE_TIMEOUT_S;
      /// How long to wait for between connection retries for UDP and TCP clients
      static const float CONNECT_RETRY_TIMEOUT_S;
      /// How long to wait for connection to be established for TCP clients
      static const float CONNECT_TIMEOUT_S;

      /**
       * INetClient default constructor
       * @param[in] theNetPool derived class to use for getting INetPackets
       * @param[in] theProtocol to use for this client
       * @param[in] theServerAddress to connect to
       * @param[in] theServerPort to connect to
       * @param[in] theClientPort to send/listen on
       * @param[in] theResendTimeout to wait before resending messages
       * @param[in] theReceiveTimeout to wait for new messages before checking resend pool
       * @param[in] theRetryTimeout to wait before retrying to connect to server
       * @param[in] theConnectTimeout to wait for connection to be established (TCP only)
       */
      INetClient(INetPool& theNetPool,
                 const NetProtocol theProtocol = NetUdp,
#if (SFML_VERSION_MAJOR < 2)
                 const sf::IPAddress theServerAddress = sf::IPAddress::LocalHost,
#else
                 const sf::IpAddress theServerAddress = sf::IpAddress::LocalHost,
#endif
                 const Uint16 theServerPort = 10101,
                 const Uint16 theClientPort = 0,
                 const Int32 theResendTimeout = RESEND_TIMEOUT_MS,
                 const float theReceiveTimeout = RECEIVE_TIMEOUT_S,
                 const float theRetryTimeout = CONNECT_RETRY_TIMEOUT_S,
                 const float theConnectTimeout = CONNECT_TIMEOUT_S);

      /**
       * INetClient deconstructor
       */
      virtual ~INetClient();

      /**
       * GetHostID is responsible for returning the HostID that has been
       * assigned by the server for this client. The caller should also use the
       * IsConnected method to determine if the value returned can be
       * considered legal.
       * @return theHostID for this client
       */
      GQE::Uint32 GetHostID(void) const;

      /**
       * IsConnected will return true if the client has connected to the
       * server.
       * @return true if connected, false otherwise
       */
      bool IsConnected(void) const;

      /**
       * GetTimestamp will return the offset adjusted time in microsecocnds
       * that can be used for comparing to the timestamp received from the
       * INetPacket class (see INetPacket::GetTimestamp()).
       * @return the offset adjusted timestamp
       */
      Int64 GetTimestamp(void) const;

      /**
       * SendPacket is the central method used to send all NetPackets. The
       * reason for centralizing this is so that NetPackets that require
       * acknowledgements can be resent when the ACK messages fail to come in.
       * @param[in] thePacket to be sent
       */
      void SendPacket(INetPacket* thePacket);

      /**
       * DisconnectClient is responsible for disconnecting this client from the
       * server.
       */
      void DisconnectClient(void);

    protected:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Network pool address to retrieve and return INetPacket derived classes from/to
      INetPool& mNetPool;

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
       * GetAcknowledgementSize is responsible for returning the size of the
       * Acknowledgement message. This way someone can modify the
       * CreateAcknowledgement method in INetServer and still have the
       * INetClient base class validate each Acknowledgement message size
       * correctly.
       * @return the Acknowledgement message size
       */
      virtual std::size_t GetAcknowledgementSize(void) const;

      /**
       * ProcessAcknowledgement is responsible for processing each
       * Acknowledgement message received.
       * @param[in] thePacket containing theAcknowledgement message
       */
      void ProcessAcknowledgement(INetPacket* thePacket);

      /**
       * CreateConnect is responsible for providing a custom Connect message
       * that will be sent from the client to the server when the client first
       * establishes contact with the server to request a HostID.
       * @return pointer to INetPacket with Connect message, NULL otherwise
       */
      virtual INetPacket* CreateConnect(void);

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
       * method in the INetServer class and still have the INetClient base
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
       * GetIdentitySize is responsible for returning the size of the Identity
       * message. This way someone can modify the CreateIdentity method in the
       * INetServer class and still have the INetClient base class validate
       * each Identity message size.
       * @return the Identity message size
       */
      virtual std::size_t GetIdentitySize(void) const;

      /**
       * ProcessIdentity is responsible for processing each Identity message
       * received. By default it calls the SetHostID method with the new
       * HostID assigned to this client from the server.
       */
      void ProcessIdentity(INetPacket* thePacket);

      /**
       * CreateTimeSync1 is responsible for creating the Sync 1 message reply
       * to the server which is used to compute the round trip time between the
       * server and each client.
       * @param[in] theSourceTime from the server Sync 1 message
       * @return pointer to INetPacket with Heartbeat message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync1(Int64 theSourceTime);

      /**
       * GetTimeSync1Size is responsible for returning the size of the
       * Time Sync 1 message. This way someone can modify the CreateTimeSync1
       * method in the INetServer class and still have the INetClient base
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
       * CreateTimeSync2 is responsible for creating the Sync 2 message reply
       * to the server which is used to compute the round trip time between the
       * server and each client.
       * @param[in] theSourceTime1 from the server Sync 1 message
       * @param[in] theDestTime1 from the server Sync 1 message
       * @param[in] theSourceTime2 from the server Sync 2 message
       * @param[in] theDestTime2 client response for the Sync 2 message reply
       * @return pointer to INetPacket with Heartbeat message, NULL otherwise
       */
      virtual INetPacket* CreateTimeSync2(Int64 theSourceTime1, Int64 theDestTime, Int64 theSourceTime2, Int64 theDestTime2);

      /**
       * GetTimeSync2Size is responsible for returning the size of the
       * Time Sync 2 message. This way someone can modify the CreateTimeSync2
       * method in the INetServer class and still have the INetClient base
       * class validate each Time Sync 2 message size.
       * @return the Time Sync 2 message size
       */
      virtual std::size_t GetTimeSync2Size(void) const;

      /**
       * ProcessTimeSync2 is responsible for processing each Time Sync 2
       * message received. By default it calculates the delay between the
       * client and the server and computes the timestamp offset to use for
       * this client to provide a network wide timestamp that both sides can
       * use to organize game time sensitive events.
       * @param[in] thePacket containing theTimeSync2 client response message
       */
      void ProcessTimeSync2(INetPacket* thePacket);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Protocol flag for this NetServer
      NetProtocol mProtocol;
      /// Server address to connect this client to
#if (SFML_VERSION_MAJOR < 2)
      sf::IPAddress mServerAddress;
#else
      sf::IpAddress mServerAddress;
#endif
      /// Server port to connect this client to
      Uint16 mServerPort;
      /// Client port to use or 0 to randomly assign one (default is 0)
      Uint16 mClientPort;
#if (SFML_VERSION_MAJOR < 2)
      /// Socket for TCP protocol client
      sf::SocketTCP mClientTcp;
      /// Socket for UDP protocol client
      sf::SocketUDP mClientUdp;
      /// TCP Socket selector for monitoring when mClient has something to receive
      sf::SelectorTCP mSelectorTcp;
      /// UDP Socket selector for monitoring when mClient has something to receive
      sf::SelectorUDP mSelectorUdp;
#else
      /// Socket for TCP protocol client
      sf::TcpSocket mClientTcp;
      /// Socket for UDP protocol client
      sf::UdpSocket mClientUdp;
      /// Socket selector for monitoring when mClient has something to receive
      sf::SocketSelector mSelector;
#endif
      /// Boolean indicating client has made contact with server
      bool mContact;
      /// Boolean indicating client is connected
      bool mConnected;
      /// Connect timeout (seconds) for TCP clients
      float mConnectTimeout;
      /// Retry timeout between connection messages for UDP clients
      float mRetryTimeout;
      /// The HostID to use for this client
      Uint32 mHostID;
      /// The last sequence number processed from the server
      Uint32 mLastSN;
      /// The resend queue for resending each message
      std::queue<INetPacket*> mResend;
      /// Resend timeout (milliseconds) is used to determine when to resend messages that require acknowledgements
      Int32 mResendTimeout;
      /// Receive timeout (seconds) is used to determine how long to wait for incoming messages
      float mReceiveTimeout;
      /// Clock for the source of all time synchronization messages
      sf::Clock mTimeSync;
      /// Delay or ping time in microseconds between the client and the server
      Int64     mDelay;
      /// Clock offset in microseconds to use when computing calcuations
      Int64     mOffset;


      /**
       * Process is the process thread responsible for calling either
       * ProcessTcp or ProcessUdp depending on the protocol specified at
       * construction time. This method overrides the Process thread in the
       * IProcess base class.
       */
      virtual void Process(void);

      /**
       * ProcessTcp is the process thread responsible for providing the TCP
       * Client thread implementation.
       */
      void ProcessTcp(void);

      /**
       * ProcessUdp is the process thread responsible for providing the UDP
       * Client thread implementation.
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
       * ReceivePacket is the central method used to receive all NetPackets.
       * The reason for centralizing this is so that Acknowledgements can be
       * processed without cluttering up the Process thread.
       * @return pointer to INetPacket to process, NULL otherwise
       */
      INetPacket* ReceivePacket(void);

      /**
       * SetHostID is responsible for setting the HostID that will be used to
       * identify this client. This HostID is assigned by the server during the
       * connection process and will be used in all future messages to and from
       * the server.
       * @param[in] theHostID received by the server for this client
       */
      void SetHostID(const Uint32 theHostID);

      /**
       * Our copy constructor is private because we do not allow copies of
       * our INetClient derived classes
       */
      INetClient(const INetClient&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our INetClient derived classes
       */
      INetClient& operator=(const INetClient&); // Intentionally undefined
  }; // INetClient class
} // namespace GQE
#endif // INET_CLIENT_HPP_INCLUDED

/**
 * @class GQE::INetClient
 * @ingroup Core
 * The INetClient class is responsible for providing both TCP and UDP client
 * services for the NetManager class. The INetClient class implements only the
 * essential messages necessary to allow for connecting, disconnecting,
 * acknowledging messages, and time synchronization with an INetServer class.
 * It is expected that the programmer will inherit from the INetClient class
 * and provide their own message types specific to their application.
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
