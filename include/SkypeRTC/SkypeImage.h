#pragma once

#include <stdint.h>

class SkypeImage {
 private:
  uint8_t *m_pBuffer;
  uint32_t m_Width;
  uint32_t m_Height;
  uint32_t m_BufferLength;
  uint32_t m_ColorModel;

 public:
  uint32_t getWidth() {return m_Width;}
  uint32_t getHeight() {return m_Height;}
  uint32_t getBufferLength() {return m_BufferLength;}
  uint32_t getColorModel() {return m_ColorModel;}
  uint8_t* getBuffer(){return m_pBuffer;}
 public:

  enum COLOR_MODEL {
    COLOR_RGB888,
    COLOR_ARGB8888,
  };

 SkypeImage() : m_pBuffer(NULL), m_Width(0), m_Height(0), m_BufferLength(0), m_ColorModel(0) {}
  SkypeImage(uint32_t width, uint32_t height, uint32_t colorModel) {
    allocImageBuffer(width, height, colorModel);
  }

  SkypeImage(uint8_t* src, uint32_t width, uint32_t height, uint32_t colorModel) {
    allocImageBuffer(width, height, colorModel);
    memcpy(m_pBuffer, src, m_BufferLength);
  }

  public:
    void allocImageBuffer(uint32_t width, uint32_t height, uint32_t colorModel) {
      m_Width = width;
      m_Height = height;
      m_ColorModel = colorModel;
      switch(colorModel) {
      case COLOR_RGB888:
	m_BufferLength = m_Width*m_Height*3;
	m_pBuffer = new uint8_t[m_Width*m_Height*3];
      break;
      case COLOR_ARGB8888:
	m_BufferLength = m_Width*m_Height*4;
	m_pBuffer = new uint8_t[m_Width*m_Height*4];
	break;
      default:
	m_pBuffer = NULL;
	m_BufferLength = 0;
	return;
      }
    }

    void copyToBuffer(const uint8_t* src, uint32_t colorModel) {
      if(colorModel == getColorModel()) {
	memcpy(m_pBuffer, src, m_BufferLength);
      } else if(getColorModel() == COLOR_RGB888 && colorModel == COLOR_ARGB8888) {
	for(int h = 0;h < m_Height;h++) {
	  for(int w = 0;w < m_Width;w++) {
	    int index = m_Width*h + w;
	    m_pBuffer[index*3+0] = src[index*4+1];
	    m_pBuffer[index*3+1] = src[index*4+2];
	    m_pBuffer[index*3+2] = src[index*4+3];
	  }
	}
      }
    }
};      
