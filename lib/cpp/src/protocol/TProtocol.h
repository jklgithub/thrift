#ifndef T_PROTOCOL_H
#define T_PROTOCOL_H

#include <transport/TTransport.h>

#include <boost/shared_ptr.hpp>

#include <netinet/in.h>
#include <sys/types.h>
#include <string>
#include <map>

namespace facebook { namespace thrift { namespace protocol { 

using namespace boost;

using namespace facebook::thrift::transport;

#define ntohll(x) (((uint64_t)(ntohl((int)((x << 32) >> 32))) << 32) | (uint32_t)ntohl(((int)(x >> 32))))

#define htonll(x) ntohll(x)

/** Forward declaration for TProtocol */
struct TBuf;

/**
 * Enumerated definition of the types that the Thrift protocol supports.
 * Take special note of the T_END type which is used specifically to mark
 * the end of a sequence of fields.
 */
enum TType {
  T_STOP       = 0,
  T_VOID       = 1,
  T_BOOL       = 2,
  T_BYTE       = 3,
  T_I08        = 3,
  T_I16        = 6,
  T_I32        = 8,
  T_U64        = 9,
  T_I64        = 10,
  T_STRING     = 11,
  T_UTF7       = 11,
  T_STRUCT     = 12,
  T_MAP        = 13,
  T_SET        = 14,
  T_LIST       = 15,
  T_UTF8       = 16,
  T_UTF16      = 17
};

/**
 * Enumerated definition of the message types that the Thrift protocol supports.
 */
enum TMessageType {
  T_CALL       = 1,
  T_REPLY      = 2
};

/**
 * Abstract class for a thrift protocol driver. These are all the methods that
 * a protocol must implement. Essentially, there must be some way of reading
 * and writing all the base types, plus a mechanism for writing out structs
 * with indexed fields. Also notice that all methods are strictly const. This
 * is by design. Protcol impelementations may NOT keep state, because the
 * same TProtocol object may be used simultaneously by multiple threads. This
 * theoretically introduces some limititations into the possible protocol
 * formats, but with the benefit of performance, clarity, and simplicity.
 *
 * @author Mark Slee <mcslee@facebook.com>
 */
class TProtocol {
 public:
  virtual ~TProtocol() {}

  /**
   * Writing functions.
   */

  virtual uint32_t writeMessageBegin(shared_ptr<TTransport> out,
				     const std::string name,
				     const TMessageType messageType,
				     const int32_t seqid) const = 0;

  virtual uint32_t writeMessageEnd(shared_ptr<TTransport> out) const = 0;


  virtual uint32_t writeStructBegin(shared_ptr<TTransport> out,
				    const std::string& name) const = 0;
  
  virtual uint32_t writeStructEnd(shared_ptr<TTransport> out) const = 0;
  
  virtual uint32_t writeFieldBegin(shared_ptr<TTransport> out,
				   const std::string& name,
				   const TType fieldType,
				   const int16_t fieldId) const = 0;

  virtual uint32_t writeFieldEnd(shared_ptr<TTransport> out) const = 0;

  virtual uint32_t writeFieldStop(shared_ptr<TTransport> out) const = 0;
                                      
  virtual uint32_t writeMapBegin(shared_ptr<TTransport> out,
				 const TType keyType,
				 const TType valType,
				 const uint32_t size) const = 0;

  virtual uint32_t writeMapEnd(shared_ptr<TTransport> out) const = 0;
  
  virtual uint32_t writeListBegin(shared_ptr<TTransport> out,
				  const TType elemType,
				  const uint32_t size) const = 0;

  virtual uint32_t writeListEnd(shared_ptr<TTransport> out) const = 0;

  virtual uint32_t writeSetBegin(shared_ptr<TTransport> out,
				 const TType elemType,
				 const uint32_t size) const = 0;

  virtual uint32_t writeSetEnd(shared_ptr<TTransport> out) const = 0;

  virtual uint32_t writeBool(shared_ptr<TTransport> out,
			     const bool value) const = 0;

  virtual uint32_t writeByte(shared_ptr<TTransport> out,
			     const int8_t byte) const = 0;

  virtual uint32_t writeI16(shared_ptr<TTransport> out,
			    const int16_t i16) const = 0;

  virtual uint32_t writeI32(shared_ptr<TTransport> out,
			    const int32_t i32) const = 0;

  virtual uint32_t writeI64(shared_ptr<TTransport> out,
			    const int64_t i64) const = 0;

  virtual uint32_t writeString(shared_ptr<TTransport> out,
			       const std::string& str) const = 0;

  /**
   * Reading functions
   */

  virtual uint32_t readMessageBegin(shared_ptr<TTransport> in,
				    std::string& name,
				    TMessageType& messageType,
				    int32_t& seqid) const = 0;
  
  virtual uint32_t readMessageEnd(shared_ptr<TTransport> in) const = 0;

  virtual uint32_t readStructBegin(shared_ptr<TTransport> in,
				   std::string& name) const = 0;

  virtual uint32_t readStructEnd(shared_ptr<TTransport> in) const = 0;

  virtual uint32_t readFieldBegin(shared_ptr<TTransport> in,
				  std::string& name,
				  TType& fieldType,
				  int16_t& fieldId) const = 0;
  
  virtual uint32_t readFieldEnd(shared_ptr<TTransport> in) const = 0;
 
  virtual uint32_t readMapBegin(shared_ptr<TTransport> in,
				TType& keyType,
				TType& valType,
				uint32_t& size) const = 0;

  virtual uint32_t readMapEnd(shared_ptr<TTransport> in) const = 0;

  virtual uint32_t readListBegin(shared_ptr<TTransport> in,
				 TType& elemType,
				 uint32_t& size) const = 0;

  virtual uint32_t readListEnd(shared_ptr<TTransport> in) const = 0;

  virtual uint32_t readSetBegin(shared_ptr<TTransport> in,
				TType& elemType,
				uint32_t& size) const = 0;

  virtual uint32_t readSetEnd(shared_ptr<TTransport> in) const = 0;

  virtual uint32_t readBool(shared_ptr<TTransport> in,
			    bool& value) const = 0;

  virtual uint32_t readByte(shared_ptr<TTransport> in,
			    int8_t& byte) const = 0;

  virtual uint32_t readI16(shared_ptr<TTransport> in,
			   int16_t& i16) const = 0;

  virtual uint32_t readI32(shared_ptr<TTransport> in,
			   int32_t& i32) const = 0;

  virtual uint32_t readI64(shared_ptr<TTransport> in,
			   int64_t& i64) const = 0;

  virtual uint32_t readString(shared_ptr<TTransport> in,
			      std::string& str) const = 0;

  /**
   * Method to arbitrarily skip over data.
   */
  uint32_t skip(shared_ptr<TTransport> in, TType type) const {
    switch (type) {
    case T_BOOL:
      {
        bool boolv;
        return readBool(in, boolv);
      }
    case T_BYTE:
      {
        int8_t bytev;
        return readByte(in, bytev);
      }
    case T_I16:
      {
        int16_t i16;
        return readI16(in, i16);
      }
    case T_I32:
      {
        int32_t i32;
        return readI32(in, i32);
      }
    case T_I64:
      {
        int64_t i64;
        return readI64(in, i64);
      }
    case T_STRING:
      {
        std::string str;
        return readString(in, str);
      }
    case T_STRUCT:
      {
        uint32_t result = 0;
        std::string name;
        int16_t fid;
        TType ftype;
        result += readStructBegin(in, name);
        while (true) {
          result += readFieldBegin(in, name, ftype, fid);
          if (ftype == T_STOP) {
            break;
          }
          result += skip(in, ftype);
          result += readFieldEnd(in);
        }
        result += readStructEnd(in);
        return result;
      }
    case T_MAP:
      {
        uint32_t result = 0;
        TType keyType;
        TType valType;
        uint32_t i, size;
        result += readMapBegin(in, keyType, valType, size);
        for (i = 0; i < size; i++) {
          result += skip(in, keyType);
          result += skip(in, valType);
        }
        result += readMapEnd(in);
        return result;
      }
    case T_SET:
      {
        uint32_t result = 0;
        TType elemType;
        uint32_t i, size;
        result += readSetBegin(in, elemType, size);
        for (i = 0; i < size; i++) {
          result += skip(in, elemType);
        }
        result += readSetEnd(in);
        return result;
      }
    case T_LIST:
      {
        uint32_t result = 0;
        TType elemType;
        uint32_t i, size;
        result += readListBegin(in, elemType, size);
        for (i = 0; i < size; i++) {
          result += skip(in, elemType);
        }
        result += readListEnd(in);
        return result;
      }
    default:
      return 0;
    }
  }

 protected:
  TProtocol() {}
};

}}} // facebook::thrift::protocol

#endif

