<<<<<<< HEAD
#ifndef _FILESERVICE_H_
#define _FILESERVICE_H_

#include "HttpService.h"

#include <string>

class FileService : public HttpService {
 public:
  FileService(std::string basedir);

  virtual void get(HTTPRequest *request, HTTPResponse *response);
  virtual void head(HTTPRequest *request, HTTPResponse *response);

private:
  bool endswith(std::string str, std::string suffix);
  std::string readFile(std::string path);

  std::string m_basedir;
};

#endif
=======
#ifndef _FILESERVICE_H_
#define _FILESERVICE_H_

#include "HttpService.h"

#include <string>

class FileService : public HttpService {
 public:
  FileService(std::string basedir);
  ~FileService();

  virtual void get(HTTPRequest *request, HTTPResponse *response);
  virtual void head(HTTPRequest *request, HTTPResponse *response);

private:
  bool endswith(std::string str, std::string suffix);
  std::string readFile(std::string path);

  std::string m_basedir;
};

#endif
>>>>>>> 4798a52751a6b037da0cd366e648c1eb89ba6287
