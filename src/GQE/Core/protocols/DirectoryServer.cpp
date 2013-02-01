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
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  const float DirectoryServer::DIRECTORY_TIME_SYNC_TIMEOUT_S = 60.0f;

  DirectoryServer::DirectoryServer(const typeServerID theServerID,
                                   const typeVersionInfo theServerVersion,
                                   INetPool& theNetPool,
                                   const DirectoryScope theScope,
                                   const Uint16 theServerPort) :
    INetServer(theServerID, theServerVersion, theNetPool,
               theScope == ScopeLocal ? NetUdp : NetTcp,
               theServerPort,
               MAX_DIRECTORY_CLIENTS,
               DIRECTORY_TIME_SYNC_TIMEOUT_S),
    mScope(theScope)
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
        if(theServerInfo.id == anServerIter->id)
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
        // Add the new server to the list of registered servers for this application
        anIter->second.servers.push_back(theServerInfo);
      }
      else
      {
        // Log a duplicate server found error
        ELOG() << "DirectoryServer::RegisterServer(" << theAppID
               << ") already has server(" << theServerInfo.id << ")"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::RegisterServer(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  void DirectoryServer::UnregisterServer(const typeAppID theAppID, const typeServerID theServerID)
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
        if(theServerID == anServerIter->id)
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
        // Use the iterator from above to remove the server now
        anIter->second.servers.erase(anServerIter);
      }
      else
      {
        // Log a warning if the server wasn't found
        WLOG() << "DirectoryServer::UnregisterServer(" << theAppID
               << ") server(" << theServerID << ") not found"
               << std::endl;
      }
    }
    else
    {
      ELOG() << "DirectoryServer::UnregisterServer(" << theAppID
             << ") application doesn't exist" << std::endl;
    }
  }

  bool DirectoryServer::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = false;

    // Get the message type now and verify the size
    Uint16 anSourceType = thePacket.GetType();

    // Validate the known message types according to size
    switch(anSourceType)
    {
      case INetPacket::NET_REGISTER_APP:
        if(GetRegisterAppSize() <= theSize)
        {
          // Message type is correct as far as we can tell
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
          // Message type is correct as far as we can tell
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
          // Message type is correct as far as we can tell
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

    // Retrieve the source message type
    Uint16 anSourceType = thePacket->GetType();

    // Switch on the known message types
    switch(anSourceType)
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
    // Header + app ID + address + port + max and active clients + version + title size and string
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32)*3 + sizeof(Uint16)*3 + sizeof(Uint8)*4;
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
    *thePacket >> anServerInfo.id;

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
    typeServerID anServerID;

    // Retrieve the application ID from the RegisterServer message
    *thePacket >> anAppID;

    // Retrieve the server ID (title)
    *thePacket >> anServerID;

    // Now attempt to unregister this server by server ID (title)
    UnregisterServer(anAppID, anServerID);
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
