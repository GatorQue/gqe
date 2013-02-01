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
  DirectoryClient::DirectoryClient(const typeClientID theClientID,
                                   const typeVersionInfo theClientVersion,
                                   const typeAppInfo theAppInfo,
                                   INetPool& theNetPool,
                                   const DirectoryScope theScope,
                                   const Uint16 theServerPort) :
    INetClient(theClientID, theClientVersion, theNetPool,
               // Use UDP for LAN servers and TCP for WAN servers
               theScope == ScopeLocal ? NetUdp : NetTcp),
    mAppInfo(theAppInfo),
    mScope(theScope),
    mDirectoryServer(NULL)
  {
    // Set the Server address depending on theScope provided
    if(theScope == ScopeLocal)
    {
#if (SFML_VERSION_MAJOR < 2)
      SetServerAddress(0xffffffff);
#else
      SetServerAddress(sf::IpAddress::Broadcast);
#endif
    }
    else
    {
      SetServerAddress("gqe.noip.com");
    }

    // Set the Server port
    SetServerPort(theServerPort);
  }

  DirectoryClient::~DirectoryClient()
  {
    // Do we have a local directory server running? then delete it
    if(mDirectoryServer)
    {
      // Unregister the server before deleting it
      mDirectoryServer->UnregisterServer(mAppInfo.id, mServerID);

      // Delete the directory server now (which will disconnect all clients)
      delete mDirectoryServer;

      // Don't keep pointers around we aren't using
      mDirectoryServer = NULL;
    }
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
    // Are we a local server and not already running a local DirectoryServer? then create one now
    if(ScopeLocal == mScope)
    {
      if(NULL == mDirectoryServer)
      {
        // Record our server ID for this local server
        mServerID = theServerInfo.id;

        // Attempt to create a local DirectoryServer
        mDirectoryServer = new (std::nothrow) DirectoryServer(
            theServerInfo.id, theServerInfo.version, mNetPool, mScope,
            theServerInfo.port);
        assert(mDirectoryServer);

        // Start the local DirectoryServer
        if(mDirectoryServer)
        {
          // Register this application with the local DirectoryServer
          mDirectoryServer->RegisterApp(mAppInfo);

          // Register this server with the local DirectoryServer
          mDirectoryServer->RegisterServer(mAppInfo.id, theServerInfo);

          // Start the local directory server to publish the local server
          mDirectoryServer->Start();
        }
        else
        {
          // Register a fatal error due to memory allocation problems
          FLOG(StatusError) << "DirectoryClient::RegisterServer() unable to allocate memory" << std::endl;
        }
      }
      else
      {
        // Log an error indicating a local server is already running
        ELOG() << "DirectoryClient::RegisterServer() local server already running" << std::endl;
      }
    }
    else
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
  }

  void DirectoryClient::UnregisterServer(void)
  {
    // Are we running in local scope? then stop the local directory server from running
    if(ScopeLocal == mScope)
    {
      // Are we running a local directory server?
      if(NULL != mDirectoryServer)
      {
        // Unregister the server before deleting it
        mDirectoryServer->UnregisterServer(mAppInfo.id, mServerID);

        // Delete the local directory server
        delete mDirectoryServer;

        // Don't keep around pointers that aren't valid anymore
        mDirectoryServer = NULL;
      }
      else
      {
        // Log a warning that no directory server is currently running
        WLOG() << "DirectoryClient::UnregisterServer() no local server is running" << std::endl;
      }
    }
    else
    {
      if(IsConnected())
      {
        // Unregister this server with the remote DirectoryServer
        SendPacket(CreateUnregisterServer(mAppInfo.id, mServerID));
      }
      else
      {
        // Log an error indicating we aren't connected to a DirectoryServer
        ELOG() << "DirectoryClient::UnegisterServer() not connected to DirectoryServer" << std::endl;
      }
    }

    // Clear our registered server ID
    mServerID.clear();
  }

  bool DirectoryClient::VerifyIncoming(INetPacket& thePacket, std::size_t theSize)
  {
    // Default implementation is to make all messages valid
    bool anResult = true;

    // Return the result determined above
    return anResult;
  }

  INetPacket* DirectoryClient::ProcessIncoming(INetPacket* thePacket)
  {
    // Default to no response necessary
    INetPacket* anResult = NULL;

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
      // Assign the message type and add the assigned HostID to the message
      anResult->SetType(INetPacket::NET_REGISTER_APP);
      anResult->SetFlag(INetPacket::FlagAckRequired, mProtocol == NetTcp ? false : true);
      anResult->SetHostID(mHostID);

      // Add the application ID, title, description and website to the message
      *anResult << theAppInfo.id;
      *anResult << theAppInfo.title;
      *anResult << theAppInfo.description;
      *anResult << theAppInfo.website;
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateRegisterServer(const typeAppID theAppID, const typeServerInfo theServerInfo)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message type and add the assigned HostID to the message
      anResult->SetType(INetPacket::NET_REGISTER_SERVER);
      anResult->SetFlag(INetPacket::FlagAckRequired, mProtocol == NetTcp ? false : true);
      anResult->SetHostID(mHostID);

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
      *anResult << theServerInfo.version.major;
      *anResult << theServerInfo.version.minor;
      *anResult << theServerInfo.version.patch;
      *anResult << theServerInfo.id;
    }

    // Return the response result created above
    return anResult;
  }

  INetPacket* DirectoryClient::CreateUnregisterServer(const typeAppID theAppID, const typeServerID theServerID)
  {
    // Get a packet for our Register App message
    INetPacket* anResult = mNetPool.GetOutgoing();

    // Make sure a valid packet was returned
    if(anResult)
    {
      // Assign the message type and add the assigned HostID to the message
      anResult->SetType(INetPacket::NET_UNREGISTER_SERVER);
      anResult->SetFlag(INetPacket::FlagAckRequired, mProtocol == NetTcp ? false : true);
      anResult->SetHostID(mHostID);

      // Add the application ID to this message first
      *anResult << theAppID;

      // Add the server ID to this message last
      *anResult << theServerID;
    }

    // Return the response result created above
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
