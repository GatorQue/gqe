/**
 * Provides the INetPacket class that represents the base class for all
 * network packets used by the TcpClient, TcpServer, UdpClient, and UdpServer
 * classes. Many of the method implementations were borrowed from the
 * sf::Packet class provided by the SFML Network library. The primary
 * difference between the two classes is the addition of some header data to
 * allow for a TCP like implementation using the UDP protocol and other similar
 * improvements.
 *
 * @file include/GQE/Core/interfaces/INetPacket.hpp
 * @author Ryan Lindeman
 * @date 20121227 - Initial Release
 * @date 20130111 - Added new Clear method, EndTransfer flag, and sort type enum
 * @date 20130112 - Added new Clock for first sent
 */
#ifndef INET_PACKET_HPP_INCLUDED
#define INET_PACKET_HPP_INCLUDED

#include <SFML/System.hpp>
#include <GQE/Core/Core_types.hpp>
#include <GQE/Core/loggers/Log_macros.hpp>

namespace GQE
{
  /// Provides the INetPacket base class for all network traffic
  class GQE_API INetPacket
  {
    // A bool-like type that cannot be converted to integer or pointer types
    typedef bool (INetPacket::*BoolType)(std::size_t);

    public:
      /// Sort type enumeration for INetPacket <,>,<=,etc operators
      enum SortType
      {
        SortSequenceNumber = 0, /// Sort by SequenceNumber
        SortLastSent       = 1, /// Sort by LastSent timestamp value
        SortTimestamp      = 2  /// Sort by Timestamp value
      };

      // Constants
      ///////////////////////////////////////////////////////////////////////////
      /// Minimum header size constant in bytes is 16=sync+version+type+flags+hostID+seq#
      static const Uint8 HEADER_SIZE_B    = 16;
      /// Default SYNC value to use
      static const Uint8 SYNC_BYTE        = 0x5A;
      /// Connect message type value
      static const Uint16 NET_CONNECT     = 1;
      /// Disconnect message type value
      static const Uint16 NET_DISCONNECT  = 2;
      /// Identity message type value
      static const Uint16 NET_IDENTITY    = 3;
      /// Acknowledge message type value
      static const Uint16 NET_ACKNOWLEDGE = 4;
      /// Time synchronization pass 1
      static const Uint16 NET_SYNC_1      = 5;
      /// Time synchronization pass 2
      static const Uint16 NET_SYNC_2      = 6;

      // Enumerations
      ///////////////////////////////////////////////////////////////////////////
      enum NetFlag
      {
        FlagNone        = 0x00000000, ///< No flags enabled (can be used to clear all flags)
        FlagAckRequired = 0x00000001, ///< Acknowledgement required for this message
        FlagYesResponse = 0x00000002, ///< Yes response provided for this message
        FlagEndTransfer = 0x00000004, ///< End transfer flag indicating the end of a series
        FlagReserved5   = 0x00000008, ///< Reserved flag 5
        FlagReserved4   = 0x00000010, ///< Reserved flag 4
        FlagReserved3   = 0x00000020, ///< Reserved flag 3
        FlagReserved2   = 0x00000040, ///< Reserved flag 2
        FlagReserved1   = 0x00000080, ///< Reserved flag 1
        FlagUser24      = 0x00000100, ///< User defined flag 24
        FlagUser23      = 0x00000200, ///< User defined flag 23
        FlagUser22      = 0x00000400, ///< User defined flag 22
        FlagUser21      = 0x00000800, ///< User defined flag 21
        FlagUser20      = 0x00001000, ///< User defined flag 20
        FlagUser19      = 0x00002000, ///< User defined flag 19
        FlagUser18      = 0x00004000, ///< User defined flag 18
        FlagUser17      = 0x00008000, ///< User defined flag 17
        FlagUser16      = 0x00010000, ///< User defined flag 16
        FlagUser15      = 0x00020000, ///< User defined flag 15
        FlagUser14      = 0x00040000, ///< User defined flag 14
        FlagUser13      = 0x00080000, ///< User defined flag 13
        FlagUser12      = 0x00100000, ///< User defined flag 12
        FlagUser11      = 0x00200000, ///< User defined flag 11
        FlagUser10      = 0x00400000, ///< User defined flag 10
        FlagUser9       = 0x00800000, ///< User defined flag 9
        FlagUser8       = 0x01000000, ///< User defined flag 8
        FlagUser7       = 0x02000000, ///< User defined flag 7
        FlagUser6       = 0x04000000, ///< User defined flag 6
        FlagUser5       = 0x08000000, ///< User defined flag 5
        FlagUser4       = 0x10000000, ///< User defined flag 4
        FlagUser3       = 0x20000000, ///< User defined flag 3
        FlagUser2       = 0x40000000, ///< User defined flag 2
        FlagUser1       = 0x80000000, ///< User defined flag 1
        FlagAll         = 0xFFFFFFFF  ///< All flags enabled (can be used to test for any flag)
      };

      /**
       * INetPacket default constructor
       * @param[in] theCapacity or maximum size to allocate for each network packet
       * @param[in] theMinimum size to use when clearing the network packet
       * @param[in] theSortType to use when sorting INetPacket classes
       * @param[in] theSync to use for validating each network packet
       */
      INetPacket(const std::size_t theCapacity = HEADER_SIZE_B,
                 const std::size_t theMinimum = HEADER_SIZE_B,
                 const SortType theSortType = SortSequenceNumber,
                 const Uint8 theSync = SYNC_BYTE);

      /**
       * INetPacket deconstructor
       */
      virtual ~INetPacket();

      /**
       * Clear is responsible for clearing/reseting this packet so it can be
       * reused to send new data. This involves reseting the sync byte, version
       * byte, and other values back to known default values.
       */
      void Clear(void);

      /**
       * HasSync will return true if the network data has a valid sync code
       * which can be used as part of the validation sequence. It does this
       * by comparing the first byte in the network data to theSync code
       * provided at construction time or by the SetSync method.
       * @return true if first byte matches sync code, false otherwise
       */
      bool HasSync(void) const;

      /**
       * IsVersionValid will return true if the version byte is the same or
       * older than the VERSION_BYTE value that was compiled in the code.
       */
      bool IsVersionValid(void) const;

      /**
       * IsValid will return true if the network data passes the validation
       * tests as provided by the derived class.
       * @return true if data payload is valid, false otherwise
       */
      bool IsValid(void) const;

      /**
       * GetSync is responsible for returning the sync that will be used in
       * validating this INetPacket.
       * @return the sync assigned for validating each INetPacket
       */
      GQE::Uint8 GetSync(void) const;

      /**
       * SetSync is responsible for setting theSync value that will be used in
       * validating this INetPacket.
       * @param[in] theSync to be used for validating each INetPacket
       */
      void SetSync(const Uint8 theSync);

      /**
       * GetVersion is responsible for returning the version assigned to this
       * INetPacket.
       * @return the version assigned to this INetPacket
       */
      GQE::Uint8 GetVersion(void) const;

      /**
       * GetType is responsible for returning the message type assigned to this
       * INetPacket.
       * @return the message type assigned to this INetPacket
       */
      GQE::Uint16 GetType(void) const;

      /**
       * SetType is responsible for setting the message type assigned to this
       * INetPacket.
       * @param[in] theType to be assigned to this INetPacket
       */
      void SetType(const Uint16 theType);

      /**
       * GetFlag is responsible for returning the flag specified from this
       * INetPacket. The result returned is a true or false for the flag bit
       * specified as opposed to just returning all flag bits (see GetFlags).
       * @return true if flag bit specified is 1, false otherwise
       */
      bool GetFlag(const NetFlag theFlag) const;

      /**
       * GetFlags is responsible for returning the flags assigned to this
       * INetPacket.
       * @return the flags assigned to this INetPacket
       */
      GQE::Uint32 GetFlags(void) const;

      /**
       * SetFlag is responsible for setting the flag specified to the value
       * specified.
       * @param[in] theFlag bit to set for this INetPacket
       * @param[in] theValue for the bit to set (true=1, false=0)
       */
      void SetFlag(const NetFlag theFlag, const bool theValue = true);

      /**
       * GetHostID is responsible for returning the source HostID that
       * has been assigned to this INetPacket.
       * @return theHostID assigned to this packet
       */
      GQE::Uint32 GetHostID(void) const;

      /**
       * SetHostID is responsible for setting the source HostID that will be
       * used for this INetPacket. If the source HostID is not known at the
       * time this INetPacket is completed then a 0 will be used instead.
       * @param[in] theHostID source to be used for this INetPacket
       */
      void SetHostID(const Uint32 theHostID);

      /**
       * GetSequenceNumber is responsible for returning the sequence number
       * that has been assigned to this INetPacket.
       * @return theSequenceNumber assigned to this packet
       */
      GQE::Uint32 GetSequenceNumber(void) const;

      /**
       * SetSequenceNumber is responsible for setting the sequence number that
       * will be used for this INetPacket. This helps with ordering NetPackets
       * that arrive out of order like UDP NetPackets and can be used for other
       * purposes for TCP NetPackets.
       * @param[in] theSequenceNumber to be used for this INetPacket
       */
      void SetSequenceNumber(const Uint32 theSequenceNumber);

      /**
       * GetData is responsible for returning the address to the array of
       * characters that is the network packet data payload (header included)
       * for transmitting or other processing.
       */
      void* GetData(void);

      /**
       * GetPayload is responsible for returning the address to the array of
       * characters that is the network packet data payload (without the
       * header) for other processing.
       */
      void* GetPayload(void);

      /**
       * GetSize is responsible for returning the size in bytes of the array of
       * characters that is the network packet data payload (header included).
       * @return the size in bytes of the network data payload
       */
      std::size_t GetSize(void) const;

      /**
       * GetCapacity is responsible for returning the size in bytes of the
       * capacity of the array of characters that is the network packet data
       * payload (header included). This starts with the minimum bytes necessary
       * to store the header and nothing more. This can be changed using the
       * SetCapacity method to create storage for receiving incoming data or
       * at construction time.
       * @return the capacity in bytes of the network data payload.
       */
      std::size_t GetCapacity(void) const;

      /**
       * SetCapacity is responsible for dynamically increasing the capacity in
       * bytes of the array of characters that is the network packet data
       * payload (header included). This is typically done at construction time
       * since this is more efficient, but this method is provided for other
       * circumstances where this might be needed.
       * @param[in] theCapacity in bytes to expand the array
       */
      void SetCapacity(std::size_t theCapacity);

      /**
       * GetFirstSent is responsible for returning the time this INetPacket
       * was first sent which can be used to determine when to stop resending
       * this INetPacket.
       * @return the time this INetPacket was last sent.
       */
#if (SFML_VERSION_MAJOR < 2)
      double GetFirstSent(void) const;
#else
      sf::Time GetFirstSent(void) const;
#endif

      /**
       * SetFirstSent is responsible for resetting the first sent clock which
       * can be used to determine when to stop resending the INetPacket.
       */
      void SetFirstSent(void);

      /**
       * GetLastSent is responsible for returning the last time this INetPacket
       * was sent which can be used to determine when to resend this INetPacket.
       * @return the time this INetPacket was last sent.
       */
#if (SFML_VERSION_MAJOR < 2)
      double GetLastSent(void) const;
#else
      sf::Time GetLastSent(void) const;
#endif

      /**
       * SetLastSent is responsible for resetting the last sent clock which can
       * be used to determine which INetPacket needs to be resent.
       */
      void SetLastSent(void);

      /**
       * GetTimestamp is responsible for returning the timestamp of when this
       * INetPacket was received. The timestamp is in microseconds and has been
       * adjusted to match the time of the remote side so each side can refer to
       * a similar game clock for time sensitive calculations.
       */
      Int64 GetTimestamp(void) const;

      /**
       * SetTimestamp is responsible for setting the timestamp of when this
       * INetPacket was received. The timestamp is in microseconds and has been
       * adjusted to match the time of the remote side so each side can refer to
       * a similar game clock.
       */
      void SetTimestamp(const Int64 theTimestamp);

      /**
       * GetSortType will return the correctly selected sort type for this
       * INetPackets <,>,<=,etc operators. This might change as this INetPacket
       * moves from one priority queue to another priority queue.
       * @return the SortType previously selected
       */
      SortType GetSortType(void) const;

      /**
       * SetSortType will set theSortType specified for this INetPacket which
       * affects the <,>,<=,etc operators. This might change as the INetPacket
       * moves from one priority queue to another priority queue.
       * @param[in] theSortType to use for the various INetPacket operators
       */
      void SetSortType(const SortType theSortType);

      /**
       * LogHeader is responsible for logging the header information using the
       * ILOG macro to assist with debugging efforts.
       */
      void LogHeader(void);

      /**
       * LogPayload is responsible for logging the payload information using
       * the ILOG macro to assist with debugging efforts.
       */
      void LogPayload(void);

      /**
       * operator BoolType is responsible for providing an indication that the
       * end of the network packet has been reached when extracting data from
       * the network packet.
       * @return true if network data extraction was successful, false otherwise
       */
      operator BoolType() const;

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(bool& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Int8& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Uint8& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Int16& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Uint16& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Int32& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Uint32& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Int64& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(Uint64& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(float& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(double& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(char* theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(std::string& theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(wchar_t* theValue);

      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(std::wstring& theValue);

#if (SFML_VERSION_MAJOR < 2)
      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(sf::Unicode::Text& theValue);
#else
      /**
       * operator>> to read data from the INetPacket into specific data types
       * similar to sf::Packet.
       * @param[out] theValue to put retrieved data into
       */
      INetPacket& operator>>(sf::String& theValue);
#endif

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(bool theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Int8 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Uint8 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Int16 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Uint16 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Int32 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Uint32 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Int64 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(Uint64 theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(float theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(double theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const char* theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const std::string& theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const wchar_t* theValue);

      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const std::wstring& theValue);

#if (SFML_VERSION_MAJOR < 2)
      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const sf::Unicode::Text& theValue);
#else
      /**
       * operator<< to write data into the INetPacket from specific data types
       * similar to sf::Packet.
       * @param[in] theValue to put into the INetPacket
       */
      INetPacket& operator<<(const sf::String& theValue);
#endif

    protected:
      // Constants
      ///////////////////////////////////////////////////////////////////////////
      /// Version byte that is empty
      static const Uint8 VERSION_NONE = 0x00;
      /// Version byte to use for all NetPackets (to be incremented when INetPacket changes)
      static const Uint8 VERSION_BYTE = 0x10; // 1.0 using 4 bits for Major and Minor values
      /// Array position for sync in header in bytes
      static const std::size_t SYNC_POSITION_B = 0;
      /// Array position for version in header in bytes
      static const std::size_t VERSION_POSITION_B = 1;
      /// Array position for message type in header in bytes
      static const std::size_t TYPE_POSITION_B = 2;
      /// Array position for flags in header in bytes
      static const std::size_t FLAG_POSITION_B = 4;
      /// Array position for source hostID in header in bytes
      static const std::size_t HOSTID_POSITION_B = 8;
      /// Array position for sequence number in header in bytes
      static const std::size_t SEQUENCE_POSITION_B = 12;

      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// The sync to use for validating network packets
      Uint8  mSync;
      /// Boolean value indicating data is valid
      bool mValid;

    private:
      // Variables
      ///////////////////////////////////////////////////////////////////////////
      /// Sort technique to use for the <,>,<=,etc operators
      SortType mSortType;
      /// Minimum size to use when clearing the packet
      std::size_t mMinimum;
      /// Vector holding the payload of each INetPacket
      std::vector<char> mData;
      /// Read position for payload data being extracted from the INetPacket
      std::size_t mReadPosition;
      /// Clock to keep track of when INetPacket was first sent
      sf::Clock mFirstSent;
      /// Clock to keep track of when INetPacket was last sent
      sf::Clock mLastSent;
      /// Offset adjusted receive timestamp
      Int64 mTimestamp;

      /**
       * Append is responsible for appending theData provided to the network
       * packet payload. This method is heavily used by the operator<< methods
       * to put data into the network packet.
       * @param[in] theData to be put into the network packet
       * @param[in] theSize of the data in bytes to put into the network packet
       */
      void Append(const void* theData, std::size_t theSize);

      /**
       * CheckSize is responsible for determining if it is possible to extract
       * theSize in bytes from the network packet at the current position in
       * the network packet. This method is heavily used by the operator>>
       * methods that extract data from the network packet.
       * @param[in] theSize to be extracted in bytes from this network packet
       * @return true if theSize bytes are available, false otherwise
       */
      bool CheckSize(std::size_t theSize);
  }; // INetPacket class

  /**
   * operator== as a non-member method for comparing two INetPacket objects to
   * each other. Only the Type, Flags, HostID, and Sequence Number fields will
   * be compared to each other to prove equality. All other fields are assumed
   * to be the same.
   * @return true if above fields match, false otherwise
   */
  inline bool operator==(const INetPacket& theLeft, const INetPacket& theRight)
  {
    // Start with a comparison of the Sequence Number fields
    bool anResult = theLeft.GetSequenceNumber() == theRight.GetSequenceNumber();

    // Compare the other fields next
    anResult = anResult && theLeft.GetType() == theRight.GetType();
    anResult = anResult && theLeft.GetHostID() == theRight.GetHostID();
    anResult = anResult && theLeft.GetFlags() == theRight.GetFlags();

    // Return anResult produced above
    return anResult;
  }

  /**
   * operator!= as a non-member method for comparing two INetPacket objects to
   * each other. Only the Type, Flags, HostID, and Sequence Number fields will
   * be compared to each other to prove equality. All other fields are assumed
   * to be the same.
   * @return true if above fields don't match, false otherwise
   */
  inline bool operator!=(const INetPacket& theLeft, const INetPacket& theRight)
  {
    return !operator==(theLeft,theRight);
  }

  /**
   * operator< as a non-member method for comparing two INetPacket objects to
   * each other. Only the Sequence Number fields are compared to each other for
   * this comparison.
   * @return true if left sequence number is less than right sequence number
   */
  inline bool operator< (const INetPacket& theLeft, const INetPacket& theRight)
  {
    // Default to false if the two INetPacket types don't agree on a sort type
    bool anResult = false;

    // Both sides must share the same sort type
    if(theLeft.GetSortType() == theRight.GetSortType())
    {
      // Which sort type are we using to sort these INetPackets?
      switch(theLeft.GetSortType())
      {
        // Default to SequenceNumber if SortType is unknown
        default:
          WLOG() << "INetPacket::operator<() unknown sort type("
                 << theLeft.GetSortType() << ")" << std::endl;
        case INetPacket::SortSequenceNumber:
          anResult = theLeft.GetSequenceNumber() < theRight.GetSequenceNumber();
          break;
        case INetPacket::SortLastSent:
#if (SFML_VERSION_MAJOR < 2)
          anResult = theLeft.GetLastSent() < theRight.GetLastSent();
#else
          anResult = theLeft.GetLastSent().asSeconds() < theRight.GetLastSent().asSeconds();
#endif
          break;
        case INetPacket::SortTimestamp:
          anResult = theLeft.GetTimestamp() < theRight.GetTimestamp();
          break;
      }
    }
    else
    {
      WLOG() << "INetPacket::operator<() mismatched sort type("
             << (Uint32)theLeft.GetSortType() << "!="
             << (Uint32)theRight.GetSortType() << ")" << std::endl;
    }

    return anResult;
  }

  /**
   * operator> as a non-member method for comparing two INetPacket objects to
   * each other. Only the Sequence Number fields are compared to each other for
   * this comparison.
   * @return true if right sequence number is less than left sequence number
   */
  inline bool operator> (const INetPacket& theLeft, const INetPacket& theRight)
  {
    return  operator< (theRight,theLeft);
  }

  /**
   * operator<= as a non-member method for comparing two INetPacket objects to
   * each other. Only the Sequence Number fields are compared to each other for
   * this comparison.
   * @return true if left sequence number is less than or equal to right sequence number
   */
  inline bool operator<=(const INetPacket& theLeft, const INetPacket& theRight)
  {
    return !operator> (theLeft,theRight);
  }

  /**
   * operator<= as a non-member method for comparing two INetPacket objects to
   * each other. Only the Sequence Number fields are compared to each other for
   * this comparison.
   * @return true if right sequence number is less than or equal to left sequence number
   */
  inline bool operator>=(const INetPacket& theLeft, const INetPacket& theRight)
  {
    return !operator< (theLeft,theRight);
  }
} // namespace GQE
#endif // INET_PACKET_HPP_INCLUDED

/**
 * @class GQE::INetPacket
 * @ingroup Core
 * The INetPacket class is the base class for all network packets used in
 * GQE. The INetPacket class provides a simple header that consists of an 8
 * byte sync code (to validate each message), 8 bits of on/off flags, message
 * type, and source HostID for identification and verification purposes.
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
