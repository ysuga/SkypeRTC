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



/**
 *
 */
class SkypeEx : public Skype::Skype {
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
 private:
  std::string m_KeyString;
  std::string m_AutoTakeUserName;
  bool m_AutoTakeCall;

  bool m_LiveSessionUp;

  bool m_VideoCapable;
  std::string m_VideoDeviceName;
  std::string m_VideoDeviceId;


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

  void setAutoTakeUserName(const std::string userName) {
    m_AutoTakeUserName = userName;
    setAutoTakeCall(true);
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

  std::vector<std::string> getContacts();
  
  bool startPreview();

  //  void getPreviewFrame();

 public:
  void onContactGroupChange(ContactGroupEx& contactGroup);

  void OnApp2AppStreamListChange(const Sid::String &appname, 
				 const APP2APP_STREAMS &listType, 
				 const Sid::List_String &streams, 
				 const Sid::List_uint &receivedSizes);

  void OnApp2AppDatagram  (const Sid::String &appname, 
			   const Sid::String &stream, 
			   const Sid::Binary &data);
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

