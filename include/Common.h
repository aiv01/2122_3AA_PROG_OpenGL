#pragma once
#include <stdexcept>
#include <iostream>

#define DIE_ON_ERROR(VALUE, MSG) if(VALUE == 0) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }
//#define DIE_ON_ERROR(VALUE, MSG) if(VALUE == 0) { std::cout << MSG << "\n"; exit(VALUE); }
#define DIE_ON_NULL(VALUE, MSG) if(VALUE == NULL) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }

#define DIE(MSG) { std::cout << MSG << "\n"; throw std::runtime_error(MSG); }