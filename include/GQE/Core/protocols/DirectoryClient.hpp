/**
 * Provides the DirectoryClient class which implements the Directory protocol
 * used to register game servers with a directory so other players can
 * discover game servers available.
 *
 * @file include/GQE/Core/protocols/DirectoryClient.hpp
 * @author Ryan Lindeman
 * @date 20130112 - Initial Release
 */
#ifndef DIRECTORY_CLIENT_HPP_INCLUDED
#define DIRECTORY_CLIENT_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/INetClient.hpp>

namespace GQE
{
  /// Provides the DirectoryClient class for providing a Directory Client service
  class GQE_API DirectoryClient : public INetClient
  {
    public:
      /**
       * DirectoryClient default constructor
       * @param[in] theNetAlias (alias) to use for this client
       * @param[in] theClientVersion to use for this client
       * @param[in] theAppInfo to use for this client
       * @param[in] theNetPool derived class to use for getting INetPackets
       * @param[in] theProtocol to use for this Directory protocol
       * @param[in] theServerPort to connect to
       */
      DirectoryClient(const typeNetAlias theNetAlias,
                      const typeVersionInfo theClientVersion,
                      const typeAppInfo theAppInfo,
                      INetPool& theNetPool,
                      const NetProtocol theProtocol,
                      const Uint16 theServerPort = DIRECTORY_SERVER_PORT);

      /**
       * DirectoryClient deconstructor
       */
      virtual ~DirectoryClient();

      /**
       * SetAppInfo allows for changing the application information that this
       * client will register with the DirectoryServer. This can only be done
       * if the DirectoryClient hasn't been started yet.
       */
      void SetAppInfo(const typeAppInfo theAppInfo);

      /**
       * RegisterServer is responsible for reporting a server that this
       * DirectoryClient represents using the Directory protocol. All game
       * servers should use this method to register their server with the
       * DirectoryServer when they want to publish their server for others
       * to join and play including local servers.
       * @param[in] theServerInfo of the server to register
       */
      void RegisterServer(const typeServerInfo theServerInfo);

      /**
       * UnregisterServer is responsible for unregistering the server that this
       * DirectoryClient represents using the Directory protocol. All game
       * servers should use this method to unregister their server from the
       * DirectoryServer as part of shutting down their game servers. When in
       * local scope it will disconnect the internal DirectoryServer that is
       * running.
       */
      void UnregisterServer(const typeNetAlias theNetAlias);

      /**
       * RegisterSubscriber is responsible for registering this client to
       * receive server info messages for theAppID specified. This way a
       * client can receive regular server updates and pick which server to
       * connect to.
       */
      void RegisterSubscriber(void);

      /**
       * UnregisterSubscriber is responsible for unregistering this client
       * from theAppID specified. This will prevent future server info
       * messages from being sent to this DirectoryClient.
       */
      void UnregisterSubscriber(void);

      /**
       * GetSubscriberList will return a copy of the current list of servers
       * for theAppID specified. The caller can then use this list to pick
       * a server to connect to.
       * @returns a copy of theServerMap for theAppID specified
       */
      typeServerMap GetSubscriberList(void);

    protected:
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
       * immediate network packet message response.
       * @param[in] theIncoming INetPacket to be processed
       * @return pointer to outgoing INetPacket response, NULL otherwise
       */
      virtual INetPacket* ProcessIncoming(INetPacket* theIncoming);

      /**
       * CreateRegisterApp is responsible for creating the RegisterApp message
       * that will be sent to the DirectoryServer by this client. This is the
       * first message sent after a connection with the DirectoryServer has been
       * sent and is used to filter the list of servers returned by the
       * DirectoryServer to the client.
       * @param[in] theAppInfo to register with the DirectoryServer
       */
      virtual INetPacket* CreateRegisterApp(const typeAppInfo theAppInfo);

      /**
       * CreateRegisterServer is responsible for creating the RegisterServer
       * message that will be sent to the DirectoryServer by this client. This
       * will cause a server to be published to all clients of the
       * DirectoryServer so game clients can find servers to play on.
       * @param[in] theAppID of the registered application to register this server for
       * @param[in] theServerInfo of the server to register
       */
      virtual INetPacket* CreateRegisterServer(const typeAppID theAppID,
                                               const typeServerInfo theServerInfo);

      /**
       * CreateUnregisterServer is responsible for creating the
       * UnregisterServer message that will be sent to the DirectoryServer by
       * this client. This will cause a server to be removed from being
       * published to all clients of the DirectoryServer so game clients won't
       * be able to see the server. This can be done to make the server private
       * after a game has started or other reasons.
       * @param[in] theAppID of the registered application to unregister this server for
       * @param[in] theNetAlias of the server to unregister
       */
      virtual INetPacket* CreateUnregisterServer(const typeAppID theAppID,
                                                 const typeNetAlias theNetAlias);

      /**
       * CreateRegisterSubscriber is responsible for creating the
       * RegisterSubscriber message that will be sent to the DirectoryServer
       * by this client. This will cause this client to receive all
       * server info changes for theAppID provided.
       * @param[in] theAppID of the registered application to register for
       */
      virtual INetPacket* CreateRegisterSubscriber(const typeAppID theAppID);

      /**
       * CreateUnregisterSubscriber is responsible for creating the
       * UnregisterSubscriber message that will be sent to the DirectoryServer
       * by this client. This will cause this client subscriber to be removed
       * from theAppID specified so no further server info messages will be
       * received.
       * @param[in] theAppID of the registered application to unregister for
       */
      virtual INetPacket* CreateUnregisterSubscriber(const typeAppID theAppID);

      /**
       * GetServerInfoSize is responsible for returning the size of the
       * ServerInfo message. This way someone can modify the CreateServerInfo
       * method in DirectoryServer and still have the DirectoryClient base
       * class validate each ServerInfo message size correctly.
       * @return the ServerInfo message size
       */
      virtual std::size_t GetServerInfoSize(void) const;

      /**
       * ProcessServerInfo is responsible for processing each ServerInfo
       * message received.
       * @param[in] thePacket containing the ServerInfo message
       */
      void ProcessServerInfo(INetPacket* thePacket);

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The application information for this client to report under
      typeAppInfo mAppInfo;
      /// Map of servers for each subscribed application
      std::map<const typeAppID, typeServerMap> mSubscriptions;
      /// Mutex to protect our subscription map above
      sf::Mutex mSubscriptionMutex;

      /**
       * Our copy constructor is private because we do not allow copies of
       * our DirectoryClient derived classes
       */
      DirectoryClient(const DirectoryClient&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our DirectoryClient derived classes
       */
      DirectoryClient& operator=(const DirectoryClient&); // Intentionally undefined
  }; // DirectoryClient class
} // namespace GQE
#endif // DIRECTORY_CLIENT_HPP_INCLUDED

/**
 * @class GQE::DirectoryClient
 * @ingroup Core
 * The DirectoryClient class is responsible for providing the Directory
 * protocol which can be used to register a game server for others with the
 * Directory server which will allow others to discover game servers that are
 * available to connect to and use.
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
