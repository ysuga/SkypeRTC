#include <iostream>
#include "SkypeEx.h"



#define SID_MAKEFOURCC(ch0, ch1, ch2, ch3) \
  ( (unsigned long)(unsigned char)(ch0)         | ( (unsigned long)(unsigned char)(ch1) << 8 ) | \
    ( (unsigned long)(unsigned char)(ch2) << 16 ) | ( (unsigned long)(unsigned char)(ch3) << 24 ) )


SkypeEx::SkypeEx() :
  m_AutoTakeCall(false), m_LiveSessionUp(false), m_VideoCapable(false)
{
  unsigned int list[2];
  list[0] = SID_MAKEFOURCC('B','I','3','2');
  list[1] = SID_MAKEFOURCC('B','I','2','4');
  m_PreviewClient.setPreferences( 2, list );
  m_IncomingClient.setPreferences( 2, list );
}

SkypeEx::~SkypeEx() {
}

void SkypeEx::loadKeyPair(const char* fileName) {
  std::cout << "load key pair(" << fileName << ")" << std::endl;
  FILE* f = 0;
  size_t fsize = 0;

  f = fopen(fileName, "r");
  if (f != 0) {
    int keyLen;
    char* keyBuf;
    
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    rewind(f);
    keyLen = fsize + 1;
    keyBuf = new char[keyLen];
    size_t read = fread(keyBuf,1,fsize,f);
    if (read != fsize) {
      printf("Error reading %s\n", fileName);
      return ;
    };
    keyBuf[fsize]=0; //cert should be null terminated                                                                                                                    
    fclose(f);
    m_KeyString = keyBuf;
    delete keyBuf;
    std::cout << "Key String Set OK." << std::endl;
    return ;
  };
  printf("Error opening app token file: %s\n", fileName);
  return ;
};

void SkypeEx::login(const std::string& skypeName, const std::string& passwd)
{
  init(m_KeyString.c_str(), "127.0.0.1", 8963, "streamlog.txt");
  start();
  std::cout << "Now Logging in...." << std::endl;;
  if(GetAccount(skypeName.c_str(), m_Account)) {
    m_Account->LoginWithPassword(passwd.c_str(), false, false);

    while(!m_Account->isLoggedIn()) ;
    return;
  }
  std::cout << "Failed." << std::endl;
  //  throw LoginException();
}

void SkypeEx::logout()
{
  m_Account->Logout(false);
  while(m_Account->isLoggedIn()) ; 
  stop();
}

void SkypeEx::onContactGroupChange(ContactGroupEx& contactGroup)
{
  contactGroup.GetContacts(m_Contacts);
  for(uint i = 0;i < m_Contacts.size();i++) {
    SEString contactName;
    Contact::TYPE contactType;
    
    m_Contacts[i]->GetPropType(contactType);
    if(contactType == Contact::SKYPE) {
      m_Contacts[i]->GetPropSkypename(contactName);
    } else {
      m_Contacts[i]->GetPropPstnnumber(contactName);
    }
    std::cout << "Buddy[" << i << "] = " << contactName << std::endl;
  }
}

std::vector<std::string> SkypeEx::getContacts() {
  ContactGroupEx::Ref contactGroup;
  this->GetHardwiredContactGroup(ContactGroup::ALL_KNOWN_CONTACTS, contactGroup);
  contactGroup->GetContacts(m_Contacts);  
  std::vector<std::string> contactNameList;
  for(uint i = 0;i < m_Contacts.size();i++) {
    SEString contactName;
    Contact::TYPE contactType;
    
    m_Contacts[i]->GetPropType(contactType);
    if(contactType == Contact::SKYPE) {
      m_Contacts[i]->GetPropSkypename(contactName);
    } else {
      m_Contacts[i]->GetPropPstnnumber(contactName);
    }
    std::cout << "Buddy[" << i << "] = " << contactName << std::endl;
    contactNameList.push_back((const char*)contactName);
  }
  return contactNameList;
}  

void SkypeEx::OnConversationListChange(const ConversationRef &conversation, const Conversation::LIST_TYPE &type, const bool &added)
{
  std::cout << __FILE__ << ":OnConversationListChange" << std::endl;
  // Keeping inbox updated..
  if ( (type == Conversation::INBOX_CONVERSATIONS) && (added) ) {
    this->GetConversationList(m_LiveSessionList, Conversation::INBOX_CONVERSATIONS);
    fetch(m_LiveSessionList);
  };

  if (type == Conversation::LIVE_CONVERSATIONS) {
    // Live call coming up
    if (added) {
      SEString whosCalling;
      //  conversation->GetPropDisplayname(whosCalling);
  conversation->GetPropIdentity(whosCalling);

#ifdef DEBUG
      std::cout << "Calling From " << (const char*)whosCalling << std::endl;
#endif
      if (isLiveSessionUp()) {
	//	QString msg = "Another call from. ";
	//	msg.append( (const char*)whosCalling );
	//	msg.append(" As we are already in a call, we will ignore it.");
	//	dispatcher->log(msg);
      } else {
	std::cout << "Start Live Session" << std::endl;
	setLiveSession(conversation);// = true;
	if(isAutoTakeCall()) {
	  if(std::string((const char*)whosCalling) == getAutoTakeUserName()) {
	    conversation->JoinLiveSession();
	    std::cout << "Set Live Session" << std::endl;
	  }
	} else {
	  std::cout << "Not Auto Call Take" << std::endl;
	}
	//	liveSession = conversation;
	//	QString msg = "Going live with ";
	//	msg.append( (const char*)whosCalling );
	//	dispatcher->log(msg);
	//	conversation->JoinLiveSession();
	//dispatcher->SendToggleCallBtnDropsCallSignal(true);
      };
    } else {    // Live call ended
      //      dispatcher->log("Call finished.");
      clearLiveSession();
      //      skype->liveSession = NULL;
      //      dispatcher->SendToggleCallBtnDropsCallSignal(false);
    };
  };
}

void SkypeEx::OnAvailableVideoDeviceListChange() {
  SEStringList deviceNames;
  SEStringList deviceIds;
  uint deviceCount;
  std::cout << __FUNCTION__ << " called." << std::endl;
  this->GetAvailableVideoDevices(deviceNames, deviceIds, deviceCount);
  if (deviceCount > 0) {
    //dispatcher->log("Video device list retrieved.");
    
    this->m_VideoDeviceName = deviceNames[0];
    this->m_VideoDeviceId = deviceIds[0];
    
    //    QString msg = "Picking ";
    //    msg.append( (const char*)videoDeviceName );
    //    dispatcher->log(msg);
    this->m_VideoCapable = true;
  } else {
    this->m_VideoCapable = false;
    //    dispatcher->log("ERROR: no video devices found.");
  };
};

bool SkypeEx::startPreview()
{
  if(m_VideoCapable) {
#ifdef DEBUG
    std::cout << __FILE__ ":Start Preview() Video Capable("<< m_VideoDeviceName <<")." << std::endl;
#endif
    if(GetPreviewVideo(Video::MEDIA_VIDEO, m_PreviewVideo, m_VideoDeviceName.c_str(), m_VideoDeviceId.c_str())) {
#ifdef DEBUG
      std::cout << __FILE__ ":Preview OK" << std::endl;
#endif
      //      m_PreviewVideo->localPreview = true;
      m_PreviewVideo->setLocalPreview(true);
      m_PreviewVideo->Start();
      m_PreviewVideo->SetRemoteRendererId(m_PreviewClient.key());
    } else {
      std::cout << __FILE__ ":Preview Failed." << std::endl;
    }

    return true;
  } 
#ifdef DEBUG
  std::cout << __FILE__ ":Start Preview() Video Uncapable" << std::endl;
#endif
  return false;
}

bool SkypeEx::updatePreviewFrame()
{
  VideoTransportBase::bufferstruct *buffer = m_PreviewClient.getNewFrame();

  if(!buffer) {return false;}
  int bufferFormat;
  int bufferWidth = buffer->width;
  int bufferHeight = buffer->height;
  if      ( buffer->fourcc == SID_MAKEFOURCC('B','I','3','2') ) {
    bufferFormat = SkypeImage::COLOR_ARGB8888;
    std::cout << "ColorModel ARGB8888" << std::endl;
  } else if ( buffer->fourcc == SID_MAKEFOURCC('B','I','2','4') ) {
    bufferFormat = SkypeImage::COLOR_RGB888;
    std::cout << "ColorModel RGB888" << std::endl;
  } else if ( buffer->fourcc == 0  ) {
    if ( buffer->bitsperpixel == 32 ) {
      bufferFormat = SkypeImage::COLOR_ARGB8888;
      std::cout << "ColorModel ARGB8888" << std::endl;
    } else if ( buffer->bitsperpixel == 24 ) {
      bufferFormat = SkypeImage::COLOR_RGB888;
      std::cout << "ColorModel RGB888" << std::endl;
    } else if ( buffer->bitsperpixel == 16 ) {
      std::cout << "Unavaiable format 16" << std::endl;
    }
  }
  
  if(bufferFormat != m_PreviewBuffer.getColorModel() ||
     bufferWidth  != m_PreviewBuffer.getWidth() ||
     bufferHeight != m_PreviewBuffer.getHeight() ) {
    m_PreviewBuffer.allocImageBuffer(bufferWidth, bufferHeight, SkypeImage::COLOR_RGB888);
  }

  m_PreviewBuffer.copyToBuffer((const uint8_t*)m_PreviewClient.bufferData(buffer), bufferFormat);

  return true;
}

void SkypeEx::OnApp2AppStreamListChange(const Sid::String &appname, 
				 const APP2APP_STREAMS &listType, 
				 const Sid::List_String &streams, 
				 const Sid::List_uint &receivedSizes)
{
  /*
  if (streams.size() != 0) {
    // Normally the streamcount in this event should be eithe 1 or 0.
    // More streams are possible when there are more than 2 connected
    // participants running the same application. For purposes of this
    // example, we will assume that there are only 2 participants.
    for (uint i= 0; i < streams.size(); i++) {
      printf("OnApp2AppStreamListChange: %s %s %s¥n",
	     (const char*)StreamListType(listType),
	     (const char*)appname,
	     (const char*)streams[i]);
      streamName = streams[i];
    }

    if (!appConnected) {
      appConnected = true;
      printf("You can now type and send datagrams to remote instance.¥n");
    }
  } else {
    if (listType == ALL_STREAMS) {
      // Remote side dropped connection.
      printf("No more app2app streams.¥n");
      streamName = "";
      quitCommand = true;
    }
  }
  */
}






void SkypeEx::OnApp2AppDatagram  (const Sid::String &appname, 
			   const Sid::String &stream, 
			   const Sid::Binary &data)
{

}

void SkypeEx::test()
{

  
  for(int i = 0;i < 10;i++) {
    updatePreviewFrame();
    //    sleep(1);
  }


}
