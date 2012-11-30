/**
 * Provides the TcpServer class that can be derived to provide TCP server
 * services for a custom protocol. This class abstracts the details of
 * handling incoming TCP client connections and processing TCP responses.
 *
 * @file src/GQE/Core/classes/TcpServer.cpp
 * @author Ryan Lindeman
 * @date 20121124 - Initial Release
 */

#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <GQE/Core/classes/TcpServer.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <assert.h>

namespace GQE
{
  TcpServer::TcpServer(const Uint16 theServerPort) :
    IProcess(),
    mServerPort(theServerPort)
  {
  }

  TcpServer::~TcpServer()
  {
  }

  sf::Packet* TcpServer::GetPacket(void)
  {
    sf::Packet* anResult = new sf::Packet;
    assert(anResult);

    return anResult;
  }

  void TcpServer::ReturnPacket(sf::Packet* thePacket)
  {
    delete thePacket;
  }

  void TcpServer::SendPacket(sf::Packet* thePacket, Uint32 theHostID)
  {
    // Validate that the packet provided is correct
    if(thePacket)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::iterator anIterator;
      anIterator = mClients.find(theHostID);

      // Was theHostID found? then prepare to send the message to this client
      if(anIterator != mClients.end())
      {
        // Send thePacket to this client
        ClientInfo anClient = anIterator->second;
        anClient.socket->send(*thePacket);
      }
      else
      {
        ELOG() << "TcpServer::SendPacket(" << theHostID << ") host not found" << std::endl;
      }
    }
    else
    {
      ELOG() << "TcpServer::SendPacket(" << theHostID << ") null packet provided" << std::endl;
    }
  }

  void TcpServer::SendAllPacket(sf::Packet* thePacket)
  {
    // Validate that the packet provided is correct
    if(thePacket)
    {
      // Attempt to find theHostID provided in our client map
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Loop through each client and send thePacket provided
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        // Send thePacket to this client
        ClientInfo anClient = anIterator->second;
        anClient.socket->send(*thePacket);
      }
    }
    else
    {
      ELOG() << "TcpServer::SendAllPacket() null packet provided" << std::endl;
    }
  }

  void TcpServer::DisconnectClient(Uint32 theHostID)
  {
    // Attempt to find theHostID provided in our client map
    std::map<const Uint32, ClientInfo>::iterator anIterator;
    anIterator = mClients.find(theHostID);

    // Was theHostID found? then prepare to send the message to this client
    if(anIterator != mClients.end())
    {
      // Send thePacket to this client
      ClientInfo anClient = anIterator->second;

      // Attempt to retrieve an empty Transaction structure
      Transaction anTransaction;
      anTransaction.outgoing = GetPacket();

      // Allow the derived class a chance to create a Disconnect packet
      bool anResponse = CreateDisconnect(*anTransaction.outgoing);

      // Was a disconnect message provided by CreateDisconnect?
      if(anResponse && anTransaction.outgoing)
      {
        // Send the custom disconnect message
        anClient.socket->send(*anTransaction.outgoing);

        // Give the client time to receive the disconnect message before exiting
        sf::sleep(sf::milliseconds(100));
      }

      // Return the outgoing packet
      ReturnPacket(anTransaction.outgoing);
      anTransaction.outgoing = NULL;

      // Drop the client now
      DropClient(anIterator);
    }
    else
    {
      ELOG() << "TcpServer::DisconnectClient(" << theHostID << ") host not found" << std::endl;
    }
  }

  Uint32 TcpServer::GetHostID(void)
  {
    static Uint32 sHostID = 0;
    return ++sHostID;
  }

  TcpServer::ServerEvents TcpServer::ProcessTransaction(Transaction& theTransaction, Uint32* theHostID)
  {
    // Default to no response necessary
    ServerEvents anResult = ServerNothing;

    // Return the result decided above
    return anResult;
  }

  bool TcpServer::CreateDisconnect(sf::Packet& thePacket)
  {
    // Default to no disconnect message necessary
    bool anResult = false;

    // Return the result decided above
    return anResult;
  }

  void TcpServer::Process(void)
  {
    sf::TcpListener anListener;
    sf::SocketSelector anSelector;

    // Attempt to listen on the port specified
    if(!anListener.listen(mServerPort))
    {
      // Unable to listen on the port specified, perhaps it is already open.
      ELOG() << "TcpServer::Process error listening on port("
             << mServerPort << ")" << std::endl;

      // Thread is no longer running
      mRunning = false;
    }
    else
    {
      // Add Listener socket to socket selector
      anSelector.add(anListener);
    }

    // While process thread is running
    while(mRunning)
    {
      // Wait for a socket to be ready with a 0.5 second timeout so our thread can exit gracefully
      if(anSelector.wait(sf::seconds(0.5)))
      {
        // Check our Listener socket first
        if(anSelector.isReady(anListener))
        {
          ClientInfo anClient;
          anClient.socket = new sf::TcpSocket();

          // Handle accepting new client connection
          if (anListener.accept(*anClient.socket) != sf::Socket::Done)
          {
            ELOG() << "TcpServer::Process error accepting client" << std::endl;

            // Process thread needs to shutdown now
            mRunning = false;
          }

          // Get the HostID to use for this client
          Uint32 anHostID = GetHostID();

          // Insert the client into our map
          mClients.insert(std::pair<const Uint32, ClientInfo>(anHostID, anClient));

          // Add new client to our selector
          anSelector.add(*anClient.socket);
        }
        // Check all our client sockets next
        else
        {
          std::map<const Uint32, ClientInfo>::iterator anIterator;
          for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
          {
            ClientInfo anClient = anIterator->second;
            if(anSelector.isReady(*anClient.socket))
            {
              Uint32 anHostID = anIterator->first;

              // Retrieve a Transaction structure from our derived class
              Transaction anTransaction;
              anTransaction.incoming = GetPacket();

              // Retrieve the incoming data message from the client socket
              anClient.socket->receive(*anTransaction.incoming);

              // Process the incoming data into an outgoing message
              ServerEvents anEvent = ProcessTransaction(anTransaction, &anHostID);

              // Do we need to send a response (we have a valid pointer)
              if(anTransaction.outgoing)
              {
                // Send this client the immediate response to the incoming message
                anClient.socket->send(*anTransaction.outgoing);

                // Return the outgoing message packet
                ReturnPacket(anTransaction.outgoing);
                anTransaction.outgoing = NULL;
              }

              // Return the incoming message packet
              ReturnPacket(anTransaction.incoming);
              anTransaction.incoming = NULL;

              // Handle server event return result now
              switch(anEvent)
              {
                default:
                case ServerUnknown:
                case ServerNothing:
                case ServerNewClient: // Ignore new clients for TcpServer
                  break;
                case ServerForceDisconnect:
                  // Wait a little while for client to receive disconnect response above
                  sf::sleep(sf::milliseconds(100));

                  // Drop this client
                  DropClient(anIterator++);
                  break;
                case ServerShutdown:
                  mRunning = false;
                  break;
              }
            } // if(anSelector.isReady(anClient.socket))
          } // for each client in mClients
        } // ! anListener socket, must be a client socket
      } // if(anSelector.wait(0.5 seconds))
    } // while(mServerRunning)

    // Close our listener socket at this point
    anListener.close();

    // Server is shutting down, send disconnect event to each connected client
    if(!mClients.empty())
    {
      // Client list iterator
      std::map<const Uint32, ClientInfo>::iterator anIterator;

      // Loop through each client and send a disconnect message
      for(anIterator = mClients.begin(); anIterator != mClients.end(); anIterator++)
      {
        ClientInfo anClient = anIterator->second;

        // Attempt to retrieve an empty Transaction structure
        Transaction anTransaction;
        anTransaction.outgoing = GetPacket();

        // Allow the derived class a chance to create a Disconnect packet
        bool anResponse = CreateDisconnect(*anTransaction.outgoing);

        // Was a disconnect message provided by CreateDisconnect?
        if(anResponse && anTransaction.outgoing)
        {
          // Send the custom disconnect message
          anClient.socket->send(*anTransaction.outgoing);

          // Give the client time to receive the disconnect message before exiting
          sf::sleep(sf::milliseconds(100));
        }

        // Return the outgoing packet
        ReturnPacket(anTransaction.outgoing);
        anTransaction.outgoing = NULL;

        // Cause this client to be dropped from the map
        DropClient(anIterator++);
      }

      // Clear the list of clients (should be empty by this point)
      mClients.clear();
    }
  }

  void TcpServer::DropClient(std::map<const Uint32, ClientInfo>::iterator theClient)
  {
    ClientInfo anClient = theClient->second;

    // Close the TcpSocket and delete it as we go
    anClient.socket->disconnect();

    // Delete each TcpSocket as we go
    delete anClient.socket;
    anClient.socket = NULL;

    // Last of all, delete the ClientInfo mapped structure
    mClients.erase(theClient);
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
