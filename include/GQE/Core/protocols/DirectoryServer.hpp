/**
 * Provides the DirectoryServer class which implements the Directory protocol
 * used to register game servers with a directory so other players can
 * discover game servers available.
 *
 * @file include/GQE/Core/protocols/DirectoryServer.hpp
 * @author Ryan Lindeman
 * @date 20130112 - Initial Release
 */
#ifndef DIRECTORY_SERVER_HPP_INCLUDED
#define DIRECTORY_SERVER_HPP_INCLUDED

#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/interfaces/INetServer.hpp>

namespace GQE
{
  /// Provides the INetServer class for providing a network server services
  class GQE_API DirectoryServer: public INetServer
  {
    public:
      // Constants
      ////////////////////////////////////////////////////////////////////////
      /// The maximum number of Directory Clients for this server
      static const Uint16 MAX_DIRECTORY_CLIENTS = 1024;
      /// The number of seconds between time sync messages to each DirectoryClient
      static const float DIRECTORY_TIME_SYNC_TIMEOUT_S;

      /**
       * DirectoryServer default constructor
       * @param[in] theNetAlias to use for this Directory Server
       * @param[in] theVersionInfo to use for this Directory Server
       * @param[in] theNetPool derived class to use for getting INetPackets
       * @param[in] theScope to use for this Directory protocol
       * @param[in] theServerPort to listen on for incoming UDP clients
       */
      DirectoryServer(const typeNetAlias theNetAlias,
                      const VersionInfo theVersionInfo,
                      INetPool& theNetPool,
                      const NetProtocol theProtocol,
                      const Uint16 theServerPort = DIRECTORY_SERVER_PORT);

      /**
       * DirectoryServer deconstructor
       */
      virtual ~DirectoryServer();

      /**
       * RegisterApp is responsible for registering theAppInfo provided with
       * this DirectoryServer. Registering theAppInfo must come before you can
       * call the RegisterServer method to register the server information.
       * @param[in] theAppInfo to register with this DirectoryServer.
       */
      void RegisterApp(const typeAppInfo theAppInfo);

      /**
       * RegisterServer is responsible for registering theServerInfo provided
       * under theAppID specified. This will make the server public to all
       * clients that connect to this DirectoryServer.
       * @param[in] theAppID to register theServerInfo under
       * @param[in] theServerInfo of the server to register for this DirectoryServer
       */
      void RegisterServer(const typeAppID theAppID, const typeServerInfo theServerInfo);

      /**
       * UnregisterServer is responsible for removing the registered server ID
       * sepecified from theAppID indicated. Its possible the server will
       * still be running but no future clients will see the registered server
       * after it has been unregistered.
       * @param[in] theAppID to register theServerInfo under
       * @param[in] theNetAlias of the server to unregister
       */
      void UnregisterServer(const typeAppID theAppID, const typeNetAlias theNetAlias);

      /**
       * RegisterSubscriber is responsible for registering theNetID client
       * subscriber provided under theAppID specified. This will make it so
       * the server information changes registered under theAppID specified is
       * broadcast to this client.
       * @param[in] theAppID to register theNetID under
       * @param[in] theNetID of the client subscriber to register
       */
      void RegisterSubscriber(const typeAppID theAppID, const typeNetID theNetID);

      /**
       * UnregisterSubscriber is responsible for removing theNetID of the
       * client subscriber from theAppID indicated.
       * @param[in] theAppID to register theServerInfo under
       * @param[in] theNetID of the client subscriber to unregister
       */
      void UnregisterSubscriber(const typeAppID theAppID, const typeNetID theNetID);

    protected:
      // Variables
      ////////////////////////////////////////////////////////////////////////

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
       * GetRegisterAppSize is responsible for returning the size of the
       * RegisterApp message. This way someone can modify the
       * CreateRegisterApp method in DirectoryClient and still have the
       * DirectoryServer base class validate each RegisterApp message size
       * correctly.
       * @return the RegisterApp message size
       */
      virtual std::size_t GetRegisterAppSize(void) const;

      /**
       * ProcessRegisterApp is responsible for processing each RegisterApp
       * message received.
       * @param[in] thePacket containing the RegisterApp message
       */
      void ProcessRegisterApp(INetPacket* thePacket);

      /**
       * GetRegisterServerSize is responsible for returning the size of the
       * RegisterServer message. This way someone can modify the
       * CreateRegisterServer method in DirectoryClient and still have the
       * DirectoryServer base class validate each RegisterServer message size
       * correctly.
       * @return the RegisterServer message size
       */
      virtual std::size_t GetRegisterServerSize(void) const;

      /**
       * ProcessRegisterServer is responsible for processing each
       * RegisterServer message received.
       * @param[in] thePacket containing the RegisterServer message
       */
      void ProcessRegisterServer(INetPacket* thePacket);

      /**
       * GetUnregisterServerSize is responsible for returning the size of the
       * UnregisterServer message. This way someone can modify the
       * CreateUnregisterServer method in DirectoryClient and still have the
       * DirectoryServer base class validate each RegisterServer message size
       * correctly.
       * @return the UnregisterServer message size
       */
      virtual std::size_t GetUnregisterServerSize(void) const;

      /**
       * ProcessUnregisterServer is responsible for processing each
       * UnregisterServer message received.
       * @param[in] thePacket containing the UnregisterServer message
       */
      void ProcessUnregisterServer(INetPacket* thePacket);

      /**
       * GetRegisterSubscriberSize is responsible for returning the size of
       * the RegisterSubscriber message. This way someone can modify the
       * CreateRegisterSubscriber method in DirectoryClient and still have the
       * DirectoryServer base class validate each RegisterSubscriber message
       * size correctly.
       * @return the RegisterSubscriber message size
       */
      virtual std::size_t GetRegisterSubscriberSize(void) const;

      /**
       * ProcessRegisterSubscriber is responsible for processing each
       * RegisterSubscriber message received.
       * @param[in] thePacket containing the RegisterSubscriber message
       */
      void ProcessRegisterSubscriber(INetPacket* thePacket);

      /**
       * GetUnregisterSubscriberSize is responsible for returning the size of
       * the UnregisterSubscriber message. This way someone can modify the
       * CreateUnregisterSubscriber method in DirectoryClient and still have
       * the DirectoryServer base class validate each RegisterSubscriber
       * message size correctly.
       * @return the UnregisterSubscriber message size
       */
      virtual std::size_t GetUnregisterSubscriberSize(void) const;

      /**
       * ProcessUnregisterSubscriber is responsible for processing each
       * UnregisterSubscriber message received.
       * @param[in] thePacket containing the UnregisterSubscriber message
       */
      void ProcessUnregisterSubscriber(INetPacket* thePacket);

      /**
       * CreateServerInfo is responsible for providing a Server Info message
       * that will be sent to a subscriber with theServerInfo data provided.
       * @param[in] theAppID of the application theServerInfo is under
       * @param[in] theServerInfo of the server being published
       * @param[in] theDeleteFlag to indicate server is now unregistered
       * @return pointer to INetPacket with Server Info message, NULL otherwise
       */
      virtual INetPacket* CreateServerInfo(const typeAppID theAppID,
                                           const typeServerInfo theServerInfo,
                                           bool theDeleteFlag = false);

    private:
      // Structures
      ////////////////////////////////////////////////////////////////////////
      /// DirectoryInfo structure holds the data needed for each registered application
      struct DirectoryInfo
      {
        typeAppInfo app;  ///< Application information
        std::list<typeServerInfo> servers;     ///< List of servers available for this app
        std::list<typeNetID>      subscribers; ///< List of subscribers for this app
      };

      // Variables
      ////////////////////////////////////////////////////////////////////////
      /// Map of each registered application with this DirectoryServer
      std::map<const typeAppID, DirectoryInfo> mDirectory;
      /// Mutex to protect our directory map above
      sf::Mutex mDirectoryMutex;


      /**
       * Our copy constructor is private because we do not allow copies of
       * our DirectoryServer derived classes
       */
      DirectoryServer(const DirectoryServer&);  // Intentionally undefined

      /**
       * Our assignment operator is private because we do not allow copies
       * of our DirectoryServer derived classes
       */
      DirectoryServer& operator=(const DirectoryServer&); // Intentionally undefined
  }; // DirectoryServer class
} // namespace GQE
#endif // DIRECTORY_SERVER_HPP_INCLUDED

/**
 * @class GQE::DirectoryServer
 * @ingroup Core
 * The DirectoryServer class is responsible for providing the Directory
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
