/****************************************************************************************************************************
  HTTPSServer.cpp

  For ESP32, and powerful-enough boards, such as ESP8266, Portenta_H7, RP2040W, Teensy 4.1, etc.

  HTTPS_Server_Generic is a library for HTTP/HTTPS server

  Based on and modified from esp32_https_server (https://github.com/fhessel/esp32_https_server)
  Built by Khoi Hoang https://github.com/khoih-prog/HTTPS_Server_Generic
  Licensed under MIT license

  Original author: Frank Hessel
 *****************************************************************************************************************************/

#include "HTTPSServer.hpp"

namespace httpsserver
{
constexpr const char * alpn_protos[] = { "http/1.1", NULL } ;

HTTPSServer::HTTPSServer(SSLCert * cert, const uint16_t port, const uint8_t maxConnections, const in_addr_t bindAddress):
  HTTPServer(port, maxConnections, bindAddress),
  _cert(cert)
	{
		
	// Check if _cert is null
  if (_cert == nullptr || 
    _cert->getCertData() == nullptr || 
    _cert->getCertLength() == 0 || 
    _cert->getPKData() == nullptr || 
    _cert->getPKLength() == 0) {
    Serial.println("ERROR: Invalid SSL certificate or private key.");
    return;  // Handle the error (e.g., return or set an error flag)
}	
		
		
	_cfg = new esp_tls_cfg_server();


	if (_cfg == nullptr) {
    Serial.println("ERROR: Failed to allocate memory for _cfg");
    return;  // Handle the error (e.g., return or set an error flag)
	}



	_cfg->alpn_protos = (const char **)alpn_protos;
	_cfg->cacert_buf = NULL;
	_cfg->cacert_bytes = 0;
	_cfg->servercert_buf =cert->getCertData();
	_cfg->servercert_bytes = cert->getCertLength();
	_cfg->serverkey_buf= cert->getPKData();
	_cfg->serverkey_bytes= cert->getPKLength();
	}

HTTPSServer::~HTTPSServer()
{
	//free(_cfg);
	delete _cfg;
}

/**
   This method starts the server and begins to listen on the port
*/
uint8_t HTTPSServer::setupSocket()
{
  if (!isRunning())
	{
    _cfg->servercert_buf= _cert->getCertData();
	_cfg->servercert_bytes = _cert->getCertLength();
	_cfg->serverkey_buf= _cert->getPKData();
	_cfg->serverkey_bytes= _cert->getPKLength();	

	//if(HTTPSServer::setupSocket())
	//	{
	//	return 1;	
	//	}
	//else
	//	{
	//	Serial.println("setupSockets failed");
	//	return 0;
	//	}
	if (!HTTPServer::setupSocket())  // Ensure this calls the base class setupSocket
    {
      Serial.println("ERROR: Failed to setup socket in HTTPSServer.");
      return 0;  // Return an error code or handle as needed
    }
    return 1;  // Successfully set up the socket
	
	
	
	}
  else
	{
	return 1;
	}
}	

void HTTPSServer::teardownSocket()
{
  HTTPServer::teardownSocket();
}

int HTTPSServer::createConnection(int idx)
{
  HTTPSConnection * newConnection = new HTTPSConnection(this);
  _connections[idx] = newConnection;
  return newConnection->initialize(_socket, _cfg, &_defaultHeaders);
}

/**
   This method configures the ssl context that is used for the server
*/
uint8_t HTTPSServer::setupCert()
	{
    // Check if _cert is null before accessing it
    if (_cert == nullptr) {
        Serial.println("ERROR: SSL Certificate is null in setupCert!");
        return 0;  // Return an error code or handle as needed
    }
	
	
	
	
	// Configure the certificate first
	_cfg->servercert_buf= _cert->getCertData();
	_cfg->servercert_bytes = _cert->getCertLength();
	_cfg->serverkey_buf= _cert->getPKData();
	_cfg->serverkey_bytes= _cert->getPKLength();
	return 1;
	}
	
} /* namespace httpsserver */