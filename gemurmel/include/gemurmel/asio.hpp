//
// Created by Jan Henke on 10.05.26.
//

#ifndef GEMURMEL_ASIO_HPP
#define GEMURMEL_ASIO_HPP

#ifdef GEMURMAL_USE_BOOST_ASIO
  #include <boost/asio.hpp>
  #include <boost/asio/ssl.hpp>
  namespace asio = boost::asio;
#else
#include <asio.hpp>
#include <asio/ssl.hpp>
// No namespace alias needed if using standalone ASIO.
#endif

#endif  //GEMURMEL_ASIO_HPP
