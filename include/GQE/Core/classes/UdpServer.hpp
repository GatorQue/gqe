/**
 * Provides the UdpServer class that can be derived to provide UDP server
 * services for a custom protocol. This class abstracts the details of
 * handling incoming UDP client connections and processing UDP responses.
 *
 * @file include/GQE/Core/classes/UdpServer.hpp
 * @author Ryan Lindeman
 * @date 20121124 - Initial Release
 */
#ifndef UDP_SERVER_HPP_INCLUDED
#define UDP_SERVER_HPP_INCLUDED

#include <SFML/Network.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/IProcess.hpp>
#include <map>

namespace GQE
{
  /// Provides the UdpServer class for providing a UDP Server service
  class GQE_API UdpServer : public IProcess
  {
    public:
      /**
       * UdpServer default constructor
       * @param[in] theServerPort to listen on for incoming UDP clients
       */
      UdpServer(const Uint16 theServerPort);

      /**
       * UdpServer deconstructor
       */
      virtual ~UdpServer();

      /**
       * GetPacket is responsible for returning a sf::Packet derived class
       * to be used for either an Incoming message. ProcessTransaction will
       * then be called to process the incoming message received and provide an
       * optional immediate response to be sent for the incoming message.
       * @return a pointer to the packet to use for incoming messages
       */
      virtual sf::Packet* GetPacket(void);

      /**
       * ReturnPacket will be called for both the incoming and outgoing
       * packets provided by GetPacket and ProcessTransaction after it has
       * completed the task of sending the immediate response.
       * @param[in] thePacket to be returned
       */
      virtual void ReturnPacket(sf::Packet* thePacket);

      /**
       * SendPacket is responsible for sending the sf::Packet provided to the
       * client mapped to theHostID provided.
       * @param[in] thePacket to be sent
       * @param[in] theHostID to send thePacket to
       */
      void SendPacket(sf::Packet* thePacket, Uint32 theHostID);

      /**
       * SendAllPacket is responsible for sending the sf::Packet provided to
       * every registered client.
       * @param[in] thePacket to be sent to all clients
       */
      void SendAllPacket(sf::Packet* thePacket);

      /**
       * DisconnectClient is responsible for disconnecting client identified by
       * theHostID provided.
       * @param[in] theHostID to be disconnected
       */
      void DisconnectClient(Uint32 theHostID);

    protected:
      // Enumerations
      ///////////////////////////////////////////////////////////////////////////
      enum ServerEvents
      {
        ServerUnknown         = 0, ///< Unknown or unexpected Server event
        ServerNothing         = 1, ///< Default value returned by ProcessTransaction
        ServerShutdown        = 2, ///< Server needs to be shutdown right now
        ServerForceDisconnect = 3, ///< Force current client to be disconnected
        ServerNewClient       = 4  ///< Register a new client
      };

      // Structures
      ///////////////////////////////////////////////////////////////////////////
      /// Transaction structure holds optionally two sf::Packet instances, one for
      /// the incoming packet to be processed and one for the outgoing response
      struct Transaction
      {
        sf::Packet* incoming;
        sf::Packet* outgoing;
        Transaction() :
          incoming(NULL),
          outgoing(NULL)
        {
        }
      };
      /// ClientInfo structure holds the data needed for each client
      struct ClientInfo
      {
        sf::IpAddress address;
        Uint16        rport;
        ClientInfo() :
          address(),
          rport(0)
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
       * @return theHostID to use for the current client
       */
      virtual GQE::Uint32 GetHostID(void);

      /**
       * ProcessTransaction is responsible for processing all incoming network
       * packet messages from each UDP client and providing an optional
       * immediate network packet message response (theTransaction.outgoing is
       * no longer null).
       * @param[in] theTransaction to process
       * @param[out] theHostID to be used for some server events like drop client
       * @return Server Event or ServerNothing if no special event needs to be done
       */
      virtual ServerEvents ProcessTransaction(Transaction& theTransaction, Uint32* theHostID);

      /**
       * CreateDisconnect is responsible for providing a custom Disconnect
       * message that will be sent to client when the UdpServer is about to
       * shutdown. The caller must return true and provide
       * theTransaction.outgoing sf::Packet.
       * @param[in] thePacket to put the outgoing Disconnect message into
       * @return true if thePacket contains a message, false otherwise
       */
      virtual bool CreateDisconnect(sf::Packet& thePacket);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Server port to listen for incoming UDP clients
      Uint16 mServerPort;
      /// Listener socket for incoming UDP clients and sending packets
      sf::UdpSocket mListener;
      /// Map of UDP clients using the Host ID returned
      std::map<const Uint32, ClientInfo> mClients;

      /**
       * Process is the process thread responsible for providing the UDP/IPv4
       * Server thread implementation and is called by the derived class
       */
      virtual void Process(void);

      /**
       * DropClient is responsible for removing theClient specified from the
       * mClients map.
       * @param[in] theClient to be dropped
       */
      void DropClient(std::map<const Uint32, ClientInfo>::iterator theClient);
  }; // UdpServer class
} // namespace GQE
#endif // UDP_SERVER_HPP_INCLUDED

/**
 * @class GQE::UdpServer
 * @ingroup Core
 * The UdpServer class is responsible for providing UDP server services for the
 * NetManager class. By itself, the UdpServer class is not sufficient to do
 * anything very useful except provide an echo server. It is expected that the
 * derived class will implement a procotol specific to their particular
 * application and register their derived class with the NetManager class.
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
