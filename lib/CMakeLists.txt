# Build Release on third-party libs
set(CMAKE_BUILD_TYPE Release)

# Set the standards required
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(USOCKETS_DIR lib/uWebSockets/uSockets/src)
set(UWEBSOCKETS_DIR lib/uWebSockets/src)

add_library(
  uSockets STATIC
  ${USOCKETS_DIR}/libusockets.h
  ${USOCKETS_DIR}/bsd.c
  ${USOCKETS_DIR}/context.c
  ${USOCKETS_DIR}/loop.c
  ${USOCKETS_DIR}/socket.c
  ${USOCKETS_DIR}/crypto/openssl.c
  ${USOCKETS_DIR}/crypto/sni_tree.cpp
  ${USOCKETS_DIR}/eventing/epoll_kqueue.c
  ${USOCKETS_DIR}/eventing/gcd.c
  ${USOCKETS_DIR}/eventing/libuv.c
)
target_compile_definitions(uSockets PRIVATE LIBUS_USE_OPENSSL)
# Enabling link time optimization with -flto is important for performance
# since it is set as PUBLIC, any library or executable linking this one will use the flag when built
target_compile_options(uSockets PUBLIC -flto)
target_link_options(uSockets
  PRIVATE -lstdc++
  PUBLIC -flto
)
target_link_libraries(uSockets ${OPENSSL_LIBRARIES} ${ZLIB_LIBRARIES})
target_include_directories(uSockets PUBLIC ${USOCKETS_DIR} ${OPENSSL_INCLUDE_DIR} ${ZLIB_INCLUDE_DIRS})

add_library(
  uWebSockets
  ${UWEBSOCKETS_DIR}/App.h
  ${UWEBSOCKETS_DIR}/AsyncSocketData.h
  ${UWEBSOCKETS_DIR}/AsyncSocket.h
  ${UWEBSOCKETS_DIR}/BloomFilter.h
  ${UWEBSOCKETS_DIR}/HttpContextData.h
  ${UWEBSOCKETS_DIR}/HttpContext.h
  ${UWEBSOCKETS_DIR}/HttpParser.h
  ${UWEBSOCKETS_DIR}/HttpResponseData.h
  ${UWEBSOCKETS_DIR}/HttpResponse.h
  ${UWEBSOCKETS_DIR}/HttpRouter.h
  ${UWEBSOCKETS_DIR}/LoopData.h
  ${UWEBSOCKETS_DIR}/Loop.h
  ${UWEBSOCKETS_DIR}/MessageParser.h
  ${UWEBSOCKETS_DIR}/MoveOnlyFunction.h
  ${UWEBSOCKETS_DIR}/Multipart.h
  ${UWEBSOCKETS_DIR}/PerMessageDeflate.h
  ${UWEBSOCKETS_DIR}/ProxyParser.h
  ${UWEBSOCKETS_DIR}/QueryParser.h
  ${UWEBSOCKETS_DIR}/TopicTree.h
  ${UWEBSOCKETS_DIR}/Utilities.h
  ${UWEBSOCKETS_DIR}/WebSocketContextData.h
  ${UWEBSOCKETS_DIR}/WebSocketContext.h
  ${UWEBSOCKETS_DIR}/WebSocketData.h
  ${UWEBSOCKETS_DIR}/WebSocketExtensions.h
  ${UWEBSOCKETS_DIR}/WebSocket.h
  ${UWEBSOCKETS_DIR}/WebSocketHandshake.h
  ${UWEBSOCKETS_DIR}/WebSocketProtocol.h
)
set_target_properties(uWebSockets PROPERTIES LINKER_LANGUAGE CXX)
target_compile_features(uWebSockets PUBLIC cxx_std_17)
target_link_libraries(uWebSockets uSockets)
target_include_directories(uWebSockets PUBLIC ${UWEBSOCKETS_DIR})