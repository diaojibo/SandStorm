cmake_minimum_required(VERSION 3.9)
project(SandStorm)

set(CMAKE_CXX_STANDARD 11)

find_package(Threads)


include_directories(./include/main)

set(COMMON_OBJ ./src/main/sandstorm/base/ByteArray.cpp
        ./src/main/sandstorm/base/DataPackage.cpp
        ./src/main/sandstorm/base/Library.cpp
        ./src/main/sandstorm/base/Values.cpp
        ./src/main/sandstorm/base/Variant.cpp
        ./src/main/sandstorm/bolt/BoltDeclarer.cpp
        ./src/main/sandstorm/collector/OutputCollector.cpp
        ./src/main/sandstorm/collector/OutputDispatcher.cpp
        ./src/main/sandstorm/message/Command.cpp
        ./src/main/sandstorm/message/CommandClient.cpp
        ./src/main/sandstorm/message/CommandServer.cpp
        ./src/main/sandstorm/network/epoll/EpollClient.cpp
        ./src/main/sandstorm/network/epoll/EpollLoop.cpp
        ./src/main/sandstorm/network/epoll/EpollServer.cpp
        ./src/main/sandstorm/network/epoll/EpollStream.cpp
        ./src/main/sandstorm/network/utils/CommonUtils.cpp
        ./src/main/sandstorm/network/EventQueueLoop.cpp
        ./src/main/sandstorm/service/Manager.cpp
        ./src/main/sandstorm/service/ManagerContext.cpp
        ./src/main/sandstorm/service/President.cpp
        ./src/main/sandstorm/spout/SpoutDeclarer.cpp
        ./src/main/sandstorm/task/BoltExecutor.cpp
        ./src/main/sandstorm/task/Executor.cpp
        ./src/main/sandstorm/task/PathInfo.cpp
        ./src/main/sandstorm/task/SpoutExecutor.cpp
        ./src/main/sandstorm/task/TaskDeclarer.cpp
        ./src/main/sandstorm/task/TaskInfo.cpp
        ./src/main/sandstorm/topology/Topology.cpp
        ./src/main/sandstorm/topology/TopologyLoader.cpp
        ./src/main/sandstorm/util/Configuration.cpp
        ./src/main/sandstorm/util/NetConnector.cpp
        ./src/main/sandstorm/util/NetListener.cpp
        ./src/main/sandstorm/util/Socket.cpp
        ./src/main/sandstorm/util/StringUtil.cpp)


add_executable(manager ./src/main/sandstorm/tool/StartManager.cpp ${COMMON_OBJ})

add_executable(president ./src/main/sandstorm/tool/StartPresident.cpp ${COMMON_OBJ})

add_library(wordcount SHARED ./src/main/sample/HelloWorld.cpp
        ./src/main/sample/SplitSentenceBolt.cpp
        ./src/main/sample/WordCountBolt.cpp
        ./src/main/sample/WordCountTopology.cpp
        ${COMMON_OBJ}
        )

target_link_libraries(manager ${CMAKE_THREAD_LIBS_INIT}
        ${CMAKE_DL_LIBS})

target_link_libraries(president ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

target_link_libraries(wordcount ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})