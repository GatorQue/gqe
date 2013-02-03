/**
 * Provides the DirectoryClient class which implements the Directory protocol
 * used to register game servers with a directory so other players can
 * discover game servers available.
 *
 * @file src/GQE/Core/protocols/DirectoryClient.cpp
 * @author Ryan Lindeman
 * @date 20130112 - Initial Release
 */
#include <GQE/Core/protocols/DirectoryClient.hpp>
#include <GQE/Core/protocols/DirectoryServer.hpp>
#include <GQE/Core/interfaces/INetPool.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>
#include <assert.h>

namespace GQE
{
  DirectoryClient::DirectoryClient(const typeNetAlias theNetAlias,
                                   const VersionInfo theVersionInfo,
                                   const typeAppInfo theAppInfo,
                                   INetPool& theNetPool,
                                   const NetProtocol theProtocol,
                                   const Uint16 theServerPort) :
    INetClient(theNetAlias, theVersionInfo, theNetPool, theProtocol, theServerPort),
    mAppInfo(theAppInfo)
  {
  }

  DirectoryClient::~DirectoryClient()
  {
  }

  void DirectoryClient::SetAppInfo(const typeAppInfo theAppInfo)
  {
    if(false == IsRunning())
    {
      mAppInfo = theAppInfo;
    }
    else
    {
      WLOG() << "DirectoryClient::SetAppInfo() can't change app info when client is running" << std::endl;
    }
  }

  void DirectoryClient::RegisterServer(const typeServerInfo theServerInfo)
  {
    if(IsConnected())
    {
      // Make sure our application is registered with the remote DirectoryServer
      SendPacket(CreateRegisterApp(mAppInfo));

      // Register this server with the remote DirectoryServer
      SendPacket(CreateRegisterServer(mAppInfo.id, theServerInfo));
    }
    else
    {
      // Log an error indicating we aren't connected to a DirectoryServer
      ELOG() << "DirectoryClient::RegisterServer() not connected to DirectoryServer" << std::endl;
    }
  }

  void DirectoryClient::UnregisterServer(const typeNetAlias theNetAlias)
  {
    if(IsConnected())
    {
      // Unregister this server with the remote DirectoryServer
      SendPacket(CreateUnregisterServer(mAppInfo.id, theNetAlias));
    }
    else
    {
      // Log an error indicating we aren't connected to a DirectoryServer
      ELOG() << "DirectoryClient::UnegisterServer() not connected to DirectoryServer" << std::endl;
    }
  }

  void DirectoryClient::RegisterSubscriber(void)
  {
    if(IsConnected())
    {
      // Make sure our application is registered with the remote DirectoryServer
      SendPacket(CreateRegisterApp(mAppInfo));

      // Register this DirectoryClient for theAppID specified
      SendPacket(CreateRegisterSubscriber(mAppInfo.id));
    }
  }

  void DirectoryClient::UnregisterSubscriber(void)
  {
    if(IsConnected())
    {
      // Unregister this DirectoryClient for theAppID specified
      SendPacket(CreateUnregisterSubscriber(mAppInfo.id));
    }
  }

  typeServerMap DirectoryClient::GetSubscriberList(void)
  {
    // Obtain a lock on our subscription mutex before getting server map
    sf::Lock anLock(mSubscriptionMutex);

    // Create an empty server map to return
    typeServerMap anResult;

    // Iterator to find our application subscription
    std::map<const typeAppID, typeServerMap>::iterator anIter;

    // Attempt to find our application subscription
    anIter = mSubscriptions.find(mAppInfo.id);

    // Did we find our application subscription?
    if(anIter != mSubscriptions.end())
    {
      // Make a copy of the server map
      anResult = anIter->second;
    }

    // Return the server map found above or an empty one by default
    return anResult;
  }

  bool DirectoryClient::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = false;

    // Get the message label now and verify the size
    typeNetLabel anSourceNetLabel = thePacket.GetNetLabel();

    // Validate the known message types according to size
    switch(anSourceNetLabel)
    {
      case INetPacket::NET_SERVER_INFO:
        if(GetServerInfoSize() <= theSize)
        {
          // Message label is correct as far as we can tell
          anResult = true;
        }
        else
        {
          ELOG() << "DirectoryClient::VerifyIncoming() invalid ServerInfo message size("
                 << (Uint32)GetServerInfoSize() << "!=" << theSize << ")" << std::endl;
        }
        break;
      default: // Unknown message types are accepted with a DisconnectClient response
        anResult = true;
        break;
    }

    // Return the result determined above
    return anResult;
  }

  INetPacket* DirectoryClient::ProcessIncoming(INetPacket* thePacket)
  {
    // Default to no response necessary
    INetPacket* anResult = NULL;

    // Retrieve the source message label
    typeNetLabel anSourceNetLabel = thePacket->GetNetLabel();

    // Switch on the known message types
    switch(anSourceNetLabel)
    {
      case INetPacket::NET_SERVER_INFO:
        // Call ProcessServerInfo to process this message
        ProcessServerInfo(thePacket);
        break;
      default: // Unknown message types receive a Disconnect message as a response
        // Create a Disconnect client response message
        anResult = CreateDisconnect();
        break;
    }

    // Return the response result decided above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateRegisterApp(const typeAppInfo theAppInfo)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Obtain a lock on our subscription mutex and add new AppID
      sf::Lock anLock(mSubscriptionMutex);

      // An empty server map to use when adding new AppID
      typeServerMap anServerMap;

      // Assign the message label and add the assigned NetID to the message
      anResult->SetNetLabel(INetPacket::NET_REGISTER_APP);
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      anResult->SetNetID(mNetID);

      // Add the application ID, title, description and website to the message
      *anResult << theAppInfo.id;
      *anResult << theAppInfo.title;
      *anResult << theAppInfo.description;
      *anResult << theAppInfo.website;

      // Add an empty server map for this new AppID
      mSubscriptions.insert(std::pair<const typeAppID, typeServerMap>(theAppInfo.id, anServerMap));
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateRegisterServer(const typeAppID theAppID,
                                                    const typeServerInfo theServerInfo)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message label and add the assigned NetID to the message
      anResult->SetNetLabel(INetPacket::NET_REGISTER_SERVER);
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      anResult->SetNetID(mNetID);

      // Add the application ID to this message first
      *anResult << theAppID;

      // Add theServerInfo to this message next
#if (SFML_VERSION_MAJOR < 2)
      *anResult << theServerInfo.address.ToInteger();
#else
      *anResult << theServerInfo.address.toInteger();
#endif
      *anResult << theServerInfo.port;
      *anResult << theServerInfo.maxClients;
      *anResult << theServerInfo.activeClients;
      *anResult << theServerInfo.version.mMajor;
      *anResult << theServerInfo.version.mMinor;
      *anResult << theServerInfo.version.mPatchBuild;
      *anResult << theServerInfo.alias;
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateUnregisterServer(const typeAppID theAppID, const typeNetAlias theNetAlias)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message label and add the assigned NetID to the message
      anResult->SetNetLabel(INetPacket::NET_UNREGISTER_SERVER);
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      anResult->SetNetID(mNetID);

      // Add the application ID to this message first
      *anResult << theAppID;

      // Add the server ID to this message last
      *anResult << theNetAlias;
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateRegisterSubscriber(const typeAppID theAppID)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message label and add the assigned NetID to the message
      anResult->SetNetLabel(INetPacket::NET_REGISTER_SUBSCRIBER);
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      anResult->SetNetID(mNetID);

      // Add the application ID to this message first
      *anResult << theAppID;
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateUnregisterSubscriber(const typeAppID theAppID)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message label and add the assigned NetID to the message
      anResult->SetNetLabel(INetPacket::NET_UNREGISTER_SUBSCRIBER);
      anResult->SetFlag(INetPacket::FlagAckRequired,
                        mProtocol == NetTcp ? false : true);
      anResult->SetNetID(mNetID);

      // Add the application ID to this message first
      *anResult << theAppID;
    }

    // Return the response result created above
    return anResult;
  }

  std::size_t DirectoryClient::GetServerInfoSize(void) const
  {
    // Header + app ID + address + port + max and active clients + version + alias size and string
    return INetPacket::HEADER_SIZE_B + sizeof(Uint32)*5 + sizeof(Uint16)*2 + sizeof(Uint8)*3;
  }

  void DirectoryClient::ProcessServerInfo(INetPacket* thePacket)
  {
    // Obtain a lock on our subscription mutex updating server info
    sf::Lock anLock(mSubscriptionMutex);

    // The application ID to register this server under
    typeAppID anAppID;

    // Iterator to find our application subscription
    std::map<const typeAppID, typeServerMap>::iterator anIter;

    // Retrieve the application ID from the RegisterServer message
    *thePacket >> anAppID;

    // Attempt to find our application subscription
    anIter = mSubscriptions.find(anAppID);

    // Did we find our application subscription?
    if(anIter != mSubscriptions.end())
    {
      // The server info structure to build
      typeServerInfo anServerInfo;

      // The server address information as an integer
      Uint32 anServerAddress;

      // Retrieve the address, port, client info, version and id
      *thePacket >> anServerAddress;
      *thePacket >> anServerInfo.port;
      *thePacket >> anServerInfo.maxClients;
      *thePacket >> anServerInfo.activeClients;
      *thePacket >> anServerInfo.version.mMajor;
      *thePacket >> anServerInfo.version.mMinor;
      *thePacket >> anServerInfo.version.mPatchBuild;
      *thePacket >> anServerInfo.alias;

      // Now convert the integer address into an sf::IPAddress
      anServerInfo.address = anServerAddress;

      // Is the Yes/No flag set? then delete this server info
      if(thePacket->GetFlag(INetPacket::FlagYesResponse))
      {
        // See if we can find the server using the alias provided
        typeServerMapIter anServerIter = anIter->second.find(anServerInfo.alias);

        // Did we find the server, then remove it from our subscription list
        if(anServerIter != anIter->second.end())
        {
          anIter->second.erase(anServerIter);
        }
        else
        {
          WLOG() << "DirectoryClient::ProcessServerInfo() server("
                 << anServerInfo.alias << ") not found" << std::endl;
        }
      }
      else
      {
        // See if we can find the server using the alias provided
        typeServerMapIter anServerIter = anIter->second.find(anServerInfo.alias);

        // Did we not find the server? then add it now
        if(anServerIter == anIter->second.end())
        {
          // Add the new server to our subscriptions list
          anIter->second.insert(typeServerMapPair(anServerInfo.alias, anServerInfo));
        }
        else
        {
          // Update the existing server with the new information provided
          anServerIter->second = anServerInfo;
        }
      }
    }
    else
    {
      WLOG() << "DirectoryClient::ProcessServerInfo() subscription for app("
             << anAppID << ") not found" << std::endl;
    }
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
