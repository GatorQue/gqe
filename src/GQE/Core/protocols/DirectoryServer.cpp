/**
 * Provides the DirectoryServer class which implements the Directory protocol
 * used to register game servers with a directory so other players can
 * discover game servers available.
 *
 * @file src/GQE/Core/protocols/DirectoryServer.cpp
 * @author Ryan Lindeman
 * @date 20130112 - Initial Release
 */
#include <GQE/Core/protocols/DirectoryServer.hpp>
#include <GQE/Core/interfaces/INetPacket.hpp>
#include <GQE/Core/interfaces/INetPool.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  const float DirectoryServer::DIRECTORY_TIME_SYNC_TIMEOUT_S = 60.0f;

  DirectoryServer::DirectoryServer(const typeNetAlias theNetAlias,
                                   const typeVersionInfo theVersionInfo,
                                   INetPool& theNetPool,
                                   const NetProtocol theProtocol,
                                   const Uint16 theServerPort) :
    INetServer(theNetAlias, theVersionInfo, theNetPool, theProtocol,
               theServerPort, MAX_DIRECTORY_CLIENTS,
               DIRECTORY_TIME_SYNC_TIMEOUT_S)
  {
  }

  DirectoryServer::~DirectoryServer()
  {
  }

  void DirectoryServer::RegisterApp(const typeAppInfo theAppInfo)
  {
    // Obtain a lock on our directory mutex before registering a new application
    sf::Lock anLock(mDirectoryMutex);

    // See if the application already exists
    std::map<const typeAppID, DirectoryInfo>::iterator anIter;
    anIter = mDirectory.find(theAppInfo.id);

    // Does the application not exist? then add it now otherwise just return
    if(anIter == mDirectory.end())
    {
      DirectoryInfo anDirectoryInfo;

      // Build the DirectoryInfo structure
      anDirectoryInfo.app = theAppInfo;

      // Add the new DirectoryInfo structure to our directory
      mDirectory.insert(std::pair<const typeAppID, DirectoryInfo>(theAppInfo.id, anDirectoryInfo));
    }
  }

  void DirectoryServer::RegisterServer(const typeAppID theAppID, const typeServerInfo theServerInfo)
  {
    // Obtain a lock on our directory mutex before registering a new server
    sf::Lock anLock(mDirectoryMutex);

    // Iterator pointing to the registered application
    std::map<const typeAppID, DirectoryInfo>::iterator anIter;
    anIter = mDirectory.find(theAppID);

    // If the application exists, add the server now otherwise register an error
    if(anIter != mDirectory.end())
    {
      // Boolean flag indicating a duplicate server was found
      bool anFound = false;

      // Iterate through each server and see if we already have this one
      std::list<typeServerInfo>::iterator anServerIter;

      // Loop through each server to look for duplicates before adding new one
      for(anServerIter = anIter->second.servers.begin();
          anServerIter != anIter->second.servers.end();
          anServerIter++)
      {
        // Is this the same server? then indicate we found an identical server
        if(theServerInfo.alias == anServerIter->alias)
        {
          // We found a duplicate server
          anFound = true;

          // Exit for loop, duplicate already found
          break;
        }
      }

      // Did we not find a duplicate? then add the server now
      if(false == anFound)
      {
        // Iterator for each subscriber registered
        std::list<typeNetID>::iterator anSubscriberIter;

        // Loop through each subscriber to send new server info
        for(anSubscriberIter = anIter->second.subscribers.begin();
            anSubscriberIter != anIter->second.subscribers.end();
            anSubscriberIter++)
        {
          // Send new server info to this subscriber now
          SendPacket(CreateServerInfo(theAppID, theServerInfo),
                     *anSubscriberIter);
        }

        // Add the new server to the list of registered servers for this application
        anIter->second.servers.push_back(theServerInfo);
      }
      else
      {
        // Log a duplicate server found error
        ELOG() << "DirectoryServer::RegisterServer(" << theAppID
               << ") already has server(" << theServerInfo.alias << ")"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::RegisterServer(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  void DirectoryServer::UnregisterServer(const typeAppID theAppID, const typeNetAlias theNetAlias)
  {
    // Obtain a lock on our directory mutex before unregistering the server
    sf::Lock anLock(mDirectoryMutex);

    // Iterator pointing to the registered application
    std::map<const typeAppID, DirectoryInfo>::iterator anIter;
    anIter = mDirectory.find(theAppID);

    // If the application exists, add the server now otherwise register an error
    if(anIter != mDirectory.end())
    {
      // Boolean flag indicating a duplicate server was found
      bool anFound = false;

      // Iterate through each server and see if we already have this one
      std::list<typeServerInfo>::iterator anServerIter;

      // Loop through each server to look for duplicates before adding new one
      for(anServerIter = anIter->second.servers.begin();
          anServerIter != anIter->second.servers.end();
          anServerIter++)
      {
        // Is this the same server? then indicate we found an identical server
        if(theNetAlias == anServerIter->alias)
        {
          // We found the server to unregister
          anFound = true;

          // Exit for loop, server was found
          break;
        }
      }

      // Did we find the server to unregister? then unregister it now
      if(true == anFound)
      {
        // Iterator for each subscriber registered
        std::list<typeNetID>::iterator anSubscriberIter;

        // Loop through each subscriber to send a delete server info message
        for(anSubscriberIter = anIter->second.subscribers.begin();
            anSubscriberIter != anIter->second.subscribers.end();
            anSubscriberIter++)
        {
          // Send new server info to this subscriber now
          SendPacket(CreateServerInfo(theAppID, *anServerIter, true),
                     *anSubscriberIter);
        }

        // Use the iterator from above to remove the server now
        anIter->second.servers.erase(anServerIter);
      }
      else
      {
        // Log a warning if the server wasn't found
        WLOG() << "DirectoryServer::UnregisterServer(" << theAppID
               << ") server(" << theNetAlias << ") not found"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::UnregisterServer(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  void DirectoryServer::RegisterSubscriber(const typeAppID theAppID, const typeNetID theNetID)
  {
    // Obtain a lock on our directory mutex before registering a new subscriber
    sf::Lock anLock(mDirectoryMutex);

    // Iterator pointing to the registered application
    std::map<const typeAppID, DirectoryInfo>::iterator anIter;
    anIter = mDirectory.find(theAppID);

    // If the application exists, add the server now otherwise register an error
    if(anIter != mDirectory.end())
    {
      // Boolean flag indicating a duplicate subscriber was found
      bool anFound = false;

      // Iterate through each subscriber and see if we already have this one
      std::list<typeNetID>::iterator anSubscriberIter;

      // Loop through each subscriber to look for duplicates before adding new one
      for(anSubscriberIter = anIter->second.subscribers.begin();
          anSubscriberIter != anIter->second.subscribers.end();
          anSubscriberIter++)
      {
        // Is this the same server? then indicate we found an identical server
        if(theNetID == *anSubscriberIter)
        {
          // We found a duplicate server
          anFound = true;

          // Exit for loop, duplicate already found
          break;
        }
      }

      // Did we not find a duplicate? then add the subscriber now
      if(false == anFound)
      {
        // Iterator for each server registered
        std::list<typeServerInfo>::iterator anServerIter;

        // Loop through each server to look for duplicates before adding new one
        for(anServerIter = anIter->second.servers.begin();
            anServerIter != anIter->second.servers.end();
            anServerIter++)
        {
          // Send existing server info to this subscriber now
          SendPacket(CreateServerInfo(theAppID, *anServerIter), theNetID);
        }

        // Add the new subscriber to this application
        anIter->second.subscribers.push_back(theNetID);
      }
      else
      {
        // Log a duplicate server found error
        ELOG() << "DirectoryServer::RegisterSubscriber(" << theAppID
               << ") already has subscriber(" << theNetID << ")"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::RegisterSubscriber(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  void DirectoryServer::UnregisterSubscriber(const typeAppID theAppID, const typeNetID theNetID)
  {
    // Obtain a lock on our directory mutex before unregistering the server
    sf::Lock anLock(mDirectoryMutex);

    // Iterator pointing to the registered application
    std::map<const typeAppID, DirectoryInfo>::iterator anIter;
    anIter = mDirectory.find(theAppID);

    // If the application exists, add the subscriber now otherwise register an error
    if(anIter != mDirectory.end())
    {
      // Boolean flag indicating a duplicate server was found
      bool anFound = false;

      // Iterate through each server and see if we already have this one
      std::list<typeNetID>::iterator anSubscriberIter;

      // Loop through each subscriber to look for duplicates before adding new one
      for(anSubscriberIter = anIter->second.subscribers.begin();
          anSubscriberIter != anIter->second.subscribers.end();
          anSubscriberIter++)
      {
        // Is this the same subscriber? then indicate we found an identical subscriber
        if(theNetID == *anSubscriberIter)
        {
          // We found the subscriber to unregister
          anFound = true;

          // Exit for loop, subscriber was found
          break;
        }
      }

      // Did we find the server to unregister? then unregister it now
      if(true == anFound)
      {
        // Use the iterator from above to remove the server now
        anIter->second.subscribers.erase(anSubscriberIter);
      }
      else
      {
        // Log a warning if the server wasn't found
        WLOG() << "DirectoryServer::UnregisterSubscriber(" << theAppID
               << ") subscriber(" << theNetID << ") not found"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::UnregisterSubscriber(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  bool DirectoryServer::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = false;

    // Get the message label now and verify the size
    typeNetLabel anSourceNetLabel = thePacket.GetNetLabel();

    // Validate the known message types according to size
    switch(anSourceNetLabel)
    {
      case INetPacket::NET_REGISTER_APP:
        if(GetRegisterAppSize() <= theSize)
        {
          // Message label is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "DirectoryServer::VerifyIncoming() invalid RegisterApp message size("
                 << (Uint32)GetRegisterAppSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_REGISTER_SERVER:
        if(GetRegisterServerSize() <= theSize)
        {
          // Message label is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "DirectoryServer::VerifyIncoming() invalid RegisterServer message size("
                 << (Uint32)GetRegisterServerSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      case INetPacket::NET_UNREGISTER_SERVER:
        if(GetUnregisterServerSize() <= theSize)
        {
          // Message label is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "DirectoryServer::VerifyIncoming() invalid UnregisterServer message size("
                 << (Uint32)GetUnregisterServerSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      default: // Unknown message types are accepted with a DisconnectClient response
        anResult = true;
        break;
    }

    // Return the result determined above
    return anResult;
  }

  INetPacket* DirectoryServer::ProcessIncoming(INetPacket* thePacket)
  {
    // Default to no response necessary
    INetPacket* anResult = NULL;

    // Retrieve the source message label
    typeNetLabel anSourceNetLabel = thePacket->GetNetLabel();

    // Switch on the known message types
    switch(anSourceNetLabel)
    {
      case INetPacket::NET_REGISTER_APP:
        // Call ProcessRegisterApp to process this message
        ProcessRegisterApp(thePacket);
        break;
      case INetPacket::NET_REGISTER_SERVER:
        // Call ProcessRegisterServer to process this message
        ProcessRegisterServer(thePacket);
        break;
      case INetPacket::NET_UNREGISTER_SERVER:
        // Call ProcessUnregisterServer to process this message
        ProcessUnregisterServer(thePacket);
        break;
      case INetPacket::NET_REGISTER_SUBSCRIBER:
        // Call ProcessRegisterServer to process this message
        ProcessRegisterSubscriber(thePacket);
        break;
      case INetPacket::NET_UNREGISTER_SUBSCRIBER:
        // Call ProcessUnregisterServer to process this message
        ProcessUnregisterSubscriber(thePacket);
        break;
      default: // Unknown message types receive a Disconnect message as a response
        // Create a Disconnect client response message
        anResult = CreateDisconnect();
        break;
    }

    // Return the response result decided above
    return anResult;
  }

  std::size_t DirectoryServer::GetRegisterAppSize(void) const
  {
    // Header + app ID + title size and string + description size and string + website size and string
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32)*4 + sizeof(Uint8);
  }

  void DirectoryServer::ProcessRegisterApp(INetPacket* thePacket)
  {
    // The application info structure to build
    typeAppInfo anAppInfo;

    // Retrieve the application ID from the RegisterApp message
    *thePacket >> anAppInfo.id;

    // Retrieve the title, description, and website
    *thePacket >> anAppInfo.title;
    *thePacket >> anAppInfo.description;
    *thePacket >> anAppInfo.website;

    // Now attempt to register this application
    RegisterApp(anAppInfo);
  }

  std::size_t DirectoryServer::GetRegisterServerSize(void) const
  {
    // Header + app ID + address + port + max and active clients + version + alias size and string
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32)*5 + sizeof(Uint16) + sizeof(Uint8)*4;
  }

  void DirectoryServer::ProcessRegisterServer(INetPacket* thePacket)
  {
    // The application ID to register this server under
    typeAppID anAppID;

    // The server info structure to build
    typeServerInfo anServerInfo;

    // The server address information as an integer
    Uint32 anServerAddress;

    // Retrieve the application ID from the RegisterServer message
    *thePacket >> anAppID;

    // Retrieve the address, port, client info, version and id
    *thePacket >> anServerAddress;
    *thePacket >> anServerInfo.port;
    *thePacket >> anServerInfo.maxClients;
    *thePacket >> anServerInfo.activeClients;
    *thePacket >> anServerInfo.version.major;
    *thePacket >> anServerInfo.version.minor;
    *thePacket >> anServerInfo.version.patch;
    *thePacket >> anServerInfo.alias;

    // Now convert the integer address into an sf::IPAddress
    anServerInfo.address = anServerAddress;

    // Now attempt to register this server
    RegisterServer(anAppID, anServerInfo);
  }

  std::size_t DirectoryServer::GetUnregisterServerSize(void) const
  {
    // Header + app ID + title size and string
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32)*2 + sizeof(Uint8)*1;
  }

  void DirectoryServer::ProcessUnregisterServer(INetPacket* thePacket)
  {
    // The application ID to register this server under
    typeAppID anAppID;

    // The server ID (title) to match against
    typeNetAlias anNetAlias;

    // Retrieve the application ID from the RegisterServer message
    *thePacket >> anAppID;

    // Retrieve the server ID (title)
    *thePacket >> anNetAlias;

    // Now attempt to unregister this server by server ID (title)
    UnregisterServer(anAppID, anNetAlias);
  }

  std::size_t DirectoryServer::GetRegisterSubscriberSize(void) const
  {
    // Header + app ID
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32);
  }

  void DirectoryServer::ProcessRegisterSubscriber(INetPacket* thePacket)
  {
    // Retrieve the NetID from thePacket
    typeNetID anNetID = thePacket->GetNetID();

    // The application ID to register this server under
    typeAppID anAppID;

    // Retrieve the application ID from the RegisterServer message
    *thePacket >> anAppID;

    // Now attempt to register this subscriber with AppID and NetID
    RegisterSubscriber(anAppID, anNetID);
  }

  std::size_t DirectoryServer::GetUnregisterSubscriberSize(void) const
  {
    // Header + app ID
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32);
  }

  void DirectoryServer::ProcessUnregisterSubscriber(INetPacket* thePacket)
  {
    // Retrieve the NetID from thePacket
    typeNetID anNetID = thePacket->GetNetID();

    // The application ID to register this server under
    typeAppID anAppID;

    // Retrieve the application ID from the UnregisterSubscriber message
    *thePacket >> anAppID;

    // Now attempt to unregister this subscriber using the AppID and NetID
    UnregisterSubscriber(anAppID, anNetID);
  }

  INetPacket* DirectoryServer::CreateServerInfo(const typeAppID theAppID,
                                                const typeServerInfo theServerInfo,
                                                bool theDeleteFlag)
  {
    // Get a packet for our ServerInfo message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message label and Yes/No flag value
      anResult->SetNetLabel(INetPacket::NET_SERVER_INFO);

      // Ack required for UDP protocol messages
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      // Set the Yes/No flag as the Delete flag
      anResult->SetFlag(INetPacket::FlagYesResponse, theDeleteFlag);

      // Set server NetID as 1
      anResult->SetNetID(1);

      // Add the application ID provided
      *anResult << theAppID;

      // Add the server contact information
#if (SFML_VERSION_MAJOR < 2)
      *anResult << theServerInfo.address.ToInteger();
#else
      *anResult << theServerInfo.address.toInteger();
#endif
      *anResult << theServerInfo.port;

      // Add the server client information
      *anResult << theServerInfo.maxClients;
      *anResult << theServerInfo.activeClients;

      // Add the server version information next
      *anResult << theServerInfo.version.major;
      *anResult << theServerInfo.version.minor;
      *anResult << theServerInfo.version.patch;

      // Add the server alias next
      *anResult << theServerInfo.alias;
    }

    // Return the message result created above
    return anResult;
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
