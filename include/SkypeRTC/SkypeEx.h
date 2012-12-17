#pragma once

#include <vector>
#include <string>
#include <exception>
#include <stdint.h>

#include "skype-embedded_2.h"

#include "AccountEx.h"
#include "ContactGroupEx.h"
#include "ParticipantEx.h"
#include "ConversationEx.h"
#include "VideoEx.h"

#include <VideoTransportClient.hpp>


#ifdef WIN32
#include <WinShm.hpp>
#else
#include <PosixShm.hpp>
#endif

#include "SkypeImage.h"


template <class T>
class SkypeRingBuffer {
 private:
  T** m_ppBuffer;
  uint32_t* m_pSizes;
  uint32_t m_BufferSize;
  uint32_t m_BufferLast;
  uint32_t m_BufferTop;

 public:
 SkypeRingBuffer(uint32_t size = 8): m_BufferLast(0), m_BufferTop(0) {
    m_ppBuffer = new T*[size];
    m_pSizes = new uint32_t[size];
    m_BufferSize = size;
    for(uint32_t i = 0;i < m_BufferSize;i++) {
      m_ppBuffer[i] = NULL;
    }
  }


  virtual ~SkypeRingBuffer() {
    for(uint32_t i = 0;i < m_BufferSize;i++) {
      delete m_ppBuffer[i];
    }
    delete m_ppBuffer;
  }

  uint32_t getSize() {
    uint32_t size = m_BufferTop - m_BufferLast;
    if(size < 0) {
      size += m_BufferSize;
    }
    return size;
  }

  bool pop_front(T* data, uint32_t bufSize, uint32_t *size) {
    if(getSize() > 0) {
      if(bufSize < m_pSizes[m_BufferLast]) {
	return false;
      }
      memcpy((void*)data, m_ppBuffer[m_BufferLast], sizeof(T)*m_pSizes[m_BufferLast]);
      *size = m_pSizes[m_BufferLast];
      m_BufferLast++;
      return true;
    } 
    return false;
  }

  bool push_back(const T* data, const uint32_t size) {
    delete m_ppBuffer[m_BufferTop];
    m_ppBuffer[m_BufferTop] = new T[size];
    memcpy((void*)m_ppBuffer[m_BufferTop], data, sizeof(T)*size);
    m_pSizes[m_BufferTop] = size;
    m_BufferTop++;
    if(m_BufferTop >= m_BufferSize) {m_BufferTop = 0;}
    if(m_BufferTop == m_BufferLast) {
      return false;
    }

    return true;
  }

};


static SEString StreamListType(const Skype::APP2APP_STREAMS type)
{
  switch (type)
    {
    case Skype::ALL_STREAMS:
      return "all streams";
      break;

    case Skype::SENDING_STREAMS:
      return "sending stream";
      break;

    case Skype::RECEIVED_STREAMS:
      return "receiving stream";
      break;

    default:
      return "unknown stream type";
      break;
    };
};

/**
 *
 */
class SkypeEx : public Skype {
 private:
#ifdef WIN32
  VideoTransportClient <WinShm> m_PreviewClient;
  VideoTransportClient <WinShm> m_IncomingClient;
#else
  VideoTransportClient <PosixShm> m_PreviewClient;
  VideoTransportClient <PosixShm> m_IncomingClient;
#endif

  AccountEx::Ref m_Account;
  ContactGroupEx::Ref m_ContactGroup;
  ContactRefs m_Contacts;
  ConversationEx::Ref m_LiveSession;
  ConversationEx::Refs m_LiveSessionList;
  VideoEx::Ref m_PreviewVideo;
  VideoEx::Ref m_IncomingVideo;
 private:
  std::string m_KeyString;
  std::string m_AutoTakeUserName;
  bool m_AutoTakeCall;

  bool m_LiveSessionUp;

  bool m_VideoCapable;
  std::string m_VideoDeviceName;
  std::string m_VideoDeviceId;
  std::string m_StreamName;
  
  bool m_StreamConnected;
  std::string m_AppName;
  SkypeRingBuffer<uint8_t> m_RingBuffer;
 private:
  SkypeImage m_PreviewBuffer;
  SkypeImage m_IncomingBuffer;

 public:
  SkypeImage* getPreviewFrame() { return &m_PreviewBuffer; }
  SkypeImage* getIncomingFrame() { return &m_IncomingBuffer; }

 public:

  SkypeEx();
  ~SkypeEx();

 public:

 public:
  SEString getSkypename() {
    SEString name;
    m_Account->GetPropSkypename(name);
    return name;
  }
    
  std::string getAutoTakeUserName() {
    return m_AutoTakeUserName;
  }

  bool isStreamConnected() {return m_StreamConnected;}

  void setAutoTakeUserName(const std::string userName) {
    m_AutoTakeUserName = userName;
    setAutoTakeCall(true);
  }

  void setAutoTakeVideo(VideoEx::Ref videoRef) {
    m_IncomingVideo = videoRef;
    m_IncomingVideo->Start();
    m_IncomingVideo->SetRemoteRendererId(m_IncomingClient.key());
  }

  void setAutoTakeCall(const bool flag) {
    m_AutoTakeCall = flag;
  }

  bool isAutoTakeCall() {
    return m_AutoTakeCall;
  }
  
  bool updatePreviewFrame();

  bool isVideoCapable() {return m_VideoCapable;}
  bool isLiveSessionUp() { return m_LiveSessionUp; }

  void setLiveSession(const ConversationRef& conversation) {
    m_LiveSession = conversation;
    m_LiveSessionUp = true;
  }

  void clearLiveSession() {
    m_LiveSessionUp = false;
    m_LiveSession = NULL;
  }

  void lock() {}
  void unlock() {}

  void loadKeyPair(const char* fileName);
  void login(const std::string& skypeName, const std::string& passwd);

  void logout();

  bool callTarget(const std::string& target);

  std::vector<std::string> getContacts();
  
  bool startPreview();

  //  void getPreviewFrame();

 public:
  void onContactGroupChange(ContactGroupEx& contactGroup);

  bool App2AppStreamConnect(std::string &AppName, std::string& buddyName);

  void OnApp2AppStreamListChange(const Sid::String &appname, 
				 const APP2APP_STREAMS &listType, 
				 const Sid::List_String &streams, 
				 const Sid::List_uint &receivedSizes);

  void OnApp2AppDatagram  (const Sid::String &appname, 
			   const Sid::String &stream, 
			   const Sid::Binary &data);

  bool isApp2AppDatagramPacketReceived() {
    if(m_RingBuffer.getSize() > 0) {
      return true;
    }
  }

  uint32_t writeApp2AppDatagram(uint8_t* buffer, uint32_t size) {
    if(isStreamConnected()) {
      SEBinary dataBuffer;
      dataBuffer.set(buffer, size);
      bool result;
      App2AppDatagram(SEString(m_AppName.c_str()), SEString(m_StreamName.c_str()), dataBuffer, result);
      if(result) {
	return size;
      }
    }
    return -1;
  }
  
  uint32_t readApp2AppDatagram(uint8_t* buffer, uint32_t bufSize) {
    uint32_t actualSize;
    if(m_RingBuffer.pop_front(buffer, bufSize, &actualSize)) {
      return actualSize;
    }
    return -1;
  }
    
  
 public:
  void OnConversationListChange(const ConversationRef &conversation, const Conversation::LIST_TYPE &type, const bool &added);

  void OnAvailableVideoDeviceListChange();
 public: 

  Account* newAccount(int oid){ return new AccountEx(oid, this);      }

  ContactGroup* newContactGroup(int oid) { return new ContactGroupEx(oid, this); }

  Participant* newParticipant(int oid) {return new ParticipantEx(oid, this); }

  Conversation* newConversation(int oid) {return new ConversationEx(oid, this); }

  Video* newVideo(int oid) {return new VideoEx(oid, this);}

 public:
  void test();
};

