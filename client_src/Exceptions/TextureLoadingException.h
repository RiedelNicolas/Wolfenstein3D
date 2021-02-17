#ifndef TEXTURELOADINGEXCEPTION_H
#define TEXTURELOADINGEXCEPTION_H

#include <exception>
#include <string>

class TextureLoadingException: public std::exception {
private:
    std::string msg_error;
public:
    explicit TextureLoadingException(const std::string& error);
    const char* what() const noexcept;
    ~TextureLoadingException(){}
};


#endif //TEXTURELOADINGEXCEPTION_H