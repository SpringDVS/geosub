project( lnroot )
set( SOURCES
    protocol_handler.cpp
    resolution_service.cpp
    http_session.cpp
    http_service.cpp
    entry.cpp )

add_executable( lnroot ${SOURCES} )
target_link_libraries( lnroot springdvs pthread )
