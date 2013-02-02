/**
 * Provides the INetPacket class that represents the base class for all
 * network packets used by the TcpClient, TcpServer, UdpClient, and UdpServer
 * classes. Many of the method implementations were borrowed from the
 * sf::Packet class provided by the SFML Network library. The primary
 * difference between the two classes is the addition of some header data to
 * allow for a TCP like implementation using the UDP protocol and other similar
 * improvements.
 *
 * @file src/GQE/Core/interfaces/INetPacket.cpp
 * @author Ryan Lindeman
 * @date 20121227 - Initial Release
 * @date 20130111 - Added new Clear method, EndTransfer flag, and sort type enum
 * @date 20130112 - Added new Clock for first sent
 */

#include <GQE/Core/interfaces/INetPacket.hpp>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <vector>

namespace GQE
{
  INetPacket::INetPacket(const std::size_t theCapacity, const std::size_t theMinimum,
                         const SortType theSortType, const Uint8 theSync) :
    mSync(theSync), // defaults to 0x5A = 'Z' = 90 in decimal
    mValid(false),
    mSortType(theSortType),
    mMinimum(theMinimum < HEADER_SIZE_B ? HEADER_SIZE_B : theMinimum),
    mData(theCapacity < HEADER_SIZE_B ? HEADER_SIZE_B : theCapacity, 0),
    mReadPosition(HEADER_SIZE_B), // Read/Write cursor starts after header
    mTimestamp(~0LL)
  {
    // Call our Clear method to intialize this packet
    Clear();
  }

  INetPacket::~INetPacket()
  {
  }

  void INetPacket::Clear(void)
  {
    // Start by making the packet invalid (to avoid someone reading it while we work)
    mValid = false;

    // Clear the data of its contents
    mData.clear();

    // Resize the data using theCapacity provided
    mData.resize(mMinimum);

    // Reset our read position variable
    mReadPosition = HEADER_SIZE_B;

    // Assign the sync byte
    mData[SYNC_POSITION_B] = mSync;

    // Assign the version byte to this data array
    mData[VERSION_POSITION_B] = VERSION_BYTE;

    // Reset our timestamp value
    mTimestamp = ~0LL;

    // Make the packet valid at the end
    mValid = true;
  }

  bool INetPacket::HasSync(void) const
  {
    return mData[SYNC_POSITION_B] == mSync;
  }

  bool INetPacket::IsVersionValid(void) const
  {
    Uint8 anVersion = mData[VERSION_POSITION_B];

    // Valid if version byte is the same or older than this code
    return anVersion != VERSION_NONE && anVersion <= VERSION_BYTE;
  }

  bool INetPacket::IsValid(void) const
  {
    return mValid;
  }

  Uint8 INetPacket::GetSync(void) const
  {
    // Return the sync validation code
    return mSync;
  }

  void INetPacket::SetSync(const Uint8 theSync)
  {
    // Assign theSync to be used for validation
    mSync = theSync;
  }

  Uint8 INetPacket::GetVersion(void) const
  {
    // Return the version from our character array
    return mData[VERSION_POSITION_B];
  }

  typeNetLabel INetPacket::GetNetLabel(void) const
  {
    // Return the label in our character array
    return ntohs(*reinterpret_cast<const typeNetLabel*>(&mData[LABEL_POSITION_B]));
  }

  void INetPacket::SetNetLabel(const typeNetLabel theNetLabel)
  {
    typeNetLabel* anNetLabel = (typeNetLabel*)&mData[LABEL_POSITION_B];

    // Assign the label to our character array
    *anNetLabel = htons(theNetLabel);
  }

  bool INetPacket::GetFlag(const NetFlag theFlag) const
  {
    Uint32 anFlags = ntohl(*reinterpret_cast<const Uint32*>(&mData[FLAG_POSITION_B]));

    // Default to a false result
    bool anResult = false;

    // See if this flag bit is non-zero when theFlag mask is applied
    if(FlagNone != (anFlags & theFlag))
    {
      anResult = true;
    }

    // Return the result found above
    return anResult;
  }

  Uint32 INetPacket::GetFlags(void) const
  {
    // Return all the flags from our character array
    return ntohl(*reinterpret_cast<const Uint32*>(&mData[FLAG_POSITION_B]));
  }

  void INetPacket::SetFlag(const NetFlag theFlag, const bool theValue)
  {
    Uint32* anFlags = (Uint32*)&mData[FLAG_POSITION_B];

    // Set the flag bit according to theValue
    if(theValue)
    {
      // Make sure the bit(s) are set by using the mask provided to set the bit
      *anFlags |=  htonl(theFlag);
    }
    else
    {
      // Clear the bit(s) by inverting the mask provided
      *anFlags &= htonl(~theFlag);
    }
  }

  typeNetID INetPacket::GetNetID(void) const
  {
    // Return the hostID from our character array
    return ntohl(*reinterpret_cast<const typeNetID*>(&mData[NETID_POSITION_B]));
  }

  void INetPacket::SetNetID(const typeNetID theNetID)
  {
    typeNetID* anNetID = (typeNetID*)&mData[NETID_POSITION_B];

    // Assign the hostID to our character array
    *anNetID = htonl(theNetID);
  }

  typeNetSequence INetPacket::GetNetSequence(void) const
  {
    // Return the sequence number from our character array
    return ntohl(*reinterpret_cast<const typeNetSequence*>(&mData[SEQUENCE_POSITION_B]));
  }

  void INetPacket::SetNetSequence(const typeNetSequence theNetSequence)
  {
    typeNetSequence* anNetSequence = (typeNetSequence*)&mData[SEQUENCE_POSITION_B];

    // Assign theNetSequence to our character array
    *anNetSequence = htonl(theNetSequence);
  }

  void* INetPacket::GetData(void)
  {
    return (!mData.empty() && mValid) ? &mData[0] : NULL;
  }

  void* INetPacket::GetPayload(void)
  {
    return (!mData.empty() && mData.size() > HEADER_SIZE_B && mValid) ? &mData[HEADER_SIZE_B] : NULL;
  }

  std::size_t INetPacket::GetSize(void) const
  {
    return mData.size();
  }

  std::size_t INetPacket::GetCapacity(void) const
  {
    return mData.capacity();
  }

  void INetPacket::SetCapacity(const std::size_t theCapacity)
  {
    // Make sure we aren't resizing to something too small
    if(theCapacity > HEADER_SIZE_B && theCapacity > mData.size())
    {
      // Resize the vector now to the new capacity specified
      mData.resize(theCapacity);
    }
    else
    {
      ILOG() << "INetPacket::SetCapacity() invalid capacity(" << theCapacity << ") provided" << std::endl;
    }
  }

#if (SFML_VERSION_MAJOR < 2)
  double INetPacket::GetFirstSent(void) const
  {
    return mFirstSent.GetElapsedTime();
  }
#else
  sf::Time INetPacket::GetFirstSent(void) const
  {
    return mFirstSent.getElapsedTime();
  }
#endif

  void INetPacket::SetFirstSent(void)
  {
#if (SFML_VERSION_MAJOR < 2)
    mFirstSent.Reset();
#else
    mFirstSent.restart();
#endif
  }

#if (SFML_VERSION_MAJOR < 2)
  double INetPacket::GetLastSent(void) const
  {
    return mLastSent.GetElapsedTime();
  }
#else
  sf::Time INetPacket::GetLastSent(void) const
  {
    return mLastSent.getElapsedTime();
  }
#endif

  void INetPacket::SetLastSent(void)
  {
#if (SFML_VERSION_MAJOR < 2)
    mLastSent.Reset();
#else
    mLastSent.restart();
#endif
  }

  Int64 INetPacket::GetTimestamp(void) const
  {
    return mTimestamp;
  }

  void INetPacket::SetTimestamp(const Int64 theTimestamp)
  {
    mTimestamp = theTimestamp;
  }

  INetPacket::SortType INetPacket::GetSortType(void) const
  {
    return mSortType;
  }

  void INetPacket::SetSortType(const SortType theSortType)
  {
    // Did they provide a valid sort type? then use it
    switch(theSortType)
    {
      case SortNetSequence:
      case SortLastSent:
      case SortTimestamp:
        mSortType = theSortType;
        break;
      default:
        /* Unknown sort type specified */
        break;
    }
  }

  void INetPacket::LogHeader(void)
  {
    ILOG() << "v" << (GetVersion() >> 4) << "." << (GetVersion() & 0x0F) << ",label="
           << GetNetLabel() << ",flags=" << std::hex << GetFlags() << std::dec << ",id="
           << GetNetID() << ",sn=" << GetNetSequence() << ",sz=" << GetSize() << ",cap="
           << GetCapacity() << std::endl;
  }

  void INetPacket::LogPayload(void)
  {
    std::stringstream anPayloadHex;
    std::stringstream anPayloadAscii;
    std::size_t count = 0;

    anPayloadHex << std::hex << std::setfill('0');
    for(std::size_t iloop = HEADER_SIZE_B; iloop<GetSize(); iloop++)
    {
      // Count each byte as we go
      count++;

      // Add the current byte to the hex stringstream
      anPayloadHex << std::setw(2) << static_cast<unsigned>(mData[iloop]);

      // Add the alphanumeric version or . to the ascii stringstream
      if(isalnum(mData[iloop]))
      {
        anPayloadAscii << mData[iloop];
      }
      else
      {
        anPayloadAscii << ".";
      }

      // Every 16 bytes we flush to log and start another line
      if((count % 16) == 0)
      {
        // Log this line
        ILOG() << (count / 16) * 16 << ":" << anPayloadHex.str() << " *" << anPayloadAscii << "*" << std::endl;

        // Clear the payload strings and keep going
        anPayloadHex.clear();
        anPayloadAscii.clear();
      }
      // Every 2 bytes we add one space
      else if((count % 2) == 0)
      {
        anPayloadHex << " ";
      }
    }

    // Log any remaining values if necessary
    if(anPayloadHex.str().length() || anPayloadAscii.str().length())
    {
      // Log this line
      ILOG() << (count / 16) * 16 << ":" << anPayloadHex.str() << " *" << anPayloadAscii.str() << "*" << std::endl;

      // Clear the payload strings and keep going
      anPayloadHex.clear();
      anPayloadAscii.clear();
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  // The methods below are almost all directly copied from sf::Packet
  ///////////////////////////////////////////////////////////////////////////
  INetPacket::operator BoolType() const
  {
    return mValid ? &INetPacket::CheckSize : NULL;
  }

  INetPacket& INetPacket::operator>>(bool& theValue)
  {
    Uint8 anValue;
    if(*this >> anValue)
    {
      theValue = (anValue != 0);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Int8& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = *reinterpret_cast<const Int8*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Uint8& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = *reinterpret_cast<const Uint8*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Int16& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = ntohs(*reinterpret_cast<const Int16*>(&mData[mReadPosition]));
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Uint16& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = ntohs(*reinterpret_cast<const Uint16*>(&mData[mReadPosition]));
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Int32& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = ntohl(*reinterpret_cast<const Int32*>(&mData[mReadPosition]));
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Uint32& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = ntohl(*reinterpret_cast<const Uint32*>(&mData[mReadPosition]));
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Int64& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      //theValue = ntohll(*reinterpret_cast<const Int64*>(&mData[mReadPosition]));
      theValue = *reinterpret_cast<const Int64*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(Uint64& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      //theValue = ntohll(*reinterpret_cast<const Uint64*>(&mData[mReadPosition]));
      theValue = *reinterpret_cast<const Uint64*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(float& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = *reinterpret_cast<const float*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(double& theValue)
  {
    if(CheckSize(sizeof(theValue)))
    {
      theValue = *reinterpret_cast<const double*>(&mData[mReadPosition]);
      mReadPosition += sizeof(theValue);
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(char* theValue)
  {
    // First extract string length
    Uint32 anLength = 0;
    *this >> anLength;

    if((anLength > 0) && CheckSize(anLength))
    {
      // Then extract characters
      std::memcpy(theValue, &mData[mReadPosition], anLength);
      theValue[anLength] = '\0';

      // Update reading position
      mReadPosition += anLength;
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(std::string& theValue)
  {
    // First extract string length
    Uint32 anLength = 0;
    *this >> anLength;

    theValue.clear();
    if((anLength > 0) && CheckSize(anLength))
    {
      // Then extract characters
      theValue.assign(&mData[mReadPosition], anLength);

      // Update reading position
      mReadPosition += anLength;
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(wchar_t* theValue)
  {
    // First extract string length
    Uint32 anLength = 0;
    *this >> anLength;

    if((anLength > 0) && CheckSize(anLength*sizeof(Uint32)))
    {
      // Then extract characters
      for(Uint32 i = 0; i < anLength; ++i)
      {
        Uint32 anCharacter = 0;
        *this >> anCharacter;
        theValue[i] = static_cast<wchar_t>(anCharacter);
      }
      theValue[anLength] = L'\0';
    }

    return *this;
  }

  INetPacket& INetPacket::operator>>(std::wstring& theValue)
  {
    // First extract string length
    Uint32 anLength = 0;
    *this >> anLength;

    theValue.clear();
    if((anLength > 0) && CheckSize(anLength*sizeof(Uint32)))
    {
      // Then extract characters
      for (Uint32 i = 0; i < anLength; ++i)
      {
        Uint32 anCharacter = 0;
        *this >> anCharacter;
        theValue += static_cast<wchar_t>(anCharacter);
      }
    }

    return *this;
  }

#if (SFML_VERSION_MAJOR < 2)
  INetPacket& INetPacket::operator>>(sf::Unicode::Text& theValue)
  {
    std::wstring anValue;
    if(*this >> anValue)
    {
      theValue = anValue;
    }

    return *this;
  }
#else
  INetPacket& INetPacket::operator>>(sf::String& theValue)
  {
    // First extract the string length
    Uint32 anLength = 0;
    *this >> anLength;

    theValue.clear();
    if((anLength > 0) && CheckSize(anLength*sizeof(Uint32)))
    {
      // Then extract characters
      for(Uint32 i = 0; i < anLength; ++i)
      {
        Uint32 anCharacter = 0;
        *this >> anCharacter;
        theValue += anCharacter;
      }
    }

    return *this;
  }
#endif

  INetPacket& INetPacket::operator<<(bool theValue)
  {
    *this << static_cast<Uint8>(theValue);
    return *this;
  }

  INetPacket& INetPacket::operator<<(Int8 theValue)
  {
    Append(&theValue, sizeof(theValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Uint8 theValue)
  {
    Append(&theValue, sizeof(theValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Int16 theValue)
  {
    Int16 anValue = htons(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Uint16 theValue)
  {
    Uint16 anValue = htons(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Int32 theValue)
  {
    Int32 anValue = htonl(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Uint32 theValue)
  {
    Uint32 anValue = htonl(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Int64 theValue)
  {
    Int64 anValue = theValue; //htonll(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(Uint64 theValue)
  {
    Uint64 anValue = theValue; //htonll(theValue);
    Append(&anValue, sizeof(anValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(float theValue)
  {
    Append(&theValue, sizeof(theValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(double theValue)
  {
    Append(&theValue, sizeof(theValue));
    return *this;
  }

  INetPacket& INetPacket::operator<<(const char* theValue)
  {
    // First insert string length
    Uint32 anLength = 0;
    for(const char* anCharacter = theValue; *anCharacter != '\0'; ++anCharacter)
    {
      ++anLength;
    }

    *this << anLength;

    // Then insert characters
    Append(theValue, anLength * sizeof(char));

    return *this;
  }

  INetPacket& INetPacket::operator<<(const std::string& theValue)
  {
    // First insert string length
    Uint32 anLength = static_cast<Uint32>(theValue.size());
    *this << anLength;

    // Then insert characters
    if(anLength > 0)
    {
      Append(theValue.c_str(), anLength * sizeof(std::string::value_type));
    }

    return *this;
  }

  INetPacket& INetPacket::operator<<(const wchar_t* theValue)
  {
    // First insert string length
    Uint32 anLength = 0;
    for (const wchar_t* anCharacter = theValue; *anCharacter != L'\0'; ++anCharacter)
    {
      ++anLength;
    }

    *this << anLength;

    // Then insert characters
    for (const wchar_t* anCharacter = theValue; *anCharacter != L'\0'; ++anCharacter)
    {
      *this << static_cast<Uint32>(*anCharacter);
    }

    return *this;
  }

  INetPacket& INetPacket::operator<<(const std::wstring& theValue)
  {
    // First insert string length
    Uint32 anLength = static_cast<Uint32>(theValue.size());
    *this << anLength;

    // Then insert characters
    if(anLength > 0)
    {
      for(std::wstring::const_iterator anCharacter = theValue.begin();
          anCharacter != theValue.end(); ++anCharacter)
      {
        *this << static_cast<Uint32>(*anCharacter);
      }
    }

    return *this;
  }

#if (SFML_VERSION_MAJOR < 2)
  INetPacket& INetPacket::operator<<(const sf::Unicode::Text& theValue)
  {
    // First convert theValue into a wstring value
    std::wstring anValue = theValue;

    // Call our overloaded method for wstring values
    *this << anValue;

    return *this;
  }
#else
  INetPacket& INetPacket::operator<<(const sf::String& theValue)
  {
    // First insert the string length
    Uint32 anLength = static_cast<Uint32>(theValue.getSize());
    *this << anLength;

    // Then insert characters
    if(anLength > 0)
    {
      for(sf::String::ConstIterator anCharacter = theValue.begin();
          anCharacter != theValue.end(); ++anCharacter)
      {
        *this << *anCharacter;
      }
    }

    return *this;
  }
#endif

  void INetPacket::Append(const void* theData, std::size_t theSize)
  {
    if(theData && (theSize > 0))
    {
      std::size_t anStart = mData.size();
      mData.resize(anStart + theSize);

      // Copy the data provided into our network packet
      std::memcpy(&mData[anStart], theData, theSize);
    }
  }

  bool INetPacket::CheckSize(std::size_t theSize)
  {
    return (mReadPosition + theSize) <= mData.size();
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
