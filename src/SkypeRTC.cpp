// -*- C++ -*-
/*!
 * @file  SkypeRTC.cpp
 * @brief Skype RTC
 * @date $Date$
 *
 * $Id$
 */

#include "SkypeRTC.h"

// Module specification
// <rtc-template block="module_spec">
static const char* skypertc_spec[] =
  {
    "implementation_id", "SkypeRTC",
    "type_name",         "SkypeRTC",
    "description",       "Skype RTC",
    "version",           "1.0.0",
    "vendor",            "ysuga_net",
    "category",          "Experimental",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    // Configuration variables
    "conf.default.skypeId", "test",
    "conf.default.passwd", "pas",
    "conf.default.keyFile", "key.pem",
    "conf.default.skypekit", "skypekit.exe",
    "conf.default.caller", "ysuga0731",
    "conf.default.target", "",
    "conf.default.requireApp2AppConnect", "0",
    // Widget
    "conf.__widget__.skypeId", "text",
    "conf.__widget__.passwd", "text",
    "conf.__widget__.keyFile", "text",
    "conf.__widget__.skypekit", "text",
	
    "conf.__widget__.requireApp2AppConnect", "checkbox",
    // Constraints
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
SkypeRTC::SkypeRTC(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_goingMessageIn("goingMessage", m_goingMessage),
    m_outputStreamIn("outputStream", m_outputStream),
    m_comingMessageOut("comingMessage", m_comingMessage),
    m_inputStreamOut("inputStream", m_inputStream),
    m_previewImageOut("previewImage", m_previewImage),
    m_incomingImageOut("incomingImage", m_incomingImage)

    // </rtc-template>
{
//	m_pSkype = new SkypeEx();
}

/*!
 * @brief destructor
 */
SkypeRTC::~SkypeRTC()
{
}



RTC::ReturnCode_t SkypeRTC::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("goingMessage", m_goingMessageIn);
  addInPort("outputStream", m_outputStreamIn);
  
  // Set OutPort buffer
  addOutPort("comingMessage", m_comingMessageOut);
  addOutPort("inputStream", m_inputStreamOut);
  addOutPort("previewImage", m_previewImageOut);
  addOutPort("incomingImage", m_incomingImageOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // Bind variables and configuration variable
  bindParameter("skypeId", m_skypeId, "suga");
  bindParameter("passwd", m_passwd, "pass");
  bindParameter("caller", m_caller, "ysuga0731");
  bindParameter("keyFile", m_keyFile, "/User/ysuga/key.pem");
  bindParameter("skypekit", m_skypekit, "skypeKit.exe");
  
  bindParameter("requireApp2AppConnect", m_requireApp2AppConnect, "0");
  bindParameter("target", m_target, "");
  // </rtc-template>
  
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SkypeRTC::onFinalize()
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SkypeRTC::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SkypeRTC::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SkypeRTC::onActivated(RTC::UniqueId ec_id)
{
  m_Skype.setAutoTakeUserName(m_caller.c_str());
  m_Skype.loadKeyPair(m_keyFile.c_str());
  m_Skype.login(m_skypeId.c_str(), m_passwd.c_str());
  m_Skype.getContacts();

  
  while(!m_Skype.isVideoCapable()) {
    coil::usleep(1000);
  }
  if(!m_Skype.startPreview()) {
    std::cout << "Start Preview Failed" << std::endl;
    return RTC::RTC_ERROR;
  }
  coil::usleep(1000);
  
  std::string appName = "SkypeRTC";
  if(m_target.length() != 0) {
	m_Skype.callTarget(m_target);
	m_Skype.App2AppStreamConnect(appName, m_target);
  } else {
    m_Skype.App2AppStreamConnect(appName, m_caller);
  }

  if(m_requireApp2AppConnect) {
    std::cout << "Require Connect" << std::endl;

    while(!m_Skype.isStreamConnected()) {
      coil::usleep(1000);
    }
  }

  return RTC::RTC_OK;
}


RTC::ReturnCode_t SkypeRTC::onDeactivated(RTC::UniqueId ec_id)
{
  m_Skype.logout();
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SkypeRTC::onExecute(RTC::UniqueId ec_id)
{
  if(m_Skype.updatePreviewFrame()) {
    //    std::cout << "Preview OK" << std::endl;
    SkypeImage *preview = m_Skype.getPreviewFrame();
    m_previewImage.width = preview->getWidth();
    m_previewImage.height = preview->getHeight();
    m_previewImage.pixels.length(preview->getBufferLength());
    //    std::cout << "W:" << m_previewImage.width << ",H:" << m_previewImage.height << ",L:" << m_previewImage.pixels.length() << std::endl;
    memcpy((void*)&(m_previewImage.pixels[0]), preview->getBuffer(), preview->getBufferLength());
    m_previewImageOut.write();
  }

  if(m_Skype.updateIncomingFrame()) {
    SkypeImage *img = m_Skype.getIncomingFrame();
    m_incomingImage.width = img->getWidth();
    m_incomingImage.height = img->getHeight();
    m_incomingImage.pixels.length(img->getBufferLength());
    //    std::cout << "W:" << m_previewImage.width << ",H:" << m_previewImage.height << ",L:" << m_previewImage.pixels.length() << std::endl;
    memcpy((void*)&(m_incomingImage.pixels[0]), img->getBuffer(), img->getBufferLength());
    m_incomingImageOut.write();

  }
  

  if(this->m_outputStreamIn.isNew()) {
	  m_outputStreamIn.read();
	  m_Skype.writeApp2AppDatagram((uint8_t*)(&(m_outputStream.data[0])), m_outputStream.data.length());
  }

  if(m_Skype.isApp2AppDatagramPacketReceived()) {
	  int size = m_Skype.getApp2AppDatagramPacketSize();
	  this->m_inputStream.data.length(size);
	  m_Skype.readApp2AppDatagram((uint8_t*)&(m_inputStream.data[0]), size);
	  m_inputStreamOut.write();
  }
  return RTC::RTC_OK;
}


RTC::ReturnCode_t SkypeRTC::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SkypeRTC::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t SkypeRTC::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t SkypeRTC::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t SkypeRTC::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void SkypeRTCInit(RTC::Manager* manager)
  {
    coil::Properties profile(skypertc_spec);
    manager->registerFactory(profile,
                             RTC::Create<SkypeRTC>,
                             RTC::Delete<SkypeRTC>);
  }
  
};


